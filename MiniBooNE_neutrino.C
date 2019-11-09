#define MiniBooNE_neutrino_cxx
#include "MiniBooNE_neutrino.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void MiniBooNE_neutrino::Loop()
{
//   In a ROOT session, you can do:
//      root> .L MiniBooNE_neutrino.C
//      root> MiniBooNE_neutrino t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntries();

   TH1D *hEMichel = new TH1D("hEMichel","hEMichel",100,0,1250);
   TH1D *hEmu = new TH1D("hEmu","hEmu",100,0,1250);
   TH1D *hEe = new TH1D("hEe","hEe",100,0,1250);
   TH1D *hE_e_hyp = new TH1D("hE_e_hyp","hE_e_hyp",100,0,1250);

   TH2D *hE01_e = new TH2D("hE01_e","hE01_e",100,0,1250,100,0,1250);

   TH2D *hEvparttype = new TH2D("hEvparttype","hEvparttype",100,0,1250,10,0,5);
   TH2D *hpartType01 = new TH2D("hpartType01","hpartType01",10,0,5,10,0,5);
   TH2D *hpartType12 = new TH2D("hpartType12","hpartType12",10,0,5,10,0,5);
   TH2D *hpartType02 = new TH2D("hpartType02","hpartType02",10,0,5,10,0,5);
   
   TH1D *hE[kMaxTOneTrackChunk];
   TH1D *hTrackTypes[kMaxTOneTrackChunk];
   for(int i=0;i<kMaxTOneTrackChunk;i++){
     hE[i] = new TH1D(Form("hE_%d",i),Form("hE_%d",i),100,0,1000);
     hTrackTypes[i] = new TH1D(Form("hTrackTypes_%d",i),Form("hTrackTypes_%d",i),40,-10,10);
   }
   TH1D *hTrackType = new TH1D("hTrackType","hTrackType",40,-10,10);
   
   TH2D *hTOneTrackChunk_EvF_e    = new TH2D("hTOneTrackChunk_EvF_e"   ,"hTOneTrackChunk_EvF_e"   ,200,0,10,200,0,2000);
   TH2D *hTOneTrackChunk_EvF_mich = new TH2D("hTOneTrackChunk_EvF_mich","hTOneTrackChunk_EvF_mich",200,0,10,200,0,2000);
   TH2D *hTOneTrackChunk_EvF_mu   = new TH2D("hTOneTrackChunk_EvF_mu"  ,"hTOneTrackChunk_EvF_mu"  ,200,0,10,200,0,2000);



   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      fChain->GetEntry(jentry);
      //std::cout << jentry << "/" << nentries << " = " << jentry*100./nentries << "% \t : \t" << TOneTrackChunk_data__E[1] << std::endl;      

      for(int i=0;i<kMaxTOneTrackChunk;i++){
	hE[i]->Fill(TOneTrackChunk_data__E[i]);
	hTrackTypes[i]->Fill(TOneTrackChunk_data__trackType[i]);
	hTrackType->Fill(TOneTrackChunk_data__trackType[i]);
	hEvparttype->Fill(TOneTrackChunk_data__E[i],TOneTrackChunk_data__trackType[i]);
	if(TOneTrackChunk_data__trackType[i] == 1){
	  //hEMichel->Fill(TOneTrackChunk_data__E[i]);
	  if(i != 1 && !(TOneTrackChunk_data__trackType[1] == 1 && TOneTrackChunk_data__E[1] < 60)){
	    hEe->Fill(TOneTrackChunk_data__E[i]);
	    hTOneTrackChunk_EvF_e->Fill(TOneTrackChunk_data__F[i],TOneTrackChunk_data__E[i]);
	  }
	}
	if(TOneTrackChunk_data__trackType[i] == 2){
	  hEmu->Fill(TOneTrackChunk_data__E[i]);
	  hTOneTrackChunk_EvF_mu->Fill(TOneTrackChunk_data__F[i],TOneTrackChunk_data__E[i]);
	}

      }

      if(TOneTrackChunk_data__trackType[1] == 1){
	hEMichel->Fill(TOneTrackChunk_data__E[1]);
	hTOneTrackChunk_EvF_mich->Fill(TOneTrackChunk_data__F[1],TOneTrackChunk_data__E[1]);
      }
      
      hE_e_hyp->Fill(TOneTrackChunk_data__E[0]);
      
      if(TOneTrackChunk_data__trackType[0] == 1 && TOneTrackChunk_data__trackType[1] == 1)hE01_e->Fill(TOneTrackChunk_data__E[0],TOneTrackChunk_data__E[1]);
      
      hpartType12->Fill(TOneTrackChunk_data__trackType[1],TOneTrackChunk_data__trackType[2]);
      hpartType01->Fill(TOneTrackChunk_data__trackType[0],TOneTrackChunk_data__trackType[1]);
      hpartType02->Fill(TOneTrackChunk_data__trackType[0],TOneTrackChunk_data__trackType[2]);
      
   }

   TCanvas *cEMichel = new TCanvas("cEMichel","cEMichel",800,600);
   hEMichel->Draw();
   hEmu->Draw("same");
   hEe->Draw("same");

   TCanvas *cTrackType = new TCanvas("cTrackType","cTrackType",800,600);
   hTrackType->Draw();
   for(int i=0;i<kMaxTOneTrackChunk;i++){
     hTrackTypes[i]->SetLineColor(i+2);
     hTrackTypes[i]->Draw("same");
   }

   TCanvas *cOneTrackChunkE = new TCanvas("cOneTrackChunkE","cOneTrackChunE",800,600);
   hE[0]->Draw();
   for(int i=0;i<kMaxTOneTrackChunk;i++){
     hE[i]->SetLineColor(i+2);
     hE[i]->Draw("same");
   }
   hE_e_hyp->SetLineColor(2);
   hE_e_hyp->SetLineWidth(2);
   hE_e_hyp->Draw("same");

   TCanvas *cEvparttype = new TCanvas("cEvparttype","cEvparttype",800,600);
   hEvparttype->Draw("colz");

   TCanvas *cpartType12 = new TCanvas("cpartType12","cpartType12",800,600);
   hpartType12->Draw("colz");

   TCanvas *cpartType01 = new TCanvas("cpartType01","cpartType01",800,600);
   hpartType01->Draw("colz");

   TCanvas *cpartType02 = new TCanvas("cpartType02","cpartType02",800,600);
   hpartType02->Draw("colz");

   TCanvas *cTOneTrackChunk_EvF_e = new TCanvas("cTOneTrackChunk_EvF_e","cTOneTrackChunk_EvF_e",800,600);
   hTOneTrackChunk_EvF_e->Draw("colz");

   TCanvas *cTOneTrackChunk_EvF_mu = new TCanvas("cTOneTrackChunk_EvF_mu","cTOneTrackChunk_EvF_mu",800,600);
   hTOneTrackChunk_EvF_mu->Draw("colz");

   TCanvas *cTOneTrackChunk_EvF_mich = new TCanvas("cTOneTrackChunk_EvF_mich","cTOneTrackChunk_EvF_mich",800,600);
   hTOneTrackChunk_EvF_mich->Draw("colz");

   TCanvas *cE01_e = new TCanvas("cE01_e","cE01_e",800,600);
   hE01_e->Draw("colz");
}
