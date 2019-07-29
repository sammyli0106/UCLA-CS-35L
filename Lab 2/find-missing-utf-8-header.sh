! /bin/bash

#internal field separator set to defined value
#split spaces between filename
OIFS="$IFS"
IFS=$'\n'

#handle the special character *
set -f

#loop throught one or more arguments
for arg1 in $@
do
    #handle the special char by inserting "./"
    #by inserting "./" in front of the
    #the passed argument
    if [[ ${arg1:0:1} != "." ]] ; then
        input="./$arg1"
    else
        input=$arg1
    fi

    #check if the input is a file
    if [[ -f $input ]] ; then
        #check if the input is a empty file
	if [[ -s $input ]] ; then
            #count the number of lines that contain nul byte char
	    nul_lines=$(sed -n "/\x00/p" $input | wc -l)
            #if the variable is zero mean not a nul byte file
	    if [ $nul_lines -eq 0 ] ; then
                #check for utf-8 chars
		#create variable to store the utf8 files
		utf8=$(grep -Pl "[^\x00-\x7f]" $input)
                   #show the comparison of the first line string
		   line=$(head -n 1 $utf8)
                    if [ $line != "-*- coding: utf-8 -*-" ] ; then
                       ls $utf8
                    fi
             fi
        fi
    fi

    #check if input is a directory
    if [[ -d $input ]] ; then
        #find every single file under the directory
	files=$(find $input -type f)
        #loop through all the founded files

        for arg2 in $files
        do
            #check for nul byte chars by checking
	    #number of lines that contain nul byte
	    nul_lines1=$(sed -n "/\x00/p" $arg2 | wc -l)
            #output equal zero mean not a nul byte file
	    if [ $nul_lines1 -eq 0 ] ; then
                # check for utf-8 files
		#create varaiable to store the utf8 files
		utf8list=$(grep -Pl "[^\x00-\x7f]" $arg2)
                for utf8files in $utf8list
                do
                    #compare the string in the first line
		    line2=$(head -n 1 $utf8files)
                    if [ $line2 != "-*- coding: utf-8 -*-" ] ; then
                       ls $utf8files
                    fi
                done
            fi
        done
     fi
done
