from fabricate import *

sources = ['init_code']

# We define our compiler flags
flags = ['-g']

def build():
    clean()
    compile()
    link()
    execute_program()

def compile():
    for source in sources:
        run('gcc', flags, '-c', source+'.c')

def link():
    objects = [s+'.o' for s in sources]
    run('gcc', '-lm', objects,'-o', 'init_code')

def execute_program():
    run('./init_code')

def clean():
    autoclean()

main()
