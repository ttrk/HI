#!/bin/sh

#http://stackoverflow.com/questions/3598664/creating-a-shell-script-to-run-java-program?rq=1
#http://git-scm.com/docs/gittutorial

#clone repository
git clone https://github.com/ttrk/HI.git
git remote add origin https://github.com/ttrk/HI.git   # the repository will be referred as "remote" from now on

#clone other repositories to the upper directory
(cd ..; git clone https://github.com/CmsHI/HiForestAnalysis.git)
(cd ..; git clone https://github.com/CmsHI/gammaJetAnalysis.git)
# how to run commands from another directory
# http://superuser.com/questions/271986/execute-a-command-from-another-directory-in-bash

# check what repositories are added as remote
git remote
git remote -v # show also the URL

# pull from remote if there is something new
git fetch origin
git pull

# check changes/commits etc. 
# basically report what happened
git status   #changes after the last commit
git log             #list of commits
git log -p          #list of commits with changes in code
git log --stat --summary 
git diff
git diff --cached

# put all recently created files into commit list
# new files are not added to commit automatically
git add .

# commit
# better do not execute this command from this script, do it on shell. Because this command requires additional input such as username and password.

# push changes to remote repo.
git push -u origin master   # push what my "master" branch here to the remote repo. that I called "origin"
# better do not execute this command from this script, do it on shell. Because this command requires additional input such as username and password.
