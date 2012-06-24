#!/usr/bin/python

import os
import sys
import re
import datetime

#Paths that need to be changed if the directory structure changes
UNIT_TEST_PATH = "test/unit_tests"
OUT_FILE_PATH  = os.path.join(UNIT_TEST_PATH, 'tests_auto_generated.h')

#Automatically generate the file "src/tests/tests_auto_generated.h"

#Generate a list of test files
#They must have names like test.*.h
#and be files(as opposed to directories)
unitTests = [] #Resulting list
regex = re.compile('.*h')
for root, dirs, files in os.walk(UNIT_TEST_PATH):
    for f in files:
        g = os.path.join(UNIT_TEST_PATH, str(f))
        if os.path.isfile(str(g)) and regex.match( str(os.path.basename(str(g))) ) and str(os.path.basename(str(g))) != 'tests_auto_generated.h':
            unitTests.append(g)

#Write to the file
fHandle = open(OUT_FILE_PATH, 'w')
fHandle.write('//Autogenerated by ' + sys.argv[0] + ' at ' + str(datetime.datetime.now()) + os.linesep )
fHandle.write( os.linesep )
for test in unitTests:
    fHandle.write('#include "' + test + '"' + os.linesep)
fHandle.close()

print 'Unit tests:'
for test in unitTests:
    print '\t', test