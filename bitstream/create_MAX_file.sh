#!/bin/bash


export MAXELEROSDIR=$MAXCOMPILERDIR/lib/maxeleros-sim
export LD_PRELOAD=$MAXELEROSDIR/lib/libmaxeleros.so:$LD_PRELOAD
export SLIC_CONF="$SLIC_CONF;use_simulation=${USER}Sim"

#source /network-raid/opt/maxcompiler-2014.1.1/settings.sh
#source "/network-raid/opt/maxcompiler-ticket7202-clang/settings.sh"
source /network-raid/opt/maxcompiler-2014.2/settings.sh
MAXAPPJCP=./bin maxJavaRun httpServer.httpServerManager
#MAXAPPJCP=./bin maxJavaRun httpServer.httpServerManager target=DFE_SIM
