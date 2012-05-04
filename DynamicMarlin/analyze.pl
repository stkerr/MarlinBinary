open(MYINPUTFILE, "time_results");
my $make_sum = 0;
my $make_count = 0;
my $fish_sum = 0;
my $fish_count = 0;

while(<MYINPUTFILE>)
{
	# Good practice to store $_ value because
	# subsequent operations may change it.
	my($line) = $_;
	
	# Good practice to always strip the trailing
	# newline from the line.
	chomp($line);

	if($line =~ "make")
	{
#		print "make: " . $line . "\n";
		$line =~ s/^.*:\ //s;
		$make_sum = $make_sum + $line;
		$make_count = $make_count + 1;
	}
	
	if($line =~ "fish")
	{
#		print "fish: " . $line . "\n";
		$line =~ s/^.*:\ //s;
		$fish_sum = $fish_sum + $line;
		$fish_count = $fish_count + 1;
	}
}

#print $make_sum;
#print $fish_sum;
print "N: " . $make_count . "\n";
print "Preprocess: " . $make_sum / $make_count . "\n";
print "Process: " . $fish_sum / $fish_count . "\n";
