###############################################################################
# Author: Jack Schneiderhan and Cindy Zhang
# Date: February 16, 2021
# Pledge: I pledge my honor that I have abided by the Stevens Honor System.
# Description: Program to simulate a recycling bin.
###############################################################################
#!/bin/bash

readonly junkdir="/home/user/.junk"

#usage message
file="/home/user/junk.sh"
usage(){
	cat << ENDOFTEXT
Usage: $(basename "$file") [-hlp] [list of files]
   -h: Display help.
   -l: List junked files.
   -p: Purge all files.
   [list of files] with no other arguments to junk those files.
ENDOFTEXT
}

#checking the number of inputs
if [ $# -le 0 ] 
	then usage
	exit 0
fi
#checking the inputs
while getopts ":hlp" option; do
	if [ $# -gt 1 ]
		then printf "Error: Too many options enabled. \n"
		     usage
		exit 1
	fi
	case "$option" in
		h) usage
		   exit 0
		   ;;
		l) #printf "Listing all files!"
		   ls -lAF "$junkdir"
		   exit 0
		   ;;
		p) #printf "Purging all files!"
		   rm -rf $junkdir/{*,.*} &>/dev/null #removing all files including hidden
		   exit 0
		   ;;
		?) printf "Error: Unknown option '-%s'.\n" $OPTARG >&2
		   usage
		   exit 1
		   ;;
	esac
done

#checking if junk directory already exists
if [ ! -d "$junkdir" ]; then
	mkdir -p "$junkdir"
fi

#adding files to the directory
for f in "$@"; do
	if [ ! -f "$f" ]; then
		#checking for directories, to account for things such as .hideme
		if [ ! -d "$f" ]; then
			echo "Warning: '$f' not found."
		fi
		if [ -d "$f" ]; then
			#echo "Successfully moved '$f' to '$junkdir'."
			mv "$f" "$junkdir"
		fi
	fi
	if [ -f "$f" ]; then
		#echo "Successfully moved '$f' to '$junkdir'."
		mv "$f" "$junkdir"
	fi
done
exit 0

