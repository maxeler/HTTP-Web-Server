import os
import sys
import subprocess
from fabricate import *

NAME="httpServerCPUcode" # name of the CPU code

IP1="169.254.10.4"
TAP1_IP="169.254.10.48"
TAP1_NETMASK="255.255.255.0"
INIT_TAP1="QSFP_BOT_10G_PORT1:" + TAP1_IP + ":" + TAP1_NETMASK

USER=os.environ['USER']
SIM_NAME=USER + "Sim"

MAXFILE="httpserver.max"

# We pick off some environment vars
MAXCOMPILERDIR = os.environ['MAXCOMPILERDIR']
#MAXMPTDIR = os.environ['MAXMPTDIR']
MAXOSDIR = os.environ['MAXELEROSDIR']
#JAVA_HOME = os.environ['JAVA_HOME']

USE_SLIC = True
SLICCOMPILE = MAXCOMPILERDIR + '/bin/sliccompile'

try:
    MAXCOMPILERDIR    
except NameError:
    print "Error: You must specify the path to the directory containing MaxCompiler in the variable MAXCOMPILERDIR.\n"    
    sys.exit(1)

#include $(MAXCOMPILERDIR)/lib/Makefile.include

#DFE_MODEL= run('perl','-n -e', 'print $1 if /ENGINE_PARAMETERS\(DFEModel, DFEMODEL, (.*?)\)/', MAXFILE)
DFE_MODEL=subprocess.Popen(['perl', '-n', '-e', 'print $1 if /ENGINE_PARAMETERS\(DFEModel, DFEMODEL, (.*?)\)/', MAXFILE], stdout=subprocess.PIPE).communicate()[0] #.communicate == get output
#DESIGN_NAME=run('grep', 'MAXFILE_BUILD_NAME', MAXFILE) '| awk -F \"','{print $2;}')

arg = 'MAXFILE_BUILD_NAME ' + MAXFILE + ' | awk -F \\" \'{print $2;}\''
print "arg: " + arg
		#DESIGN_NAME=subprocess.Popen(['grep', arg], stdout=subprocess.PIPE)
DESIGN_NAME="httpServer"
#grep MAXFILE_BUILD_NAME httpServer.max | awk -F \" '{print $2;}'


#for e in DFE_MODEL:
#	print e
print "DFE_MODEL: %s " % DFE_MODEL
#for e in DESIGN_NAME:
#	print e
print "DESIGN_NAME: " + DESIGN_NAME

#sys.exit(1)

#grep MAXFILE_BUILD_NAME httpServer.max | awk -F \" '{print $2;}'

#DFE_MODEL := $(shell perl -ne 'print $$1 if /ENGINE_PARAMETERS\(DFEModel, DFEMODEL, (.*?)\)/' $(MAXFILE))
#DESIGN_NAME := $(shell grep MAXFILE_BUILD_NAME $(MAXFILE) | awk -F \" '{print $$2}')


#perl -n -e 'print $1 if /ENGINE_PARAMETERS\(DFEModelo DFEMODEL, (.*?)\)/' "httpServer.max"

sources = ["%s.c" % NAME]

def get_maxcompiler_inc():
    """Return the includes to be used in the compilation."""
    if USE_SLIC:
        return ['-I.', '-I%s/include' % MAXOSDIR, '-I%s/include/slic' % MAXCOMPILERDIR]
    return ['-I.', '-I%s/include' % MAXOSDIR, '-I%s/include' % MAXCOMPILERDIR]

def get_maxcompiler_libs():
    """Return the libraries to be used in linking."""
    if USE_SLIC:
        return ['-L%s/lib' % MAXCOMPILERDIR, '-L%s/lib' % MAXOSDIR, '-lslic', '-lmaxeleros', '-lm', '-lpthread']
    return ['-L%s/lib' % MAXCOMPILERDIR, '-L%s/lib' % MAXOSDIR, '-lmaxcompilerrt', '-lmaxeleros', '-lm', '-lpthread']

def get_ld_libs():
    """Returns the libraries to be used for linking."""
    #return ['-L%s/lib' % MAXMPTDIR] + MAXMPTLIBS + SYSTEMLIBS + get_maxcompiler_libs()
    return get_maxcompiler_libs()

# We define our compiler flags
flags = ['-std=gnu99','-Wall', '-Werror','-ggdb', '-rdynamic', '-DDESIGN_NAME=%s' % DESIGN_NAME, get_maxcompiler_inc()]

def build():
    clean()
    compile()
    link()
    restart_sim()
    execute_CPUprogram()

def compile():
    print "\n*** Compiling *** \n"
    for source in sources:
        run('gcc', flags, '-c', source, '-o', source.replace('.c', '.o'))

def link():
    print "\n*** Linking *** \n"
    objects = [s.replace('.c','.o') for s in sources]
    run(SLICCOMPILE, MAXFILE, MAXFILE.replace('.max','.o'))
    run('gcc', objects, MAXFILE.replace('.max','.o'), get_ld_libs(), '-o', NAME)

def execute_CPUprogram():
    run('./%s' % NAME, '%s' % IP1,  '%s' % TAP1_NETMASK)

def restart_sim():
    subprocess.check_call(['maxcompilersim', '-n', SIM_NAME, '-c', DFE_MODEL, '-e', INIT_TAP1, 'restart'])

def clean():
    #autoclean()
    run('rm','-f', '*.o', '*.pcap', '*.dot', '*.png')
	


main()






















