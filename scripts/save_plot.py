#!/usr/bin/python

#  Script for easy generation of vector images from GNUPlot data
#  Meant to be used in conjunction with the code in src/color/SpecrumPlotter.h
#
#  If I actually knew how to use GNUPlot is suspect this script would not be necessary
#

import sys
import os
import tempfile


#Constants
USE_WHITE_BG   = False
TEMP_FILE_NAME = "temp_file.txt"
DISPLAY_X11    = False  #If this is on we just show the plot

#Check for OK arguments
if len(sys.argv) < 3:
    print 'Usage: ', sys.argv[0], ' gnuplot_file  output_file'
    exit(0)

#Parse args
gnuplotFile = sys.argv[1]
outputFile  = sys.argv[2]

#Read in the in file
fhandle = open(gnuplotFile, "r")
cmds    = fhandle.read()

if DISPLAY_X11:
    setupLine  = "set terminal X11 persist"
else:
    #setupLine  = "set terminal svg size 1000,1000 dynamic enhanced fname 'arial' fsize 12 butt solid"
    setupLine  = "set terminal postscript enhanced  color"
    #setupLine  = "set terminal canvas"
outputLine = "set output \"" + outputFile + "\""


#Make a temp file
#temp = tempfile.NamedTemporaryFile(prefix='gnuplot_temp_', suffix='.gnuplot', dir='/tmp', delete=True)
tempFile = open(TEMP_FILE_NAME, "w")
tempFile.write(setupLine + "\n")
tempFile.write(outputLine + "\n")
if USE_WHITE_BG:
    tempFile.write("set object 1 rect from screen 0, 0, 0 to screen 1, 1, 0 behind\n")
    tempFile.write("set object 1 rect fc  rgb \"white\"  fillstyle solid 1.0\n")
tempFile.write(cmds + "\n")

#Execute the GNUPlot command
os.system("gnuplot  " + TEMP_FILE_NAME)

