#!/usr/bin/perl
use strict;
use warnings;
use pinkpill::PinkPill;

my $peanuts_includes = 'src dependencies/boost_1_53_0 dependencies/glsdk_0.4.4/glload/include '; 
my $pp = new PinkPill;
$pp->set_options(
    src_folders => 'example/main.cpp',
    inc_folders => $peanuts_includes . 'example',
    compiler_flags => '-std=c++11 -Wall -Wextra -Wno-unused-parameter -DPEANUTS_LINUX',
    build_folder => 'example/bin/',
    obj_folder => 'example/bin/obj',
    linker_flags => '-Lbin -Ldependencies/bin',
    link_libraries => 'peanuts X11 GL glload ',
    program_name => 'peanuts_demo',
);
$, = "\n";
print "\nError logs...\n" and print $pp->error_logs and print "\n\n" unless $pp->build;
