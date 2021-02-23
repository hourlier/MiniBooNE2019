#define dirtMC_cxx
#include "dirtMC.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void dirtMC::Loop2()
{

    if (fChain == 0) return;

    Long64_t nentries = fChain->GetEntriesFast();


    for (Long64_t jentry=0; jentry<nentries;jentry++) {
        fChain->GetEntry(jentry);
        if(el_EnuQE < Enuqemin || el_EnuQE > Enuqemax)continue;
        if(el_EnuQE > 2000)el_EnuQE = 1900;
        if(_forward == 1 && el_UZ < 0.9)continue;
        if(_forward == -1 && el_UZ > 0)continue;
        
    }



}

void dirtMC::Loop()
{

    if (fChain == 0) return;

    Long64_t nentries = fChain->GetEntriesFast();

    hEvisCosTheta = new TH2D("hEvisCosTheta_dirt","",NbinsXpperp, EvisMin, Evismax,NbinsR,-1,1);
    hEvisCosTheta_09 = new TH2D("hEvisCosTheta_09_dirt","",NbinsXpperp/2, EvisMin, Evismax,10,0.9,1);
    hEvisSinTheta = new TH2D("hEvisSinTheta_dirt",";E_{vis} [MeV]; sin #theta",NbinsXpperp, EvisMin, Evismax,NbinsR,0,1);
    hEvisPperp = new TH2D("hEvisPperp_dirt",";E_{vis} [MeV]; p_{perp}",NbinsXpperp,EvisMin, Evismax,  Nbinspperp, pperpMin,pperpMax);

    hREvis = new TH2D("hREvis_dirt","; Evis [MeV]; R [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,Rmin, Rmax);
    hXEvis = new TH2D("hXEvis_dirt","; Evis [MeV]; X [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,-Rmax, Rmax);
    hYEvis = new TH2D("hYEvis_dirt","; Evis [MeV]; Y [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,-Rmax, Rmax);
    hZEvis = new TH2D("hZEvis_dirt","; Evis [MeV]; Z [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,-Rmax, Rmax);
    hRnormEvis = new TH2D("hRnormEvis_dirt","; Evis [MeV]; R/(500 cm)",NbinsXpperp, EvisMin, Evismax,NbinsR,0, 1);

    hDistToMeanCerEvis = new TH2D("hDistToMeanCerEvis_dirt","; Evis [MeV]; Dist to Mean center [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,50,200);
    hFEvis = new TH2D("hFEvis_dirt","; Evis [MeV]; F",NbinsXpperp, EvisMin, Evismax,NbinsR,0,10);
    hRendEvis = new TH2D("hRendEvis_dirt","; Evis [MeV]; Rend [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,Rmin, Rmax);
    hUdotREvis = new TH2D("hUdotREvis_dirt","; Evis [MeV]; U.R [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,-Rmax, Rmax);
    hX2Y2Evis  = new TH2D("hX2Y2Evis_dirt",";Evis [MeV]; X^{2}+Y^{2}",NbinsXpperp, EvisMin, Evismax,NbinsR,0, Rmax*Rmax);

    hPperpEvis_Coherent = new TH2D("hPperpEvis_Coherent_dirt",";E_{vis} [MeV]; p_{perp}",NbinsXpperp,EvisMin, Evismax,  Nbinspperp, pperpMin,pperpMax);

    //hEnuqe = new TH1D("hEnuqe_dirt","hEnuqe_FullOsc",NbinsEnuqe,Enuqemin,Enuqemax);
    hEnuqe = new TH1D("hEnuqe_dirt","hEnuqe_FullOsc",sizeof(Ebins)/sizeof(Float_t) - 1,Ebins);
    hCosTheta = new TH1D("hCosTheta_dirt","",20,-1,1);

    hR = new TH1D("hR_dirt","hR_dirt",NbinsR, Rmin, Rmax);
    for (Long64_t jentry=0; jentry<nentries;jentry++) {
        fChain->GetEntry(jentry);
        if(el_EnuQE < Enuqemin || el_EnuQE > Enuqemax)continue;
        if(el_EnuQE > 2000)el_EnuQE = 1900;
        if(_forward == 1 && el_UZ < 0.9)continue;
        if(_forward == -1 && el_UZ > 0)continue;
        pperp = sqrt((el_E+me)-me*me)*sqrt(el_UX*el_UX+el_UY*el_UY);
        TVector3 dir(el_UX,el_UY,el_UZ);
        
        hEvisPperp->Fill(el_E,pperp,wgt);
        hEvisCosTheta->Fill(el_E,el_UZ,wgt);
        hEvisCosTheta_09->Fill(el_E,el_UZ,wgt);
        hEvisSinTheta->Fill(el_E,sin(dir.Theta()),wgt);
        hEnuqe->Fill(el_EnuQE/1000.,wgt);
        hR->Fill(el_R,wgt);
        hRnormEvis->Fill(el_E,el_R/500.,wgt);
        hREvis->Fill(el_E,el_R,wgt);
        hXEvis->Fill(el_E,el_X,wgt);
        hYEvis->Fill(el_E,el_Y,wgt);
        hZEvis->Fill(el_E,el_Z,wgt);
        hDistToMeanCerEvis->Fill(el_E,el_distToMeanCer,wgt);
        hFEvis->Fill(el_E,el_F,wgt);
        hRendEvis->Fill(el_E,el_REnd,wgt);
        hUdotREvis->Fill(el_E,el_UdotR,wgt);
        hX2Y2Evis->Fill(el_E,el_X*el_X+el_Y*el_Y,wgt);
        hCosTheta->Fill(el_UZ,wgt);
        if(MCEVNT_EVWT == 96 || MCEVNT_EVWT == 97)hPperpEvis_Coherent->Fill(el_E,pperp,wgt);
    }

    hEvisCosTheta->Scale(ScaleFactor);
    hEvisCosTheta_09->Scale(ScaleFactor);
    hEvisSinTheta->Scale(ScaleFactor);
    hEvisPperp->Scale(ScaleFactor);
    hEnuqe->Scale(ScaleFactor);
    hR->Scale(ScaleFactor);
    hREvis->Scale(ScaleFactor);
    hXEvis->Scale(ScaleFactor);
    hYEvis->Scale(ScaleFactor);
    hZEvis->Scale(ScaleFactor);
    hDistToMeanCerEvis->Scale(ScaleFactor);
    hFEvis->Scale(ScaleFactor);
    hRendEvis->Scale(ScaleFactor);
    hUdotREvis->Scale(ScaleFactor);
    hX2Y2Evis->Scale(ScaleFactor);
    hPperpEvis_Coherent->Scale(ScaleFactor);
    hRnormEvis->Scale(ScaleFactor);
    hCosTheta->Scale(ScaleFactor);

}

