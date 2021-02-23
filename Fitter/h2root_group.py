import os, sys
import glob

#inputFiles = glob.glob("../data_root_files/data_nue_ntuple_oct19/*/*.hbk")
inputFiles = glob.glob("../data_root_files/data_numu_ntuple_oct19/*/*.hbk")
for inputFile in inputFiles:
   outputFile = inputFile.replace(".hbk",".hbk.root")
   print "h2root", inputFile, outputFile
