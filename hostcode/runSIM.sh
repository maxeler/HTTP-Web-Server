#!/bin/bash

#source "/network-raid/opt/maxcompiler-2014.2/settings.sh"
source "/home/mvorkapic/workspace/MaxCompiler-2014.2-CompiledLocalTrunk/settings.sh"

export MAXELEROSDIR=$MAXCOMPILERDIR/lib/maxeleros-sim
export LD_PRELOAD=$MAXELEROSDIR/lib/libmaxeleros.so:$LD_PRELOAD
export SLIC_CONF="$SLIC_CONF;use_simulation=${USER}Sim"



# How to set a BASH variable equal to the output from a command?
# You need `, not '. This character is called "backticks" 
srcMax=`maxGuessBuildDir -s httpserver_ISCA_DFE_SIM`results/httpserver.max
srcH=`maxGuessBuildDir -s httpserver_ISCA_DFE_SIM`results/httpserver.h
echo "Copying .max and .h files to the /hostname directory"
echo $srcMax
echo $srcH
cp $srcMax /home/${USER}/workspace/workingNonMaxIDE/hostcode
cp $srcH /home/${USER}/workspace/workingNonMaxIDE/hostcode
echo "Finished copying"

# 1) run 'make_initCode.py' (python fabricate)
python make_initCode.py
# 2) run 'make_httpServerCPUcode.py' (python fabricate) 
python make_httpServerCPUcode.py
