#!/usr/bin/perl

$queryNum_file = $ARGV[0];
$click_file = $ARGV[1];
$out_file = $ARGV[2];

open FILE, $queryNum_file or die "open $queryNum_file failed\n";
while (<FILE>)
{
	chop;
	@tmp = split /\t/;
	$query = substr($tmp[0],1);
	chop $query; # 去[]
	next if ($query eq "");
	$pv = $tmp[1];
	$query_pv_map{$query} = $pv;
}
close FILE;

open FILE, $click_file or die "open $click_file failed\n";
while (<FILE>)
{
	chop;
	@tmp = split /\t/;
	$query = substr($tmp[1],1);
	chop $query; # 去[]
	next if ($query eq "");
	$url = "http://".$tmp[4];
	$key = $query."".$url;
	if (exists $url_click_map{$key})
	{
		$url_click_map{$key}[1]++; # click
	}
	else
	{
		$url_click_map{$key}[0] = $query_pv_map{$query} + 0; # pv
		$url_click_map{$key}[1] = 1; # click
	}
}
close FILE;

open OUT, ">$out_file" or die "open $out_file failed\n";
while (($key, $value) = each %url_click_map)
{
	@info = @{$value};
	$pv = $info[0];
	$click = $info[1];
	if ($pv > 0) # 实验表明，查不到pv的query约10%，故不考虑
	{
		$ctr = $click/$pv;
		print OUT "$key\t$pv\t$click\t$ctr\n";
	}
	else
	{
		print OUT "$key\t0\t$click\t0\n";
	}
}
