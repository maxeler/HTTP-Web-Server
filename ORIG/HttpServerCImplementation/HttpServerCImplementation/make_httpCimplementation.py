import os
import sys
import subprocess
from fabricate import *

NAME="httpServerCImpl"

from fabricate import *

sources = ['main','HttpRequest', 'initCodeCImplementation']

# We define our compiler flags
flags = ['-g']

def build():
    clean()
    compile()
    link()
    execute_program()

def compile():
    for source in sources:
        run('gcc', flags, '-c', source + '.c')

def link():
    objects = [s+'.o' for s in sources]
    run('gcc', '-lm', objects,'-o', NAME)

def execute_program():
    run('./%s' % NAME)

def clean():
    autoclean()

main()
