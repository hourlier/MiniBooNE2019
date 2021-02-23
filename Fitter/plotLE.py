# This program is to plot L/E
#!/usr/bin/env python
import numpy as np 
import matplotlib.pyplot as plt 
import ROOT
from MakeStackPlot import StackPlots
import matplotlib as mpl
#import seaborn as sns
from matplotlib import rc 
from scipy.stats import chi2


rc('font',**{'family':'sans-serif','sans-serif':['Helvetica']})
rc('text', usetex=True)

mpl.style.use("classic")
fileNames      = ["output/fit_apr18_histNunubar.root", "output/fit_apr18_histNunubar.root"]
MCNames        = [ "input/nu12.84_nubar11.27/filtered_Neutrino_nueccqe_Likelihood_Final_Full_Dirtcut_0.hbk.root", 
						 "input/nu12.84_nubar11.27/filtered_AntiNeutrino_nueccqe_Likelihood_Final_Full_Dirtcut_0.hbk.root"]
POTMCs         = [ 314022 * 4.229e14, 3000 * 1.083e18]
POTDatas       = [12.84e20, 11.27e20]
MCfilters      = [11, 11]
#xShifts        = [0., 0.]
xShifts        = [-0.01, 0.01]
colors         = [plt.cm.Set1.colors[0], plt.cm.Set1.colors[1]]
bars           = ["", "bar"]
labels         = [r'$\nu$ mode: $12.84\times10^{20}$ POT', r'$\bar\nu$ mode: $11.27\times10^{20}$ POT']
#labels         = [r'MiniBooNE $\nu$ mode', r'MiniBooNE $\bar\nu$ mode']
NSets          = 2

band1sFileName = "fake_data/cont_fake_apr18_contNunubar_1s.txt"
points1s       = np.genfromtxt(band1sFileName, names = ["sinSq2Theta", "deltaMSq"])
Npoints        = len(points1s)

xLSND     = np.array([0.44843, 0.54933, 0.65022, 0.75112, 0.85000, 0.94918, 1.05007, 1.30045])
yLSNDErrN = np.array([ 0.000548326851443 , 0.000682855999425 , 0.000130886291572 , 0.000540014981888 , 
							  0.000880493786621 , 0.00142702486378 , -0.00166499061067 , -0.00297621368688  ])
yLSND     = np.array([ 0.00185267467753 , 0.00123205508409 , 0.00068008537624 , 0.00150111338006 , 
	                    0.00293999035413 , 0.00479086925737 , 0.00375835035043 , 0.00464392361289  ])
yLSNDErrP = np.array([ 0.00308837261803 , 0.00184990405435 , 0.00122928446091 , 0.00246221177823 , 
	                    0.00499958953731 , 0.00829201342213 , 0.00918169131153 , 0.0121954110271  ])


distance      = 520.      # 520 meters 
BFsinSq2Theta = 0.917743
BFdeltaMSq    = 0.041092
BFsinSq2Theta2 = 0.01
BFdeltaMSq2    = 0.4
LSNDsinSq2Theta = 0.003
LSNDdeltaMSq    = 1.2


fData        = range(NSets)
fMC          = range(NSets) # fullosc MC
TMC          = range(NSets) # fullosc MC
hData        = range(NSets)
hFullosc     = range(NSets)
hExcess      = range(NSets)
hMC          = range(NSets)
hAppProb     = range(NSets) # appearance probability of data
xLE          = range(NSets) # L/E values of each bin
yOscRatio    = range(NSets) # oscilation probability from data
yOscRatioErr = range(NSets)
xLEErr       = range(NSets)

hTemp     = ROOT.TH1D("htemp", "htemp", 500,0,5)
for iSet in range(NSets):
	# Get excess
	fData[iSet]  = ROOT.TFile(fileNames[iSet])
	hData[iSet]  = fData[iSet].Get("Nue%s Data"%(bars[iSet]) )
	hMC[iSet]    = fData[iSet].Get("Nue%s MC constrained Null"%(bars[iSet]))
	hData[iSet].SetDirectory(0)
	hMC[iSet].SetDirectory(0)
	for iBin in range(1, hData[iSet].GetNbinsX()+1):
		hData[iSet].SetBinError(iBin, np.sqrt(hData[iSet].GetBinContent(iBin)))

	# Get fullosc
	hFullosc[iSet] = hData[iSet].Clone("hFullosc%d"%(iSet))	# get histogram 
	hFullosc[iSet].SetName("hFullosc%d"%(iSet))
	hFullosc[iSet].SetTitle("Fullosc") 
	hFullosc[iSet].Reset()										# remove all values
	fMC[iSet] = ROOT.TFile(MCNames[iSet])
	TMC[iSet] = fMC[iSet].Get("h55")
	c1 = ROOT.TCanvas("c1", "c1", 800, 600)
	hFullosc[iSet].SetDirectory(TMC[iSet].GetDirectory())
	TMC[iSet].Draw("energy>>%s"%(hFullosc[iSet].GetName()), "(iflux==%d)*wgt"%MCfilters[iSet])
	hFullosc[iSet].SetDirectory(0)
	hFullosc[iSet].Scale(POTDatas[iSet] / POTMCs[iSet] )
	# get excess
	tmpErrors = [hData[iSet].GetBinError(i) for i in range(hData[iSet].GetNbinsX()+1)]
	hExcess[iSet] = hData[iSet].Clone("hExcess")
	hExcess[iSet].Add(hMC[iSet],-1)
	for i in range(1, hExcess[iSet].GetNbinsX()+1): 
		hExcess[iSet].SetBinError(i, tmpErrors[i])


	# get appearance osc probability nu
	hAppProb[iSet]     = hExcess[iSet].Clone("hAppProb%d"%(iSet))  # appearance oscillation probability of data
	xLE[iSet]          = np.zeros(hAppProb[iSet].GetNbinsX(), dtype=np.float) # LE value for neutrino
	xLEErr[iSet]       = np.zeros(hAppProb[iSet].GetNbinsX(), dtype=np.float) # LE value for neutrino
	yOscRatio[iSet]    = np.zeros(hAppProb[iSet].GetNbinsX(), dtype=np.float) # oscillation probability of neutrino
	yOscRatioErr[iSet] = np.zeros(hAppProb[iSet].GetNbinsX(), dtype=np.float) # oscillation probability error of neutrino

	for i in range(1, hAppProb[iSet].GetNbinsX() + 1):
		binError            = hAppProb[iSet].GetBinError(i)
		binValue            = hAppProb[iSet].GetBinContent(i)
		binValueFullosc     = hFullosc[iSet].GetBinContent(i)
		binValue            /= binValueFullosc
		binError            /= binValueFullosc
		hAppProb[iSet].SetBinContent(i, binValue)
		hAppProb[iSet].SetBinError(i, binError)
		yOscRatio[iSet][i-1]    = binValue
		yOscRatioErr[iSet][i-1] = binError   # Error only includes the data statistical error

		# get true L/E from Monte Carlo Truth center
		hTemp.Reset()
		hTemp.SetDirectory(TMC[iSet].GetDirectory())
		temp = TMC[iSet].Draw("nuleng/enugen>>htemp", 
			            "(iflux==%d && energy>=%.4f && energy<%.4f)*wgt"%(MCfilters[iSet], hAppProb[iSet].GetBinLowEdge(i), 
			            												  hAppProb[iSet].GetBinLowEdge(i+1)))
		print hTemp.GetMean(), hAppProb[iSet].GetBinCenter(i)
		#xLE[iSet][i-1]          =  hTemp.GetMean()
		xLE[iSet][i-1]          = 0.520/hAppProb[iSet].GetBinCenter(i)
		xLEErr[iSet][i-1]       =  hTemp.GetRMS()



def getOscProb(sinSq2Theta, deltaMSq, LEratio):
	return sinSq2Theta * np.power( np.sin(1.267 * deltaMSq * LEratio), 2)
# Plot the prediction
fig, ax = plt.subplots(figsize=(8,6))
#fig, ax = plt.subplots(figsize=(15,6))
xLEFunc  = np.linspace(0, 2.3, 100)
yPred = getOscProb(BFsinSq2Theta, BFdeltaMSq, xLEFunc)
yPred2 = getOscProb(BFsinSq2Theta2, BFdeltaMSq2, xLEFunc)
yPredLSND = getOscProb(LSNDsinSq2Theta, LSNDdeltaMSq, xLEFunc)
ax.plot(xLEFunc, yPred, "-", label = r'MiniBooNE best fit (%.3f, %.3f eV$^2$)'%(BFsinSq2Theta, BFdeltaMSq), color="k", linewidth=2, zorder=1)
#ax.plot(xLEFunc, yPredLSND, "--", label = r'LSND best fit (%.3f,%.1f eV$^2$)'%(LSNDsinSq2Theta, LSNDdeltaMSq), color = "0.5", linewidth=1, zorder=1)
ax.plot(xLEFunc, yPred2, "--", label = r'(%.2f, %.1f eV$^2$)'%(BFsinSq2Theta2, BFdeltaMSq2), color="k")

oscProb1s = []
for point in points1s:
	oscProb =getOscProb(point["sinSq2Theta"], point["deltaMSq"], xLEFunc)
	oscProb1s.append(oscProb)
oscProb1s = np.array(oscProb1s)
oscProb1sMax = oscProb1s.max(axis=0)
oscProb1sMin = oscProb1s.min(axis=0)
ax.fill_between(xLEFunc, oscProb1sMin, oscProb1sMax, facecolor='0.8', label=r'MiniBooNE 1$\sigma$ allowed band', lw=0)

markerStyles = ["o", "^", "s"]
for iSet in range(NSets):
	ax.errorbar(xLE[iSet] + xShifts[iSet], yOscRatio[iSet],yerr=yOscRatioErr[iSet], 
					fmt=markerStyles[iSet], label=labels[iSet],color=colors[iSet], elinewidth=2, zorder=2)
ax.errorbar(xLSND, yLSND, yerr=(yLSNDErrP - yLSNDErrN)/2., fmt="s", label="LSND", color=plt.cm.Set1.colors[2], elinewidth=2, zorder=2)
ax.legend(loc=2, numpoints=1, fontsize=16, framealpha=1, frameon=False)
#ax.legend(loc=2, numpoints=1, fontsize=20)
ax.set_xlabel("L/E [meters/MeV]")
ax.set_ylabel("Appearance Probability")

#TEMP
#plt.tick_params(axis='both', which='major', labelsize=20)
#TEMP

ax.axhline(y=0, color='0.5')
ax.set_xlim([0,2.3])
ax.set_ylim([-0.005,0.02])
fig.savefig("LoverE.png")
fig.savefig("LoverE.pdf")

print "\n\nLSND"
print xLSND
print yLSND
print (yLSNDErrP - yLSNDErrN)/2.
#
# calculate chiSq from error
#
print "\n\nchiSq"
for iSet in range(NSets):
	print labels[iSet]
	print "xLE", xLE[iSet]
	print "oscProb data", yOscRatio[iSet]
	print "oscProb pred", getOscProb(BFsinSq2Theta, BFdeltaMSq, xLE[iSet])
	print "oscProb err", yOscRatioErr[iSet]
	chiSq = np.sum(np.power( (yOscRatio[iSet]-getOscProb(BFsinSq2Theta, BFdeltaMSq, xLE[iSet])) / yOscRatioErr[iSet] , 2))
	print labels[iSet], chiSq, "prob=",1.-chi2.cdf(chiSq, len(xLE[iSet]))
	print "\n\n"
chiSq = np.sum(np.power( (yLSND-getOscProb(BFsinSq2Theta, BFdeltaMSq, xLSND)) / ((yLSNDErrP - yLSNDErrN)/2.) , 2))
print "LSND", chiSq, "prob=",1.-chi2.cdf(chiSq, len(xLSND))



# fit curve
def func(x, A, n):
	return A * np.power(x, n)
from scipy.optimize import curve_fit
popt, pcov = curve_fit(func, xLE[0], yOscRatio[0])
plt.plot(xLEFunc, func(xLEFunc, *popt), 'r-',label='fit: A=%5.3f, n=%5.3f' % tuple(popt))
ax.legend(loc=2, numpoints = 1)
fig.savefig("LoverEFit.png")


