import math
import sys, os

#-- base input xml file
# will use this file to generate all the others by replacing FAKE_S2T and FAKE_DM2 (and few other keywords)

xmlfile_base = "/scratch/condor-tmp/hourlier/Fitter/xml/fake_oct19_contNunubar.xml"
outputdir    = "/scratch/condor-tmp/hourlier/Fitter/fake_data/fake_oct19_1pointNull_contNunubar"
ROOTFileName = "/scratch/condor-tmp/hourlier/Fitter/output/fit_oct19_contNunubar_NueLastNotAsConstrain.root"
executable   = "/scratch/condor-tmp/hourlier/Fitter/src/CombinedFitPlus"


total_pot    = 18.75e20       # only used when add fake data to true data
isLocal      = False
jobNumbers   = range(1,60) # job 1 to 60
n_fake_exp   = 10000
isNULL       = True


cmdJobSubmit = "/miniboone/app/app/Condor/condor/Linux64bit+2.6-2.12/bin/boone-jobsub"

LD_LIBRARY_PATH = os.path.expandvars("$LD_LIBRARY_PATH")

xmlfile_base = os.path.expanduser(xmlfile_base)
outputdir    = os.path.expanduser(outputdir)   
executable   = os.path.expanduser(executable)   

#-- number of fake experiment

# Get best fit value
import ROOT
from array import array
from ctypes import c_int
fROOT = ROOT.TFile(ROOTFileName)
TChi2 = fROOT.Get("chi2")
minBin = TChi2.GetMinimumBin()
binX, binY, binZ = c_int(0), c_int(0), c_int(0)
TChi2.GetBinXYZ(minBin, binX, binY, binZ)
if isNULL == True:
   sinSq2ThetaBF = 0.0003
   deltaMSqBF    = 0.01
else:
   sinSq2ThetaBF = TChi2.GetXaxis().GetBinCenter(binX.value)
   deltaMSqBF = TChi2.GetYaxis().GetBinCenter(binY.value)


# examine
if os.path.isfile(executable) == False:
   sys.exit("Error: executable %s not found"%(executable))
if os.path.isdir(outputdir) == False:
   print "mkdir", outputdir
   os.mkdir(outputdir)
dirXML = os.path.join(outputdir, "xml")
if os.path.isdir(dirXML) == False:
   print "mkdir", dirXML
   os.mkdir(dirXML)
dirLog = os.path.join(outputdir, "logs")
if os.path.isdir(dirLog) == False:
   print "mkdir", dirLog
   os.mkdir(dirLog)

# running each job
wrapperfiles = []
for job_no in jobNumbers:

   #-- make input files for jobs -------------------------------------------------
   xmlfile_name = os.path.basename(xmlfile_base)
   xmlfile = os.path.join(dirXML, "gen_%03d_%s"%(job_no, xmlfile_name) )
   assert open(xmlfile_base), "ERROR: Can't open xmlfile_base"
   with open(xmlfile_base) as f:
      newlines = f.read()
      newlines = newlines.replace("IJOB", "%03d"%( job_no))
      newlines = newlines.replace("NO_EXPERIMENTS","%d"%(n_fake_exp))
      newlines = newlines.replace("FAKEDATA_OUTPUT_PATH", outputdir)
      newlines = newlines.replace("DELTAMSQ_BF", "%.7f"%(deltaMSqBF))
      newlines = newlines.replace("SINSQ2THETA_BF", "%.7f"%(sinSq2ThetaBF))
   with open(xmlfile, "w") as f:
      f.write(newlines)
   
   # below is for server only
   if isLocal==True: continue

   wrapper_file_name = os.path.basename(xmlfile).replace(".xml","")
   wrapper_file_name = "%s/%s_wrapper.sh"%(outputdir, wrapper_file_name)
   with open(wrapper_file_name, "w") as f:
      f.write("#! /bin/bash\n")
      f.write("source /miniboone/app/app/Scripts/setup_miniboone.sh\n")
      f.write("export PRODUCTS=/grid/fermiapp/products/uboone:/grid/fermiapp/products/common/db:/grid/fermiapp/products/larsoft/:$PRODUCTS\n")
      #f.write("export LD_LIBRARY_PATH=%s\n"%(LD_LIBRARY_PATH))
      f.write("setup root v6_06_04a -qe10:prof\n")
      f.write("export scratch=/scratch/condor-tmp/hourlier\n")
      f.write("cd $scratch/Fitter\n")
      f.write("%s %s \n"%(executable, xmlfile ))
   os.system("chmod 755 %s"%(wrapper_file_name))
   wrapperfiles.append(wrapper_file_name)
         


if isLocal == True:
   print "byebye"

else: # is server
   commandFileName = "jobSubmit"
   with open(commandFileName, "w") as f:
      for wrapperfile in wrapperfiles:
         f.write("%s -M -g all %s\n"%(cmdJobSubmit, wrapperfile))
