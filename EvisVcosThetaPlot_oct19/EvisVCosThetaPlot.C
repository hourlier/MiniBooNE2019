double EvisVcosTheta_data[22][20] = {
    {51,45,35,37,36,32,22,15,23,35,22,26,20,16,23,22,19,14,18,15},
    {60,43,38,25,25,37,29,24,16,20,24,23,16,18,24,14, 8, 9,13,11},
    {49,44,39,29,24,20,23,14,22,16,13, 9,13, 8,10, 5,12, 7, 7, 4},
    {48,30,40,22,21,26,20,11,27,13,16, 6, 2, 7, 2, 4, 4, 4, 2, 2},
    {48,24,41,29,25,18,11,14, 9,13, 3, 6, 3, 3, 4, 3, 2, 0, 0, 0},
    {29,39,25,17,16,21,11, 5,11,10,11, 3, 3, 2, 3, 0, 1, 1, 0, 0},
    {39,20,22,12,14,16, 7, 5, 5, 3, 4, 0, 3, 1, 1, 0, 0, 0, 0, 0},
    {25,18,19,20,12,17, 6, 2, 3, 2, 2, 0, 0, 1, 0, 0, 1, 0, 0, 0},
    {30,21,17,10, 8,12, 3, 3, 1, 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {26,18,17, 5, 9, 7, 5, 1, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {29,21,17, 4, 8, 3, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {23,25,16, 6,11, 1, 2, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {23,11,12,11, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {20,12, 8, 6, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {22,22,11, 4, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {22,20, 8, 2, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {24,15, 7, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {20,13, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {24, 8, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {12,11, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {20, 5, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {18,12, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

double EvisVcosTheta_MC[22][20] = {
    {38.7, 29.9, 30.7, 22.4, 17.8, 23.7, 20.1, 23.3, 18.2, 18.2, 12.5, 12.7, 17.8, 11.7, 13  , 14.5, 11.5,  7.9,  9.6, 12  },
    {41.9, 31.2, 23.6, 23.5, 25  , 19.3, 28.7, 18.5, 20.7, 22.3, 15.9, 15.7, 10.3,  5.8,  6.4, 14.6, 11.4,  8.3,  6.9,  8.2},
    {37.8, 24  , 22.9, 24  , 19.7, 18.5, 17.5, 10.1, 14.8, 13.8, 14.1, 14.4,  9.1,  9.4,  5.3,  5.6,  6.2,  6.4,  1  ,  4.2},
    {38.3, 26.9, 23.4, 29.4, 20.6, 14.5, 22.7, 13.2, 10.8,  9.5,  5.1, 12.8,  4.9,  7.3,  6.6,  3.7,  4.2,  3  ,  4.1,  0.8},
    {28.4, 23.1, 21.6, 20.2, 18.9, 16.8, 16.8,  8.3,  6.2,  8.7,  3.7,  2.3,  4.4,  2.5,  3.2,  1.6,  1.2,  1.3,  1.2,  1.5},
    {29.6, 28.3, 19.7, 19.6, 14.5, 17.1,  8.5,  8.8,  7.1,  3.1,  6.3,  4  ,  1.4,  3.5,  0.6,  1.8,  0.6,  0  ,  0  ,  0  },
    {28.4, 20.2, 19.6, 15.8, 17  ,  9.9, 12.1,  6.2,  4.6,  5.2,  2.3,  0.7,  0  ,  0  ,  0  ,  0  ,  0.2,  0  ,  0  ,  0  },
    {16.4, 18.8, 19.4, 13.1, 15.1,  8  ,  5  ,  5.6,  5.1,  2.2,  1.6,  1  ,  0.4,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0.7},
    {18.4, 13.9, 20.4, 18.2,  8.1,  8.6,  7  ,  1.9,  1  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  1.1},
    {25.5, 16.6, 13.7,  9.5, 10.5,  3.9,  4  ,  2.5,  2.5,  0.4,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0.6},
    {15.8, 22.8, 14.1,  9.1,  7.4,  3.5,  2.4,  0  ,  1.7,  0  ,  0.4,  0  ,  0  ,  0  ,  0  ,  0.6,  0  ,  0  ,  0  ,  0  },
    { 8.9, 16.7, 19.7,  9.5,  7.6,  2.2,  0.9,  0  ,  0  ,  1.2,  0  ,  0  ,  0  ,  0  ,  0.4,  0  ,  0  ,  0  ,  0  ,  0.6},
    {18  , 20.5,  9.7,  7.4,  4.5,  1.8,  0.4,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0.5},
    {22  , 14.6, 10.7,  7.6,  3.1,  0  ,  0  ,  0.4,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  },
    {18.1, 18.9,  9.3,  4.8,  1.4,  0.4,  0  ,  0.4,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  },
    {17.9, 15.7,  7.9,  1.9,  1.4,  0  ,  0.6,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0.4},
    {19.3, 12.8,  8.3,  2.6,  0.8,  0.7,  0.4,  0  ,  0  ,  0  ,  0.4,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  },
    {17.6, 11.8,  4.6,  1.2,  0  ,  0.4,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  },
    {13.3, 10.1,  4  ,  1  ,  0  ,  0.4,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  },
    {20.3, 12.4,  2.2,  2.9,  0.5,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  },
    {12.2,  7.5,  0.9,  0.4,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  },
    {18.8,  7.6,  2  ,  0.8,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  }
};

TH2D *hEvisVcosTheta_data = new TH2D("hEvisVcosTheta_data","Data;Evis (MeV);cos#theta",22,150,1250,20,-1,1);
TH2D *hEvisVcosTheta_BG = new TH2D("hEvisVcosTheta_BG","Prediction;Evis (MeV);cos#theta",22,150,1250,20,-1,1);
TH2D *hEvisVcosTheta_Excess = new TH2D("hEvisVcosTheta_Excess","Data-Prediction;Evis (MeV);cos#theta",22,150,1250,20,-1,1);

TH1D *h1D_data = new TH1D("h1D_data","h1D_data;cos #theta",440,0,1);
TH1D *h1D_BG = new TH1D("h1D_BG","h1D_BG;cos #theta",440,0,1);
TH1D *h1D_Excess = new TH1D("h1D_Excess","h1D_Excess;cos #theta",440,0,1);

void fig14(){
    for(int i=0;i<22;i++){
        for(int j=0;j<20;j++){
            //hEvisVcosTheta_data->SetBinContent(i+1,20-j,EvisVcosTheta_data[i][j]);
            hEvisVcosTheta_data->SetBinContent(i+1,j+1,EvisVcosTheta_data[i][19-j]);
        }
    }
}

void fig15(){
    for(int i=0;i<22;i++){
        for(int j=0;j<20;j++){
            hEvisVcosTheta_BG->SetBinContent(i+1,j+1,EvisVcosTheta_MC[i][19-j]);
        }
    }
}

void plot2Dexcess(){
    for(int i=0;i<22;i++){
        for(int j=0;j<20;j++){
            if(hEvisVcosTheta_data->GetBinContent(i+1,j+1) - hEvisVcosTheta_BG->GetBinContent(i+1,j+1) != 0){
                hEvisVcosTheta_Excess->SetBinContent(i+1,j+1,hEvisVcosTheta_data->GetBinContent(i+1,j+1)-hEvisVcosTheta_BG->GetBinContent(i+1,j+1));
            }
        }
    }
}


std::vector<TH1D*> h1D_data_v(22);
std::vector<TH1D*> h1D_BG_v(22);

void plot1DEvisCosT_data_2(){
    TH1D *h1D_data_2;
    for(int i=0;i<22;i++){
        h1D_data_2 = new TH1D(Form("h1D_data_2_%d",i),Form("h1D_data_2_%d",i),20,-1,1);
        for(int j=0;j<20;j++){
            h1D_data_2->SetBinContent(j+1,EvisVcosTheta_data[i][19-j]);
        }
        h1D_data_v[i] = h1D_data_2;
    }
}

void plot1DEvisCosT_BG_2(){
    TH1D *h1D_BG_2;
    for(int i=0;i<22;i++){
        h1D_BG_2 = new TH1D(Form("h1D_BG_2_%d",i),Form("h1D_BG_2_%d",i),20,-1,1);
        for(int j=0;j<20;j++){
            h1D_BG_2->SetBinContent(j+1,EvisVcosTheta_MC[i][19-j]);
        }
        h1D_BG_v[i] = h1D_BG_2;
    }
}

void plot1DEvisCosT_data(){
    for(int i=0;i<22;i++){
        for(int j=0;j<20;j++){
            h1D_data->SetBinContent(20*i+j,EvisVcosTheta_data[i][19-j]);
        }
    }
}

void plot1DEvisCosT_BG(){
    for(int i=0;i<22;i++){
        for(int j=0;j<20;j++){
            h1D_BG->SetBinContent(20*i+j,EvisVcosTheta_MC[i][19-j]);
        }
    }
}

void plot1DEvisCosT_Excess(){
    for(int i=0;i<440;i++){
        h1D_Excess->SetBinContent(i+1,h1D_data->GetBinContent(i+1)-h1D_BG->GetBinContent(i+1));
    }
}

void EvisVCosThetaPlot(){
    gStyle->SetOptStat(0);
    //gStyle->SetPalette(kInvertedDarkBodyRadiator);
    //gStyle->SetPalette(kThermometer);
    gStyle->SetPalette(kTemperatureMap);

    double Evis[22];
    for(int i=0;i<22;i++){
        Evis[i] = 150+(i+0.5)*((1250-150)/22.);
    }

    fig14();
    fig15();
    plot2Dexcess();


    TCanvas *cData = new TCanvas("cData","cData",800,600);
    hEvisVcosTheta_data->SetContour(100);
    hEvisVcosTheta_data->Draw("colz");
    cData->SaveAs("../figures/oct19/2020_06_16/cData.pdf");


    TCanvas *cBG = new TCanvas("cBG","cBG",800,600);
    hEvisVcosTheta_BG->GetZaxis()->SetRangeUser(0,60);
    hEvisVcosTheta_BG->Draw("colz");
    hEvisVcosTheta_BG->SetContour(100);
    cBG->SaveAs("../figures/oct19/2020_06_16/cBG.pdf");

    TCanvas *cExcess = new TCanvas("cExcess","cExcess",800,600);
    //hEvisVcosTheta_Excess->GetZaxis()->SetRangeUser(0,60);
    hEvisVcosTheta_Excess->Draw("colz");
    hEvisVcosTheta_Excess->SetContour(100);
    cExcess->SaveAs("../figures/oct19/2020_06_16/cExcess.pdf");

    TCanvas *c1D_data = new TCanvas("c1D_data","c1D_data",800,600);
    plot1DEvisCosT_data();
    h1D_data->Draw();
    //TLatex *pl = new TLatex(0.5,35,Form("%.0f",Evis[0]));
    //pl->Draw();
    c1D_data->SaveAs("../figures/oct19/2020_06_16/c1D_data.pdf");
    //return;

    TCanvas *c1D_BG = new TCanvas("c1D_BG","c1D_BG",800,600);
    plot1DEvisCosT_BG();
    h1D_BG->Draw();
    c1D_BG->SaveAs("../figures/oct19/2020_06_16/c1D_BG.pdf");

    TCanvas *c1D_Excess = new TCanvas("c1D_Excess","c1D_Excess",800,600);
    plot1DEvisCosT_Excess();
    h1D_Excess->Draw();
    c1D_Excess->SaveAs("../figures/oct19/2020_06_16/c1D_Excess.pdf");

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Draw multi-pannel 1D histogram for data and prediction
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int Length = 24*1000;
    TCanvas *c1D_Data_2 = new TCanvas("c1D_Data_2","c1D_Data_2",Length,5000);
    plot1DEvisCosT_data_2();
    plot1DEvisCosT_BG_2();
    std::vector<TPad*> p(22);
    double dL = 1./24.;

    for(int i=0;i<22;i++){
        h1D_data_v[i]->SetTitle("");
        h1D_data_v[i]->GetXaxis()->SetTitleOffset(0.05);
        h1D_data_v[i]->GetXaxis()->SetTitleSize(0.36);
        h1D_data_v[i]->GetYaxis()->SetRangeUser(0,70);
        h1D_data_v[i]->SetLineColor(1);
        h1D_data_v[i]->SetLineWidth(1);
        h1D_data_v[i]->SetFillColorAlpha(4,0.5);
        h1D_BG_v[i]->SetLineColor(1);
        h1D_BG_v[i]->SetLineWidth(1);
        h1D_BG_v[i]->SetFillColorAlpha(5,0.5);

    }

    for(int i=0;i<22;i++){
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

        TLatex *tXaxis = new TLatex(-1.15,-2,"-1   0   1 ");
        tXaxis->SetTextSize(0.3);
        tXaxis->Draw();

        p[i]->Modified();
        p[i]->Update();
    }

    c1D_Data_2->cd();
    TPad *pLeg = new TPad("pLeg","pLeg",17*dL,0.7,(23-0.025)*dL,0.87);
    pLeg->Draw();
    pLeg->cd();
    TLegend *leg1D = new TLegend(0,0,1,1);
    leg1D->AddEntry(h1D_data_v[21],"Data","F");
    leg1D->AddEntry(h1D_BG_v[21],"Prediction","F");
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

    c1D_Data_2->SaveAs("../figures/oct19/2020_06_16/c1D_Data_2.png");
    c1D_Data_2->SaveAs("../figures/oct19/2020_06_16/c1D_Data_2.jpg");
    c1D_Data_2->SaveAs("../figures/oct19/2020_06_16/c1D_Data_2.pdf");


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// same but with the excess
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    TCanvas *c1D_Excess_2 = new TCanvas("c1D_Excess_2","c1D_Excess_2",Length,5000);
    std::vector<TPad*> pE(22);

    TH1D *hExcess;
    for(int i=0;i<22;i++){
        c1D_Excess_2->cd();
        pE[i] = new TPad(Form("pE_%d",i),Form("pE_%d",i),i*dL+dL,0,(i+1)*dL+dL,1);
        pE[i]->Draw();
        pE[i]->cd();
        pE[i]->SetLeftMargin(0.05);
        pE[i]->SetRightMargin(0.05);
        hExcess = (TH1D*)h1D_data_v[i]->Clone("hExcess");
        hExcess->Add(h1D_BG_v[i],-1);
        hExcess->GetYaxis()->SetRangeUser(-10,30);
        hExcess->Draw();

        TLatex *plE = new TLatex(-0.3,71,Form("%.0f MeV",Evis[i]));
        plE->Draw();
        double angle = 60;
        double size = 4*plE->GetTextSize();
        auto text = plE->GetTitle();
        plE->PaintLatex(plE->GetX(),plE->GetY(),angle,size,text);

        TLatex *tXaxis = new TLatex(-1.15,-11,"-1   0   1 ");
        tXaxis->SetTextSize(0.3);
        tXaxis->Draw();

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
    axisE[0].DrawLatex(0.6,0.125,"-10");
    axisE[1].DrawLatex(0.6,0.31,"  0");
    axisE[2].DrawLatex(0.6,0.125+2*(0.31-0.125)," 10");
    axisE[3].DrawLatex(0.6,0.125+3*(0.31-0.125)," 20");
    axisE[4].DrawLatex(0.6,0.125+4*(0.31-0.125)," 30");

    c1D_Excess_2->SaveAs("../figures/oct19/2020_06_16/c1D_Excess_2.png");
    c1D_Excess_2->SaveAs("../figures/oct19/2020_06_16/c1D_Excess_2.jpg");
    c1D_Excess_2->SaveAs("../figures/oct19/2020_06_16/c1D_Excess_2.pdf");
}


