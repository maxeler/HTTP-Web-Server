#!/bin/bash

source "/network-raid/opt/maxcompiler-2015.1-rc1/settings.sh"

#export MAXELEROSDIR=$MAXCOMPILERDIR/lib/maxeleros-sim
#export LD_PRELOAD=$MAXELEROSDIR/lib/libmaxeleros.so:$LD_PRELOAD
#export SLIC_CONF="$SLIC_CONF;use_simulation=${USER}Sim"

# 1) run 'make_initCode.py' (python fabricate)
python make_initCode.py
# 2) run 'make_httpServerCPUcode.py' (python fabricate) 
python make_httpServerCPUcodeDFE.py
