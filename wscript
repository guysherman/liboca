#! /usr/bin/env python
# encoding: utf-8
# Guy Sherman, 2015

import subprocess
import waftools

LIBOCA_VERSION			=	'0.0.1'
LIBOCA_MAJOR_VERSION 	=	'0'


VERSION = LIBOCA_VERSION
APPNAME = 'liboca'

top = '.'
out = 'build'
libs = ['boost_system']

defines = ''


def options(opt):
    opt.load('compiler_cxx')
    opt.load('cppcheck', tooldir=waftools.location)
    opt.add_option("--shared", action="store_true", help="build shared library")
    opt.add_option("--static", action="store_true", help="build static library")


def configure(conf):
    conf.load('compiler_cxx')
    conf.load('cppcheck')
    conf.env.CXXFLAGS = ['-Wall', '-ansi', '-ggdb', '-Werror', '-pedantic-errors']
    conf.env.LINKFLAGS


def buildLibShared(bld):
    bld.shlib(source = bld.path.ant_glob('src/*.cxx'), target="oca", includes = ['./include'], install_path='${LIBDIR}', lib=libs, defines=defines)


def buildLibStatic(bld):
    bld.stlib(source = bld.path.ant_glob('src/*.cxx'), target="oca", includes = ['./include'], install_path='${LIBDIR}', lib=libs, defines=defines)


def build(bld):
    if bld.options.shared:
        buildLibShared(bld)
    elif bld.options.static:
        buildLibStatic(bld)
    else:
        buildLibShared(bld)

    bld.program(source = bld.path.ant_glob('examples/hello/*.cxx'),
                includes = ['./include'],
                lib = libs,
                target = 'examples/example-hello',
                install_path = '${BINDIR}',
                defines = defines)
    bld.program(source = bld.path.ant_glob('tests/**/*.cxx'),
                includes = ['./include', './src'],
                lib = ['gtest', 'gtest_main', 'pthread', 'boost_system'],
                target = 'tests/all',
                install_path = '${BINDIR}',
                defines = defines,
                use=['oca'])


def test(bld):
    subprocess.check_call("./run_tests.sh")
