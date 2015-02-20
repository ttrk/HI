#!/bin/sh

#http://stackoverflow.com/questions/3598664/creating-a-shell-script-to-run-java-program?rq=1
#http://git-scm.com/docs/gittutorial

#clone repository
git clone https://github.com/ttrk/HI.git
git remote add origin https://github.com/ttrk/HI.git   # the repository will be referred as "remote" from now on

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

# put all recently created files into commit list
# new files are not added to commit automatically
git add .

# commit
# better do not execute this command from this script, do it on shell. Because this command requires additional input such as username and password.

# push changes to remote repo.
git push -u origin master   # push what my "master" branch here to the remote repo. that I called "origin"
# better do not execute this command from this script, do it on shell. Because this command requires additional input such as username and password.
