#! /bin/bash

#internal field separator set to defined value
#split spaces between filename

OIFS="$IFS"
IFS=$'\n'

#handle the special character *

set -f

#accept one or more arguments

for arg1 in $@
do
    #handle the special character filename and directory name
    #by inserting "./" in front of the
    #the pass in argument

    if [[ ${arg1:0:1} != "." ]] ; then
        input="./$arg1"
      else
        input=$arg1
    fi

    #check if the input is a file

    if [[ -f $input ]] ; then
        #check if the input is an empty file
        if [[ -s $input ]] ; then
            #count the number of lines that contain nul byte characters
            nul_lines=$(sed -n "/\x00/p" $input | wc -l)
            #if the variable is zero mean not a nul byte file
	    if [ $nul_lines -eq 0 ] ; then
                #search for ascii text files
		grep -PL "[^\x00-\x7F]" $input
            fi
        fi
    fi
    #check if the input is a directory
    if [[ -d $input ]] ; then
        #find every single file under the directory
	#include the ones in subdirectory also
	files=$(find $input -type f)
        #loop through all the founded files
	for arg2 in $files
        do
	  #check if it is empty files
	  if [[ -s $arg2 ]] ; then
               #count the number of lines that contain nul byte characters
	       nul_lines1=$(sed -n "/\x00/p" $arg2 | wc -l)
                #if the variable is zero mean not a nul byte file
		if [ $nul_lines1 -eq 0 ] ; then
                 # search for ascii text files
		 grep -PL "[^\x00-\x7F]" $arg2
                fi
             fi
        done
     fi
done
