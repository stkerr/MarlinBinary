sub find_file_text_start
{

open FILE, $_[0] or die $!;
	my $symbol_file = do { local $/; <FILE> };
close FILE;


# get only the .text line
$symbol_file =~ s/^.*\.text//s;
$symbol_file =~ s/\n.*//s;

$symbol_file =~ s/[\ ]* //s;
$symbol_file =~ s/[^\ ]*\ [^\ ]*\ [^\ ]*\ [^\ ]*\ [^\ ]*\ [^\ ]*\ //s;
$symbol_file =~ s/\ .*$//s;

#print $symbol_file . "\n";
return "0x" . $symbol_file;
}

sub number_of_spaces
{
	$text = $_[0];
	$count = 0;

	while($text ne "")
	{
		$text =~ s/[^\ ]*\ //s;
#		print $text . "\n";
		$count = $count + 1;
	}
	#print "calc: " . $count . "\n";
	return $count;
}

sub find_text_start
{
	open FILE, $_[0] or die $!;
	my $symbol_file = do { local $/; <FILE> };
	close FILE;

	# get only the .text line
	$symbol_file =~ s/^.*\.text//s;
	$symbol_file =~ s/\n.*//s;

	$symbol_file =~ s/[\ ]* //s;
	$symbol_file =~ s/[^\ ]*\ [^\ ]*\ //s;
	$symbol_file =~ s/\ .*$//s;

	#print $symbol_file . "\n";
	return "0x" . $symbol_file;
}



sub find_plt_start
{
	open FILE, $_[0] or die $!;
	my $symbol_file = do { local $/; <FILE> };
	close FILE;

	# get only the .text line
	$symbol_file =~ s/^.*\ \.plt\ //s;
	$symbol_file =~ s/\n.*//s;

	$symbol_file =~ s/[\ ]* //s;
	$symbol_file =~ s/[^\ ]*\ [^\ ]*\ //s;
	$symbol_file =~ s/\ .*$//s;

	print $symbol_file . "\n";
	return "0x" . $symbol_file;
}

sub find_file_plt_start
{
	open FILE, $_[0] or die $!;
	my $symbol_file = do { local $/; <FILE> };
	close FILE;

	# get only the .text line
	$symbol_file =~ s/^.*\ \.plt\ //s;
	$symbol_file =~ s/\n.*//s;

	$symbol_file =~ s/[\ ]* //s;
	$symbol_file =~ s/[^\ ]*\ [^\ ]*\ [^\ ]*\ [^\ ]*\ [^\ ]*\ [^\ ]*\ //s;
	$symbol_file =~ s/\ .*$//s;

	print $symbol_file . "\n";
	return "0x" . $symbol_file;
}

# Returns all sets of data relevant to PLT entries
# Returns:
#	1 -> 1st main.cpp entry (current_address_map.insert)
#	2 -> 2nd main.cpp entry (symbols.push_back)
#	3 -> text_symbols.h entry
sub generate_plt_list
{


	open FILE, $_[0] or die $!;
	my $plt_section = do { local $/; <FILE> };
	close FILE;

	$plt_section =~ s/^.*?.Disassembly of section .plt://s;
	$plt_section =~ s/Disassembly of section.*$//s;
	$plt_section =~ s/^\s+|\s+$//g;

	my @array = split(/\n{2,}/, $plt_section); # note the new pattern

	my $first = "";
	my $second = "";
	my $third = "";

	foreach(@array)
	{
		my @lines = split(/\n/);

		my $function_address = $lines[0];
		$function_address =~ s/\ .*$//s;
		$function_address = "0x" . $function_address;

		# is this a plt symbol like <name@plt-0x10> ?
		my $function_name = $lines[0];
		if($function_name =~ "0x")
		{
			next;
		}

		
		$function_name =~ s/^.*<//s;
		$function_name =~ s/\@plt.*$//s;
		$function_name = $function_name . "_plt";

		#print $function_name . "\n";

		# have we already processed this symbol?
		if($first =~ $function_name)
		{
			next;
		}



		#current_address_map.insert(pair<string, unsigned int>("main", text_symbols::fun_main.address));
		$first = $first . "current_address_map.insert(pair<string, unsigned int>(\"" . $function_name . "\", text_symbols::fun_" . $function_name . ".address));\n";

		#symbols.push_back(text_symbols::fun_main);
		$second = $second . "symbols.push_back(text_symbols::fun_" . $function_name . ");\n";

		#const text_symbol fun_function1 = { 0x1D, TEXT_TO_FILE(0x80484c4), "function1", NOPATCHNEEDED};
		$third = $third . "const text_symbol fun_" . $function_name . " = { 16, TEXT_TO_FILE(" . $function_address . "), \"" . $function_name . "\", NOPATCHNEEDED};\n";
	}

#	print $first;
#	print "\n";
#	print $second;
#	print "\n";
#	print $third;
	return ($first, $second, $third);
}

# needed if this file is included with REQUIRE
1;