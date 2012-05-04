#!/usr/bin/perl -w
use strict;

# don't line buffer STDOUT
$| = 1;

require "generator_functions.pl";
require "generator_strings.pl";

# The 1st argument is the disassembly
# The 2nd argument is the symbol list

# read the file
open FILE, $ARGV[0] or die $!;
my $text_section= do { local $/; <FILE> };
close FILE;

# parse the section header file out to get some start addresses
my $text_file_start = find_file_text_start($ARGV[1]);
my $text_start = find_text_start($ARGV[1]);
my $plt_start = find_plt_start($ARGV[1]);
my $plt_file_start = find_file_plt_start($ARGV[1]);

my ($plt_first, $plt_second, $plt_third, $plt_fourth) = generate_plt_list($ARGV[0]);

# only use the .text section
$text_section =~ s/^.*?.Disassembly of section .text://s;
$text_section =~ s/Disassembly of section.*$//s;
#print $text_section;

$text_section =~ s/^\s+|\s+$//g;

# open the files we dynamically generate
open PATCHING, ">", 'patch.cpp' || die("Cannot Open File");
open TEXT_SYMBOLS, ">", 'text_symbols.h' || die("Cannot Open File");
open MAIN, ">", 'main.cpp' || die("Cannot open main file");
open JUMPPATCH, ">", 'jumppatching.h' || die("Cannot open jumppatching.h");


open PATCHFILE, ">", 'patchlist.txt' || die("Cannot open file");
open FUNCLIST, ">", 'symbollist.txt' || die("Cannot open file");

our $patch_start;
our $symbols_start;



print PATCHING $patch_start;

print PATCHFILE $plt_fourth;


my @array = split(/\n{2,}/, $text_section); # note the new pattern

# forbidden symbols we do not want to touch
my @forbidden = ("_start", "call_gmon_start", "__libc_csu_init", "__libc_csu_fini", "__gmon_start___plt");


# wildcards to match against and ignore
my @wildcards = (qr/^__.*$/, qr/^libc.*$/);

# the address of the first symbol of interest
my $first_symbol = "-1";

# the string to print in the namespace
my $symbol_namespace = "";

# the pushback list (needed for main.cpp)
my $pushback_list = "";

# address lists (needed for main.cpp)
my $address_list = "";

# length list (needed for main.cpp)
my $length_list = "";

# iterate through each symbol stripped out
foreach(@array)
{
	# get the lines of the entry as an array
	my @lines = split(/\n/);

	# parse the name of this function
	my $function_name = $lines[0];
	$function_name =~ s/^.*<//s;
	$function_name =~ s/>.*$//s;
	
	# ignore forbidden symbols
	if (grep {$_ eq $function_name} @forbidden)
	{
		print $function_name . " is forbidden\n";
		next;
	}
	else
	{
		print "Processing: " . $function_name . "\n";
	}

	my $function_address = $lines[0];
	$function_address =~ s/\ .*$//s;
	$function_address = "0x" . $function_address;
	
	if($first_symbol eq -1)
	{
		$first_symbol = $function_address;
#		print "New first: " . $first_symbol . "\n";
	}

	# grab the last line of the entry
	my $last_line = $lines[scalar(@lines)-1];

	# mark the address of the last line
	my $last_line_address = $last_line;
	$last_line_address =~ s/^\ *//s;
	$last_line_address =~ s/:.*$//s;
#	print $last_line_address;

	# count how many bytes are on this line
	my $bytes = $last_line;
	$bytes =~ s/^[^\t]*\t//s;
	$bytes =~ s/\t.*$//s;
	#print $bytes . "\n";
	my $count = 0;
	$count = int(number_of_spaces($bytes));

	# calculate the length of this function
	my $function_length = hex($last_line_address) + $count - hex($function_address);

#	print $function_name . ":" . $function_length . "\n";
#	print $last_line_address . " + " . $count . " - " . $function_address . "\n";

	# append the length to the length list (not the main function though)
	if($function_name ne "main")
	{
		$length_list = $length_list . " + " . $function_length;
	}

	# we want to make
	# const text_symbol fun_function1 = { 0x1D, TEXT_TO_FILE(0x80484c4), "function1", NOPATCHNEEDED};
	$symbol_namespace = $symbol_namespace . "const text_symbol fun_" . $function_name . " = { " . $function_length
		. ", TEXT_TO_FILE(" . $function_address . "), \"" . $function_name
		. "\", ";
	
	# append this symbol to the pushback list
	$pushback_list = $pushback_list . "symbols.push_back(text_symbols::fun_" . $function_name . ");\n";

	# generate the function address lines
	#        function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("function1", pair<unsigned int, unsigned int>(text_symbols::fun_function1.address, 0x1d)));
	#		 current_address_map.insert(pair<string, unsigned int>("function1", text_symbols::fun_function1.address));
	if($function_name ne "main")
	{
		$address_list = $address_list . "function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >(\"" . $function_name . 
			"\", pair<unsigned int, unsigned int>(text_symbols::fun_" . $function_name . ".address, " . $function_length . ")));\n";
	}

	$address_list = $address_list . "current_address_map.insert(pair<string, unsigned int>(\"" . $function_name . "\", text_symbols::fun_" . $function_name . ".address));\n";


	# sentinel value if this function needs patching
	my $patch_needed = 0;
	


	#######################
	# Calculate jumps now #
	#######################
	
	my $jump_count = 0;

	foreach(@lines)
	{
		my $plt_mode = 0;

		if($_ eq $lines[0])
		{
			next;
		}

		my $jump_line = $_;

		my $jump_name = $jump_line;
		$jump_name =~ s/^.*<//s;
		$jump_name =~ s/\>.*$//s;
		$jump_name =~ s/\ .*$//s;


		if($jump_name ne "" and $jump_name !~ $function_name 
			#and $jump_name !~ "plt"
			)
		{
			my $jump_address = $jump_line;
			$jump_address =~ s/^\ //s;
			$jump_address =~ s/:.*$//s;
			$jump_address = "0x" . $jump_address;

			#print $jump_name . "\n";

			if($jump_name =~ "plt")
			{
				$plt_mode = 1;
				#print ".plt:" . $jump_name . "\n";
				$jump_name =~ s/\@plt.*$/_plt/s;

			}

			$patch_needed = 1;


			my $offset = hex($jump_address)-hex($function_address);
			$offset = $offset + 1; # add 1, since the first is an opcode

#			print $offset;
#			print "\n";
#			print $jump_name;
			if($jump_count == 0)
			{
				print PATCHFILE $function_name . " " . $jump_name  . " " . $offset . " " . $jump_count . "\n";

				print PATCHING "MASTER(" . $function_name . ", " . $jump_name
					. ", " . $offset . ", 0, " . $jump_count . ");";
			}
			else
			{
				print PATCHFILE $function_name . " " . $jump_name  . " " . $offset . " " . $jump_count . "\n";

				print PATCHING "ADDITION(" . $function_name . ", " . $jump_name
					. ", " . $offset . ", 0, " . $jump_count . ");";
			}
			print PATCHING "\n";
				
			$jump_count = $jump_count + 1;
		}

	}

	if($patch_needed != 0)
	{
		print FUNCLIST $function_name . " " . $function_address . " " . $function_length . " " . "0x100" . "\n";
		$symbol_namespace = $symbol_namespace . "PATCHNEEDED};";
	}
	else
	{
		print FUNCLIST $function_name . " " . $function_address . " " . $function_length . " " . "0x0" . "\n";
		$symbol_namespace = $symbol_namespace . "NOPATCHNEEDED};";
	}
	$symbol_namespace = $symbol_namespace . "\n";

}

#print STDOUT $pushback_list . "\n";

our $patching_end;
print PATCHING $patching_end;

# print the text_symbols.h header
print TEXT_SYMBOLS $symbols_start;

# print the start address of the .text section in text_symbols.h
print TEXT_SYMBOLS $text_start;

# print the FILE_START constant
print TEXT_SYMBOLS ")
#define FILE_START (";
#	print $file_start . "\n";
print TEXT_SYMBOLS $text_file_start;
print TEXT_SYMBOLS ")";

our $symbols_mid;
print TEXT_SYMBOLS $symbols_mid;
print TEXT_SYMBOLS $symbol_namespace;
print TEXT_SYMBOLS $plt_third;

#print $file_start;
#	0x3f0

our $symbols_end;
print TEXT_SYMBOLS $symbols_end;


#### Make main.cpp ###
our $main_start;
our $main_mid;
our $main_end;
print MAIN $main_start;

# print the start and length lines
#	unsigned int start = 0x80484c4;
#	unsigned int end = start + 10 * 0x1D + 10 * 0xA;
print MAIN "unsigned int start = " . $first_symbol . ";\n";
print MAIN "unsigned int end = start " . $length_list . ";\n";

# print the addresses
print MAIN $address_list;
print MAIN $plt_first;
print MAIN $main_mid;

# print the pushback list
print MAIN $pushback_list;
print MAIN $plt_second;

print MAIN $main_end;


### Make jumppatching.h ###
our $jumppatching_whole;
print JUMPPATCH $jumppatching_whole;


#print FUNCLIST "\n";
#print PATCHFILE "\n";

### Execute the MarlinProgram now
my $command = "/Users/stkerr/Documents/Code/MarlinBinaryProcessor/dist/Debug/GNU-MacOSX/marlinbinaryprocessor " . $ARGV[2] . " symbollist.txt patchlist.txt " . $text_file_start . " " . $text_start . "\n" ;
print $command;
system($command);