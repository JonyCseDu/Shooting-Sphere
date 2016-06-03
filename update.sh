#!/bin/bash
git init
echo "INITIATING PROJECT -- setting remote's origin"
git remote add origin https://github.com/JonyCseDu/Shooting-Sphere.git

echo "POSSIBLE COMMAND : exit, push, pull"
com="command"

while [ "$com" != "exit" ]
do
	read -p "Command : " com
 	clear

	if [ $com == "push" ]
	then
		echo "PULLING DATA FROM REMOTE"
		git pull https://github.com/JonyCseDu/Shooting-Sphere.git
		echo "PUSHING ALL FILES TO REMOTE(MASTER)"
		git add -A
		read -p "Enter Commit Message: " msg
		git commit -m "$msg"
		git push https://github.com/JonyCseDu/Shooting-Sphere.git

	elif [ $com == "pull" ] 
	then
		echo "PULLING DATA FROM REMOTE"
		git pull https://github.com/JonyCseDu/Shooting-Sphere.git

	elif [ "$com" != "exit" ]
	then
		echo "wrong command... try again"
	fi
done
exit
exit
