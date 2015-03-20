#!/bin/sh

#http://stackoverflow.com/questions/3598664/creating-a-shell-script-to-run-java-program?rq=1

progName="photonCuts";
outFileDir="/net/hisrv0001/home/tatar/output/out_HI/20032015/";
outFileNamePrefix="photonCuts_20032015";
outFilePrefix=$outFileDir$outFileNamePrefix;

g++ $progName.C $(root-config --cflags --libs) -Wall -Wextra -O2 -o $progName.exe

# create the output directory if it does not exist
# create also any intermediate directories
mkdir -p $outFileDir

./$progName.exe $outFilePrefix
