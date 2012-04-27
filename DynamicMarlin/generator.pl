#!/usr/bin/perl -w
use strict;

require "generator_functions.pl";
require "generator_strings.pl";

# The 1st argument is the disassembly
# The 2nd argument is the symbol list

# read the file
open FILE, $ARGV[0] or die $!;
my $text_section= do { local $/; <FILE> };
close FILE;

my $file_start = find_file_start($ARGV[1]);

# only use the .text section
$text_section =~ s/^.*?.Disassembly of section .text://s;
$text_section =~ s/Disassembly of section.*$//s;
#print $text_section;

$text_section =~ s/^\s+|\s+$//g;

#print $text_section;

#my @array;
#{
#  local $/ = '\n';
#  @array = $text_section;
#}

open PATCHING, ">", 'patch.cpp' || die("Cannot Open File");
open TEXT_SYMBOLS, ">", 'symbols.cpp' || die("Cannot Open File");

our $patch_start;
our $symbols_start;

print PATCHING $patch_start;



my @array = split(/\n{2,}/, $text_section); # note the new pattern

# forbidden symbols we do not want to touch
my @forbidden = ("_start", "call_gmon_start", "__libc_csu_init", "__libc_csu_fini", "printf");


# wildcards to match against and ignore
my @wildcards = (qr/^__.*$/, qr/^libc.*$/);

# the address of the first symbol of interest
my $first_symbol = "-1";

# the string to print in the namespace
my $symbol_namespace = "";

foreach(@array)
{
	my @lines = split(/\n/);

#	print $lines[0];


	my $function_name = $lines[0];
	$function_name =~ s/^.*<//s;
	$function_name =~ s/>.*$//s;
	
	if (grep {$_ eq $function_name} @forbidden)
	{
#		print $function_name . " is forbidden\n";
		next;
	}
	else
	{
	}

	my $function_address = $lines[0];
	$function_address =~ s/\ .*$//s;
	$function_address = "0x" . $function_address;
	
	if($first_symbol eq -1)
	{
		$first_symbol = $function_address;
#		print "New first: " . $first_symbol . "\n";
	}

#	print $function_address;

	my $last_line = $lines[scalar(@lines)-1];
#	my $last_line = $lines[2];
#	print $last_line;

	my $last_line_address = $last_line;
	$last_line_address =~ s/^\ *//s;
	$last_line_address =~ s/:.*$//s;
#	print $last_line_address;

	my $bytes = $last_line;
	$bytes =~ s/^[^\t]*\t//s;
	$bytes =~ s/\t.*$//s;
	my $count = $bytes =~ tr/e//;
	$count = $count + 1;
	
	my $function_length = hex($last_line_address) + $count - hex($function_address);
#	printf("%x", $function_length);

# we want to make
# const text_symbol fun_function1 = { 0x1D, TEXT_TO_FILE(0x80484c4), "function1", NOPATCHNEEDED};
	$symbol_namespace = $symbol_namespace . "const text_symbol fun_" . $function_name . " = { " . $function_length
		. ", TEXT_TO_FILE(" . $function_address . "), \"" . $function_name
		. "\", ";
	
	my $patch_needed = 0;

	


#######################
# Calculate jumps now #
#######################
	
	my $jump_count = 0;

	foreach(@lines)
	{
		if($_ eq $lines[0])
		{
			next;
		}

		my $jump_line = $_;

		my $jump_name = $jump_line;
		$jump_name =~ s/^.*<//s;
		$jump_name =~ s/\>.*$//s;
		$jump_name =~ s/\ .*$//s;


		if($jump_name ne "" and $jump_name !~ $function_name and 
			$jump_name !~ "plt")
		{
			$patch_needed = 1;

			my $jump_address = $jump_line;
			$jump_address =~ s/^\ //s;
			$jump_address =~ s/:.*$//s;
			$jump_address = "0x" . $jump_address;

			my $offset = hex($jump_address)-hex($function_address);
			$offset = $offset + 1; # add 1, since the first is an opcode

#			print $offset;
#			print "\n";
#			print $jump_name;
			if($jump_count == 0)
			{
				print PATCHING "MASTER(" . $function_name . ", " . $jump_name
					. ", " . $offset . ", 0, " . $jump_count . ");";
			}
			else
			{
				print PATCHING "ADDITION(" . $function_name . ", " . $jump_name
					. ", " . $offset . ", 0, " . $jump_count . ");";
			}
			print PATCHING "\n";
				
			$jump_count = $jump_count + 1;
		}

	}

	if($patch_needed != 0)
	{
		$symbol_namespace = $symbol_namespace . "PATCHNEEDED};";
	}
	else
	{
		$symbol_namespace = $symbol_namespace . "NOPATCHNEEDED};";
	}
	$symbol_namespace = $symbol_namespace . "\n";

}

our $patching_end;
print PATCHING $patching_end;

print TEXT_SYMBOLS $symbols_start;
print TEXT_SYMBOLS $first_symbol;

print TEXT_SYMBOLS ")
#define FILE_START (";
 
print TEXT_SYMBOLS $file_start;
print TEXT_SYMBOLS ")";

our $symbols_mid;
print TEXT_SYMBOLS $symbols_mid;
print TEXT_SYMBOLS $symbol_namespace;

#print $file_start;
#	0x3f0

our $symbols_end;
print TEXT_SYMBOLS $symbols_end;

