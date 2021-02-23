std::vector<std::vector<double> > LoadFile(std::string fileName){
    std::vector<std::vector<double> > v;
    ifstream myFile(fileName);
    if(!myFile.is_open()){std::cout << "ERROR file " << fileName << " could not be opened" << std::endl; return v;}
    bool goOn = true;

    string line;
    getline (myFile,line);
    getline (myFile,line);
    getline (myFile,line);

    int row;
    char star;
    double cosTheta, Evis, weight;


    int iter = 0;
    while(goOn){
        iter++;
        myFile >> star >> row >> star >> cosTheta >> star >> Evis >> star >> weight >> star;
        if((iter > 1 && row == 0) || myFile.eof()){goOn=false;break;}
        //std::cout  << row << "\t" << cosTheta << "\t" << Evis << std::endl;
        std::vector<double> v1(3);
        v1[0] = cosTheta; v1[1] = Evis; v1[2] = weight;
        v.push_back(v1);
    }
    return v;
}

std::vector<std::vector<double> > LoadFile_EvisCorr(){
    std::vector<std::vector<double> > v;
    std::string fileName = "twod_Data_correctedE.txt";
    ifstream myFile(fileName);
    if(!myFile.is_open()){std::cout << "ERROR file " << fileName << " could not be opened" << std::endl; return v;}
    bool goOn = true;

    string line;
    getline (myFile,line);
    getline (myFile,line);
    getline (myFile,line);

    int row, run, subrun, event;
    char star;
    double cosTheta, Evis, weight(1);


    int iter = 0;
    while(goOn){
        iter++;
        myFile >> star >> row >> star >> run >> star >> subrun >> star >> event >> star >> Evis >> star >> cosTheta >> star ;
        if((iter > 1 && row == 0) || myFile.eof()){goOn=false;break;}
        //std::cout  << row << "\t" << cosTheta << "\t" << Evis << std::endl;
        std::vector<double> v1(3);
        v1[0] = cosTheta; v1[1] = Evis; v1[2] = 1;
        v.push_back(v1);
    }
    return v;
}

std::vector<TH1D*> Get1DDistributionX(TH2D *h){
    std::vector<TH1D*> h_v(h->GetNbinsX());
    for(int i=0;i<h->GetNbinsX();i++){
        h_v[i] = new TH1D(Form("%s_%d",h->GetName(),i),Form("%s_%d",h->GetName(),i),h->GetNbinsY(),h->GetYaxis()->GetXmin(),h->GetYaxis()->GetXmax());
        for(int j=0;j<h->GetNbinsY();j++){
            h_v[i]->SetBinContent(j+1,h->GetBinContent(i+1,j+1));
        }
    }
    return h_v;
}

std::vector<TH1D*> Get1DDistributionY(TH2D *h){
    std::vector<TH1D*> h_v(h->GetNbinsY());
    for(int i=0;i<h->GetNbinsY();i++){
        h_v[i] = new TH1D(Form("%s_Y_%d",h->GetName(),i),Form("%s_Y_%d",h->GetName(),i),h->GetNbinsX(),h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
        for(int j=0;j<h->GetNbinsX();j++){
            h_v[i]->SetBinContent(j+1,h->GetBinContent(j+1,i+1));
        }
    }
    return h_v;
}

void LoadTextFiles(){
    gStyle->SetOptStat(0);
    gStyle->SetPalette(kTemperatureMap);

    int NbinsEvis = 20;
    int NbinsCosTheta = 20;
    double EvisMin = 100;
    double EvisMax = 1250;




    double POT_data      = 18.75e20;
    double POT_BF        = 6.639e19;
    double POT_delta     = 2.054e21;
    double POT_Intrinsic = 2.054e21;
    double POT_Other     = 2.054e21;
    double POT_dirt      = 2.054e21;
    double POT_numu      = 2.054e21;
    double POT_pi0       = 2.054e21;

    TCanvas *cData_2D = new TCanvas("cData_2D","cData_2D",800,600);
    std::string Data = "twod_Data.txt";
    std::vector<std::vector<double> > Data_v = LoadFile(Data);
    std::vector<std::vector <double> > Eviscor_v = LoadFile_EvisCorr();
    TH2D *hData = new TH2D("hData","Data;Evis [MeV];cos#theta",NbinsEvis,EvisMin,EvisMax,NbinsCosTheta,-1,1);
    TH1D *hData_Evis = new TH1D("hData_Evis","hData_Evis;Evis [MeV]",NbinsEvis,EvisMin,EvisMax);
    for(int i=0;i<Eviscor_v.size();i++){
        hData->Fill(Eviscor_v[i][1],Eviscor_v[i][0],Eviscor_v[i][2]);
        hData_Evis->Fill(Eviscor_v[i][1],Eviscor_v[i][2]);
    }
    hData->SetContour(100);
    hData->GetZaxis()->SetRangeUser(0,60);
    hData->Draw("colz");
    cData_2D->SaveAs("~/Desktop/cData2D.pdf");



    TCanvas *cDelta_2D = new TCanvas("cDelta_2D","cDelta_2D",800,600);
    std::string Delta = "twod_Delta.txt";
    std::vector<std::vector<double> > Delta_v = LoadFile(Delta);
    TH2D *hDelta = new TH2D("hDelta","hDelta;Evis [MeV];cos#theta",NbinsEvis,EvisMin,EvisMax,NbinsCosTheta,-1,1);
    TH1D *hDelta_Evis = new TH1D("hDelta_Evis","hDelta_Evis;Evis [MeV]",NbinsEvis,EvisMin,EvisMax);
    for(int i=0;i<Delta_v.size();i++){
        hDelta->Fill(Delta_v[i][1],Delta_v[i][0],Delta_v[i][2]);
        hDelta_Evis->Fill(Delta_v[i][1],Delta_v[i][2]);
    }
    //hDelta->Sumw2();
    //hDelta_Evis->Sumw2();
    hDelta->Scale(POT_data/POT_delta);
    hDelta_Evis->Scale(POT_data/POT_delta);
    hDelta->SetContour(100);
    hDelta->Draw("colz");
    cDelta_2D->SaveAs("~/Desktop/cDelta_2D.pdf");

    TCanvas *cIntrinsic_2D = new TCanvas("cIntrinsic_2D","cIntrinsic_2D",800,600);
    std::string IntrinsicNue = "twod_IntrinsicNue.txt";
    std::vector<std::vector<double> > IntrinsicNue_v = LoadFile(IntrinsicNue);
    TH2D *hIntrinsicNue = new TH2D("hIntrinsicNue","hIntrinsicNue;Evis [MeV];cos#theta",NbinsEvis,EvisMin,EvisMax,NbinsCosTheta,-1,1);
    TH1D *hIntrinsicNue_Evis = new TH1D("hIntrinsicNue_Evis","hIntrinsicNue_Evis;Evis [MeV]",NbinsEvis,EvisMin,EvisMax);
    for(int i=0;i<IntrinsicNue_v.size();i++){
        hIntrinsicNue->Fill(IntrinsicNue_v[i][1],IntrinsicNue_v[i][0],IntrinsicNue_v[i][2]);
        hIntrinsicNue_Evis->Fill(IntrinsicNue_v[i][1],IntrinsicNue_v[i][2]);
    }
    //hIntrinsicNue->Sumw2();
    //hIntrinsicNue_Evis->Sumw2();
    hIntrinsicNue->Scale(POT_data/POT_Intrinsic);
    hIntrinsicNue_Evis->Scale(POT_data/POT_Intrinsic);
    hIntrinsicNue->SetContour(100);
    hIntrinsicNue->Draw("colz");
    cIntrinsic_2D->SaveAs("~/Desktop/cIntrinsic_2D.pdf");

    //TCanvas *cNumu_2D = new TCanvas("cNumu_2D","cNumu_2D",800,600);
    std::string Numu = "twod_Numu.txt";
    std::vector<std::vector<double> > Numu_v = LoadFile(Numu);
    TH2D *hNumu = new TH2D("hNumu","hNumu;Evis [MeV];cos#theta",NbinsEvis,EvisMin,EvisMax,NbinsCosTheta,-1,1);
    TH1D *hNumu_Evis = new TH1D("hNumu_Evis","hNumu_Evis;Evis [MeV]",NbinsEvis,EvisMin,EvisMax);
    for(int i=0;i<Numu_v.size();i++){
        hNumu->Fill(Numu_v[i][1],Numu_v[i][0],Numu_v[i][2]);
        hNumu_Evis->Fill(Numu_v[i][1],Numu_v[i][2]);
    }
    //hNumu->Sumw2();
    //hNumu_Evis->Sumw2();
    hNumu->Scale(POT_data/POT_numu);
    hNumu_Evis->Scale(POT_data/POT_numu);
    //hNumu->SetContour(100);
    //hNumu->Draw("colz");
    //cNumu_2D->SaveAs("~/Desktop/cNumu_2D.pdf");

    TCanvas *cOther_2D = new TCanvas("cOther_2D","cOther_2D",800,600);
    std::string Other = "twod_Other.txt";
    std::vector<std::vector<double> > Other_v = LoadFile(Other);
    TH2D *hOther = new TH2D("hOther","hOther;Evis [MeV];cos#theta",NbinsEvis,EvisMin,EvisMax,NbinsCosTheta,-1,1);
    TH1D *hOther_Evis = new TH1D("hOther_Evis","hOther_Evis;Evis [MeV]",NbinsEvis,EvisMin,EvisMax);
    for(int i=0;i<Other_v.size();i++){
        hOther->Fill(Other_v[i][1],Other_v[i][0],Other_v[i][2]);
        hOther_Evis->Fill(Other_v[i][1],Other_v[i][2]);
    }
    //hOther->Sumw2();
    //hOther_Evis->Sumw2();
    hOther->Scale(POT_data/POT_Other);
    hOther_Evis->Scale(POT_data/POT_Other);
    hOther->SetContour(100);
    hOther->Add(hNumu,1);
    hOther->Draw("colz");
    cOther_2D->SaveAs("~/Desktop/cOther_2D.pdf");

    TCanvas *cDirt_2D = new TCanvas("cDirt_2D","cDirt_2D",800,600);
    std::string Dirt = "twod_dirt.txt";
    std::vector<std::vector<double> > Dirt_v = LoadFile(Dirt);
    TH2D *hDirt = new TH2D("hDirt","hDirt;Evis [MeV];cos#theta",NbinsEvis,EvisMin,EvisMax,NbinsCosTheta,-1,1);
    TH1D *hDirt_Evis = new TH1D("hDirt_Evis","hDirt_Evis;Evis [MeV]",NbinsEvis,EvisMin,EvisMax);
    for(int i=0;i<Dirt_v.size();i++){
        hDirt->Fill(Dirt_v[i][1],Dirt_v[i][0],Dirt_v[i][2]);
        hDirt_Evis->Fill(Dirt_v[i][1],Dirt_v[i][2]);
    }
    //hDirt->Sumw2();
    //hDirt_Evis->Sumw2();
    hDirt->Scale(POT_data/POT_dirt);
    hDirt_Evis->Scale(POT_data/POT_dirt);
    hDirt->SetContour(100);
    hDirt->Draw("colz");
    cDirt_2D->SaveAs("~/Desktop/cDirt_2D.pdf");

    TCanvas *cPi0_2D = new TCanvas("cPi0_2D","cPi0_2D",800,600);
    std::string Pi0 = "twod_pi0.txt";
    std::vector<std::vector<double> > Pi0_v = LoadFile(Pi0);
    TH2D *hPi0 = new TH2D("hPi0","hPi0;Evis [MeV];cos#theta",NbinsEvis,EvisMin,EvisMax,NbinsCosTheta,-1,1);
    TH1D *hPi0_Evis = new TH1D("hPi0_Evis","hPi0_Evis;Evis [MeV]",NbinsEvis,EvisMin,EvisMax);
    for(int i=0;i<Pi0_v.size();i++){
        hPi0->Fill(Pi0_v[i][1],Pi0_v[i][0],Pi0_v[i][2]);
        hPi0_Evis->Fill(Pi0_v[i][1],Pi0_v[i][2]);
    }
    //hPi0->Sumw2();
    //hPi0_Evis->Sumw2();
    hPi0->Scale(POT_data/POT_pi0);
    hPi0_Evis->Scale(POT_data/POT_pi0);
    hPi0->SetContour(100);
    hPi0->Draw("colz");
    cPi0_2D->SaveAs("~/Desktop/cPi0_2D.pdf");

    TCanvas *cMC_BG = new TCanvas("cMC_BG","cMC_BG",800,600);
    TH2D *hMC_BG = new TH2D("hMC_BG","Backgrounds;Evis [MeV];cos#theta",NbinsEvis,EvisMin,EvisMax,NbinsCosTheta,-1,1);
    hMC_BG->Add(hPi0);
    hMC_BG->Add(hDirt);
    hMC_BG->Add(hOther);
    //hMC_BG->Add(hNumu);
    hMC_BG->Add(hIntrinsicNue);
    hMC_BG->Add(hDelta);
    hMC_BG->SetContour(100);
    hMC_BG->GetZaxis()->SetRangeUser(0,60);
    hMC_BG->Draw("colz");
    cMC_BG->SaveAs("~/Desktop/cMC_BG.pdf");

    TCanvas *cExcess = new TCanvas("cExcess","cExcess",800,600);
    TH2D *hExcess = new TH2D("hExcess","Data-Backgrounds;Evis [MeV];cos#theta",NbinsEvis,EvisMin,EvisMax,NbinsCosTheta,-1,1);
    hExcess->Add(hData);
    hExcess->Add(hMC_BG,-1);
    hExcess->SetContour(100);
    hExcess->GetZaxis()->SetRangeUser(-15,30);
    hExcess->Draw("colz");
    cExcess->SaveAs("~/Desktop/cExcess2D.pdf");

    TCanvas *cBestFit_2D = new TCanvas("cBestFit_2D","cBestFit_2D",800,600);
    std::string BestFit = "twod_BestFit.txt";
    std::vector<std::vector<double> > BestFit_v = LoadFile(BestFit);
    TH2D *hBestFit = new TH2D("hBestFit","Best Fit;Evis [MeV];cos#theta",NbinsEvis,EvisMin,EvisMax,NbinsCosTheta,-1,1);
    TH1D *hBestFit_Evis = new TH1D("hBestFit_Evis","hBestFit_Evis;Evis [MeV]",NbinsEvis,EvisMin,EvisMax);
    for(int i=0;i<BestFit_v.size();i++){
        hBestFit->Fill(BestFit_v[i][1],BestFit_v[i][0],BestFit_v[i][2]);
        hBestFit_Evis->Fill(BestFit_v[i][1],BestFit_v[i][2]);

    }
    //hBestFit->Sumw2();
    //hBestFit_Evis->Sumw2();
    hBestFit->Scale(POT_data/POT_BF);
    hBestFit_Evis->Scale(POT_data/POT_BF);
    hBestFit->SetContour(100);
    //hBestFit->GetZaxis()->SetRangeUser(-15,30);
    hBestFit->Draw("colz");
    cBestFit_2D->SaveAs("~/Desktop/cBestFit_2D.pdf");

    TCanvas *cMC_BG_BF = new TCanvas("cMC_BG_BF","cMC_BG_BF",800,600);
    TH2D *hMC_BG_BF = new TH2D("hMC_BG_BF","hMC_BG_BF;Evis [MeV];cos#theta",NbinsEvis,EvisMin,EvisMax,NbinsCosTheta,-1,1);
    hMC_BG_BF->Add(hMC_BG);
    hMC_BG_BF->Add(hBestFit);
    hMC_BG_BF->Draw("colz");



    TCanvas *cExcess_BF = new TCanvas("cExcess_BF","cExcess_BF",800,600);
    TH2D *hExcess_BF = new TH2D("hExcess_BF","data - Backgrounds - BF;Evis [MeV];cos#theta",NbinsEvis,EvisMin,EvisMax,NbinsCosTheta,-1,1);
    hExcess_BF->Add(hData);
    hExcess_BF->Add(hMC_BG,-1);
    hExcess_BF->Add(hBestFit,-1);
    hExcess_BF->Draw("colz");

    TCanvas *c2DplotsPaper = new TCanvas("c2DplotsPaper","c2DplotsPaper",1600,1200);
    c2DplotsPaper->Divide(2,2);
    c2DplotsPaper->cd(1);
    hData->Draw("colz");
    c2DplotsPaper->cd(2);
    hMC_BG->Draw("colz");
    c2DplotsPaper->cd(3);
    hExcess->Draw("colz");
    c2DplotsPaper->cd(4);
    hBestFit->Draw("colz");
    c2DplotsPaper->SaveAs("~/Desktop/c2DplotsPaper.pdf");

    TCanvas *cEvisComp = new TCanvas("cEvisComp","cEvisComp",800,600);
    TH1D *hEvisCorr = new TH1D("hEvisCorr",";E_{vis} [MeV]",NbinsEvis,EvisMin,EvisMax);
    TH1D *hEvis = new TH1D("hEvis","hEvis",NbinsEvis,EvisMin,EvisMax);
    THStack *hMC_Evis = new THStack("hMC_Evis","hMC_Evis");

    hOther_Evis->Add(hNumu_Evis);

    hBestFit_Evis->SetFillColor(0);
    hOther_Evis->SetFillColor(2);
    hDirt_Evis->SetFillColor(3);
    hPi0_Evis->SetFillColor(5);
    hDelta_Evis->SetFillColor(6);
    hIntrinsicNue_Evis->SetFillColor(7);
    hBestFit_Evis->SetLineColor(1);
    hBestFit_Evis->SetLineStyle(7);


    hMC_Evis->Add(hOther_Evis);
    hMC_Evis->Add(hDirt_Evis);
    hMC_Evis->Add(hDelta_Evis);
    hMC_Evis->Add(hPi0_Evis);
    hMC_Evis->Add(hIntrinsicNue_Evis);
    hMC_Evis->Add(hBestFit_Evis);

    for(int i=0;i<Data_v.size();i++){hEvis->Fill(Data_v[i][1]);}
    for(int i=0;i<Eviscor_v.size();i++){hEvisCorr->Fill(Eviscor_v[i][1]);}
    hEvisCorr->SetLineColor(1);
    hEvisCorr->SetMarkerStyle(20);
    hEvisCorr->Draw("e");
    hMC_Evis->Draw("hist same");
    hEvisCorr->Draw("same e");

    TLegend *legStack = new TLegend(0.6,0.6,0.87,0.87);
    legStack->AddEntry(hEvisCorr,Form("Data (%.0f)",hEvisCorr->GetEntries()),"PL");
    legStack->AddEntry(hOther_Evis,Form("Others (%.2f)",hOther_Evis->Integral(1,NbinsEvis)),"F");
    legStack->AddEntry(hDirt_Evis,Form("Dirt (%.2f)",hDirt_Evis->Integral(1,NbinsEvis)),"F");
    legStack->AddEntry(hPi0_Evis,Form("pi0 (%.2f)",hPi0_Evis->Integral(1,NbinsEvis)),"F");
    legStack->AddEntry(hDelta_Evis,Form("Delta (%.2f)",hDelta_Evis->Integral(1,NbinsEvis)),"F");
    legStack->AddEntry(hIntrinsicNue_Evis,Form("intrinsic nue (%.2f)",hIntrinsicNue_Evis->Integral(1,NbinsEvis)),"F");
    legStack->AddEntry(hBestFit_Evis,Form("Best Fit (%.2f)",hBestFit_Evis->Integral(1,NbinsEvis)),"F");
    legStack->Draw("same");

    cEvisComp->SaveAs("~/Desktop/cEvisComp.pdf");

    //////////////////////////////////////////////////////////////////////////////////////
    ///1D distribution of the data and MC
    //////////////////////////////////////////////////////////////////////////////////////

    double Evis[NbinsEvis];
    for(int i=0;i<NbinsEvis;i++){
        Evis[i] = EvisMin+(i+0.5)*((EvisMax-EvisMin)/NbinsEvis);
    }

    std::vector<TH1D*> h1D_data_v = Get1DDistributionX(hData);
    std::vector<TH1D*> h1D_BG_v = Get1DDistributionX(hMC_BG);
    std::vector<TH1D*> h1D_BestFit_v = Get1DDistributionX(hBestFit);

    int Length = (NbinsEvis+2)*1000;
    TCanvas *c1D_Data_2 = new TCanvas("c1D_Data_2","c1D_Data_2",Length,5000);
    std::vector<TPad*> p(NbinsEvis);
    double dL = 1./(NbinsEvis+2);

    for(int i=0;i<NbinsEvis;i++){
        h1D_data_v[i]->SetTitle("");
        h1D_data_v[i]->GetXaxis()->SetTitleOffset(0.05);
        h1D_data_v[i]->GetXaxis()->SetTitleSize(0.36);
        h1D_data_v[i]->GetYaxis()->SetRangeUser(0,70);
        h1D_data_v[i]->SetLineColor(4);
        h1D_data_v[i]->SetLineWidth(1);
        h1D_data_v[i]->SetFillColorAlpha(4,0.5);
        h1D_BG_v[i]->SetLineColor(5);
        h1D_BG_v[i]->SetLineWidth(1);
        h1D_BG_v[i]->SetFillColorAlpha(5,0.5);
    }

    for(int i=0;i<NbinsEvis;i++){
        c1D_Data_2->cd();
        p[i] = new TPad(Form("p_%d",i),Form("p_%d",i),i*dL+dL,0,(i+1)*dL+dL,1);
        p[i]->Draw();
        p[i]->cd();
        p[i]->SetLeftMargin(0.05);
        p[i]->SetRightMargin(0.05);
        h1D_data_v[i]->Draw();
        h1D_BG_v[i]->Draw("same");
        TLatex *pl = new TLatex(-0.3,71,Form("%.0f MeV",Evis[i]));
        pl->Draw();
        double angle = 60;
        double size = 4*pl->GetTextSize();
        auto text = pl->GetTitle();
        pl->PaintLatex(pl->GetX(),pl->GetY(),angle,size,text);

        TLatex *tXaxis;

        if(Evis[i] > 700){
            h1D_data_v[i]->GetXaxis()->SetRangeUser(0,1);
            pl->PaintLatex(0.3,pl->GetY(),angle,size,text);
            tXaxis = new TLatex(-0.15,-2,"  0       1 ");
        }

        else{tXaxis = new TLatex(-1.15,-2,"-1   0   1 ");}
        tXaxis->SetTextSize(0.3);
        tXaxis->Draw();

        p[i]->Modified();
        p[i]->Update();
    }

    c1D_Data_2->cd();
    TPad *pLeg = new TPad("pLeg","pLeg",(17+0.025)*dL,0.6,(NbinsEvis+1-0.025)*dL,0.87);
    pLeg->Draw();
    pLeg->cd();
    TLegend *leg1D = new TLegend(0,0,1,1);
    leg1D->AddEntry(h1D_data_v[1],"Data","F");
    leg1D->AddEntry(h1D_BG_v[1],"Backgrounds","F");
    leg1D->Draw();

    c1D_Data_2->cd();
    TPad *pXtitle = new TPad("pXtitle","pXtitle",0,0,1,0.075);
    pXtitle->Draw();pXtitle->cd();
    TLatex *Xtitle = new TLatex(0.462,0.6,"cos #theta");
    Xtitle->SetTextAlign(12);
    Xtitle->SetTextSize(0.75);
    Xtitle->Draw();

    c1D_Data_2->cd();
    TPad *pYtitle = new TPad("pYtitle","pYtitle",0,0,dL,1);
    pYtitle->Draw();pYtitle->cd();
    TLatex *Ytitle = new TLatex(0.4,0.425,"Events");
    Ytitle->PaintLatex(Ytitle->GetX(),Ytitle->GetY(),90,0.6,"Events");
    Ytitle->SetTextAlign(12);
    Ytitle->Draw();
    TLatex axis[4];
    for(int i=0;i<4;i++){axis[i].SetTextSize(0.3);}
    axis[0].DrawLatex(0.71,0.125,"0");
    axis[1].DrawLatex(0.71,0.336,"20");
    axis[2].DrawLatex(0.71,0.125+2*(0.336-0.125),"40");
    axis[3].DrawLatex(0.71,0.125+3*(0.336-0.125),"60");

    c1D_Data_2->SaveAs("~/Desktop/c1D_Data_2.pdf");


    //////////////////////////////////////////////////////////////////////////////////////
    ///Excess only
    //////////////////////////////////////////////////////////////////////////////////////

    TCanvas *c1D_Excess_2 = new TCanvas("c1D_Excess_2","c1D_Excess_2",Length,5000);
    std::vector<TPad*> pE(NbinsEvis);

    TH1D *hExcess_1D;
    for(int i=0;i<NbinsEvis;i++){
        c1D_Excess_2->cd();
        pE[i] = new TPad(Form("pE_%d",i),Form("pE_%d",i),i*dL+dL,0,(i+1)*dL+dL,1);
        pE[i]->Draw();
        pE[i]->cd();
        pE[i]->SetLeftMargin(0.05);
        pE[i]->SetRightMargin(0.05);
        hExcess_1D = (TH1D*)h1D_data_v[i]->Clone("hExcess_1D");
        hExcess_1D->Add(h1D_BG_v[i],-1);
        hExcess_1D->SetFillColor(0);
        hExcess_1D->SetLineColor(1);
        hExcess_1D->SetFillStyle(0);
        hExcess_1D->GetYaxis()->SetRangeUser(-20,70);
        hExcess_1D->Rebin(4);
        hExcess_1D->SetMarkerStyle(20);
        hExcess_1D->Draw("e");

        h1D_BestFit_v[i]->SetLineColor(2);
        h1D_BestFit_v[i]->SetLineWidth(3);
        h1D_BestFit_v[i]->Rebin(4);
        h1D_BestFit_v[i]->Draw("same");

        TLatex *plE = new TLatex(-0.3,71,Form("%.0f MeV",Evis[i]));
        plE->Draw();
        double angle = 60;
        double size = 4*plE->GetTextSize();
        auto text = plE->GetTitle();
        plE->PaintLatex(plE->GetX(),plE->GetY(),angle,size,text);

        TLatex *tXaxis;
        TLine *lZero;
        if(Evis[i] > 700){
            hExcess_1D->GetXaxis()->SetRangeUser(0,1);
            plE->PaintLatex(0.3,plE->GetY(),angle,size,text);
            tXaxis = new TLatex(-0.15,-23," 0       1 ");
            lZero = new TLine(0,0,1,0);
        }
        else{
            tXaxis = new TLatex(-1.15,-23,"-1   0   1 ");
            lZero = new TLine(-1,0,1,0);
        }
        tXaxis->SetTextSize(0.3);
        tXaxis->Draw();
        lZero->SetLineColor(1);
        lZero->SetLineWidth(1);
        lZero->Draw("same");



        pE[i]->Modified();
        pE[i]->Update();
    }

    c1D_Excess_2->cd();
    TPad *pXtitleE = new TPad("pXtitleE","pXtitleE",0,0,1,0.075);
    pXtitleE->Draw();pXtitleE->cd();
    TLatex *XtitleE = new TLatex(0.462,0.6,"cos #theta");
    XtitleE->SetTextAlign(12);
    XtitleE->SetTextSize(0.75);
    XtitleE->Draw();

    c1D_Excess_2->cd();
    TPad *pYtitleE = new TPad("pYtitleE","pYtitleE",0,0,dL,1);
    pYtitleE->Draw();pYtitleE->cd();
    TLatex *YtitleE = new TLatex(0.4,0.425,"Events");
    YtitleE->PaintLatex(YtitleE->GetX(),YtitleE->GetY(),90,0.6,"Events");
    YtitleE->SetTextAlign(12);
    YtitleE->Draw();
    TLatex axisE[5];
    for(int i=0;i<5;i++){axisE[i].SetTextSize(0.3);}
    double originY = 0.125;
    double unitY = 0.289;
    axisE[0].DrawLatex(0.6,originY,"-20");
    axisE[1].DrawLatex(0.6,unitY,"  0");
    axisE[2].DrawLatex(0.6,originY+2*(unitY-originY)," 20");
    axisE[3].DrawLatex(0.6,originY+3*(unitY-originY)," 40");
    axisE[4].DrawLatex(0.6,originY+4*(unitY-originY)," 60");

    c1D_Excess_2->cd();
    TPad *pLegE = new TPad("pLegE","pLegE",(18+0.025)*dL,0.7,(NbinsEvis+1-0.025)*dL,0.87);
    pLegE->Draw();
    pLegE->cd();
    TLegend *leg1DE = new TLegend(0,0,1,1);
    leg1DE->AddEntry(hExcess_1D,"Excess","PL");
    leg1DE->AddEntry(h1D_BestFit_v[1],"Best Fit","L");
    leg1DE->Draw();

    c1D_Excess_2->SaveAs("~/Desktop/c1D_Excess_2.pdf");


    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///1D distribution of the data and MC for slices of cos theta
    ////////////////////////////////////////////////////////////////////////////////////////////////

    double cosTheta_a[NbinsEvis];
    for(int i=0;i<NbinsCosTheta;i++){
        cosTheta_a[i] = -1+(i+0.5)*((2.)/NbinsCosTheta);
    }

    std::vector<TH1D*> h1D_dataY_v = Get1DDistributionY(hData);
    std::vector<TH1D*> h1D_BGY_v = Get1DDistributionY(hMC_BG);
    std::vector<TH1D*> h1D_BestFitY_v = Get1DDistributionY(hBestFit);

    int LengthY = (NbinsEvis+2)*1000;
    TCanvas *c1D_DataY_2 = new TCanvas("c1D_DataY_2","c1D_DataY_2",LengthY,5000);
    std::vector<TPad*> pY(NbinsEvis);
    double dLY = 1./(NbinsCosTheta+2);

    for(int i=0;i<NbinsCosTheta;i++){
        h1D_dataY_v[i]->SetTitle("");
        h1D_dataY_v[i]->GetXaxis()->SetTitleOffset(0.05);
        h1D_dataY_v[i]->GetXaxis()->SetTitleSize(0.36);
        h1D_dataY_v[i]->GetYaxis()->SetRangeUser(0,70);
        h1D_dataY_v[i]->SetLineColor(4);
        h1D_dataY_v[i]->SetLineWidth(1);
        h1D_dataY_v[i]->SetFillColorAlpha(4,0.5);
        h1D_BGY_v[i]->SetLineColor(5);
        h1D_BGY_v[i]->SetLineWidth(1);
        h1D_BGY_v[i]->SetFillColorAlpha(5,0.5);
    }

    for(int i=0;i<NbinsCosTheta;i++){
        c1D_DataY_2->cd();
        pY[i] = new TPad(Form("pY_%d",i),Form("pY_%d",i),i*dLY+dLY,0,(i+1)*dLY+dLY,1);
        pY[i]->Draw();
        pY[i]->cd();
        pY[i]->SetLeftMargin(0.05);
        pY[i]->SetRightMargin(0.05);
        h1D_dataY_v[i]->Draw();
        h1D_BGY_v[i]->Draw("same");
        TLatex *plY = new TLatex(600,71,Form("%.2f",cosTheta_a[i]));
        plY->Draw();
        double angle = 60;
        double size = 4*plY->GetTextSize();
        auto text = plY->GetTitle();
        plY->PaintLatex(plY->GetX(),plY->GetY(),angle,size,text);

        TLatex *tXaxis = new TLatex(-1.15,-2," 200 1250");
        tXaxis->SetTextSize(0.25);
        tXaxis->Draw();

        pY[i]->Modified();
        pY[i]->Update();
    }

    c1D_DataY_2->cd();
    TPad *pLegY = new TPad("pLegY","pLegY",(1+0.035)*dLY,0.6,(7-0.03)*dLY,0.87);
    pLegY->Draw();
    pLegY->cd();
    TLegend *leg1DY = new TLegend(0,0,1,1);
    leg1DY->AddEntry(h1D_dataY_v[NbinsCosTheta-1],"Data","F");
    leg1DY->AddEntry(h1D_BGY_v[NbinsCosTheta-1],"Backgrounds","F");
    leg1DY->Draw();

    c1D_DataY_2->cd();
    TPad *pXtitleY = new TPad("pXtitleY","pXtitleY",0,0,1,0.075);
    pXtitleY->Draw();pXtitleY->cd();
    TLatex *XtitleY = new TLatex(0.462,0.6,"E_{vis} [MeV]");
    XtitleY->SetTextAlign(12);
    XtitleY->SetTextSize(0.75);
    XtitleY->Draw();



    c1D_DataY_2->cd();
    TPad *pYtitleY = new TPad("pYtitleY","pYtitleY",0,0,dLY,1);
    pYtitleY->Draw();pYtitleY->cd();
    TLatex *YtitleY = new TLatex(0.4,0.425,"Events");
    YtitleY->PaintLatex(YtitleY->GetX(),YtitleY->GetY(),90,0.6,"Events");
    YtitleY->SetTextAlign(12);
    YtitleY->Draw();
    TLatex axisY[4];
    for(int i=0;i<4;i++){axisY[i].SetTextSize(0.3);}
    axisY[0].DrawLatex(0.71,0.125,"0");
    axisY[1].DrawLatex(0.71,0.336,"20");
    axisY[2].DrawLatex(0.71,0.125+2*(0.336-0.125),"40");
    axisY[3].DrawLatex(0.71,0.125+3*(0.336-0.125),"60");

    c1D_DataY_2->cd();
    TPad *pTitleTop = new TPad("pTitleTop","pTitleTop",0.01,0.86,dLY,0.95);
    pTitleTop->Draw(); pTitleTop->cd();
    TLatex *YtitleYtop = new TLatex(0,0.5,"cos #theta");
    YtitleYtop->SetTextSize(0.4);
    YtitleYtop->SetTextAlign(12);
    YtitleYtop->Draw();

    c1D_DataY_2->SaveAs("~/Desktop/c1D_DataY_2.pdf");


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////1D distribution of the excess and best fit for slices of cos theta
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////


    TCanvas *c1D_ExcessY_2 = new TCanvas("c1D_ExcessY_2","c1D_ExcessY_2",LengthY,5000);
    std::vector<TPad*> pEY(NbinsEvis);

    TH1D *hExcessY_1D;

    for(int i=0;i<NbinsCosTheta;i++){
        c1D_ExcessY_2->cd();
        pEY[i] = new TPad(Form("pEY_%d",i),Form("pEY_%d",i),i*dLY+dLY,0,(i+1)*dLY+dLY,1);
        pEY[i]->Draw();
        pEY[i]->cd();
        pEY[i]->SetLeftMargin(0.05);
        pEY[i]->SetRightMargin(0.05);

        hExcessY_1D = (TH1D*)h1D_dataY_v[i]->Clone("hExcessY_1D");
        hExcessY_1D->Add(h1D_BGY_v[i],-1);
        hExcessY_1D->SetFillColor(0);
        hExcessY_1D->SetLineColor(1);
        hExcessY_1D->SetFillStyle(0);
        hExcessY_1D->GetYaxis()->SetRangeUser(-20,70);
        hExcessY_1D->Rebin(4);
        hExcessY_1D->SetMarkerStyle(20);
        hExcessY_1D->Draw("e");

        h1D_BestFitY_v[i]->SetLineColor(2);
        h1D_BestFitY_v[i]->SetLineWidth(3);
        h1D_BestFitY_v[i]->Rebin(4);
        h1D_BestFitY_v[i]->Draw("same");

        TLatex *plEY = new TLatex(600,71,Form("%.2f",cosTheta_a[i]));
        plEY->Draw();
        double angle = 60;
        double size = 4*plEY->GetTextSize();
        auto text = plEY->GetTitle();
        plEY->PaintLatex(plEY->GetX(),plEY->GetY(),angle,size,text);

        TLatex *tXaxis = new TLatex(-1.15,-22," 200 1250");
        tXaxis->SetTextSize(0.25);
        tXaxis->Draw();

        TLine *lZero = new TLine(EvisMin,0,EvisMax,0);
        lZero->SetLineWidth(1);
        lZero->SetLineColor(1);
        lZero->Draw("same");

        pEY[i]->Modified();
        pEY[i]->Update();
    }



    c1D_ExcessY_2->cd();
    TPad *pXtitleYE = new TPad("pXtitleYE","pXtitleYE",0,0,1,0.075);
    pXtitleYE->Draw();pXtitleYE->cd();
    TLatex *XtitleYE = new TLatex(0.462,0.6,"E_{vis} [MeV]");
    XtitleYE->SetTextAlign(12);
    XtitleYE->SetTextSize(0.75);
    XtitleYE->Draw();

    c1D_ExcessY_2->cd();
    TPad *pYtitleYE = new TPad("pYtitleYE","pYtitleYE",0,0,dL,1);
    pYtitleYE->Draw();pYtitleYE->cd();
    TLatex *tYtitleYE = new TLatex(0.4,0.425,"Events");
    tYtitleYE->PaintLatex(tYtitleYE->GetX(),tYtitleYE->GetY(),90,0.6,"Events");
    tYtitleYE->SetTextAlign(12);
    tYtitleYE->Draw();
    TLatex axisEY[5];
    for(int i=0;i<5;i++){axisEY[i].SetTextSize(0.3);}
    axisEY[0].DrawLatex(0.6,originY,"-20");
    axisEY[1].DrawLatex(0.6,unitY,"  0");
    axisEY[2].DrawLatex(0.6,originY+2*(unitY-originY)," 20");
    axisEY[3].DrawLatex(0.6,originY+3*(unitY-originY)," 40");
    axisEY[4].DrawLatex(0.6,originY+4*(unitY-originY)," 60");


    c1D_ExcessY_2->cd();
    TPad *pLegEY = new TPad("pLegEY","pLegEY",(1+0.035)*dLY,0.7,(7-0.03)*dLY,0.87);
    pLegEY->Draw();
    pLegEY->cd();
    TLegend *leg1DEY = new TLegend(0,0,1,1);
    leg1DEY->AddEntry(hExcessY_1D,"Excess","PL");
    leg1DEY->AddEntry(h1D_BestFitY_v[NbinsCosTheta-1],"Best Fit","L");
    leg1DEY->Draw();

    c1D_ExcessY_2->cd();
    pTitleTop->Draw();

    c1D_ExcessY_2->SaveAs("~/Desktop/c1D_ExcessY_2.pdf");
}
