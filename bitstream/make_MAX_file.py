import os
import sys
import subprocess
from fabricate import *

MAXCOMPILERDIR = os.environ['MAXCOMPILERDIR']

MAXCOMPILER = 'maxjc'
MAXDEBUG = 'source,lines,vars'
MAXSOURCE = '1.6'
MAXTARGET = '1.6'
MAXCLASSPATH = ['/home/mvorkapic/workspace/maxeda/bin/',
		'%s/lib/MaxCompiler.jar' % MAXCOMPILERDIR]

MAXDESTDIR = 'bin'
MAXSRCDIR = 'src'

JAVARTENV = '/network-raid/opt/jdk1.6.0_02/jre/bin/java'
JAVAMAXHEAPSIZE = 'mx12288m' #-Xmx<size> set maximum Java heap size
HTTPSERVERCLASSPATH = MAXCLASSPATH + ['bin']
#TARGET = 'DFE_SIM'
#MAXFILENAME = 'httpServer_SIM'
HTTPSERVERSTARTUPCLASS = 'httpServer.httpServerManager'


def build():
    clean()
    compile_java()
    execute_java()


# compile java
# maxjc -nowarn -source 1.6 -target 1.6 -classpath /home/mvorkapic/workspace/maxeda/bin/:/network-raid/opt/maxcompiler-ticket7202-clang/lib/MaxCompiler.jar -d bin src
def compile_java():
    print "\n*** Compile Java *** \n"
    run(MAXCOMPILER, '-nowarn', '-g:%s' % MAXDEBUG, '-source', '%s' % MAXSOURCE, 
        '-target', '%s' % MAXTARGET, '-classpath', '%s' % ':'.join(MAXCLASSPATH), 
        '-d', '%s' % MAXDESTDIR, MAXSRCDIR)

# run java (create .max file)
#java -Xmx12288m -classpath bin:/home/mvorkapic/workspace/maxeda/bin/:/network-raid/opt/maxcompiler-ticket7202-clang/lib/MaxCompiler.jar httpServer.httpServerManager
def execute_java():
    print "\n*** Execute Java *** \n"
    run(JAVARTENV, '-X%s' % JAVAMAXHEAPSIZE, '-classpath', '%s' % ':'.join(HTTPSERVERCLASSPATH), 
        HTTPSERVERSTARTUPCLASS)
#	'target=%s' % TARGET, 'maxFileName=%s' % MAXFILENAME

def clean():
    autoclean()

main()
