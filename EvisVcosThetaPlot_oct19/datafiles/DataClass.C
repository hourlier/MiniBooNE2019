#define DataClass_cxx
#include "DataClass.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void DataClass::Loop2()
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

void DataClass::Loop()
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
    hEvisCosTheta = new TH2D("hEvisCosTheta_data",";E_{vis} [MeV]; cos #theta",NbinsXpperp, EvisMin, Evismax,NbinsR,-1,1);
    hEvisCosTheta_09 = new TH2D("hEvisCosTheta_09_data",";E_{vis} [MeV]; cos #theta",NbinsXpperp/2, EvisMin, Evismax,10,0.9,1);
    hEvisSinTheta = new TH2D("hEvisSinTheta_data","; E_{vis} [MeV]; sin #theta",NbinsXpperp,EvisMin,Evismax,NbinsR,0,1);
    hEvisPperp = new TH2D("hEvisPperp_data",";E_{vis} [MeV]; p_{perp}",NbinsXpperp,EvisMin, Evismax,  Nbinspperp, pperpMin,pperpMax);
    hEnuqe = new TH1D("hEnuqe_data","",sizeof(Ebins)/sizeof(Float_t) - 1,Ebins);
    hCosTheta = new TH1D("hCosTheta_data","",20,-1,1);
    hR = new TH1D("hR_data","hR_data",NbinsR, Rmin, Rmax);
    hREvis = new TH2D("hREvis","; Evis [MeV]; R [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,Rmin, Rmax);
    hXEvis = new TH2D("hXEvis","; Evis [MeV]; X [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,-Rmax, Rmax);
    hYEvis = new TH2D("hYEvis","; Evis [MeV]; Y [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,-Rmax, Rmax);
    hZEvis = new TH2D("hZEvis","; Evis [MeV]; Z [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,-Rmax, Rmax);
    hRnormEvis = new TH2D("hRnormEvis","; Evis [MeV]; R/(500 cm)",NbinsXpperp, EvisMin, Evismax,NbinsR,0, 1);

    hDistToMeanCerEvis = new TH2D("hDistToMeanCerEvis","; Evis [MeV]; Dist to Min Cer [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,50,200);
    hFEvis = new TH2D("hFEvis","; Evis [MeV]; F",NbinsXpperp, EvisMin, Evismax,NbinsR,0,10);
    hRendEvis = new TH2D("hRendEvis","; Evis [MeV]; Rend [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,Rmin, Rmax);
    hUdotREvis = new TH2D("hUdotREvis","; Evis [MeV]; U.R [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,-Rmax, Rmax);
    hX2Y2Evis  = new TH2D("hX2Y2Evis",";Evis [MeV]; X^{2}+Y^{2}",NbinsXpperp, EvisMin, Evismax,NbinsR,0, Rmax*Rmax);

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
       fChain->GetEntry(jentry);
       if(el_EnuQE < Enuqemin || el_EnuQE > Enuqemax)continue;
       if(el_EnuQE > 2000)el_EnuQE = 1900;
       if(_forward == 1 && el_UZ < 0.9)continue;
       if(_forward == -1 && el_UZ > 0)continue;
       pperp = sqrt((el_E+me)-me*me)*sqrt(el_UX*el_UX+el_UY*el_UY);
       TVector3 dir(el_UX,el_UY,el_UZ);
       
       hEvisPperp->Fill(el_E,pperp);
       hEvisCosTheta->Fill(el_E,el_UZ);
       hEvisCosTheta_09->Fill(el_E,el_UZ);
       hEvisSinTheta->Fill(el_E,sin(dir.Theta()));
       hEnuqe->Fill(el_EnuQE/1000.);
       hR->Fill(el_R);
       hREvis->Fill(el_E,el_R);
       hXEvis->Fill(el_E,el_X);
       hYEvis->Fill(el_E,el_Y);
       hZEvis->Fill(el_E,el_Z);
       hDistToMeanCerEvis->Fill(el_E,el_distToMeanCer);
       hFEvis->Fill(el_E,el_F);
       hRendEvis->Fill(el_E,el_REnd);
       hUdotREvis->Fill(el_E,el_UdotR);
       hX2Y2Evis->Fill(el_E,el_X*el_X+el_Y*el_Y);
       hRnormEvis->Fill(el_E,el_R/500.);
       hCosTheta->Fill(el_UZ);
   }

}
