#!/usr/bin/python

#  Script for easy generation of vector images from GNUPlot data
#  Meant to be used in conjunction with the code in src/color/SpecrumPlotter.h
#
#  If I actually knew how to use GNUPlot is suspect this script would not be necessary
#

import sys
import os
import os.path
import tempfile



#Constants
USE_WHITE_BG   = True
TEMP_FILE_NAME = "test/tmp/temp_file.txt"

#Check for OK arguments
if len(sys.argv) < 2:
    print 'Usage: ', sys.argv[0], ' gnuplot_file  [output_file]'
    print 'Output file can have extensions:'
    print '\t.svg (This is generally the best choice)'
    print '\t.eps'
    print '\t.pdf'
    print '\t.png'
    print '\t.tex'
    print '\t\tNo output_file argument defaults to showing the plot onscreen.'
    exit(0)

#Define a dict that allows us to map from file suffices->gnuplot terminal command text
GNUPLOT_TERM_MAP = {
    '.eps': ' postscript enhanced color eps',
    '.pdf': ' postscript enhanced color',
    '.svg': ' svg size 1000,1000 dynamic enhanced fname \'arial\' fsize 12 butt solid',
    '.png': ' png notransparent size 1000,1000',
    '.tex': ' latex',
    ''    : ' X11 persist'
}

#Parse args
gnuplotFile = sys.argv[1]

#Maybe we have no output
noOutput    = len(sys.argv) < 3
outputFile  = ''
if not noOutput:
    outputFile  = sys.argv[2]


#Figure out what kind of plot to make
gnuTermLine = GNUPLOT_TERM_MAP[''] #Default to no file output

#If we are outputting to a file find the right terminal line
if not noOutput:
    (base,suffix) = os.path.splitext(outputFile)
    if GNUPLOT_TERM_MAP[suffix] is None: #Check for a bad suffix
        print 'Warning - This script cant output: ', suffix, 'type files!'
        print 'You specified the output path: \'', outputFile, '\''
        print 'Defaulting to just displaying your plot onscreen...'
    else:
        gnuTermLine = GNUPLOT_TERM_MAP[suffix]
setupLine = 'set terminal ' + gnuTermLine
if not noOutput:
    outputLine = "set output \"" + outputFile + "\""
else:
    outputLine = ''
unsetLine  = "\nunset output\n"

#Read in the in file
fhandle = open(gnuplotFile, "r")
cmds    = fhandle.read()

#Make a temp file
#temp = tempfile.NamedTemporaryFile(prefix='gnuplot_temp_', suffix='.gnuplot', dir='/tmp', delete=True)
tempFile = open(TEMP_FILE_NAME, "w")
tempFile.write(setupLine + "\n")
tempFile.write(outputLine + "\n")
if USE_WHITE_BG:
    tempFile.write("set object 1 rect from screen 0, 0, 0 to screen 1, 1, 0 behind\n")
    tempFile.write("set object 1 rect fc  rgb \"white\"  fillstyle solid 1.0\n")
tempFile.write(cmds + "\n")
tempFile.write(unsetLine + "\n")
tempFile.close()

#Execute the GNUPlot command
os.system("gnuplot  " + TEMP_FILE_NAME)

