sub find_file_start
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
return $symbol_file;
}

# needed if this file is included with REQUIRE
1;