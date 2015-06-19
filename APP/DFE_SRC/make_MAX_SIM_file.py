import os
import sys
import subprocess
from fabricate import *

MAXCOMPILERDIR = '/network-raid/opt/maxcompiler-2015.1-rc1/' # os.environ['MAXCOMPILERDIR']
CLASSPATH = os.environ['CLASSPATH']

MAXCOMPILER = 'maxjc'
MAXDEBUG = 'source,lines,vars'
MAXSOURCE = '1.6'
MAXTARGET = '1.6'
MAXCLASSPATH = ['%s/lib/MaxCompiler.jar' % MAXCOMPILERDIR]

MAXDESTDIR = 'bin'
MAXSRCDIR = 'HTTP-Web-Server'

# build directory - .max file location
os.environ["MAXCOMPILER_BUILD_CONF"] = "build.copy_results_to = /tmp/maxbuild/maxdc_builds/DFE_SIM/;"
result=os.environ.get('MAXCOMPILER_BUILD_CONF')

JAVARTENV = '/network-raid/opt/jdk1.6.0_02/jre/bin/java'
JAVAMAXHEAPSIZE = 'mx12288m' #-Xmx<size> set maximum Java heap size
HTTPSERVERCLASSPATH = ['./bin']  + [CLASSPATH]
# MAXFILENAME = 'httpServer_SIM'
DFEMODEL='ISCA' 
MAXFILENAME='httpserver'
TARGET = 'DFE_SIM'
HTTPSERVERSTARTUPCLASS = 'httpServer.HttpServerManager'


def build():
    clean()
    compile_java()
    execute_java_maxJavaRun()


# compile java
# command
def compile_java():
    print "\n*** Compile Java *** \n"
    run(MAXCOMPILER, '-nowarn', '-g:%s' % MAXDEBUG, '-source', '%s' % MAXSOURCE, 
        '-target', '%s' % MAXTARGET, '-classpath', '%s' % ':'.join(MAXCLASSPATH), 
        '-d', '%s' % MAXDESTDIR, MAXSRCDIR)

def execute_java_maxJavaRun():
    print "\n*** Execute Java *** \n"
    Temp = ' '.join(['CLASSPATH=%s' % ':'.join(HTTPSERVERCLASSPATH), 'maxJavaRun', HTTPSERVERSTARTUPCLASS, 'DFEModel=%s' % DFEMODEL, 'maxFileName=%s' % MAXFILENAME, 'target=%s' % TARGET])
    subprocess.check_call(Temp, shell=True)

def clean():
    autoclean()

main()
