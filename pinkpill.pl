#!/usr/bin/perl
use strict;
use warnings;
use PinkPill;

my $common_folders = 'src !^src/platform src/platform/common nix^src/platform/linux win^src/platform/windows osx^src/platform/osx '; 
my $pp = new PinkPill;
$pp->set_options(
    src_folders => $common_folders,
    inc_folders => $common_folders . 'dependencies/boost_1_53_0 dependencies/glsdk_0.4.4/glload/include',
    compiler_flags => '-std=c++11 -Wall -Wextra -DPEANUTS_LINUX',
);
$, = "\n";
print "\nError logs...\n" and print $pp->error_logs and print "\n\n" unless $pp->build;
