#!/usr/bin/perl
use strict;
use warnings;
use PinkPill;

#gll_c.c
#gll_cpp.cpp
#gll_gl_ext_base.h
#gll_gl_ext.c
#gll_gl_ext.h
#gll_util.h
#glxl_ext.c
#glxl_ext.h
#wgll_ext_base.h
#wgll_ext.c
#wgll_ext.h

my $common_folder = 'dependencies/glsdk_0.4.4/glload';
my $src_folder = $common_folder . '/source/';
my $pp = new PinkPill;
$pp->set_options(
    src_folders => $src_folder . 'gll_cpp.cpp ' . $src_folder . 'gll_c.c ' . $src_folder . 'gll_gl_ext.c nix^' . $src_folder . 'glxl_ext.c' ,
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