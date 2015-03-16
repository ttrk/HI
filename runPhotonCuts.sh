#!/bin/sh

#http://stackoverflow.com/questions/3598664/creating-a-shell-script-to-run-java-program?rq=1

progName="photonCuts";
outFileDir="~/output/out_HI/16032015/";
outFileNamePrefix="photonCuts_16032015";
outFilePrefix=$outFileDir$outFileNamePrefix;

g++ $progName.C $(root-config --cflags --libs) -Wall -Wextra -O2 -o $progName.exe

# create the output directory if it does not exist
# create also any intermediate directories
mkdir -p $outFileDir

./$progName.exe $outFilePrefix
