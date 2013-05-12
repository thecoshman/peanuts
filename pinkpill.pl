#!/usr/bin/perl
use strict;
use warnings;
use PinkPill;

my $pp = new PinkPill;
$pp->set_options(
    src_folders => 'src !^src/platform src/platform/common nix^src/platform/linux win^src/platform/windows osx^src/platform/osx',
    build_folder => 'bin',
    obj_folder => 'bin/obj',
    compiler_flags => '-std=C++11 -Wall -Wextra',
);
$, = "\n";
#my @options = PinkPill->config_options();
#print @options;
#print "\n\n";
my @options = $pp->config_options();
print @options;
print "\n\n";
print "\nError logs...\n" and print $pp->error_logs and print "\n\n" unless $pp->build;

