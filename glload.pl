#!/usr/bin/perl
use strict;
use warnings;
use pinkpill::PinkPill;

my $common_folder = 'dependencies/glsdk_0.5.0/glload';
my $src_folder = $common_folder . '/source/';
my $pp = new PinkPill;
$pp->set_options(
    #src_folders => $src_folder . 'gl_load_cpp.cpp ' . $src_folder . 'glx_load_cpp.cpp',
    src_folders => $src_folder . 'gl_load_cpp.cpp ' . $src_folder . 'gl_load.c ' . $src_folder . 'glx_load_cpp.cpp ' . $src_folder . 'glx_load.c',
    inc_folders => $common_folder . '/include',
    compiler_flags => '-DLOAD_X11',
    build_folder => 'dependencies/bin/',
    obj_folder => 'dependencies/bin/obj',
    mode => 'static',
    program_name => 'glload',
    compiler => 'gcc',
);
$, = "\n";
print "\nError logs...\n" and print $pp->error_logs and print "\n\n" unless $pp->build;