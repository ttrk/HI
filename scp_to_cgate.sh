#!/bin/sh

#http://stackoverflow.com/questions/3598664/creating-a-shell-script-to-run-java-program?rq=1

#http://linuxconfig.org/bash-scripting-tutorial#h4-passing-arguments-to-the-bash-script
sourceFile=$1;  # first argument of this script is the file to be transferred.
remoteMachine="tatar@cgate.mit.edu";
destinationInRemote="~/code";
destination=$remoteMachine":"$destinationInRemote;

scp $sourceFile $destination
