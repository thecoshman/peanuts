#!/usr/bin/perl
use strict;
use warnings;
use pinkpill::PinkPill;

my $common_folders = 'src/common nix^src/linux win^src/windows osx^src/osx '; 
my $pp = new PinkPill;
$pp->set_options(
    src_folders => $common_folders . 'src/main.cpp',
    inc_folders => $common_folders . 'src dependencies/boost_1_53_0 dependencies/glsdk_0.4.4/glload/include',
    compiler_flags => '-std=c++11 -Wall -Wextra -Wno-unused-parameter -DPEANUTS_LINUX',
    linker_flags => '-Ldependencies/bin',
    link_libraries => 'X11 GL glload',
    program_name => 'peanuts_demo',
);
$, = "\n";
print "\nError logs...\n" and print $pp->error_logs and print "\n\n" unless $pp->build;
