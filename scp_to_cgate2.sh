#!/bin/sh

#http://stackoverflow.com/questions/3598664/creating-a-shell-script-to-run-java-program?rq=1

sourceFiles="
*.C 
*.h 
*.cpp
testDir";         # do not copy all sort of files
remoteMachine="tatar@cgate.mit.edu";
destinationInRemote="~/code/HI";
destination=$remoteMachine":"$destinationInRemote;

scp -r $sourceFiles $destination
