#! /usr/bin/env python
# encoding: utf-8
# Guy Sherman, 2015

import subprocess

LIBOCA_VERSION			=	'0.0.1'
LIBOCA_MAJOR_VERSION 	=	'0'


VERSION = LIBOCA_VERSION
APPNAME = 'liboca'

top = '.'
out = 'build'

def options(opt):
    opt.load('compiler_cxx')


def configure(conf):
    conf.load('compiler_cxx')
    conf.env.CXXFLAGS = ['-Wall', '-ansi', '-ggdb', '-Werror', '-pedantic-errors']

def build(bld):
    libs = ''
    defines = ''
    bld.program(source = bld.path.ant_glob('examples/hello/*.cxx'),
                includes = [],
                lib = libs,
                target = 'examples/example-hello',
                install_path = '${BINDIR}',
                defines = defines)
    bld.program(source = bld.path.ant_glob('tests/*.cxx'),
                includes = [],
                lib = ['gtest', 'gtest_main', 'pthread'],
                target = 'tests/all',
                install_path = '${BINDIR}',
                defines = defines)
