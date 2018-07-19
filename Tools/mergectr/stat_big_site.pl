#!/usr/bin/perl

while (<STDIN>)
{
	chop;
	@tmp = split /\t/;
	@info = split(//, $tmp[0]);
	$site_map{$info[0]}[0] += $tmp[1];
	$site_map{$info[0]}[1] += $tmp[2];
}

foreach $site (reverse sort {$site_map{$a}[0] <=> $site_map{$b}[0]} keys %site_map)
{
	$pv = $site_map{$site}[0];
	$click = $site_map{$site}[1];
	$ctr = $pv>0 ? $click/$pv : 0;
	print STDOUT "$site\t$pv\t$click\t$ctr\n";
}
