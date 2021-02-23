import math
import sys, os

#-- base input xml file
# will use this file to generate all the others by replacing FAKE_S2T and FAKE_DM2 (and few other keywords)

xmlfile_base = "/scratch/condor-tmp/hourlier/Fitter/xml/fake_oct19_contNunubar.xml"
outputdir    = "/scratch/condor-tmp/hourlier/Fitter/fake_data/fake_oct19_contNunubar"
executable   = "/scratch/condor-tmp/hourlier/Fitter/src/CombinedFitPlus"
ROOTFileName  = "/scratch/condor-tmp/hourlier/Fitter/output/fit_oct19_contNunubar.root"
total_pot    = 18.75e20       # only used when add fake data to true data
isLocal      = False
extraNo      = 3          # extra runs for 4 sigma


# nunubar
#xmlfile_base = "/scratch/condor-tmp/echuang/Fitter/xml/fake_nu_12.84_scaleE1.02.xml"
#outputdir    = "/scratch/condor-tmp/echuang/Fitter/fake_data/fake_nunubar_9.38_11.27_200"
#executable   = "/scratch/condor-tmp/echuang/Fitter/src/CombinedFitPlus"
#isLocal      = False
#total_pot    = 9.38e20

cmdJobSubmit = "/miniboone/app/app/Condor/condor/Linux64bit+2.6-2.12/bin/boone-jobsub"

LD_LIBRARY_PATH = os.path.expandvars("$LD_LIBRARY_PATH")

xmlfile_base = os.path.expanduser(xmlfile_base)
outputdir    = os.path.expanduser(outputdir)   
executable   = os.path.expanduser(executable)   

#-- number of fake experiment
n_fake_exp       = 40000
job_no           = 0
random_seed_init = 100000	#-- random number seed

if extraNo >= 10:
   n_fake_exp = 40000
   random_seed_init = 100000 + 10000 * extraNo
   import ROOT
   fROOT = ROOT.TFile(ROOTFileName)
   hChi2 = fROOT.Get("chi2")
   chi2Min = hChi2.GetMinimum()
   print "chi2Min = ", chi2Min

#-- grid definitions
grid_dm2_points = 190
grid_lo_dm2     = 0.01
grid_hi_dm2     = 100.

grid_s2t_points = 190
grid_lo_s2t     = 3.0e-4
grid_hi_s2t     = 1


add_fake_data_to_real_data = "false"

#-- definition of points to run
def run_this_point(s2t, dm2):
   keep = False
   if dm2>1 and s2t<0.015: keep = True
   elif dm2<1 and s2t<0.015: keep = True
   elif dm2<1 and s2t>0.015:
      if dm2<upbound(s2t): keep = True
   return keep

def run_this_point_as_extra(s2t, dm2):
   keep = True
   if (math.log10(s2t) + 1.893541 * math.log10(dm2)) < -4.0:
      keep = False
   else:
      xbin = hChi2.GetXaxis().FindBin(s2t)
      ybin = hChi2.GetYaxis().FindBin(dm2)
      chi2 = hChi2.GetBinContent(xbin, ybin)
      if dm2>1.2:
         if chi2< (chi2Min+11.):
            keep = False
         if chi2> (chi2Min+32):
            keep = False
      else:
         if chi2> (chi2Min+55):
            keep = False
         elif (math.log10(s2t) + 1.893541 * math.log10(dm2)) > -2.46355:
            keep = True
         else:
            if chi2<(chi2Min+11):
               keep = False
   return keep

def upbound(s2t):
   y1 = math.log10(1.)
   y2 = math.log10(0.1)
   x1 = math.log10(0.015)
   x2 = math.log10(1.)
   x  = math.log10(s2t)
   y = y1 + (x-x1) * (y2-y1) / (x2-x1)
   return math.pow(10,y)



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

#-- decide which dm2 and s2t pairs to run ----------------------------------------
dx_dm2 = ( math.log10(grid_hi_dm2) - math.log10(grid_lo_dm2) ) / ( grid_dm2_points - 1)
dx_s2t = ( math.log10(grid_hi_s2t) - math.log10(grid_lo_s2t) ) / ( grid_s2t_points - 1)
dm2s = range(grid_dm2_points)
s2ts = range(grid_s2t_points)
for i in range(grid_dm2_points):
   dm2s[i] = math.pow(10, math.log10(grid_lo_dm2) + i*dx_dm2)
for i in range(grid_s2t_points):
   s2ts[i] = math.pow(10, math.log10(grid_lo_s2t) + i*dx_s2t)

NJOBS = 0
ijob  = 0

fFakePoints = open( os.path.join(outputdir,"fake_points_%d.vec"%(extraNo)), "w")
wrapperfiles = []
for iDm2, dm2 in enumerate(dm2s):
   for iS2t, s2t in enumerate(s2ts):
      if run_this_point(s2t, dm2) == False: continue
      if ((iDm2+1)%5 == 0 and (iS2t+1)%5 == 0)==False and \
         ((iS2t+1)%5==0 and iDm2==0)==False and \
         ((iDm2+1)%5==0 and iS2t==0)==False: continue

      if extraNo>=10:
         if run_this_point_as_extra(s2t, dm2) == False:
            continue
      fFakePoints.write("%7.6f\t%7.6f\n"%(s2t, dm2))

      random_seed = job_no*random_seed_init + iS2t*grid_dm2_points + iDm2
      random_seed = 0

      #-- make input files for jobs -------------------------------------------------
      xmlfile = xmlfile_base.replace("base", "dm2_%7.6f_s2t_%7.6f_%d"%(dm2, s2t, extraNo))
      xmlfile_name = os.path.basename(xmlfile)
      xmlfile = os.path.join(dirXML, "gen_%d_%03d_%s"%(extraNo, job_no, xmlfile_name) )
      assert open(xmlfile_base), "ERROR: Can't open xmlfile_base"
      with open(xmlfile_base) as f:
         newlines = f.read()
         newlines = newlines.replace("IJOB", "%d_%03d"%(extraNo, job_no))
         newlines = newlines.replace("FAKE_DM2","%7.6f"%(dm2))
         newlines = newlines.replace("FAKE_S2T","%7.6f"%(s2t))
         newlines = newlines.replace("NO_EXPERIMENTS","%d"%(n_fake_exp))
         newlines = newlines.replace("RANDOM_SEED","%d"%(random_seed))
         newlines = newlines.replace("TOTAL_POT","%.4e"%(total_pot))
         newlines = newlines.replace("ADD_FAKE_DATA_TO_REAL_DATA","%s"%(add_fake_data_to_real_data))
         newlines = newlines.replace("FAKEDATA_OUTPUT_PATH", outputdir)
      with open(xmlfile, "w") as f:
         f.write(newlines)
      
      NJOBS  += 1
      job_no += 1 
      
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
         f.write("export scratch=/scratch/condor-tmp/echuang\n")
         f.write("cd $scratch/Fitter\n")
         f.write("%s %s \n"%(executable, xmlfile ))
      os.system("chmod 755 %s"%(wrapper_file_name))
      wrapperfiles.append(wrapper_file_name)
            


fFakePoints.close()
print "number of jobs:", NJOBS
if isLocal == True:
   import ROOT
   c1 = ROOT.TCanvas("c1","c1",500,500)
   gr = ROOT.TGraph(os.path.join(outputdir,"fake_points_%d.vec"%(extraNo)))
   gr.Draw("AP")
   ROOT.gPad.SetLogy()
   ROOT.gPad.SetLogx()
   c1.SaveAs(os.path.join(outputdir, "fake_points.png"))

else: # is server
   commandFileName = "jobSubmit"
   with open(commandFileName, "w") as f:
      for wrapperfile in wrapperfiles:
         f.write("%s -M -g all %s\n"%(cmdJobSubmit, wrapperfile))
