import ROOT
import numpy as np 
import sys

class StackPlots(object):
   def __init__(self, file_name, bar="", i=-1, printFix=False, fix_last_bin=True):
      self.fix_last_bin    = fix_last_bin
      self.last_bin_length = 0.2
      self.NBKG            = 11
      self.bkgs            = ["pi+","pi-","k0l","k+","k-","mu+","mu-","Dirt","Pi0","Delta","unknown"]
      self.fillc           =[29,29,32,30,30,29,29,28,46,42,15]
      self.bar             = bar
      self.file_name       = file_name
      # set up preliminiary sign
      self.prelim = ROOT.TLatex()
      self.prelim.SetTextSize(0.04)
      self.prelim.SetTextColor(ROOT.kRed)
      if i!=-1: suffix= "_%i"%i
      else :suffix= ""
      

      self.f = ROOT.TFile(file_name)

      # Get histograms and matrices
      self.frac_error_matrix = self.f.Get("frac_error_matrix")
      self.data              = self.f.Get( "Nue%s Data%s"%(bar, suffix) )
      self.mc                = self.f.Get( "Nue%s MC constrained Null"%(bar) )
      self.signal            = self.f.Get( "Nue%s Signal%s"%(bar, suffix) )
      self.mcbg              = dict()
      for bkgName in self.bkgs:
         self.mcbg[bkgName]  = self.f.Get( "Nue%s MC %s"%(bar, bkgName)) 
         self.mcbg[bkgName].SetDirectory(0)



      self.data.SetDirectory(0)
      self.signal.SetDirectory(0)
      self.mc.SetDirectory(0)
      #self.f.Close()

      self.signal.Add(self.mc)

      self.mcbg["pi+"].Add( self.mcbg["pi-"] )
      self.mcbg["k+"].Add( self.mcbg["k-"] )
      self.mcbg["mu+"].Add( self.mcbg["mu-"] )

      for i in xrange(1, self.data.GetNbinsX()+1):
         self.data.SetBinError(    i, np.sqrt(self.data.GetBinContent(i))/self.data.GetBinWidth(i)/1000.)
         self.data.SetBinContent(  i, self.data.GetBinContent(i)/self.data.GetBinWidth(i)/1000.)
         self.signal.SetBinContent(i, self.signal.GetBinContent(i)/self.signal.GetBinWidth(i)/1000.)
         tempErrSq = pow(self.mc.GetBinError(i),2) - self.mc.GetBinContent(i)
         if tempErrSq<0: tempErrSq = 0.
         self.mc.SetBinError(i,np.sqrt( tempErrSq ) / self.mc.GetBinWidth(i) /1000. ) #mc considers only sys err, thus stat err removed.
         #self.mc.SetBinError(i, self.mc.GetBinError(i) / self.mc.GetBinWidth(i) /1000. )
         self.mc.SetBinContent(i,self.mc.GetBinContent(i)/self.mc.GetBinWidth(i)/1000.)
         for hist in self.mcbg.itervalues():
            hist.SetBinContent(i,hist.GetBinContent(i)/hist.GetBinWidth(i)/1000.)


      ###/*
      #for (Int_t i=1;i<hlsnd_lo.GetNbinsX()+1;i++){
      # hlsnd_lo.SetBinContent(i,hlsnd_lo.GetBinContent(i)/hlsnd_lo.GetBinWidth(i)/1000.)
      # hlsnd_hi.SetBinContent(i,hlsnd_hi.GetBinContent(i)/hlsnd_hi.GetBinWidth(i)/1000.)

      if self.fix_last_bin: self.FixLastBin(self.data,  self.last_bin_length, printFix)
      if self.fix_last_bin: self.FixLastBin(self.mc,    self.last_bin_length, printFix)
      if self.fix_last_bin: self.FixLastBin(self.signal,self.last_bin_length, printFix)
      #if self.fix_last_bin: self.FixLastBin(hlsnd_lo,self.last_bin_length, printFix)
      #if self.fix_last_bin: self.FixLastBin(hlsnd_hi,self.last_bin_length, printFix)





      for b, bgName in enumerate(self.bkgs):
         hist = self.mcbg[bgName]
         if self.fix_last_bin: self.FixLastBin(hist,self.last_bin_length, printFix)
         hist.SetFillColor(self.fillc[b])
         hist.SetLineColor(ROOT.kBlack)

      self.bgstack= ROOT.THStack("Background","")
      self.bgstack.Add(self.mcbg["unknown"])
      self.bgstack.Add(self.mcbg["Dirt"])
      self.bgstack.Add(self.mcbg["Delta"])
      self.bgstack.Add(self.mcbg["Pi0"])
      self.bgstack.Add(self.mcbg["k0l"])
      self.bgstack.Add(self.mcbg["k+"])
      self.bgstack.Add(self.mcbg["pi+"])

   def printBinContent(self, sumBinRange = [1,9]):
      # print each bin
      nbins = self.data.GetNbinsX()
      print "Within", self.data.GetName(), "we have", nbins, "bins"
      print "%15.3f"%self.data.GetXaxis().GetBinLowEdge(1),
      for i in xrange(1, nbins+1):
         print "%10.3f"%self.data.GetXaxis().GetBinUpEdge(i),
      print "%10.3f-%5.3f"%(self.data.GetXaxis().GetBinLowEdge(sumBinRange[0]), self.data.GetXaxis().GetBinUpEdge(sumBinRange[1]))
      print "--" * 80

      BGitems = ["pi+", "k+", "k0l", "Pi0", "Delta", "Dirt", "unknown"]
      predMC = [0.] * (nbins+1) # predicted MC

      for BGitem in BGitems:
         sumRange = 0
         print "%10s"%(BGitem),
         for i in xrange(1, nbins+1):
            print "%10.3f"%(self.mcbg[BGitem].GetBinContent(i)),
            if sumBinRange[0] <= i <= sumBinRange[1]:
               sumRange += self.mcbg[BGitem].GetBinContent(i) * (self.mcbg[BGitem].GetBinLowEdge(i+1) -self.mcbg[BGitem].GetBinLowEdge(i) ) * 1e3
            predMC[i] += self.mcbg[BGitem].GetBinContent(i)
         print "%20.2f"%(sumRange)
      print "--" * 80

      # print data
      sumRangeData = 0.
      print "%10s"%("data"),
      for i in xrange(1, nbins+1):  
         print "%10.3f"%(self.data.GetBinContent(i)),
         if sumBinRange[0] <= i <= sumBinRange[1]:
            sumRangeData += self.data.GetBinContent(i) * (self.data.GetBinLowEdge(i+1) -self.data.GetBinLowEdge(i) ) * 1e3
      print "%20.2f"%(sumRangeData)

      #
      # total BG
      #

      # Null Constraint BG/Excess
      sumRangeNullConstMC = 0.  # null & constraint
      sumRangePredMC = 0.       # predicted
      sumErrSyst = 0.
      print "%10s"%("BG CN"),
      for i in xrange(1, nbins+1):  
         print "%10.3f"%(self.mc.GetBinContent(i)),
         binWidthMeV = (self.data.GetBinLowEdge(i+1) -self.data.GetBinLowEdge(i) ) * 1e3
         if sumBinRange[0] <= i <= sumBinRange[1]:
            sumRangeNullConstMC += self.mc.GetBinContent(i) * (self.mc.GetBinLowEdge(i+1) -self.mc.GetBinLowEdge(i) ) * 1e3
            sumErrSyst          += np.power(self.mc.GetBinError(i) * binWidthMeV,2)
      print "%20.2f+/-%4.2f+/-%4.2f"%(sumRangeNullConstMC,  np.sqrt(sumRangeNullConstMC), np.sqrt(sumErrSyst) )

      print "%10s"%("excess CN"),
      for i in xrange(1, nbins+1):  
         print "%10.3f"%(self.data.GetBinContent(i)-self.mc.GetBinContent(i)),
      print "%20.2f"%(sumRangeData-sumRangeNullConstMC)

      # Predicted BG/Excess
      print "%10s"%("BG Pred"),
      for i in xrange(1, nbins+1):  
         print "%10.3f"%(predMC[i]),
         binWidthMeV = (self.data.GetBinLowEdge(i+1) -self.data.GetBinLowEdge(i) ) * 1e3
         if sumBinRange[0] <= i <= sumBinRange[1]:
            sumRangePredMC      += predMC[i] * binWidthMeV
      print "%20.2f"%(sumRangePredMC)

      print "%10s"%("excessPred"),
      for i in xrange(1, nbins+1):  
         print "%10.3f"%(self.data.GetBinContent(i)-predMC[i]),
      print "%20.2f"%(sumRangeData-sumRangePredMC)

      sumSignal = 0.
      print "%10s"%("signal"),
      for i in xrange(1, nbins+1):  
         print "%10.3f"%(self.signal.GetBinContent(i)-self.mc.GetBinContent(i)),
         temp = self.signal.GetBinContent(i) * (self.signal.GetBinLowEdge(i+1) -self.signal.GetBinLowEdge(i) ) * 1e3
         sumSignal += temp
      print "%20.2f"%(sumSignal-sumRangeNullConstMC)

      totalExcess = 0
      print "--" * 80

      print ""
      print "CN   = constrained Null"
      print "pred = original prediction"
      # Print 

   def printCombinedExcess(self, binRanges=[1,1]):
      # This function is to print the excess for the combined analysis with correlation considered
      # This code is limited to only one bin for nue and nue bar so far
      f           = ROOT.TFile(self.file_name)
      nueData     = f.Get("Nue Data")
      numuData    = f.Get("Numu Data")
      nueMC       = f.Get("Nue MC constrained Null")      # use constrained
      nuebarData  = f.Get("Nuebar Data")
      numubarData = f.Get("Numubar Data")
      nuebarMC    = f.Get("Nuebar MC constrained Null")
      covM        = f.Get("collapsed_error_matrix_constrained_Null")

      # examine
      assert nueData                !=None, "nue data is missing"
      assert numuData               !=None, "numu data is missing"
      assert nueMC                  !=None, "nue MC is missing"
      assert nuebarData             !=None, "nuebar data is missing"
      assert nuebarMC               !=None, "nuebar MC is missing"
      assert covM                   !=None, "collapsed_error_matrix_constrained_BF is missing"


      nNuBins    = nueData.GetNbinsX() + numuData.GetNbinsX()  # nuebar is at this bin!
      # calculate excess
      NnueMC        = nueMC.Integral(binRanges[0], binRanges[1])                     # nue nuebar only got one bin
      NnuebarMC     = nuebarMC.Integral(binRanges[0], binRanges[1])
      NnueData      = nueData.Integral(binRanges[0], binRanges[1])                     # nue nuebar only got one bin
      NnuebarData   = nuebarData.Integral(binRanges[0], binRanges[1])
      NnumuData     = numuData.Integral()                     # nue nuebar only got one bin
      NnumubarData  = numubarData.Integral()
      excessNue     = NnueData    - NnueMC 
      excessNuebar  = NnuebarData - NnuebarMC
      excessTotal   = excessNue + excessNuebar

      # getting total error
      totalErrNue, totalErrNuebar, offdiag = 0., 0., 0.
      print "NbinsX = ", covM.GetNcols()
      print "nNuBins = ", nNuBins
      for iBin in range(binRanges[0], binRanges[1]+1):
         for jBin in range(binRanges[0], binRanges[1]+1):
            print "totalErrNue    : %10.2f += %10.2f constrained_Null[%d][%d]"%(totalErrNue,covM(iBin-1, jBin-1), iBin-1, jBin-1 )
            print "totalErrNuebar : %10.2f += %10.2f constrained_Null[%d][%d]"%(totalErrNue,covM(nNuBins+iBin-1, nNuBins+jBin-1), nNuBins+iBin-1, nNuBins+jBin-1 )
            print "offdiag        : %10.2f += %10.2f constrained_Null[%d][%d]"%(totalErrNue,covM(iBin-1        , nNuBins+jBin-1 ), iBin-1        , nNuBins+jBin-1)
            totalErrNue    += covM(iBin-1        , jBin-1)
            totalErrNuebar += covM(nNuBins+iBin-1, nNuBins+jBin-1)
            offdiag        += covM(iBin-1        , nNuBins+jBin-1)

      totalErrNue   = np.sqrt(totalErrNue)
      totalErrNuebar= np.sqrt(totalErrNuebar)
      statErrNue    = np.sqrt(NnueMC)                          # statistic error is just from the MC n events
      statErrNuebar = np.sqrt(NnuebarMC)
      sysErrNue     = np.sqrt( pow(totalErrNue, 2) - NnueMC )  # rest is systematic error
      sysErrNuebar  = np.sqrt( pow(totalErrNuebar, 2) - NnuebarMC )      


      correlation   = offdiag / totalErrNue / totalErrNuebar
      #correlation   = covM(0,nNuBins) / np.sqrt(covM(0,0) * covM(nNuBins, nNuBins))
      #totalSysErr   = np.sqrt( sysErrNue*sysErrNue + sysErrNuebar*sysErrNuebar + 2.*correlation*sysErrNue*sysErrNuebar )
      totalSysErr   = np.sqrt( sysErrNue*sysErrNue + sysErrNuebar*sysErrNuebar + 2*offdiag )
      totalStatErr  = np.sqrt( statErrNue*statErrNue + statErrNuebar* statErrNuebar)
      totalErr      = np.sqrt( totalSysErr*totalSysErr + totalStatErr*totalStatErr)

      strFormat     = "%15s%15.2f%15.2f%15.3f"
      print ""
      print "Excess and error calcuated for nue/nuebar within %.0f - %.0f MeV"%(nueMC.GetBinLowEdge(binRanges[0])*1e3, nueMC.GetBinLowEdge(binRanges[1]+1)*1e3)
      print "%15s%15s%15s%15s"%("", "nue", "nuebar", "combined")
      print strFormat%("data", NnueData, NnuebarData, NnueData + NnuebarData)
      print strFormat%("dataMu", NnumuData, NnumubarData, NnumuData + NnumubarData)
      print strFormat%("MC_Null_Const", NnueMC, NnuebarMC, NnueMC + NnuebarMC)
      print strFormat%("Excess", excessNue, excessNuebar, excessNue + excessNuebar)
      print strFormat%("StatErr", statErrNue, statErrNuebar, totalStatErr)
      print strFormat%("SysErr",  sysErrNue, sysErrNuebar, totalSysErr)
      print strFormat%("TotalErr", totalErrNue, totalErrNuebar, totalErr)
      print strFormat%("sigmas", excessNue/totalErrNue, excessNuebar/totalErrNuebar, excessTotal / totalErr)
      print "correlation = ", correlation
      print "Note: TotalErr is calculated by the correlation between Nue total error and Nuebar total error, not the sys errors only."

      table = dict()
      table["data"]          = [ NnueData, NnuebarData, NnueData + NnuebarData ]
      table["dataMu"]        = [ NnumuData, NnumubarData, NnumuData + NnumubarData ]
      table["MC_Null_Const"] = [ NnueMC, NnuebarMC, NnueMC + NnuebarMC ]
      table["Excess"]        = [ excessNue, excessNuebar, excessNue + excessNuebar ]
      table["StatErr"]       = [ statErrNue, statErrNuebar, totalStatErr ]
      table["SysErr"]        = [  sysErrNue, sysErrNuebar, totalSysErr ]
      table["TotalErr"]      = [ totalErrNue, totalErrNuebar, totalErr ]
      f.Close()
      return table


   def Plot(self,out_stem, fileType="png", errorType = "stat"):
      # Note: errorType only applies to excess
      pcover = ROOT.TPave(0.8,0.06,0.92,0.0995,0,"NDC")
      pcover.SetFillColor(ROOT.kWhite)
      label = ROOT.TLatex()
      label.SetTextSize(self.data.GetXaxis().GetLabelSize())
      label.SetTextFont(self.data.GetXaxis().GetLabelFont())
      label.SetNDC(True)

      #------------------------------------------
      #stacked plot with best fit
      #------------------------------------------
      c1 = ROOT.TCanvas("c1","",800,600)
      self.data.SetStats(0)
      self.data.GetYaxis().SetNoExponent()
      self.data.SetMarkerStyle(20)
      self.data.SetMarkerSize(1.)
      self.data.SetTitle(";E_{#nu}^{QE} (GeV);Events/MeV")
      self.data.SetMinimum(0.)
      #self.data.SetMaximum(3.) #TEMP
      self.data.GetXaxis().SetTitleOffset(1.1)
      self.data.DrawCopy("PE1")
      self.bgstack.Draw("samehist")
      self.mc.SetMarkerStyle(1)
      self.mc.DrawCopy("samehist")
      self.mc.DrawCopy("samepe1")
      self.signal.SetLineStyle(2)
      self.signal.DrawCopy("samehist")
      self.data.DrawCopy("samePE1")
      leg = ROOT.TLegend(0.5,0.5,0.88,0.88)
      leg.SetFillStyle(0)
      leg.SetBorderSize(0)
      leg.AddEntry(self.data,"Data (stat err.)","P")
      leg.AddEntry(self.mcbg["pi+"],"#nu_{e} from #mu^{+/-}","F")
      leg.AddEntry(self.mcbg["k+"],"#nu_{e} from K^{+/-}","F")
      leg.AddEntry(self.mcbg["k0l"],"#nu_{e} from K^{0}","F")
      leg.AddEntry(self.mcbg["Pi0"],"#pi^{0} misid","F")
      leg.AddEntry(self.mcbg["Delta"],"#Delta #rightarrow N#gamma","F")
      leg.AddEntry(self.mcbg["Dirt"],"dirt","F")
      leg.AddEntry(self.mcbg["unknown"],"other","F")
      leg.AddEntry(self.mc,"Constr. Syst. Error","L")
      leg.AddEntry(self.signal,"Best Fit","L")
      leg.Draw()
      pcover.Draw()
      label.DrawText(0.88,0.067,"3.0")

      #prelim.DrawTextNDC(0.25,0.86,"PRELIMINARY")

      #------------------------------------------
      #stacked plot without best fit
      #------------------------------------------

      c2= ROOT.TCanvas("c2","",800,600)
      self.data.DrawCopy("PE1")
      self.bgstack.Draw("samehist")
      self.mc.DrawCopy("samehist")
      self.mc.DrawCopy("samepe1")
      self.data.DrawCopy("samePE1")
      legb= ROOT.TLegend(0.5,0.5,0.88,0.88)
      legb.SetFillStyle(0)
      legb.SetBorderSize(0)
      legb.AddEntry(self.data,"Data (stat err.)","P")
      legb.AddEntry(self.mcbg["pi+"],"#nu_{e} from #mu^{+/-}","F")
      legb.AddEntry(self.mcbg["k+"],"#nu_{e} from K^{+/-}","F")
      legb.AddEntry(self.mcbg["k0l"],"#nu_{e} from K^{0}","F")
      legb.AddEntry(self.mcbg["Pi0"],"#pi^{0} misid","F")
      legb.AddEntry(self.mcbg["Delta"],"#Delta #rightarrow N#gamma","F")
      legb.AddEntry(self.mcbg["Dirt"],"dirt","F")
      legb.AddEntry(self.mcbg["unknown"],"other","F")
      legb.AddEntry(self.mc,"Constr. Syst. Error","L")
      legb.Draw()
      pcover.Draw()
      label.DrawText(0.88,0.067,"3.0")
      #prelim.DrawTextNDC(0.25,0.86,"PRELIMINARY")

      #------------------------------------------
      #stacked plot self.mc only
      #------------------------------------------
      c3 = ROOT.TCanvas("c3","",800,600)
      c1.Update()
      self.mc.SetMaximum(c1.GetPad(0).GetUymax())

      self.mc.SetStats(0)
      self.mc.GetYaxis().SetNoExponent()
      self.mc.SetTitle(";E_{#nu}^{QE} (GeV);Events/MeV")
      self.mc.GetXaxis().SetTitleOffset(1.2)
      self.mc.DrawCopy("hist")
      self.bgstack.Draw("samehist")
      self.mc.DrawCopy("samepe1")
      legc= ROOT.TLegend(0.5,0.5,0.88,0.88)
      legc.SetFillStyle(0)
      legc.SetBorderSize(0)
      legc.AddEntry(self.mcbg["pi+"],"#nu_{e} from #mu^{+/-}","F")
      legc.AddEntry(self.mcbg["k+"],"#nu_{e} from K^{+/-}","F")
      legc.AddEntry(self.mcbg["k0l"],"#nu_{e} from K^{0}","F")
      legc.AddEntry(self.mcbg["Pi0"],"#pi^{0} misid","F")
      legc.AddEntry(self.mcbg["Delta"],"#Delta #rightarrow N#gamma","F")
      legc.AddEntry(self.mcbg["Dirt"],"dirt","F")
      legc.AddEntry(self.mcbg["unknown"],"other","F")
      legc.AddEntry(self.mc,"Constr. Syst. Error","L")
      legc.Draw()
      pcover.Draw()
      label.DrawText(0.88,0.067,"3.0")
      #prelim.DrawTextNDC(0.25,0.86,"PRELIMINARY")

      #------------------------------------------
      #excess with lsnd
      #root (v5_26_00) data hist has stat err, self.mc hist has sys err, when subtracting those added in quadrature
      c4 =  ROOT.TCanvas("c4","",800,600)
      #TEMP
      print "data bin 1 error", self.data.GetBinError(1)
      tmpErrors = [self.data.GetBinError(i) for i in range(self.data.GetNbinsX()+1)]
      self.data.Add(self.mc,-1)
      for i in range(1, self.data.GetNbinsX()+1): self.data.SetBinError(i, tmpErrors[i])

      if self.bar=="" :
       self.data.SetMinimum(-0.2)
       #self.data.SetMaximum(1.2) # TEMP
       #self.data.SetMaximum(0.9) # TEMP
      else:
       self.data.SetMinimum(-0.1)
       self.data.SetMaximum(0.45)
      
      self.signal.Add(self.mc,-1)
      self.data.GetYaxis().SetTitle("Excess Events/MeV")
      self.data.DrawCopy("PE1")
      self.signal.DrawCopy("samehist")
      line= ROOT.TLine(self.data.GetBinLowEdge(1),0.,self.data.GetBinLowEdge(self.data.GetNbinsX()+1),0.)
      line.Draw()
      pcover.Draw()
      label.DrawText(0.88,0.067,"3.0")

      #hlsnd_lo.SetLineColor(kMagenta)
      #hlsnd_hi.SetLineColor(kGreen+2)
      #hlsnd_lo.Draw("samehist")
      #hlsnd_hi.Draw("samehist")
      leg2= ROOT.TLegend(0.5,0.5,0.88,0.88)
      leg2.SetFillStyle(0)
      leg2.SetBorderSize(0)
      leg2.AddEntry(self.data,"Data - expected background","P")
      leg2.AddEntry(self.signal,"Best Fit","L")
      #leg2.AddEntry(hlsnd_hi,"sin^{2}2#theta=0.004, #Deltam^{2}=1.0eV^{2}","L")
      #leg2.AddEntry(hlsnd_lo,"sin^{2}2#theta=0.2, #Deltam^{2}=0.1eV^{2}","L")
      leg2.Draw()
      #prelim.DrawTextNDC(0.25,0.86,"PRELIMINARY")

      #------------------------------------------
      c1.Update()
      c2.Update()
      c3.Update()
      c4.Update()


      c1.SaveAs("%s_stacked_wbf.%s"   %(out_stem, fileType) )
      c2.SaveAs("%s_stacked_nobf.%s"  %(out_stem, fileType) )
      c3.SaveAs("%s_stacked_mconly.%s"%(out_stem, fileType) )
      c4.SaveAs("%s_excess.%s"        %(out_stem, fileType) )

   
      return [c1, c2, c3, c4]



   def FixLastBin(self, h,  bin_length, printFix=False):
      nbins=h.GetNbinsX()
      bins = [0.] * (nbins+2)
      cont = [0.] * (nbins+2)
      err = [0.] * (nbins+2)
      for i in range(nbins+2):
         cont[i] = h.GetBinContent(i)
         err[i] = h.GetBinError(i)
         bins[i]= h.GetBinLowEdge(i+1)

      bins[nbins] = bins[nbins-1]+bin_length
      if printFix:
         print h.GetName()
         for i in range(nbins+2):
            print "%2d %10.2f %10.2f %10.2f"%(i, bins[i], cont[i], err[i])
      #h.Reset();
      bins = np.array(bins, dtype = np.float)
      h.SetBins(nbins,bins)
      for i in range(nbins+1):
         h.SetBinContent(i,cont[i])
         h.SetBinError(i,err[i])

   def PrintBackground(self, bar=""):
      bgItems = dict()
      bgItems["numuCCQE"]  = ["mu+", "mu-"]        # ? numumubar CCQE
      bgItems["pi0"]       = ["Pi0"]               # NC pi0
      bgItems["delta"]     = ["Delta"]             # NC Delta -> N gamma 
      bgItems["Dirt"]      = ["Dirt"]              # Dirt
      bgItems["OtherNumu"] = ["unknown"]                    # ?
      bgItems["mu+-"]      = ["pi+", "pi-"]        # mu +/- decay
      bgItems["K+-"]       = ["k+", "k-"]          # K +/- decay
      bgItems["K0l"]       = ["k0l"]               # K0l
      bgItems["OtherNue"]  = []                    # ?

      keys = ["numuCCQE", "pi0", "delta", "Dirt", "OtherNumu", "mu+-", "K+-", "K0l", "OtherNue"]


      totalBG = 0.
      for key in self.f.GetListOfKeys():
         if "Nue%s MC"%(bar) in key.GetName():
            h = self.f.Get(key.GetName())
            print "%15s%10.1f"%(key.GetName(), h.GetBinContent(1))
            if ("Null" not in key.GetName()) and ("BF" not in key.GetName()) and ("Nue%s MC"%(bar)!=key.GetName()):
               totalBG += h.GetBinContent(1)

      if bar=="":
         totalBGErr = np.sqrt(totalBG * self.frac_error_matrix(1,1) * totalBG)
      elif bar=="bar":
         tempNbins = self.f.Get("Nue Signal").GetNbinsX() + self.f.Get("Nue MC").GetNbinsX() + self.f.Get("Numu MC").GetNbinsX()+self.f.Get("Nuebar Signal").GetNbinsX()
         print "Nbins = ", tempNbins
         totalBGErr = np.sqrt(totalBG * self.frac_error_matrix(tempNbins, tempNbins) * totalBG)

      print "Total unconstrained BG: %7.2f+/-%7.2f"%(totalBG, totalBGErr)

      print "\n"


      for key in keys:
         subBGNames = bgItems[key]
         totalBG = 0.
         errBGSq = 0.
         for subBGName in subBGNames:
            h = self.f.Get("Nue%s MC %s"%(bar,subBGName))
            if h.__bool__()==False:
               print "cannot find", subBGName
               continue
            else:
               totalBG += h.GetBinContent(1)
               errBGSq += h.GetBinError(1) * h.GetBinError(1)
         print "%15s%10.1f+/-%10.1f"%(key, totalBG, np.sqrt(errBGSq))






if __name__ == "__main__":
   # Apr18 results: prepared for publication and Neutrino 2018
   # excess
   #nue = StackPlots("/scratch/condor-tmp/echuang/Fitter/output/fit_apr18_excess.root", printFix= True, fix_last_bin=False,bar = "")
   #nue.printCombinedExcess()                 # 1bin
   #nue.PrintBackground()
   #nue.PrintBackground("bar")

   #numode histogram
   nue = StackPlots("/scratch/condor-tmp/hourlier/Fitter/output/fit_oct19_contNunubar_NueLastNotAsConstrain.root", printFix= True, fix_last_bin=True,bar = "")
   nue.printBinContent()
   nue.printCombinedExcess()   
   nue.Plot("figures/oct19/histNu", "pdf")

   #nubarmode histogram
   #nue = StackPlots("output/fit_apr18_histNubar.root", printFix= True, fix_last_bin=True,bar = "bar")
   #nue.printBinContent()
   #nue.Plot("figures/apr18/histNubar", "pdf")

   sys.exit()

   ##### Excess Published  #####
   # nu 6.46
   nue6_46 = StackPlots("/scratch/condor-tmp/echuang/Fitter/output/pub_nu6.46_1bin.root", printFix = True, fix_last_bin=False)
   nue6_38 = StackPlots("/scratch/condor-tmp/echuang/Fitter/output/fit_nu6.38_nubar11.27_1bin.root", printFix = True, fix_last_bin=False)
   nue6_38.printCombinedExcess()
   nue6_46.printCombinedExcess()
   #nue6_46.printBinContent(sumBinRange=[1,1])
   #nue6_46.PrintBackground("")
   sys.exit()
   # nubar 11.27
   #nuebarPub = StackPlots("output/pub_nu6.46_1bin.root", printFix = True, fix_last_bin=False, bar="bar")
   #nuebarPub.printBinContent(sumBinRange=[1,1])
   #nunubar 6.46 11.27
   #nunubarPub = StackPlots("output/pub_nunubar6.46_11.27_1bin.root", printFix = True, fix_last_bin=False, bar = "bar")
   #nunubarPub.printBinContent(sumBinRange=[1,1])
   #nunubarPub.printCombinedExcess()
   #sys.exit()

   # nu 6.46
   #nue = StackPlots("output/pub_nunubar6.46_11.27_1bin_inc_matrices.root", printFix= True, fix_last_bin=False,bar = "")
   #nue = StackPlots("output/fit_nunubar_13.02_E1.02_11.27_1bin_OM1.0_numumubarC.root", printFix= True, fix_last_bin=False,bar = "")
   #nue = StackPlots("output/fit_nunubar_13.02_E1.02_11.27_1bin_OM2.015_numumubarC.root", printFix= True, fix_last_bin=False,bar = "")
   #nue = StackPlots("output/fit_nunubar_9.38_E1.02_1bin_numumubarC.root", printFix= True, fix_last_bin=False,bar = "")
   #nue = StackPlots("output/examine_histNu.root", printFix= True, fix_last_bin=True,bar = "")
   #nue.Plot("figures/reproduction/examine_histNu","png")
   #nue.printBinContent(sumBinRange=[1,1])
   #nue.printCombinedExcess()
   #sys.exit()
   # nubar 11.27
   #nuebarPub = StackPlots("output/pub_nubar11.27_200_1300.root", printFix= True, fix_last_bin=False, bar = "bar")
   #nuebarPub.Plot("figures/reproduction/pub_nubar11.27_200_1300","png")
   #nuebarPub.printBinContent()

   # 6.46 1 bin
   #nue6_46_play = StackPlots("output/fit_nu.root.play.xml_1bin.root", printFix = False, fix_last_bin=False)
   #nue6_46_play.printBinContent(sumBinRange=[1,1])

   # nu 12.84 contour
   #nue = StackPlots("output/fit_nu12.84_nubar11.27_Exp_contNunubar.root", printFix= True, fix_last_bin=True,bar = "bar")
   #nue.printBinContent()
   #nue.Plot("figures/nu_12.84/fit_nu12.84_nubar11.27_Exp_contNubar", "png")
   #sys.exit()
   
   # nu 12.84
   #nue = StackPlots("output/fit_nu12.84_nubar11.27_excess.root", printFix= True, fix_last_bin=False,bar = "")
   #nue.printBinContent()                     # 1bin
   #nue.printCombinedExcess([4,9])          
   #nue.printCombinedExcess()                 # 1bin
   #sys.exit()
   

   # nu 12.84
   nue = StackPlots("/scratch/condor-tmp/echuang/Fitter/output/fit_nu12.84_nubar11.27_excess.root", printFix= True, fix_last_bin=False,bar = "")
   #nue.printBinContent()                     # 1bin
   #nue.printCombinedExcess([4,9])          
   nue.printCombinedExcess()                 # 1bin
   sys.exit()

   nue = StackPlots("/scratch/condor-tmp/echuang/Fitter/output/fit_nu12.84_nubar11.27_histNu.root", printFix= True, fix_last_bin=True,bar = "")
   nue.printBinContent()
   #nue.printCombinedExcess([4,9])          
   #nue.printCombinedExcess()                 # 1bin
   nue.Plot("figures/nu_12.84/hist_fit_nu12.84_nubar11.27", "pdf")

   # ------------
   # nu 9.38 POT
   # ------------

   # histogram
   #nue9_38 = StackPlots("output/fit_nunubar_9.38_E1.02_200_1300_numumubarC.root", printFix = False, fix_last_bin=True)
   #nue9_38.printBinContent(sumBinRange=[1,9])
   #nue9_38.Plot("figures/nu_9.38/fit200_1300_numumubarC", "png")
   #sys.exit()
   # excess
   #inputFile = "output/fit_nu_9.38_E1.02_1bin_numumubarC.root" #  constrained by both numu and numubar, but fit nue only
   #inputFile = "output/fit_nunubar_9.38_E1.02_1bin_numumubarC.root" #  constrained by both numu and numubar, and fit both nue and nuebar
   #nue9_38 = StackPlots( inputFile, printFix = False, fix_last_bin=False, bar="bar")  
   #nue9_38.printBinContent(sumBinRange=[1,1])
   sys.exit()

   # ------------
   # nu + nubar
   # ------------
   
   # one bin
   #inputFile = "output/fit_nunubar.root.xml_1bin.root"
   inputFile = "output/fit_nunubar938_1127.root.xml_1bin.root"
   print "Neutrino Mode 9.38 POT"
   nueComb = StackPlots( inputFile, bar="", printFix = False, fix_last_bin = False)
   nueComb.printBinContent(sumBinRange=[1,1])
   print "Anti-Neutrino Mode 11.27 POT"
   nuebarComb = StackPlots(inputFile, bar="bar", printFix = False, fix_last_bin = False)
   nuebarComb.printBinContent(sumBinRange=[1,1])

