#!/bin/sh

#http://stackoverflow.com/questions/3598664/creating-a-shell-script-to-run-java-program?rq=1

progName="eventMatcher";

g++ $progName.C $(root-config --cflags --libs) -Wall -Wextra -O2 -o $progName.exe

./$progName.exe
