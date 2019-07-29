#!/usr/bin/python

import random, sys
import argparse
from optparse import OptionParser

class shuf:
    def __init__(self, input_file, inputRange, count, repeat):
        line1 = ""
        #handle zero non option, single non option and filename
        #below is no option i, want to go inside
        if inputRange == "":
           if input_file  == "-" or input_file == "No input file specify":
              #read from standard input enter line by line
              #handle one specific case  
              if count is 0 and repeat is False:
                  exit()
              self.lines = sys.stdin.readlines()
              line1 = self.lines
           elif input_file != "No input file specify":
              #read as filename
              f = open(input_file, 'r')
              #check if the file is empty
              self.lines = f.readlines()
              if len(self.lines) > 0:
                  #self.lines = f.readlines()
                  line1 = self.lines
              elif len(self.lines) == 0 and repeat == True:
                  #when empty files with option n and r
                  #display message
                  #empty files with option n, display nothing
                  sys.exit('shuf: no lines to repeat')
              f.close()
              
        #create a list to store the string input read from the
        #file and standard input
        
        string_list = []
        
        if inputRange != "":
           #this is where option i presents 
           #break apart the inputRange input first
           #not yet handle the weird input for the range
           number = [int(x) for x in inputRange.split("-")]
           num_list = [number[0], number[1]]
           
           ran_list = list(range(num_list[0], num_list[1] + 1))
           
           if repeat == True and count is not None:
               #option i, option n, option r
               #append the same list to the original list
               #use random choice and dont use shuffle
               for i in range(0, count):
                   print(random.choice(ran_list))
           elif count is not None:
               #option i with option n
               #create a new list that store up the number of count
               #dont allow for repeat, use shuffle??
               if (len(ran_list) < count):
                   count = len(ran_list)
               random.shuffle(ran_list)
               ran_list2 = ran_list[:count]
               for i in ran_list2:
                  print (i)
           elif repeat == True and count is None:
               #option i with option r, repeat forever
               while True:
                   for i in ran_list:
                       print (i)
           else:
                #normal version of using option i
                random.shuffle(ran_list)
                for i in ran_list:
                    print (i)
        elif inputRange == "":
           #option i is not present here
           for i in range(len(line1)):
               string_list.append(line1[i].rstrip('\n'))
           #need to worry about the standard output and filename
           #maybe use the chooseline method, try it tomorrow
           #add the rest of the options also
           if repeat == True and count is not None:
               #self.lines stores line by line from
               #standard output or from the filename
               #use choose line and allow for repeats
               if len(string_list) == 0:
                   sys.exit('shuf: no lines to repeat')
               for i in range(0, count):
                   print(random.choice(string_list))
           elif count is not None:
               # count need to be the length of input list
               if (len(string_list) < count):
                   count = len(string_list)
               random.shuffle(string_list)
               ran_list6 = string_list[:count]
               for i in ran_list6:
                   print (i)
           elif repeat == True and count is None:
               #no option i, just option r, run input forever
               if len(line1) > 0:
                   while True:
                       for i in string_list:
                           print (i)
               elif len(line1) == 0:
                   sys.exit('shuf: no lines to repeat')
                          
           else:
                #using shuf command alone
                random.shuffle(string_list)
                count = len(string_list)
                ran_list7 = string_list[:count]
                for i in ran_list7:
                    print (i)
def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE

     %prog -i LO-HI [OPTION]...
     %prog -n COUNT FILE
     %prog -n COUNT ... STD INPUT
     %prog -n COUNT -r ... FILE
     %prog -n COUNT -r ... STD INPUT
Output randomly selected lines from FILE.

With no FILE, or when FILE is -, read standard input."""

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)

    parser.add_option("-i", "--input-range",
                        action="store", dest="inputRange", default="",
                        help="treat each number LO through HI as an input line")

    parser.add_option("-n", "--head-count",
                       action="store", dest="count",
                       help="output at most COUNT lines")

    parser.add_option("-r", "--repeat",
                       action="store_true", dest="repeat", default=False,
                       help="output lines can be repeated")
    
    options, args = parser.parse_args(sys.argv[1:])
    
    try:
        inputRange = str(options.inputRange)
    except:
        parser.error("invalid input-range: '{0}'".format(options.inputRange))
        
    count = options.count
    if count is not None:
        try:
            count = int(options.count)
        except:
            sys.exit("{0}: invalid line count: '{1}'".
                     format(sys.argv[0], options.count))
        if count < 0:
            sys.exit("{0}: invalid line count: '{1}'".
                     format(sys.argv[0], options.count))
    try:
        repeat = bool(options.repeat)
    except:
        parser.error("invalid repeat: {0}".
                     format(options.repeat))
        
    if inputRange != "":
       try:
           number = [int(x) for x in inputRange.split("-")]
           #check the len of the argument, to solve 1-5-6 case
           #check the check the low and high, to solve the 9-6 case
           
           if number[0] > number[1] or len(number) > 2:
               sys.exit("{0}: invalid input range: '{0}'"
                        .format(options.inputRange))
       except:
           sys.exit("{0}: invalid input range: '{1}'"
                    .format(sys.argv[0], options.inputRange))
           
    #check if the input is filename for option i    
    if inputRange.find(".") == True:
        parser.error("invalid input range: '{0}'".
                     format(inputRange))
    #check if both input ranges and filename exists at the same time    
    if inputRange != "" and len(args) >= 1:
        sys.exit("{0}: extra operand: '{1}'".
                 format(sys.argv[0], args[0]))

    #possible to not have a filename
    try:
       input_file = args[0]
    except:
        input_file = "No input file specify"

    try:
        generator = shuf(input_file, inputRange, count, repeat)
    except IOError as err:
        exit("I/O error({0}): {1}".
                     format(errno, strerror))
if __name__ == "__main__":
    main()
