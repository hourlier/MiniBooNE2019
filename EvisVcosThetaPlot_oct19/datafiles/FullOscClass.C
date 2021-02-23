#define FullOscClass_cxx
#include "FullOscClass.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void FullOscClass::Loop2()
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

void FullOscClass::Loop()
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
    hEvisCosTheta       = new TH2D(Form("hEvisCosTheta_FullOsc_%.05f_%.05f",sin22theta,dm2),"",NbinsXpperp, EvisMin, Evismax,NbinsR,-1,1);
    hEvisCosTheta_09    = new TH2D(Form("hEvisCosTheta_09_FullOsc_%.05f_%.05f",sin22theta,dm2),"",NbinsXpperp/2, EvisMin, Evismax,10,0.9,1);
    hEvisSinTheta       = new TH2D(Form("hEvisSinTheta_FullOsc_%.05f_%.05f",sin22theta,dm2),";E_{vis} [MeV]; sin #theta",NbinsXpperp,EvisMin,Evismax,NbinsR,0,1);
    hEvisPperp          = new TH2D(Form("hEvisPperp_%.05f_%.05f",sin22theta,dm2),";E_{vis} [MeV]; p_{perp}",NbinsXpperp,EvisMin, Evismax,  Nbinspperp, pperpMin,pperpMax);
    hEnuqe              = new TH1D(Form("hEnuqe_FullOsc_%.05f_%.05f",sin22theta,dm2),"",sizeof(Ebins)/sizeof(Float_t) - 1,Ebins);
    hCosTheta = new TH1D(Form("hCosTheta_FullOsc_%.05f_%.05f",sin22theta,dm2),"",20,-1,1);
    hR                  = new TH1D(Form("hR_FullOsc_%.05f_%.05f",sin22theta,dm2),"",NbinsR, Rmin, Rmax);
    hMCEVNT_EVWT        = new TH1D(Form("hMCEVNT_EVWT_FullOsc_%.05f_%.05f",sin22theta,dm2),"",102,-1,101);

    hREvis              = new TH2D(Form("hREvis_fullOsc_%.05f_%.05f",sin22theta,dm2),"; Evis [MeV]; R [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,Rmin, Rmax);
    hXEvis              = new TH2D(Form("hXEvis_fullOsc_%.05f_%.05f",sin22theta,dm2),"; Evis [MeV]; X [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,-Rmax, Rmax);
    hYEvis              = new TH2D(Form("hYEvis_fullOsc_%.05f_%.05f",sin22theta,dm2),"; Evis [MeV]; Y [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,-Rmax, Rmax);
    hZEvis              = new TH2D(Form("hZEvis_fullOsc_%.05f_%.05f",sin22theta,dm2),"; Evis [MeV]; Z [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,-Rmax, Rmax);
    hRnormEvis          = new TH2D(Form("hRnormEvis_fullOsc_%.05f_%.05f",sin22theta,dm2),"; Evis [MeV]; R/(500 cm)",NbinsXpperp, EvisMin, Evismax,NbinsR,0, 1);

    hDistToMeanCerEvis  = new TH2D(Form("hDistToMeanCerEvis_fullOsc_%.05f_%.05f",sin22theta,dm2),"; Evis [MeV]; Dist to Mean Cer [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,50, 200);
    hFEvis              = new TH2D(Form("hFEvis_fullOsc_%.05f_%.05f",sin22theta,dm2),"; Evis [MeV]; F",NbinsXpperp, EvisMin, Evismax,NbinsR,0,10);
    hRendEvis           = new TH2D(Form("hRendEvis_fullOsc_%.05f_%.05f",sin22theta,dm2),"; Evis [MeV]; Rend [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,Rmin, Rmax);
    hUdotREvis          = new TH2D(Form("hUdotREvis_fullOsc_%.05f_%.05f",sin22theta,dm2),"; Evis [MeV]; U.R [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,-Rmax, Rmax);
    hX2Y2Evis           = new TH2D(Form("hX2Y2Evis_fullOsc_%.05f_%.05f",sin22theta,dm2),";Evis [MeV]; X^{2}+Y^{2}",NbinsXpperp, EvisMin, Evismax,NbinsR,0, Rmax*Rmax);
    hPperpEvis_Coherent = new TH2D(Form("hPperpEvis_Coherent_fullOsc_%.05f_%.05f",sin22theta,dm2),";E_{vis} [MeV]; p_{perp}",NbinsXpperp,EvisMin, Evismax,  Nbinspperp, pperpMin,pperpMax);

    ofstream fout("fulloscMC.txt");
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
       fChain->GetEntry(jentry);
       fout << el_EnuQE << " " << enugen*1000. << " " << nuleng*100000. << " " << wgt << std::endl;
       if(el_EnuQE < Enuqemin || el_EnuQE > Enuqemax)continue;
       if(el_EnuQE > 2000)el_EnuQE = 1900;
       if(_forward == 1 && el_UZ < 0.9)continue;
       if(_forward == -1 && el_UZ > 0)continue;
       if(!(backgroundType == 1 || backgroundType == 2 || backgroundType == 3))continue;
       wgt = wgt*sin22theta*pow(sin(1.267*dm2*nuleng/enugen),2);
       pperp = sqrt((el_E+me)-me*me)*sqrt(el_UX*el_UX+el_UY*el_UY);
       TVector3 dir(el_UX,el_UY,el_UZ);
       
       hEvisPperp->Fill(el_E,pperp,wgt);
       hEvisCosTheta->Fill(el_E,el_UZ,wgt);
       hEvisCosTheta_09->Fill(el_E,el_UZ,wgt);
       hEvisSinTheta->Fill(el_E,sin(dir.Theta()),wgt);
       hEnuqe->Fill(el_EnuQE/1000.,wgt);
       hR->Fill(el_R,wgt);
       hMCEVNT_EVWT->Fill(MCEVNT_EVWT,wgt);
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
