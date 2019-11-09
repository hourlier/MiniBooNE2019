#include "TChain.h"
#include "MiniBooNE_neutrino.C"

void LoopMiniBooNE_neutrino(){

  int Nfiles = 50;
  std::string Date = "dec18";
  std::string Preselect = "neutrino";
  std::string DataBasedir = "/e898-data/data60/reprocessing/"+Date+"/nu/stream/"+Preselect;
  std::vector<std::string> FileNames;
  
  for(int i=0;i<Nfiles;i++){
    std::string thisFile = Form("%s/boone_%07d.root.%s",DataBasedir.c_str(),i+40471,Preselect.c_str());
    ifstream f(thisFile.c_str());
    if(f.good())FileNames.push_back(thisFile);
    else continue;
  }
  
  std::cout << "FileNames.size() = " << FileNames.size() << std::endl;
  TChain *chain = new TChain("MiniBooNE");
  for(int i=0;i<FileNames.size();i++){
    if(i == 30  || i == 42  || i == 43  || i == 53  || i == 55  || i == 58  || i == 68 ) continue;
    if(i == 70  || i == 78  || i == 83  || i == 85  || i == 87  || i == 90  || i == 99 ) continue;
    if(i == 100 || i == 101 || i == 105 || i == 109 || i == 111 || i == 125 || i == 128) continue;
    if(i == 130 || i == 134 || i == 146 || i == 148 || i == 151 || i == 155 || i == 156) continue;
    if(i == 160 || i == 165 || i == 193 || i == 197 || i == 200 || i == 220 || i == 230) continue;
    if(i == 234 || i == 251 || i == 266 || i == 273 || i == 274 || i == 288 || i == 293) continue;
    if(i == 316 || i == 321 || i == 326 || i == 327 || i == 331 || i == 340 || i == 344) continue;
    if(i == 346 || i == 355 || i == 363 || i == 366 || i == 371 || i == 374 || i == 375) continue;
    if(i == 379 || i == 381 || i == 383 || i == 387 || i == 388 || i == 389 || i == 390) continue;
    if(i == 392 || i == 401 || i == 403 || i == 406 || i == 408 || i == 418 || i == 454) continue;
    if(i == 423 || i == 432 || i == 435 || i == 441 || i == 446 || i == 461 || i == 462) continue;
    if(i == 465 || i == 469 || i == 483 || i == 489 || i == 493 || i == 495 || i == 511) continue;
    if(i == 514 || i == 515 || i == 518 || i == 520 || i == 529 || i == 530 || i == 531) continue;
    if(i == 532 || i == 537 || i == 544 || i == 563 || i == 583 || i == 591 || i == 594) continue;
    if(i == 596 || i == 610 || i == 612 || i == 614 || i == 625)continue;


    std::cout << FileNames[i] << std::endl;
    TFile *fIN = TFile::Open(Form("%s",FileNames[i].c_str()),"READ");
    TTree *MiniBooNE = (TTree*)fIN->Get("MiniBooNE");
    int nentries = MiniBooNE->GetEntries();
    if(nentries == 0)continue;
    fIN->Close();
    chain->AddFile(Form("%s",FileNames[i].c_str()));
  }
  std::cout << "try with TChain" << std::endl;
  MiniBooNE_neutrino t(chain); 
  t.Loop();
}
