import ROOT
ROOT.gROOT.SetBatch(True)
import numpy as np 
import os, sys
class myContour(object):
   def __init__(self, fileName, hist_name="chi2", is_sensitivity=False):
      self.fileName       = fileName
      self.hist_name      = hist_name
      self.is_sensitivity = is_sensitivity
      self.f              = ROOT.TFile(fileName)
      print "Drawing contour for histogram ", hist_name , " in file ", fileName
      self.SetStyle()

   def Draw(self, outputName="temp", logx=True, logy=True, titles=";sin^{2}2#theta;#Deltam^{2} (eV^{2})", 
            plotLSND=True, dof=2, LSNDPath ="./scripts/lsnd_data/"):
      #ROOT.ResetColorMap("set1", 9, 21)
      #ROOT.ResetColorMap("blues", 6, 30)
      self.LSNDPath = LSNDPath
      ncont = 8
      #levels_1dof = np.array([1.64, 9.00,  25.00])     # sensitivity - 1dof, 1 sided  (90%CL, 3sig, 5sig)
      levels_2dof = np.array([2.30 , 4.61 , 5.99 ,  9.21 , 11.83 , 19.334, 28.744, 40.087]) 
      levels_3dof = np.array([3.527, 6.251, 7.815, 11.345, 14.156, 22.061, 31.812, 43.452])
      cnames      = ["68%", "90%","95%","99%", "3#sigma", "4#sigma", "5#sigma", "6#sigma"]
      levels_col =[1, 21, 22, 23, 24, 25, 27, 28 ]
      #levels_col =[20, 21, 22, 23, 24]
      if self.is_sensitivity:
         levels = levels_1dof
      elif dof==2:
         levels = levels_2dof
      elif dof==3:
         levels = levels_3dof
      else:
         sys.exit("I don't know the degree of freedom %d"%(dof))

      chi2 = self.f.Get(self.hist_name).Clone()
      chi2.SetDirectory(0)
       
      c1 = ROOT.TCanvas("c1","Contours",610,0,500,650)
      c1.SetLogx(logx)
      c1.SetLogy(logy)
      ROOT.gStyle.SetOptStat(0)
      hr1 = chi2.Clone("hr1")
      hr1.Reset()
      hr1.SetTitle(titles)
      hr1.Draw()
      hr1.GetYaxis().SetTitleOffset(1.2)
      if(plotLSND): grsLSND = self.plotLSND(c1)
      
      leg = ROOT.TLegend(0.6,0.6,0.88,0.82)
      leg.SetFillStyle(0)
      leg.SetBorderSize(0)  
      leg.SetTextSize(0.03)  
      dchi2= chi2.Clone("dchi2")
      minchi2=chi2.GetMinimum()
      for bini in range(dchi2.GetNbinsX()):
         for binj in range(dchi2.GetNbinsY()):
            dchi2.SetBinContent(bini+1,binj+1,chi2.GetBinContent(bini+1,binj+1)-minchi2)

      grConts = range(ncont)
      for i in range(ncont):
         vg = self.GetContour(levels[i], dchi2)
         grConts[i] = range(vg.GetSize())
         for j in range(vg.GetSize()):
            grConts[i][j] = ROOT.TGraph(vg.At(j).Clone())
            grConts[i][j].SetLineColor(levels_col[i])
            grConts[i][j].SetLineWidth(2)
            c1.cd()
            grConts[i][j].Draw("C")
            if  i==0 and j==0: how="RECREATE"
            else:              how = "UPDATE"
            fout = ROOT.TFile("contours.root",how)
            grConts[i][j].Write("contour_%i_%i"%(i,j))
            fout.Close()
         leg.AddEntry(grConts[i][0],cnames[i],"L")

      ix = ROOT.Long(0)
      iy = ROOT.Long(0)
      iz = ROOT.Long(0)
      chi2.GetMinimumBin(ix,iy,iz)
      xx=chi2.GetXaxis().GetBinCenter(ix)
      yy=chi2.GetYaxis().GetBinCenter(iy)

      bf = ROOT.TGraph(1)

      bf.SetPoint(1,xx,yy)
      bf.SetMarkerStyle(20)
      bf.Draw("P")
      leg.Draw()

      leg2= ROOT.TLegend(0.13,0.15,0.57,0.30)
      leg2.SetFillStyle(0)
      leg2.SetBorderSize(0)
      leg2.SetTextSize(0.03)
      leg2.AddEntry(bf,"(sin^{2}2#theta, #Deltam^{2})_{BF} = (%5.4f,%4.3f eV^{2})"%(xx,yy),"")
      #leg2.AddEntry(bf,"#chi^{2}_{BF}   = %4.2f"%(chi2.GetMinimum()),"")
      #leg2.AddEntry(bf,"#chi^{2}_{Null} = %4.2f"%(chi2.GetBinContent(1,1)),"")
      leg2.Draw()
      if self.is_sensitivity==False:
         #leg2.Draw()
         print " Best fit point: "
         print " s2t = ", xx
         print " dm2 = ", yy
         print " chi2 = ",  chi2.GetMinimum()
         print " chi2(null) = ",  chi2.GetBinContent(1,1)

      c1.Print( "%s_simple_chi2_cut.png"%outputName)
      c1.Print( "%s_simple_chi2_cut.pdf"%outputName)

   def SetStyle(self):
      style_label_font=42
      ROOT.gStyle.SetLabelFont(style_label_font,"xyz")
      ROOT.gStyle.SetLabelSize(0.045,"xyz")
      ROOT.gStyle.SetLabelOffset(0.015,"xyz")
      ROOT.gStyle.SetStatFont(style_label_font)
      ROOT.gStyle.SetTitleFont(style_label_font,"xyz")   # axis titles
      ROOT.gStyle.SetTitleFont(style_label_font,"h")     # histogram title
      ROOT.gStyle.SetTitleSize(0.05,"xyz")               # axis titles
      ROOT.gStyle.SetTitleSize(0.05,"h")                 # histogram title
      ROOT.gStyle.SetTitleOffset(1.1,"x")
      ROOT.gStyle.SetTitleOffset(1.,"y")
      ROOT.gStyle.SetStripDecimals(False)               # if we have 1.5 do not set 1.0 . 1
      ROOT.gStyle.SetTitleX(0.12)                        # spot where histogram title goes
      ROOT.gStyle.SetTitleW(0.78)                        # width computed so that title is centered
      ROOT.TGaxis.SetMaxDigits(2)                        # restrict the number of digits in labels

      ROOT.gStyle.SetPadTopMargin(0.08)
      ROOT.gStyle.SetPadBottomMargin(0.12)
      ROOT.gStyle.SetPadLeftMargin(0.14)
      ROOT.gStyle.SetPadRightMargin(0.1)

   def GetContour(self, level, h):

      hist = h.Clone()
      hist.SetContour(1, np.array([level]))
      c = ROOT.TCanvas("c","",500,600)
      hist.Draw("CONT Z LIST")
      c.Update() # Needed to force the plotting and retrieve the contours in TGraphs

      conts = ROOT.gROOT.GetListOfSpecials().FindObject("contours")
      
      nGraphs    = 0
      TotalConts = 0

      if (conts == None):
         print "*** No Contours Were Extracted!"
         TotalConts = 0
         return None
      else:
         TotalConts = conts.GetSize()

      for i in range(TotalConts):
         contLevel = conts.At(i)
         nGraphs += contLevel.GetSize()
      nGraphs = 0
      return contLevel



   def plotLSND(self, c1):

      
      file_list = ["llreg_608_1.vec",
                   "llreg_608_2.vec",
                   "llreg_608_3.vec",
                   "llreg_607_1.vec",
                   "llreg_607_2.vec",
                   "llreg_607_3.vec",
                   "llreg_607_4.vec",
                   "llreg_607_5.vec",
                   "llreg_607_6.vec",
                   "llreg_607_7.vec",
                   "llreg_607_8.vec"]
      NDATAFILES = len(file_list)
      graph_color = [29, 29, 29, 26, 26, 26, 26, 26, 26, 26, 26]
      graph_color = [31, 31, 31, 32, 32, 32, 32, 32, 32, 32, 32]

      
      gr = range(len(file_list))
      for i, txtFile in enumerate(file_list):
         fin = open(os.path.join(self.LSNDPath, txtFile))
         lines = fin.readlines()
         x = np.array([float(line.rstrip()) for line in lines[2::4]])
         y = np.array([float(line.rstrip()) for line in lines[3::4]])
         nmin = min(len(x), len(y))
         gr[i] = ROOT.TGraph(nmin, x, y)
         gr[i].SetFillColor(graph_color[i])
         gr[i].Draw("LF")
      return gr

if __name__=="__main__":
   
   #cont = myContour("output/fit_nu12.84_nubar11.27_Exp_contNunubar.root")
   #cont.Draw("figures/nu_12.84/fit_nu12.84_nubar11.27_Exp_contNunubar", logx=False, logy=True, titles=";n;A")

   #cont = myContour("output/fit_nu12.84_nubar11.27_contNunubar.root")
   #cont.Draw("figures/nu_12.84/contNoFake_fit_nu12.84_nunubar")

   #cont = myContour("output/fit_nu12.84_nubar11.27_contNu.root")
   #cont.Draw("figures/nu_12.84/contNoFake_fit_nu12.84_nu")
   
   #cont = myContour("output/fit_apr18_contNunubar_LSND.root")
   #cont.Draw("figures/apr18/contNoFake_contNunubarLSND")

   #cont = myContour("output/fit_nu6.38_nubar11.27_contNu.root")
   #cont.Draw("figures/nu_12.84/cont_fit_nu6.38")
   
   #cont = myContour("output/verify_contNu.root")
   #cont.Draw("figures/nu_12.84/cont_verify")

   cont = myContour("output/fit_oct19_contNunubar_NueLastNotAsConstrain.root")
   cont.Draw("figures/oct19/contNoFake_contNunubar",plotLSND=False)



