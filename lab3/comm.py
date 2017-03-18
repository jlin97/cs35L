#!/usr/bin/python

"""
Compare two sorted files line by line

Copyright 2016 Jonathan @ Nyan Tun.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Please see <http://www.gnu.org/licenses/> for a copy of the license.

"""

import sys, locale
from optparse import OptionParser

class comm:
    def __init__(self, filename):
        f = open(filename, 'r')
        self.lines = f.readlines()
        f.close()

def main():
    version_msg = "%prog 2.0"
    usage_msg = "Compare two files line-by-line."
    
    locale.setlocale(locale.LC_COLLATE, 'C')

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)

    parser.add_option('-1', action="store_true", dest="one", 
        default=False, help="suppress column 1 (lines unique to FILE1)")
    parser.add_option('-2', action="store_true", dest="two", 
        default=False, help="suppress column 2 (lines unique to FILE2)")
    parser.add_option('-3', action="store_true", dest="three", 
        default=False, help="suppress column 3 (lines common to both files)")
    parser.add_option('-u', action="store_true", dest="unsorted",
        default=False, help="enable comm for unsorted input")
    options, args = parser.parse_args(sys.argv[1:])

    if len(args) < 1:
        parser.error("missing operand")
    if len(args) == 1:
       parser.error("missing operand after '%s'" % str(sys.argv[1]))
    if len(args) > 2:
        parser.error("extra operand '%s'" % str(sys.argv[3]))

    if args[0] == '-':
        f2 = open (args[1], 'r')
        firstList = sys.stdin.readlines()
        secondList = f2.readlines()
        f2.close()
    elif args[1] == '-':
        f1 = open (args[0], 'r')
        firstList = f1.readlines()
        secondList = sys.stdin.readlines()
        f1.close()
    else:
        f1 = open (args[0], 'r')
        f2 = open (args[1], 'r')
        firstList = f1.readlines()
        secondList = f2.readlines()
        f1.close()
        f2.close()

    # adding items unique to file1 into list
    # adding common items into a list
    uni1List = []
    uni2List = []
    commonList = []
    for item in firstList:
        if item in secondList:
            commonList.append(item)
        else:
            uni1List.append(item)

    # adding items unique to file2 in a list
    for item in secondList:
        if item not in commonList:
            uni2List.append(item)

    # get union of sets and sort them
    union = list(set(firstList+secondList))
    
    union.sort()

    # case for when the output should be unsorted
    if options.unsorted:
        union = firstList
        for item in secondList:
            if item not in firstList:
                union.append(item)

    resultList = []

    # print out formatted list based on options chosen
    for item in union:
        if (options.one and options.two and options.three):
            pass
        elif (options.one and options.two):
            if item in commonList:
                resultList.append(item.strip())
        elif (options.two and options.three):
            if item in uni1List:
                resultList.append(item.strip())
        elif (options.one and options.three):
            if item in uni2List:
                resultList.append(item.strip())
        elif (options.one):
            if item in uni2List:
                resultList.append(item.strip())
            elif item in commonList:
                resultList.append("\t%s" % item.strip())
        elif (options.two):
            if item in uni1List:
                resultList.append(item.strip())
            elif item in commonList:
                resultList.append("\t%s" % item.strip())
        elif (options.three):
            if item in uni1List:
                resultList.append(item.strip())
            elif item in uni2List:
                resultList.append("\t%s" % item.strip())
        else:
            if item in uni1List:
                resultList.append(item.strip())
            elif item in uni2List:
                resultList.append("\t%s" % item.strip())
            elif item in commonList:
                resultList.append("\t\t%s" % item.strip())
            
    for item in resultList:
        print(item)


#    try:
 #       generator = randline(input_file)
  #      for index in range(numlines):
   #         sys.stdout.write(generator.chooseline())
    #except IOError as (errno, strerror):
     #   parser.error("I/O error({0}): {1}".
      #               format(errno, strerror))

if __name__ == "__main__":
    main()