#define MCclass_cxx
#include "MCclass.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void MCclass::Loop2()
{

    if (fChain == 0) return;

    Long64_t nentries = fChain->GetEntriesFast();

    std::cout << "MCEVNT_E" << "\t" << "el_E" << "\t" << "el_UZ" << "\t" << "true lepton energy" << "true lepton theta" << "wgt" << std::endl;
    for (Long64_t jentry=0; jentry<nentries;jentry++) {
        fChain->GetEntry(jentry);
        if(el_EnuQE < Enuqemin || el_EnuQE > Enuqemax)continue;
        if(el_EnuQE > 2000)el_EnuQE = 1900;
        if(_forward == 1 && el_UZ < 0.9)continue;
        if(_forward == -1 && el_UZ > 0)continue;
        int kPID = 1;

        //std::cout << jentry << "/" << nentries << " : " << MCEVNT_PFSPZ.size() << std::endl;

        //std::cout << MCEVNT_E << "  " << el_E << "  " << el_UZ << "  " << MCEVNT_PFSPP[kPID] << "  " << MCEVNT_PFSPZ[kPID] << "  " << wgt << std::endl;
    }



}

void MCclass::Loop()
{

   if (fChain == 0) return;

    Long64_t nentries = fChain->GetEntriesFast();

    hEvisPperp = new TH2D("hEvisPperp_MC","all backgrounds;E_{vis} [MeV]; p_{perp}",NbinsXpperp,EvisMin, Evismax,  Nbinspperp, pperpMin,pperpMax);
    hBackgroundtype = new TH1D("hBackgroundtype","hBackgroundtype",100,1,-1);
    hR = new TH1D("hR_MC","hR_MC",NbinsR, Rmin, Rmax);

    std::string sampleNames[9];
    sampleNames[kOther]  = "others";
    sampleNames[kpi0]    = "pi0";
    sampleNames[kdelta]  = "delta";
    sampleNames[kk0nue]  = "k0nue";
    sampleNames[kkpmnue] = "kpmnue";
    sampleNames[kmunue]  = "kmunue";
    sampleNames[kElastic] = "elastic";
    sampleNames[kNumu]    = "numu";
    sampleNames[kCoherent]= "coherent";

    PlotColors[kOther]  = kGray;
    PlotColors[kpi0]    = kRed-6;
    PlotColors[kdelta]  = kYellow+1;
    PlotColors[kk0nue]  = kGreen-5;
    PlotColors[kkpmnue] = kGreen-6;
    PlotColors[kmunue]  = kGreen-8;
    PlotColors[kElastic]  = 1;
    PlotColors[kNumu]  = 2;
    PlotColors[kCoherent]  = 3;

    int Nnumuothers    = 0;
    int Nnumubarothers = 0;
    int Nnueothers     = 0;
    int Nnuebarothers  = 0;
    int Nothers        = 0;
    int NElasticothers = 0;

    for(int i=0;i<9;i++){
        TH2D *ThishEvisPperpComponents = new TH2D(Form("hEvisPperpComponents_%s",sampleNames[i].c_str()),";E_{vis} [MeV]; p_{perp}"  ,NbinsXpperp,EvisMin, Evismax,  Nbinspperp, pperpMin,pperpMax);
        TH1D *ThishR_a = new TH1D(Form("hR_%s",sampleNames[i].c_str()),";R [cm]",NbinsR, Rmin, Rmax);
        TH1D *ThishEnuqe = new TH1D(Form("hEnuqe_MC_%s",sampleNames[i].c_str()),"",sizeof(Ebins)/sizeof(Float_t) - 1,Ebins);
        TH1D *ThishCosTheta = new TH1D(Form("ThishCosTheta_MC_%s",sampleNames[i].c_str()),"",20,-1,1);
        TH2D *ThishREvis = new TH2D(Form("hREvis_MC_%s",sampleNames[i].c_str()),"; Evis [MeV]; R [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,Rmin, Rmax);
        TH2D *ThishXEvis = new TH2D(Form("hXEvis_MC_%s",sampleNames[i].c_str()),"; Evis [MeV]; X [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,-Rmax, Rmax);
        TH2D *ThishYEvis = new TH2D(Form("hYEvis_MC_%s",sampleNames[i].c_str()),"; Evis [MeV]; Y [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,-Rmax, Rmax);
        TH2D *ThishZEvis = new TH2D(Form("hZEvis_MC_%s",sampleNames[i].c_str()),"; Evis [MeV]; Z [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,-Rmax, Rmax);
        TH2D *ThishDistToMeanCerEvis = new TH2D(Form("hDistToMeanCerEvis_MC_%s",sampleNames[i].c_str()),"",NbinsXpperp, EvisMin, Evismax,NbinsR,50,200);
        TH2D *ThishFEvis = new TH2D(Form("hFEvis_MC_%s",sampleNames[i].c_str()),"; Evis [MeV]; F",NbinsXpperp, EvisMin, Evismax,NbinsR,0,10);
        TH2D *ThishRendEvis = new TH2D(Form("hRendEvis_MC_%s",sampleNames[i].c_str()),"; Evis [MeV]; Rend [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,Rmin, Rmax);
        TH2D *ThishUdotREvis = new TH2D(Form("hUdotREvis_MC_%s",sampleNames[i].c_str()),"; Evis [MeV]; U.R [cm]",NbinsXpperp, EvisMin, Evismax,NbinsR,-Rmax, Rmax);
        TH2D *ThishEvisCosTheta = new TH2D(Form("hEvisCosTheta_MC_%s",sampleNames[i].c_str()),"",NbinsXpperp, EvisMin, Evismax,NbinsR,-1,1);
        TH2D *ThishEvisCosTheta_09 = new TH2D(Form("hEvisCosTheta_09_MC_%s",sampleNames[i].c_str()),"",NbinsXpperp/2, EvisMin, Evismax,10,0.9,1);
        TH2D *ThishX2Y2Evis  = new TH2D(Form("hX2Y2Evis_MC_%s",sampleNames[i].c_str()),";Evis [MeV]; X^{2}+Y^{2}",NbinsXpperp, EvisMin, Evismax,NbinsR,0, Rmax*Rmax);
        TH2D *ThishPperpEvis_Coherent = new TH2D(Form("hPperpEvis_Coherent_MC_%s",sampleNames[i].c_str()),";E_{vis} [MeV]; p_{perp}",NbinsXpperp,EvisMin, Evismax,  Nbinspperp, pperpMin,pperpMax);
        TH2D *ThishEvisSinTheta = new TH2D(Form("hEvisSinTheta_MC_%s",sampleNames[i].c_str()),";E_{vis} [MeV]; sin #theta"  ,NbinsXpperp, EvisMin, Evismax,NbinsR,0,1);
        TH2D *ThishRnormEvis = new TH2D(Form("hRnormEvis_MC_%s",sampleNames[i].c_str()),"; Evis [MeV]; R/(500 cm)",NbinsXpperp, EvisMin, Evismax,NbinsR,0, 1);

        ThishREvis->SetFillColor(PlotColors[i]);
        ThishXEvis->SetFillColor(PlotColors[i]);
        ThishYEvis->SetFillColor(PlotColors[i]);
        ThishZEvis->SetFillColor(PlotColors[i]);
        ThishDistToMeanCerEvis->SetFillColor(PlotColors[i]);
        ThishFEvis->SetFillColor(PlotColors[i]);
        ThishRendEvis->SetFillColor(PlotColors[i]);
        ThishUdotREvis->SetFillColor(PlotColors[i]);
        ThishEvisCosTheta->SetFillColor(PlotColors[i]);
        ThishEvisCosTheta_09->SetFillColor(PlotColors[i]);
        ThishX2Y2Evis->SetFillColor(PlotColors[i]);
        ThishPperpEvis_Coherent->SetFillColor(PlotColors[i]);
        ThishEnuqe->SetFillColor(PlotColors[i]);
        ThishR_a->SetFillColor(PlotColors[i]);
        ThishEvisPperpComponents->SetFillColor(PlotColors[i]);
        ThishEvisSinTheta->SetFillColor(PlotColors[i]);
        ThishRnormEvis->SetFillColor(PlotColors[i]);
        ThishCosTheta->SetFillColor(PlotColors[i]);



        hREvis.push_back(ThishREvis);
        hXEvis.push_back(ThishXEvis);
        hYEvis.push_back(ThishYEvis);
        hZEvis.push_back(ThishZEvis);
        hDistToMeanCerEvis.push_back(ThishDistToMeanCerEvis);
        hFEvis.push_back(ThishFEvis);
        hRendEvis.push_back(ThishRendEvis);
        hUdotREvis.push_back(ThishUdotREvis);
        hEvisCosTheta.push_back(ThishEvisCosTheta);
        hEvisCosTheta_09.push_back(ThishEvisCosTheta_09);
        hX2Y2Evis.push_back(ThishX2Y2Evis);
        hPperpEvis_Coherent.push_back(ThishPperpEvis_Coherent);
        hEnuqe.push_back(ThishEnuqe);
        hR_a.push_back(ThishR_a);
        hEvisPperpComponents.push_back(ThishEvisPperpComponents);
        hEvisSinTheta.push_back(ThishEvisSinTheta);
        hRnormEvis.push_back(ThishRnormEvis);
        hCosTheta.push_back(ThishCosTheta);

    }

    hMCEVNT_EVWT = new TH1D("hMCEVNT_EVWT_MC","hMCEVNT_EVWT_MC",102,-1,101);

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
       
       fChain->GetEntry(jentry);
       if(el_EnuQE < Enuqemin || el_EnuQE > Enuqemax)continue;
       if(el_EnuQE > 2000)el_EnuQE = 1900;
       if(_forward == 1 && el_UZ < 0.9)continue;
       if(_forward == -1 && el_UZ > 0)continue;

       pperp = sqrt((el_E+me)-me*me)*sqrt(el_UX*el_UX+el_UY*el_UY);
       TVector3 dir(el_UX,el_UY,el_UZ);

       if(backgroundType == 11){
           hEnuqe[kOther]->Fill(el_EnuQE/1000.,wgt);
           hEvisPperpComponents[kOther]->Fill(el_E,pperp,wgt);
           hEvisSinTheta[kOther]->Fill(el_E,sin(dir.Theta()),wgt);
           hR_a[kOther]->Fill(el_R,wgt);
           hRnormEvis[kOther]->Fill(el_E,el_R/500.,wgt);

           hREvis[kOther]->Fill(el_E,el_R,wgt);
           hXEvis[kOther]->Fill(el_E,el_X,wgt);
           hYEvis[kOther]->Fill(el_E,el_Y,wgt);
           hZEvis[kOther]->Fill(el_E,el_Z,wgt);
           hDistToMeanCerEvis[kOther]->Fill(el_E,el_distToMeanCer,wgt);
           hFEvis[kOther]->Fill(el_E,el_F,wgt);
           hRendEvis[kOther]->Fill(el_E,el_REnd,wgt);
           hUdotREvis[kOther]->Fill(el_E,el_UdotR,wgt);
           hEvisCosTheta[kOther]->Fill(el_E,el_UZ,wgt);
           hEvisCosTheta_09[kOther]->Fill(el_E,el_UZ,wgt);
           hX2Y2Evis[kOther]->Fill(el_E,el_X*el_X+el_Y*el_Y,wgt);
           hCosTheta[kOther]->Fill(el_UZ,wgt);
           //if(MCBEAM_ntp <= 2){std::cout << "found a nue in others" << std::endl;}
           if(MCBEAM_ntp > 2){
               hEvisCosTheta[kNumu]->Fill(el_E,el_UZ,wgt);
               hEvisCosTheta_09[kNumu]->Fill(el_E,el_UZ,wgt);
           }
           if(el_UZ > 0.9){
               Nothers++;
               if(MCBEAM_ntp == 1)   Nnueothers++;
               if(MCBEAM_ntp == 2)   Nnuebarothers++;
               if(MCBEAM_ntp == 3)   Nnumuothers++;
               if(MCBEAM_ntp == 4)   Nnumubarothers++;
               if(MCEVNT_EVWT == 98) NElasticothers++;
           }

           if(MCEVNT_EVWT == 98){
               hEvisCosTheta[kElastic]->Fill(el_E,el_UZ,wgt);
               hEvisCosTheta_09[kElastic]->Fill(el_E,el_UZ,wgt);
               hCosTheta[kElastic]->Fill(el_UZ,wgt);

           }
           if(MCEVNT_EVWT == 96 || MCEVNT_EVWT == 97){
               hPperpEvis_Coherent[kmunue]->Fill(el_E,pperp, wgt);
           }

       }
       if(backgroundType == 10){
           hEnuqe[kdelta]->Fill(el_EnuQE/1000.,wgt);
           hEvisPperpComponents[kdelta]->Fill(el_E,pperp,wgt);
           hEvisSinTheta[kdelta]->Fill(el_E,sin(dir.Theta()),wgt);
           hR_a[kdelta]->Fill(el_R,wgt);
           hREvis[kdelta]->Fill(el_E,el_R,wgt);
           hXEvis[kdelta]->Fill(el_E,el_X,wgt);
           hYEvis[kdelta]->Fill(el_E,el_Y,wgt);
           hZEvis[kdelta]->Fill(el_E,el_Z,wgt);
           hDistToMeanCerEvis[kdelta]->Fill(el_E,el_distToMeanCer,wgt);
           hFEvis[kdelta]->Fill(el_E,el_F,wgt);
           hRendEvis[kdelta]->Fill(el_E,el_REnd,wgt);
           hUdotREvis[kdelta]->Fill(el_E,el_UdotR,wgt);
           hEvisCosTheta[kdelta]->Fill(el_E,el_UZ,wgt);
           hEvisCosTheta_09[kdelta]->Fill(el_E,el_UZ,wgt);
           hX2Y2Evis[kdelta]->Fill(el_E,el_X*el_X+el_Y*el_Y,wgt);
           hRnormEvis[kdelta]->Fill(el_E,el_R/500.,wgt);
           hCosTheta[kdelta]->Fill(el_UZ,wgt);

           
       }
       if(backgroundType == 9){
           hEnuqe[kpi0]->Fill(el_EnuQE/1000.,wgt);
           hEvisPperpComponents[kpi0]->Fill(el_E,pperp,wgt);
           hEvisSinTheta[kpi0]->Fill(el_E,sin(dir.Theta()),wgt);
           hR_a[kpi0]->Fill(el_R,wgt);
           hREvis[kpi0]->Fill(el_E,el_R,wgt);
           hXEvis[kpi0]->Fill(el_E,el_X,wgt);
           hYEvis[kpi0]->Fill(el_E,el_Y,wgt);
           hZEvis[kpi0]->Fill(el_E,el_Z,wgt);
           hDistToMeanCerEvis[kpi0]->Fill(el_E,el_distToMeanCer,wgt);
           hFEvis[kpi0]->Fill(el_E,el_F,wgt);
           hRendEvis[kpi0]->Fill(el_E,el_REnd,wgt);
           hUdotREvis[kpi0]->Fill(el_E,el_UdotR,wgt);
           hEvisCosTheta[kpi0]->Fill(el_E,el_UZ,wgt);
           if(el_UZ>0.9)hEvisCosTheta_09[kpi0]->Fill(el_E,el_UZ,wgt);
           hX2Y2Evis[kpi0]->Fill(el_E,el_X*el_X+el_Y*el_Y,wgt);
           hCosTheta[kpi0]->Fill(el_UZ,wgt);
           if(pperp>5 && pperp < 10 && el_E < 300)hMCEVNT_EVWT->Fill(MCEVNT_EVWT,wgt);
           hRnormEvis[kpi0]->Fill(el_E,el_R/500.,wgt);

           if(MCEVNT_EVWT == 96 || MCEVNT_EVWT == 97){
               hEvisCosTheta[kCoherent]->Fill(el_E,el_UZ,wgt);
               if(el_UZ>0.9)hEvisCosTheta_09[kCoherent]->Fill(el_E,el_UZ,wgt);
           }


       }
       if(backgroundType == 5){
           hEnuqe[kk0nue]->Fill(el_EnuQE/1000.,wgt);
           hEvisPperpComponents[kk0nue]->Fill(el_E,pperp,wgt);
           hEvisSinTheta[kk0nue]->Fill(el_E,sin(dir.Theta()),wgt);
           hR_a[kk0nue]->Fill(el_R,wgt);
           hREvis[kk0nue]->Fill(el_E,el_R,wgt);
           hXEvis[kk0nue]->Fill(el_E,el_X,wgt);
           hYEvis[kk0nue]->Fill(el_E,el_Y,wgt);
           hZEvis[kk0nue]->Fill(el_E,el_Z,wgt);
           hDistToMeanCerEvis[kk0nue]->Fill(el_E,el_distToMeanCer,wgt);
           hFEvis[kk0nue]->Fill(el_E,el_F,wgt);
           hRendEvis[kk0nue]->Fill(el_E,el_REnd,wgt);
           hUdotREvis[kk0nue]->Fill(el_E,el_UdotR,wgt);
           hEvisCosTheta[kk0nue]->Fill(el_E,el_UZ,wgt);
           hEvisCosTheta_09[kk0nue]->Fill(el_E,el_UZ,wgt);
           hX2Y2Evis[kk0nue]->Fill(el_E,el_X*el_X+el_Y*el_Y,wgt);
           hCosTheta[kk0nue]->Fill(el_UZ,wgt);
           hRnormEvis[kk0nue]->Fill(el_E,el_R/500.,wgt);
       }
       if((backgroundType == 3 || backgroundType == 4)){
           hEnuqe[kkpmnue]->Fill(el_EnuQE/1000.,wgt);
           hEvisPperpComponents[kkpmnue]->Fill(el_E,pperp,wgt);
           hEvisSinTheta[kkpmnue]->Fill(el_E,sin(dir.Theta()),wgt);
           hR_a[kkpmnue]->Fill(el_R,wgt);
           hREvis[kkpmnue]->Fill(el_E,el_R,wgt);
           hXEvis[kkpmnue]->Fill(el_E,el_X,wgt);
           hYEvis[kkpmnue]->Fill(el_E,el_Y,wgt);
           hZEvis[kkpmnue]->Fill(el_E,el_Z,wgt);
           hDistToMeanCerEvis[kkpmnue]->Fill(el_E,el_distToMeanCer,wgt);
           hFEvis[kkpmnue]->Fill(el_E,el_F,wgt);
           hRendEvis[kkpmnue]->Fill(el_E,el_REnd,wgt);
           hUdotREvis[kkpmnue]->Fill(el_E,el_UdotR,wgt);
           hEvisCosTheta[kkpmnue]->Fill(el_E,el_UZ,wgt);
           hEvisCosTheta_09[kkpmnue]->Fill(el_E,el_UZ,wgt);
           hX2Y2Evis[kkpmnue]->Fill(el_E,el_X*el_X+el_Y*el_Y,wgt);
           hCosTheta[kkpmnue]->Fill(el_UZ,wgt);
           hRnormEvis[kkpmnue]->Fill(el_E,el_R/500.,wgt);
       }
       if((backgroundType == 1 || backgroundType == 2)){
           hEnuqe[kmunue]->Fill(el_EnuQE/1000.,wgt);
           hEvisPperpComponents[kmunue]->Fill(el_E,pperp,wgt);
           hEvisSinTheta[kmunue]->Fill(el_E,sin(dir.Theta()),wgt);
           hR_a[kmunue]->Fill(el_R,wgt);
           hREvis[kmunue]->Fill(el_E,el_R,wgt);
           hXEvis[kmunue]->Fill(el_E,el_X,wgt);
           hYEvis[kmunue]->Fill(el_E,el_Y,wgt);
           hZEvis[kmunue]->Fill(el_E,el_Z,wgt);
           hDistToMeanCerEvis[kmunue]->Fill(el_E,el_distToMeanCer,wgt);
           hFEvis[kmunue]->Fill(el_E,el_F,wgt);
           hRendEvis[kmunue]->Fill(el_E,el_REnd,wgt);
           hUdotREvis[kmunue]->Fill(el_E,el_UdotR,wgt);
           hEvisCosTheta[kmunue]->Fill(el_E,el_UZ,wgt);
           hEvisCosTheta_09[kmunue]->Fill(el_E,el_UZ,wgt);
           hX2Y2Evis[kmunue]->Fill(el_E,el_X*el_X+el_Y*el_Y,wgt);
           hCosTheta[kmunue]->Fill(el_UZ,wgt);
           hRnormEvis[kmunue]->Fill(el_E,el_R/500.,wgt);
       }




       hEvisPperp->Fill(el_E,pperp,wgt);
       hBackgroundtype->Fill(backgroundType,wgt);
       hR->Fill(el_R,wgt);

   }

    for(int i=0;i<hEvisSinTheta.size();i++){
        hEnuqe[i]->Scale(ScaleFactor);
        hR_a[i]->Scale(ScaleFactor);
        hEvisSinTheta[i]->Scale(ScaleFactor);
        hEvisCosTheta[i]->Scale(ScaleFactor);
        hEvisCosTheta_09[i]->Scale(ScaleFactor);
        hEvisPperpComponents[i]->Scale(ScaleFactor);
        hREvis[i]->Scale(ScaleFactor);
        hXEvis[i]->Scale(ScaleFactor);
        hYEvis[i]->Scale(ScaleFactor);
        hZEvis[i]->Scale(ScaleFactor);
        hDistToMeanCerEvis[i]->Scale(ScaleFactor);
        hFEvis[i]->Scale(ScaleFactor);
        hRendEvis[i]->Scale(ScaleFactor);
        hUdotREvis[i]->Scale(ScaleFactor);
        hX2Y2Evis[i]->Scale(ScaleFactor);
        hPperpEvis_Coherent[i]->Scale(ScaleFactor);
        hRnormEvis[i]->Scale(ScaleFactor);
        hCosTheta[i]->Scale(ScaleFactor);
    }
    hEvisPperp->Scale(ScaleFactor);
    hBackgroundtype->Scale(ScaleFactor);
    hR->Scale(ScaleFactor);
    hMCEVNT_EVWT->Scale(ScaleFactor);

    std::cout << std::endl;
    std::cout << "breakdown of the Others category" << std::endl;
    std::cout << "nu-e elastic fraction = " << hEvisCosTheta[kElastic]->GetEntries()*1./hEvisCosTheta[kOther]->GetEntries() << " = " << NElasticothers*1./Nothers << std::endl;
    std::cout << "numu/numubar fraction = " << hEvisCosTheta[kNumu]->GetEntries()*1./hEvisCosTheta[kOther]->GetEntries() << std::endl;
    std::cout << "nue/nuebar fraction = " << 1-(hEvisCosTheta[kNumu]->GetEntries()*1./hEvisCosTheta[kOther]->GetEntries()) << std::endl;

    std::cout << "numu fraction    = ..." << Nnumuothers*1./Nothers << std::endl;
    std::cout << "numubar fraction = ..." << Nnumubarothers*1./Nothers << std::endl;
    std::cout << "nue fraction     = ..." << Nnueothers*1./Nothers << std::endl;
    std::cout << "nuebar fraction  = ..." << Nnuebarothers*1./Nothers << std::endl;
    std::cout << std::endl;
    std::cout << "pi0 coherent/pi0 mis-ID fraction = " << hEvisCosTheta_09[kCoherent]->GetEntries()*1./hEvisCosTheta_09[kpi0]->GetEntries() << std::endl;
    std::cout << std::endl;
}
