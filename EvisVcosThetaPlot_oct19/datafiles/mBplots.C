#include "DataClass.C"
#include "FullOscClass.C"
#include "FullOscClass_nuebar.C"
#include "MCclass.C"
#include "dirtMC.C"

TFile *fFitOutput = TFile::Open("../../Fitter/output/fit_oct19_histNunubar.root");
//TFile *fFitOutput = TFile::Open("../../Fitter/output/fit_oct19_histNunubar_kengSystematics.root");


int kDirtColor = kOrange+3;
const int kData    = 0;
const int kOtherMC = 1;
const int kDirt    = 2;
const int kFullOsc = 3;

const double BFst = 0.8072;
const double BFdm = 0.043;

string HistNames[10] = {"Others","dirt","#Delta #rightarrow N#gamma","#pi^{0} mis-ID","#nu_{e} from K^{0}","#nu_{e} from K^{#pm}","#nu_{e} from #mu^{#pm}","#nu_{x}-e elastic scattering","#nu_{#mu}, #bar#nu_{#mu}","#pi^{0} coherent"};
int PlotColors[10] = {kGray,kDirtColor,kYellow+1,kRed-6,kGreen-5,kGreen-6,kGreen-8,1,2,4};
double POTlist[4];

DataClass data;
FullOscClass fullOsc;
FullOscClass_nuebar fullOsc_nuebar;
MCclass OtherMC;
dirtMC DirtMC;

TGraph *g1sigma = new TGraph();
TGraph *g4sigma = new TGraph();
TGraph *g3sigma = new TGraph();
TGraph *g90pc   = new TGraph();
TGraph *g95pc   = new TGraph();
TGraph *g99pc   = new TGraph();
std::vector<std::vector<double> > OneSigmaContour;

TCanvas *cCLgrid = new TCanvas("cCLgrid","cCLgrid",600,600);


std::vector<std::vector<TH1D*> > Project1DX(std::vector<TH2D*> h){
    std::vector<std::vector<TH1D*> > h1DX_v;
    if(h.size() == 0)return h1DX_v;
    for(int i=0;i<h.size();i++){
        std::vector<TH1D*> h1DX(h[i]->GetNbinsX());
        for(int j=0;j<h[i]->GetNbinsX();j++){
            h1DX[j] = new TH1D(Form("h1DX_%s_%d",h[i]->GetName(),j),Form(";%s",h[i]->GetYaxis()->GetTitle()),h[i]->GetNbinsY(),h[i]->GetYaxis()->GetXmin(),h[i]->GetYaxis()->GetXmax());
            for(int k=0;k<h[i]->GetNbinsY();k++){
                h1DX[j]->SetBinContent(k+1,h[i]->GetBinContent(j+1,k+1));
            }
        }
        h1DX_v.push_back(h1DX);
    }
    return h1DX_v;
}
std::vector<std::vector<TH1D*> > Project1DX(TH2D *hdata, TH2D *hdirt, TH2D *hfullOsc, std::vector<TH2D*> hMC){
    std::vector<TH2D*> h;
    h.push_back(hdata);
    h.push_back(hMC[0]);
    h.push_back(hdirt);
    for(int i=1;i<hMC.size();i++){
        h.push_back(hMC[i]);
    }
    h.push_back(hfullOsc);
    return Project1DX(h);
}
std::vector<std::vector<TH1D*> > Project1DY(std::vector<TH2D*> h){
    std::vector<std::vector<TH1D*> > h1DY_v;
    if(h.size() == 0)return h1DY_v;
    //for(int i=0;i<h.size();i++){h[i]->Rebin2D(5,5);}
    for(int i=0;i<h.size();i++){
        std::vector<TH1D*> h1DY(h[i]->GetNbinsY());
        for(int j=0;j<h[i]->GetNbinsY();j++){
            h1DY[j] = new TH1D(Form("h1DY_%s_%d",h[i]->GetName(),j),Form(";%s",h[i]->GetXaxis()->GetTitle()),h[i]->GetNbinsY(),h[i]->GetXaxis()->GetXmin(),h[i]->GetXaxis()->GetXmax());
            for(int k=0;k<h[i]->GetNbinsX();k++){
                h1DY[j]->SetBinContent(k+1,h[i]->GetBinContent(k+1,j+1));
            }
        }
        h1DY_v.push_back(h1DY);
    }
    return h1DY_v;
}
std::vector<std::vector<TH1D*> > Project1DY(TH2D *hdata, TH2D *hdirt, TH2D *hfullOsc, std::vector<TH2D*> hMC){
    std::vector<TH2D*> h;
    h.push_back(hdata);
    h.push_back(hMC[0]);
    h.push_back(hdirt);
    for(int i=1;i<hMC.size();i++){
        h.push_back(hMC[i]);
    }
    h.push_back(hfullOsc);
    return Project1DY(h);
}

void Plot1DXStack(std::vector<std::vector<TH1D*> > h1DX_v, std::vector<std::vector<TH1D*> > h1DY_v){

    TCanvas *c1DX = new TCanvas(Form("c1DX_%s",h1DX_v[0][0]->GetName()),Form("c1DX_%s",h1DX_v[0][0]->GetName()),5000,2000);

    std::vector<TPad*> padX(h1DX_v[0].size()+1);
    double dL = 1./(h1DX_v[0].size()+2);

    std::vector<THStack*> hStack_v(h1DX_v[0].size());
    for(int j=0;j<h1DX_v[0].size();j++){
        hStack_v[j] = new THStack(Form("hStack_%d",j),"");
        for(int i=1;i<h1DX_v.size()-4;i++){
            h1DX_v[i][j]->SetLineColor(1);
            h1DX_v[i][j]->SetLineWidth(0);
            h1DX_v[i][j]->SetFillColor(PlotColors[i-1]);
            hStack_v[j]->Add(h1DX_v[i][j]);
        }
    }
    double ThisMin = 1000;
    double ThisMax = -1000;
    for(int j=0;j<h1DX_v[0].size();j++){
        for(int i= 0;i<h1DX_v[0][j]->GetNbinsX();i++){
            if(h1DX_v[0][j]->GetBinContent(i+1)+h1DX_v[0][j]->GetBinError(i+1)>ThisMax)ThisMax = h1DX_v[0][j]->GetBinContent(i+1)+h1DX_v[0][j]->GetBinError(i+1);
            if(h1DX_v[0][j]->GetBinContent(i+1)-h1DX_v[0][j]->GetBinError(i+1)<ThisMin)ThisMin = h1DX_v[0][j]->GetBinContent(i+1)-h1DX_v[0][j]->GetBinError(i+1);
        }
    }

    for(int j=0;j<h1DX_v[0].size();j++){
        c1DX->cd();
        padX[j+1] = new TPad(Form("c1DX_%s_%d",h1DX_v[0][0]->GetName(),j),Form("c1DX_%s_%d",h1DX_v[0][0]->GetName(),j),(j+1)*dL,0,(j+2)*dL,1);
        padX[j+1]->Draw();
        padX[j+1]->cd();
        padX[j+1]->SetLeftMargin(0.01);
        padX[j+1]->SetRightMargin(0.01);
        padX[j+1]->SetTopMargin(0.25);
        padX[j+1]->SetBottomMargin(0.13);
        h1DX_v[0][j]->SetMarkerStyle(20);
        h1DX_v[0][j]->SetMarkerSize(0.1);
        h1DX_v[0][j]->SetLineColor(1);
        h1DX_v[0][j]->SetLineWidth(1);
        h1DX_v[0][j]->Draw("e");
        h1DX_v[0][j]->GetYaxis()->SetRangeUser(ThisMin-5,1.05*ThisMax);
        hStack_v[j]->Draw("same hist");

        h1DX_v[h1DX_v.size()-4][j]->SetLineColor(1);
        h1DX_v[h1DX_v.size()-4][j]->SetLineStyle(3);
        h1DX_v[h1DX_v.size()-4][j]->SetLineWidth(2);
        h1DX_v[h1DX_v.size()-4][j]->SetFillStyle(3004);
        h1DX_v[h1DX_v.size()-4][j]->SetFillColor(1);
        h1DX_v[h1DX_v.size()-3][j]->SetLineColor(2);
        h1DX_v[h1DX_v.size()-3][j]->SetLineStyle(3);
        h1DX_v[h1DX_v.size()-3][j]->SetLineWidth(2);
        h1DX_v[h1DX_v.size()-3][j]->SetFillStyle(3004);
        h1DX_v[h1DX_v.size()-3][j]->SetFillColor(2);
        h1DX_v[h1DX_v.size()-2][j]->SetLineColor(4);
        h1DX_v[h1DX_v.size()-2][j]->SetLineStyle(3);
        h1DX_v[h1DX_v.size()-2][j]->SetLineWidth(1);
        h1DX_v[h1DX_v.size()-2][j]->SetFillStyle(3005);
        h1DX_v[h1DX_v.size()-2][j]->SetFillColor(4);
        h1DX_v[h1DX_v.size()-4][j]->Draw("hist same");
        //h1DX_v[h1DX_v.size()-3][j]->Draw("hist same");
        //h1DX_v[h1DX_v.size()-2][j]->Draw("hist same");


        h1DX_v[0][j]->Draw("same e");

        TLatex *tXaxis = new TLatex(h1DX_v[0][j]->GetXaxis()->GetXmin(),h1DX_v[0][j]->GetYaxis()->GetXmin()-10,Form(" %.0f     %.0f",h1DX_v[0][j]->GetXaxis()->GetXmin(),h1DX_v[0][j]->GetXaxis()->GetXmax()));
        tXaxis->SetTextSize(0.3);
        tXaxis->Draw();
    }
    c1DX->cd();
    padX[0] = new TPad(Form("c1DX_%s_axis",h1DX_v[0][0]->GetName()),Form("c1DX_%s_axis",h1DX_v[0][0]->GetName()),0,0,dL,1);
    padX[0]->Draw();
    padX[0]->cd();
    TGaxis *axis1 = new TGaxis(1,padX[1]->GetBottomMargin(),1,1-padX[1]->GetTopMargin(),ThisMin-5,1.05*ThisMax,105,"");
    axis1->SetName("axis1");
    axis1->SetLabelSize(0.2);
    axis1->SetLabelOffset(0.1);
    axis1->Draw();

    c1DX->cd();

    TPad *padXaxis = new TPad(Form("p1DX_%s_Xaxis",h1DX_v[0][0]->GetName()),Form("p1DX_%s_Xaxis",h1DX_v[0][0]->GetName()),0,0,1,padX[1]->GetBottomMargin());
    padXaxis->Draw();
    padXaxis->cd();
    TGaxis *axis12 = new TGaxis(dL,1,2*dL,1,h1DX_v[0][0]->GetXaxis()->GetXmin(),h1DX_v[0][0]->GetXaxis()->GetXmax(),105,"");
    axis12->SetName("axis12");
    axis12->SetLabelSize(0.1);
    axis12->SetLabelOffset(0.05);
    axis12->Draw();

    TLatex *lTitle = new TLatex(0.3,0.55,"Data compared to stacked backgrounds");
    lTitle->SetTextSize(0.4);
    lTitle->SetTextAlign(12);
    lTitle->Draw();

    TLatex *lVariable = new TLatex(0.301,0.2,Form("%s for slices of %s", h1DX_v[0][0]->GetXaxis()->GetTitle(), h1DY_v[0][0]->GetXaxis()->GetTitle()));
    lVariable->SetTextSize(0.2);
    lVariable->SetTextAlign(12);
    lVariable->Draw();

    c1DX->cd();
    TPad *padXaxisTop = new TPad(Form("p1DX_%s_XaxisTop",h1DX_v[0][0]->GetName()),Form("p1DX_%s_XaxisTop",h1DX_v[0][0]->GetName()),0,1-padX[1]->GetTopMargin()+0.01,1,1);
    padXaxisTop->Draw();
    padXaxisTop->cd();
    for(int i=0;i<h1DX_v[0].size();i++){
        TLatex *taxisTopX = new TLatex((i+1)*dL,0,
                                       Form("%4.0f-%4.0f MeV",
                                            h1DY_v[0][0]->GetXaxis()->GetXmin()+i*h1DY_v[0][0]->GetBinWidth(1),
                                            h1DY_v[0][0]->GetXaxis()->GetXmin()+(i+1)*h1DY_v[0][0]->GetBinWidth(1)));
        taxisTopX->SetTextSize(0.12);
        taxisTopX->Draw();
        taxisTopX->PaintLatex(taxisTopX->GetX(),taxisTopX->GetY(),60,taxisTopX->GetTextSize(),taxisTopX->GetTitle());
    }

    c1DX->cd();
    TPad *pLegend = new TPad("pLegend","pLengend",(0.3*h1DX_v[0].size()+1)*dL,0.6,(h1DX_v[0].size()+1)*dL,0.75);
    pLegend->Draw();
    pLegend->cd();
    TLegend *legStackX = new TLegend(0,0,1,1);
    legStackX->SetNColumns(3);
    legStackX->AddEntry(h1DX_v[0][1],"data","LP");
    for(int i=1;i<h1DX_v.size()-3;i++){
        legStackX->AddEntry(h1DX_v[i][1],Form("%s",HistNames[i-1].c_str()),"F");
    }
    legStackX->Draw();
    c1DX->SaveAs(Form("c1DX_%s.pdf",h1DX_v[0][0]->GetName()));
}
void Plot1DXExcessBF(std::vector<std::vector<TH1D*> > h1DX_v, std::vector<std::vector<TH1D*> > h1DY_v){
    double dL = 1./(h1DX_v[0].size()+2);
    double ThisMin = 1000;
    double ThisMax = -1000;

    TCanvas *c1DXExcess = new TCanvas(Form("c1DXExcess_%s",h1DX_v[0][0]->GetName()),Form("c1DXExcess_%s",h1DX_v[0][0]->GetName()),5000,2000);
    std::vector<TH1D*> hExcess(h1DX_v[0].size());
    std::vector<TPad*> padXExcess(h1DX_v[0].size()+1);
    for(int j=0;j<h1DX_v[0].size();j++){
        hExcess[j] = (TH1D*) h1DX_v[0][j]->Clone("hExcess");
        for(int i=1;i<h1DX_v.size()-4;i++){
            hExcess[j]->Add(h1DX_v[i][j],-1);
        }
    }

    for(int j=0;j<h1DX_v[0].size();j++){
        for(int i= 0;i<h1DX_v[0][j]->GetNbinsX();i++){
            if(hExcess[j]->GetBinContent(i+1)+hExcess[j]->GetBinError(i+1)>ThisMax)ThisMax = hExcess[j]->GetBinContent(i+1)+hExcess[j]->GetBinError(i+1);
            if(hExcess[j]->GetBinContent(i+1)-hExcess[j]->GetBinError(i+1)<ThisMin)ThisMin = hExcess[j]->GetBinContent(i+1)-hExcess[j]->GetBinError(i+1);
        }
    }
    for(int j=0;j<h1DX_v[0].size();j++){
        c1DXExcess->cd();
        padXExcess[j+1] = new TPad(Form("padXExcess_%s_%d",h1DX_v[0][0]->GetName(),j),Form("padXExcess_%s_%d",h1DX_v[0][0]->GetName(),j),(j+1)*dL,0,(j+2)*dL,1);
        padXExcess[j+1]->Draw();
        padXExcess[j+1]->cd();
        padXExcess[j+1]->SetLeftMargin(0.01);
        padXExcess[j+1]->SetRightMargin(0.01);
        padXExcess[j+1]->SetTopMargin(0.25);
        padXExcess[j+1]->SetBottomMargin(0.13);

        hExcess[j]->GetYaxis()->SetRangeUser(ThisMin-5,ThisMax+5);
        hExcess[j]->Draw("e");
        h1DX_v[h1DX_v.size()-1][j]->Draw("same hist");
    }
    c1DXExcess->cd();
    padXExcess[0] = new TPad(Form("padXExcess_%s_axis",h1DX_v[0][0]->GetName()),Form("padXExcess_%s_axis",h1DX_v[0][0]->GetName()),0,0,dL,1);
    padXExcess[0]->Draw();
    padXExcess[0]->cd();
    TGaxis *axis2 = new TGaxis(1,padXExcess[1]->GetBottomMargin(),1,1-padXExcess[1]->GetTopMargin(),ThisMin-5,ThisMax+5,105,"");
    axis2->SetName("axis2");
    axis2->SetLabelSize(0.2);
    axis2->SetLabelOffset(0.1);
    axis2->Draw();

    c1DXExcess->cd();
    TPad *padXaxis = new TPad(Form("p1DX_%s_Xaxis",h1DX_v[0][0]->GetName()),Form("p1DX_%s_Xaxis",h1DX_v[0][0]->GetName()),0,0,1,padXExcess[1]->GetBottomMargin());
    padXaxis->Draw();
    padXaxis->cd();
    TGaxis *axis12 = new TGaxis(dL,1,2*dL,1,h1DX_v[0][0]->GetXaxis()->GetXmin(),h1DX_v[0][0]->GetXaxis()->GetXmax(),105,"");
    axis12->SetName("axis12");
    axis12->SetLabelSize(0.1);
    axis12->SetLabelOffset(0.05);
    axis12->Draw();

    TLatex *lTitle = new TLatex(0.3,0.55,"Excess compared to best fit");
    lTitle->SetTextSize(0.4);
    lTitle->SetTextAlign(12);
    lTitle->Draw();

    TLatex *lVariable = new TLatex(0.301,0.2,Form("%s for slices of %s", h1DX_v[0][0]->GetXaxis()->GetTitle(), h1DY_v[0][0]->GetXaxis()->GetTitle()));
    lVariable->SetTextSize(0.2);
    lVariable->SetTextAlign(12);
    lVariable->Draw();

    c1DXExcess->cd();
    TPad *padXaxisTop = new TPad(Form("p1DX_%s_XaxisTop",h1DX_v[0][0]->GetName()),Form("p1DX_%s_XaxisTop",h1DX_v[0][0]->GetName()),0,1-padXExcess[1]->GetTopMargin()+0.01,1,1);
    padXaxisTop->Draw();
    padXaxisTop->cd();
    for(int i=0;i<h1DX_v[0].size();i++){
        TLatex *taxisTopX = new TLatex((i+1)*dL,0,
                                       Form("%4.0f-%4.0f MeV",
                                            h1DY_v[0][0]->GetXaxis()->GetXmin()+i*h1DY_v[0][0]->GetBinWidth(1),
                                            h1DY_v[0][0]->GetXaxis()->GetXmin()+(i+1)*h1DY_v[0][0]->GetBinWidth(1)));
        taxisTopX->SetTextSize(0.12);
        taxisTopX->Draw();
        taxisTopX->PaintLatex(taxisTopX->GetX(),taxisTopX->GetY(),60,taxisTopX->GetTextSize(),taxisTopX->GetTitle());
    }

    c1DXExcess->SaveAs(Form("c1DXExcess_%s.pdf",h1DX_v[0][0]->GetName()));
}
void Plot1DXExcessPion(std::vector<std::vector<TH1D*> > h1DX_v, std::vector<std::vector<TH1D*> > h1DY_v){
    double dL = 1./(h1DX_v[0].size()+2);
    std::vector<TH1D*> hExcess(h1DX_v[0].size());
    for(int j=0;j<h1DX_v[0].size();j++){
        hExcess[j] = (TH1D*) h1DX_v[0][j]->Clone("hExcess");
        for(int i=1;i<h1DX_v.size()-4;i++){
            hExcess[j]->Add(h1DX_v[i][j],-1);
        }
    }
    double ThisMin = 1000;
    double ThisMax = -1000;
    for(int j=0;j<h1DX_v[0].size();j++){
        for(int i= 0;i<h1DX_v[0][j]->GetNbinsX();i++){
            if(hExcess[j]->GetBinContent(i+1)+hExcess[j]->GetBinError(i+1)>ThisMax)ThisMax = hExcess[j]->GetBinContent(i+1)+hExcess[j]->GetBinError(i+1);
            if(hExcess[j]->GetBinContent(i+1)-hExcess[j]->GetBinError(i+1)<ThisMin)ThisMin = hExcess[j]->GetBinContent(i+1)-hExcess[j]->GetBinError(i+1);
        }
    }

    TCanvas *c1DXExcessPi0scaled = new TCanvas(Form("c1DXExcessPi0scaled_%s",h1DX_v[0][0]->GetName()),Form("c1DXExcessPi0scaled_%s",h1DX_v[0][0]->GetName()),5000,2000);
    std::vector<TH1D*> hExcessPi0scaled(h1DX_v[0].size());
    std::vector<TPad*> padXExcessPi0scaled(h1DX_v[0].size()+1);
    double IntPi0 = 0;
    double IntExcess = 0;
    for(int j=0;j<h1DX_v[0].size();j++){
        hExcessPi0scaled[j] = (TH1D*) h1DX_v[4][j]->Clone("hExcessPi0scaled");
        for(int i = 0;i<hExcessPi0scaled[j]->GetNbinsX();i++){
            IntPi0+=hExcessPi0scaled[j]->GetBinContent(i+1);
            IntExcess+=hExcess[j]->GetBinContent(i+1);
        }
    }
    for(int j=0;j<h1DX_v[0].size();j++){
        c1DXExcessPi0scaled->cd();
        padXExcessPi0scaled[j+1] = new TPad(Form("padXExcessPi0scaled_%s_%d",h1DX_v[0][0]->GetName(),j),Form("padXExcessPi0scaled_%s_%d",h1DX_v[0][0]->GetName(),j),(j+1)*dL,0,(j+2)*dL,1);
        padXExcessPi0scaled[j+1]->Draw();
        padXExcessPi0scaled[j+1]->cd();
        padXExcessPi0scaled[j+1]->SetLeftMargin(0.01);
        padXExcessPi0scaled[j+1]->SetRightMargin(0.01);
        padXExcessPi0scaled[j+1]->SetTopMargin(0.25);
        padXExcessPi0scaled[j+1]->SetBottomMargin(0.13);
        hExcess[j]->Draw("e");
        hExcess[j]->GetYaxis()->SetRangeUser(ThisMin-5,ThisMax+5);
        if(IntPi0!=0)hExcessPi0scaled[j]->Scale(IntExcess*1./IntPi0);
        hExcessPi0scaled[j]->Draw("hist same");
        hExcess[j]->Draw("same e");
    }
    c1DXExcessPi0scaled->cd();
    padXExcessPi0scaled[0] = new TPad(Form("padXExcessPi0scaled_%s_axis",h1DX_v[0][0]->GetName()),Form("padXExcessPi0scaled_%s_axis",h1DX_v[0][0]->GetName()),0,0,dL,1);
    padXExcessPi0scaled[0]->Draw();
    padXExcessPi0scaled[0]->cd();
    TGaxis *axis3 = new TGaxis(1,padXExcessPi0scaled[1]->GetBottomMargin(),1,1-padXExcessPi0scaled[1]->GetTopMargin(),ThisMin-5,ThisMax+5,105,"");
    axis3->SetName("axis2");
    axis3->SetLabelSize(0.2);
    axis3->SetLabelOffset(0.1);
    axis3->Draw();

    c1DXExcessPi0scaled->cd();
    TPad *padXaxis = new TPad(Form("p1DX_%s_Xaxis",h1DX_v[0][0]->GetName()),Form("p1DX_%s_Xaxis",h1DX_v[0][0]->GetName()),0,0,1,padXExcessPi0scaled[1]->GetBottomMargin());
    padXaxis->Draw();
    padXaxis->cd();
    TGaxis *axis12 = new TGaxis(dL,1,2*dL,1,h1DX_v[0][0]->GetXaxis()->GetXmin(),h1DX_v[0][0]->GetXaxis()->GetXmax(),105,"");
    axis12->SetName("axis12");
    axis12->SetLabelSize(0.1);
    axis12->SetLabelOffset(0.05);
    axis12->Draw();

    TLatex *lTitle = new TLatex(0.3,0.55,"Excess compared to #pi^{0} mis-ID shape");
    lTitle->SetTextSize(0.4);
    lTitle->SetTextAlign(12);
    lTitle->Draw();

    TLatex *lVariable = new TLatex(0.301,0.2,Form("%s for slices of %s", h1DX_v[0][0]->GetXaxis()->GetTitle(), h1DY_v[0][0]->GetXaxis()->GetTitle()));
    lVariable->SetTextSize(0.2);
    lVariable->SetTextAlign(12);
    lVariable->Draw();

    c1DXExcessPi0scaled->cd();
    TPad *padXaxisTop = new TPad(Form("p1DX_%s_XaxisTop",h1DX_v[0][0]->GetName()),Form("p1DX_%s_XaxisTop",h1DX_v[0][0]->GetName()),0,1-padXExcessPi0scaled[1]->GetTopMargin()+0.01,1,1);
    padXaxisTop->Draw();
    padXaxisTop->cd();
    for(int i=0;i<h1DX_v[0].size();i++){
        TLatex *taxisTopX = new TLatex((i+1)*dL,0,
                                       Form("%4.0f-%4.0f MeV",
                                            h1DY_v[0][0]->GetXaxis()->GetXmin()+i*h1DY_v[0][0]->GetBinWidth(1),
                                            h1DY_v[0][0]->GetXaxis()->GetXmin()+(i+1)*h1DY_v[0][0]->GetBinWidth(1)));
        taxisTopX->SetTextSize(0.12);
        taxisTopX->Draw();
        taxisTopX->PaintLatex(taxisTopX->GetX(),taxisTopX->GetY(),60,taxisTopX->GetTextSize(),taxisTopX->GetTitle());
    }


    c1DXExcessPi0scaled->SaveAs(Form("c1DXExcessPi0scaled_%s.pdf",h1DX_v[0][0]->GetName()));
}
void Plot1DXExcessGamma(std::vector<std::vector<TH1D*> > h1DX_v, std::vector<std::vector<TH1D*> > h1DY_v){

    double dL = 1./(h1DX_v[0].size()+2);
    double ThisMin = 1000;
    double ThisMax = -1000;


    std::vector<TH1D*> hExcess(h1DX_v[0].size());
    for(int j=0;j<h1DX_v[0].size();j++){
        hExcess[j] = (TH1D*) h1DX_v[0][j]->Clone("hExcess");
        for(int i=1;i<h1DX_v.size()-4;i++){
            hExcess[j]->Add(h1DX_v[i][j],-1);
        }
    }
    for(int j=0;j<h1DX_v[0].size();j++){
        for(int i= 0;i<h1DX_v[0][j]->GetNbinsX();i++){
            if(hExcess[j]->GetBinContent(i+1)+hExcess[j]->GetBinError(i+1)>ThisMax)ThisMax = hExcess[j]->GetBinContent(i+1)+hExcess[j]->GetBinError(i+1);
            if(hExcess[j]->GetBinContent(i+1)-hExcess[j]->GetBinError(i+1)<ThisMin)ThisMin = hExcess[j]->GetBinContent(i+1)-hExcess[j]->GetBinError(i+1);
        }
    }


    TCanvas *c1DXExcessDeltascaled = new TCanvas(Form("c1DXExcessDeltascaled_%s",h1DX_v[0][0]->GetName()),Form("c1DXExcessDeltascaled_%s",h1DX_v[0][0]->GetName()),5000,2000);
    std::vector<TH1D*> hExcessDeltascaled(h1DX_v[0].size());
    std::vector<TPad*> padXExcessDeltascaled(h1DX_v[0].size()+1);
    double IntDelta = 0;
    double IntExcess = 0;
    for(int j=0;j<h1DX_v[0].size();j++){
        hExcessDeltascaled[j] = (TH1D*) h1DX_v[3][j]->Clone("hExcessDeltascaled");
        for(int i = 0;i<hExcessDeltascaled[j]->GetNbinsX();i++){
            IntDelta+=hExcessDeltascaled[j]->GetBinContent(i+1);
            IntExcess+=hExcess[j]->GetBinContent(i+1);
        }
    }
    for(int j=0;j<h1DX_v[0].size();j++){
        c1DXExcessDeltascaled->cd();
        padXExcessDeltascaled[j+1] = new TPad(Form("padXExcessDeltascaled_%s_%d",h1DX_v[0][0]->GetName(),j),Form("padXExcessDeltascaled_%s_%d",h1DX_v[0][0]->GetName(),j),(j+1)*dL,0,(j+2)*dL,1);
        padXExcessDeltascaled[j+1]->Draw();
        padXExcessDeltascaled[j+1]->cd();
        padXExcessDeltascaled[j+1]->SetLeftMargin(0.01);
        padXExcessDeltascaled[j+1]->SetRightMargin(0.01);
        padXExcessDeltascaled[j+1]->SetTopMargin(0.25);
        padXExcessDeltascaled[j+1]->SetBottomMargin(0.13);
        hExcess[j]->Draw("e");
        hExcess[j]->GetYaxis()->SetRangeUser(ThisMin-5,ThisMax+5);
        if(IntDelta!=0)hExcessDeltascaled[j]->Scale(IntExcess*1./IntDelta);
        hExcessDeltascaled[j]->Draw("hist same");
        hExcess[j]->Draw("same e");
    }
    c1DXExcessDeltascaled->cd();
    padXExcessDeltascaled[0] = new TPad(Form("padXExcessDeltascaled_%s_axis",h1DX_v[0][0]->GetName()),Form("padXExcessDeltascaled_%s_axis",h1DX_v[0][0]->GetName()),0,0,dL,1);
    padXExcessDeltascaled[0]->Draw();
    padXExcessDeltascaled[0]->cd();
    TGaxis *axis4 = new TGaxis(1,padXExcessDeltascaled[1]->GetBottomMargin(),1,1-padXExcessDeltascaled[1]->GetTopMargin(),ThisMin-5,ThisMax+5,105,"");
    axis4->SetName("axis2");
    axis4->SetLabelSize(0.2);
    axis4->SetLabelOffset(0.1);
    axis4->Draw();

    c1DXExcessDeltascaled->cd();
    TPad *padXaxis = new TPad(Form("p1DX_%s_Xaxis",h1DX_v[0][0]->GetName()),Form("p1DX_%s_Xaxis",h1DX_v[0][0]->GetName()),0,0,1,padXExcessDeltascaled[1]->GetBottomMargin());
    padXaxis->Draw();
    padXaxis->cd();
    TGaxis *axis12 = new TGaxis(dL,1,2*dL,1,h1DX_v[0][0]->GetXaxis()->GetXmin(),h1DX_v[0][0]->GetXaxis()->GetXmax(),105,"");
    axis12->SetName("axis12");
    axis12->SetLabelSize(0.1);
    axis12->SetLabelOffset(0.05);
    axis12->Draw();

    TLatex *lVariable = new TLatex(0.301,0.2,Form("%s for slices of %s", h1DX_v[0][0]->GetXaxis()->GetTitle(), h1DY_v[0][0]->GetXaxis()->GetTitle()));
    lVariable->SetTextSize(0.2);
    lVariable->SetTextAlign(12);
    lVariable->Draw();

    TLatex *lTitle = new TLatex(0.3,0.55,"Excess compared to #Delta #rightarrow N#gamma shape");
    lTitle->SetTextSize(0.4);
    lTitle->SetTextAlign(12);
    lTitle->Draw();

    c1DXExcessDeltascaled->cd();
    TPad *padXaxisTop = new TPad(Form("p1DX_%s_XaxisTop",h1DX_v[0][0]->GetName()),Form("p1DX_%s_XaxisTop",h1DX_v[0][0]->GetName()),0,1-padXExcessDeltascaled[1]->GetTopMargin()+0.01,1,1);
    padXaxisTop->Draw();
    padXaxisTop->cd();
    for(int i=0;i<h1DX_v[0].size();i++){
        TLatex *taxisTopX = new TLatex((i+1)*dL,0,
                                       Form("%4.0f-%4.0f MeV",
                                            h1DY_v[0][0]->GetXaxis()->GetXmin()+i*h1DY_v[0][0]->GetBinWidth(1),
                                            h1DY_v[0][0]->GetXaxis()->GetXmin()+(i+1)*h1DY_v[0][0]->GetBinWidth(1)));
        taxisTopX->SetTextSize(0.12);
        taxisTopX->Draw();
        taxisTopX->PaintLatex(taxisTopX->GetX(),taxisTopX->GetY(),60,taxisTopX->GetTextSize(),taxisTopX->GetTitle());
    }

    c1DXExcessDeltascaled->SaveAs(Form("c1DXExcessDeltascaled_%s.pdf",h1DX_v[0][0]->GetName()));
}

void Plot1DX(std::vector<std::vector<TH1D*> > h1DX_v, std::vector<std::vector<TH1D*> > h1DY_v){
    Plot1DXStack(h1DX_v,h1DY_v);
    Plot1DXExcessBF(h1DX_v,h1DY_v);
    Plot1DXExcessPion(h1DX_v,h1DY_v);
    Plot1DXExcessGamma(h1DX_v,h1DY_v);
}

void Plot1DYStack(std::vector<std::vector<TH1D*> > h1DX_v,std::vector<std::vector<TH1D*> > h1DY_v){

    TCanvas *c1DY = new TCanvas(Form("c1DY_%s",h1DY_v[0][0]->GetName()),Form("c1DY_%s",h1DY_v[0][0]->GetName()),5000,2000);

    std::vector<TPad*> padY(h1DY_v[0].size()+1);
    double dL = 1./(h1DY_v[0].size()+2);

    std::vector<THStack*> hStack_v(h1DY_v[0].size());
    for(int j=0;j<h1DY_v[0].size();j++){
        hStack_v[j] = new THStack(Form("hStack_%d",j),"");
        for(int i=1;i<h1DY_v.size()-4;i++){
            h1DY_v[i][j]->SetLineColor(1);
            h1DY_v[i][j]->SetLineWidth(0);
            h1DY_v[i][j]->SetFillColor(PlotColors[i-1]);
            hStack_v[j]->Add(h1DY_v[i][j]);

        }
    }
    double ThisMin = 1000;
    double ThisMax = -1000;
    for(int j=0;j<h1DY_v[0].size();j++){
        for(int i= 0;i<h1DY_v[0][j]->GetNbinsX();i++){
            if(h1DY_v[0][j]->GetBinContent(i+1)+h1DY_v[0][j]->GetBinError(i+1)>ThisMax)ThisMax = h1DY_v[0][j]->GetBinContent(i+1)+h1DY_v[0][j]->GetBinError(i+1);
            if(h1DY_v[0][j]->GetBinContent(i+1)-h1DY_v[0][j]->GetBinError(i+1)<ThisMin)ThisMin = h1DY_v[0][j]->GetBinContent(i+1)-h1DY_v[0][j]->GetBinError(i+1);
        }
    }

    for(int j=0;j<h1DY_v[0].size();j++){
        c1DY->cd();
        padY[j+1] = new TPad(Form("c1DY_%s_%d",h1DY_v[0][0]->GetName(),j),Form("c1DY_%s_%d",h1DY_v[0][0]->GetName(),j),(j+1)*dL,0,(j+2)*dL,1);
        padY[j+1]->Draw();
        padY[j+1]->cd();
        padY[j+1]->SetLeftMargin(0.01);
        padY[j+1]->SetRightMargin(0.01);
        padY[j+1]->SetTopMargin(0.25);
        padY[j+1]->SetBottomMargin(0.13);
        h1DY_v[0][j]->SetMarkerStyle(20);
        h1DY_v[0][j]->SetMarkerSize(0.1);
        h1DY_v[0][j]->SetLineColor(1);
        h1DY_v[0][j]->SetLineWidth(1);
        h1DY_v[0][j]->Draw("e");
        h1DY_v[0][j]->GetYaxis()->SetRangeUser(ThisMin-5,1.5*ThisMax);
        hStack_v[j]->Draw("same hist");

        h1DY_v[h1DY_v.size()-4][j]->SetLineColor(1);
        h1DY_v[h1DY_v.size()-4][j]->SetLineStyle(3);
        h1DY_v[h1DY_v.size()-4][j]->SetLineWidth(1);
        h1DY_v[h1DY_v.size()-4][j]->SetFillStyle(3004);
        h1DY_v[h1DY_v.size()-4][j]->SetFillColor(1);
        h1DY_v[h1DY_v.size()-4][j]->Draw("hist same");

        h1DY_v[h1DY_v.size()-3][j]->SetLineColor(2);
        h1DY_v[h1DY_v.size()-3][j]->SetLineStyle(3);
        h1DY_v[h1DY_v.size()-3][j]->SetLineWidth(1);
        h1DY_v[h1DY_v.size()-3][j]->SetFillStyle(3004);
        h1DY_v[h1DY_v.size()-3][j]->SetFillColor(2);

        h1DY_v[h1DY_v.size()-2][j]->SetLineColor(4);
        h1DY_v[h1DY_v.size()-2][j]->SetLineStyle(3);
        h1DY_v[h1DY_v.size()-2][j]->SetLineWidth(1);
        h1DY_v[h1DY_v.size()-2][j]->SetFillStyle(3005);
        h1DY_v[h1DY_v.size()-2][j]->SetFillColor(4);

        //h1DY_v[h1DY_v.size()-3][j]->Draw("hist same");
        //h1DY_v[h1DY_v.size()-2][j]->Draw("hist same");
        h1DY_v[0][j]->Draw("same e");

        TLatex *tXaxis = new TLatex(h1DY_v[0][j]->GetXaxis()->GetXmin(),h1DY_v[0][j]->GetYaxis()->GetXmin()-10,Form(" %.0f     %.0f",h1DY_v[0][j]->GetXaxis()->GetXmin(),h1DY_v[0][j]->GetXaxis()->GetXmax()));
        tXaxis->SetTextSize(0.3);
        tXaxis->Draw();
    }
    c1DY->cd();
    padY[0] = new TPad(Form("c1DY_%s_axis",h1DY_v[0][0]->GetName()),Form("c1DY_%s_axis",h1DY_v[0][0]->GetName()),0,0,dL,1);
    padY[0]->Draw();
    padY[0]->cd();
    TGaxis *axis1 = new TGaxis(1,padY[1]->GetBottomMargin(),1,1-padY[1]->GetTopMargin(),ThisMin-5,1.5*ThisMax,105,"");
    axis1->SetName("axis1");
    axis1->SetLabelSize(0.2);
    axis1->SetLabelOffset(0.1);
    axis1->Draw();

    c1DY->cd();
    TPad *padYaxis = new TPad(Form("p1DY_%s_Xaxis",h1DY_v[0][0]->GetName()),Form("p1DY_%s_Xaxis",h1DY_v[0][0]->GetName()),0,0,1,padY[1]->GetBottomMargin());
    padYaxis->Draw();
    padYaxis->cd();
    TGaxis *axis12 = new TGaxis(dL,1,2*dL,1,h1DY_v[0][0]->GetXaxis()->GetXmin(),h1DY_v[0][0]->GetXaxis()->GetXmax(),105,"");
    axis12->SetName("axis12");
    axis12->SetLabelSize(0.1);
    axis12->SetLabelOffset(0.05);
    axis12->Draw();

    TLatex *lTitle = new TLatex(0.3,0.55,"Excess compared to stacked backgrounds");
    lTitle->SetTextSize(0.4);
    lTitle->SetTextAlign(12);
    lTitle->Draw();

    TLatex *lVariable = new TLatex(0.301,0.2,Form("%s for slices of %s", h1DY_v[0][0]->GetXaxis()->GetTitle(), h1DX_v[0][0]->GetXaxis()->GetTitle()));
    lVariable->SetTextSize(0.2);
    lVariable->SetTextAlign(12);
    lVariable->Draw();

    c1DY->cd();
    TPad *padYaxisTop = new TPad(Form("p1DY_%s_XaxisTop",h1DY_v[0][0]->GetName()),Form("p1DY_%s_XaxisTop",h1DY_v[0][0]->GetName()),0,1-padY[1]->GetTopMargin()+0.01,1,1);
    padYaxisTop->Draw();
    padYaxisTop->cd();
    for(int i=0;i<h1DY_v[0].size();i++){
        TLatex *taxisTopX = new TLatex((i+1)*dL,0,
                                       Form("%4.02f-%4.02f",
                                            h1DX_v[0][0]->GetXaxis()->GetXmin()+i*h1DX_v[0][0]->GetBinWidth(1),
                                            h1DX_v[0][0]->GetXaxis()->GetXmin()+(i+1)*h1DX_v[0][0]->GetBinWidth(1)));
        taxisTopX->SetTextSize(0.12);
        taxisTopX->Draw();
        taxisTopX->PaintLatex(taxisTopX->GetX(),taxisTopX->GetY(),60,taxisTopX->GetTextSize(),taxisTopX->GetTitle());
    }

    c1DY->cd();
    TPad *pLegend = new TPad("pLegend","pLengend",(0.3*h1DY_v[0].size()+1)*dL,0.6,(h1DY_v[0].size()+1)*dL,0.75);
    //TPad *pLegend = new TPad("pLegend","pLengend",1-dL,0.5,1,0.75);
    pLegend->Draw();
    pLegend->cd();
    TLegend *legStackY = new TLegend(0,0,1,1);
    legStackY->SetNColumns(3);
    legStackY->AddEntry(h1DY_v[0][1],"data","LP");
    for(int i=1;i<h1DY_v.size()-3;i++){
        legStackY->AddEntry(h1DY_v[i][1],Form("%s",HistNames[i-1].c_str()),"F");
    }
    legStackY->Draw();

    for(int j=0;j<h1DY_v[0].size();j++){
        c1DY->cd();
        padY[j+1]->cd();
        h1DY_v[0][j]->Draw("same e");
    }
    c1DY->SaveAs(Form("c1DY_%s.pdf",h1DY_v[0][0]->GetName()));
}
void Plot1DYExcessBF(std::vector<std::vector<TH1D*> > h1DX_v,std::vector<std::vector<TH1D*> > h1DY_v){
    double dL = 1./(h1DY_v[0].size()+2);
    double ThisMin = 1000;
    double ThisMax = -1000;

    TCanvas *c1DYExcess = new TCanvas(Form("c1DYExcess_%s",h1DY_v[0][0]->GetName()),Form("c1DYExcess_%s",h1DY_v[0][0]->GetName()),5000,2000);
    std::vector<TH1D*> hExcess(h1DY_v[0].size());
    std::vector<TPad*> padYExcess(h1DY_v[0].size()+1);
    for(int j=0;j<h1DY_v[0].size();j++){
        hExcess[j] = (TH1D*) h1DY_v[0][j]->Clone("hExcess");
        for(int i=1;i<h1DY_v.size()-4;i++){
            hExcess[j]->Add(h1DY_v[i][j],-1);
        }
    }

    for(int j=0;j<h1DY_v[0].size();j++){
        for(int i= 0;i<h1DY_v[0][j]->GetNbinsX();i++){
            if(hExcess[j]->GetBinContent(i+1)+hExcess[j]->GetBinError(i+1)>ThisMax)ThisMax = hExcess[j]->GetBinContent(i+1)+hExcess[j]->GetBinError(i+1);
            if(hExcess[j]->GetBinContent(i+1)-hExcess[j]->GetBinError(i+1)<ThisMin)ThisMin = hExcess[j]->GetBinContent(i+1)-hExcess[j]->GetBinError(i+1);
        }
    }
    for(int j=0;j<h1DY_v[0].size();j++){
        c1DYExcess->cd();
        padYExcess[j+1] = new TPad(Form("padYExcess_%s_%d",h1DY_v[0][0]->GetName(),j),Form("padYExcess_%s_%d",h1DY_v[0][0]->GetName(),j),(j+1)*dL,0,(j+2)*dL,1);
        padYExcess[j+1]->Draw();
        padYExcess[j+1]->cd();
        padYExcess[j+1]->SetLeftMargin(0.01);
        padYExcess[j+1]->SetRightMargin(0.01);
        padYExcess[j+1]->SetTopMargin(0.25);
        padYExcess[j+1]->SetBottomMargin(0.13);

        hExcess[j]->GetYaxis()->SetRangeUser(ThisMin-5,ThisMax+5);
        hExcess[j]->Draw("e");
        h1DY_v[h1DY_v.size()-1][j]->Draw("same hist");
    }
    c1DYExcess->cd();
    padYExcess[0] = new TPad(Form("padYExcess_%s_axis",h1DY_v[0][0]->GetName()),Form("padYExcess_%s_axis",h1DY_v[0][0]->GetName()),0,0,dL,1);
    padYExcess[0]->Draw();
    padYExcess[0]->cd();
    TGaxis *axis2 = new TGaxis(1,padYExcess[1]->GetBottomMargin(),1,1-padYExcess[1]->GetTopMargin(),ThisMin-5,ThisMax+5,105,"");
    axis2->SetName("axis2");
    axis2->SetLabelSize(0.2);
    axis2->SetLabelOffset(0.1);
    axis2->Draw();

    c1DYExcess->cd();
    TPad *padYaxis = new TPad(Form("p1DY_%s_Xaxis",h1DY_v[0][0]->GetName()),Form("p1DY_%s_Xaxis",h1DY_v[0][0]->GetName()),0,0,1,padYExcess[1]->GetBottomMargin());
    padYaxis->Draw();
    padYaxis->cd();
    TGaxis *axis12 = new TGaxis(dL,1,2*dL,1,h1DY_v[0][0]->GetXaxis()->GetXmin(),h1DY_v[0][0]->GetXaxis()->GetXmax(),105,"");
    axis12->SetName("axis12");
    axis12->SetLabelSize(0.1);
    axis12->SetLabelOffset(0.05);
    axis12->Draw();

    TLatex *lTitle = new TLatex(0.3,0.55,"Excess compared to Best Fit");
    lTitle->SetTextSize(0.4);
    lTitle->SetTextAlign(12);
    lTitle->Draw();

    TLatex *lVariable = new TLatex(0.301,0.2,Form("%s for slices of %s", h1DY_v[0][0]->GetXaxis()->GetTitle(), h1DX_v[0][0]->GetXaxis()->GetTitle()));
    lVariable->SetTextSize(0.2);
    lVariable->SetTextAlign(12);
    lVariable->Draw();

    c1DYExcess->cd();
    TPad *padYaxisTop = new TPad(Form("p1DY_%s_XaxisTop",h1DY_v[0][0]->GetName()),Form("p1DY_%s_XaxisTop",h1DY_v[0][0]->GetName()),0,1-padYExcess[1]->GetTopMargin()+0.01,1,1);
    padYaxisTop->Draw();
    padYaxisTop->cd();
    for(int i=0;i<h1DY_v[0].size();i++){
        TLatex *taxisTopX = new TLatex((i+1)*dL,0,
                                       Form("%4.02f-%4.02f",
                                            h1DX_v[0][0]->GetXaxis()->GetXmin()+i*h1DX_v[0][0]->GetBinWidth(1),
                                            h1DX_v[0][0]->GetXaxis()->GetXmin()+(i+1)*h1DX_v[0][0]->GetBinWidth(1)));
        taxisTopX->SetTextSize(0.12);
        taxisTopX->Draw();
        taxisTopX->PaintLatex(taxisTopX->GetX(),taxisTopX->GetY(),60,taxisTopX->GetTextSize(),taxisTopX->GetTitle());
    }

    c1DYExcess->SaveAs(Form("c1DYExcess_%s.pdf",h1DY_v[0][0]->GetName()));
}
void Plot1DYExcessPion(std::vector<std::vector<TH1D*> > h1DX_v,std::vector<std::vector<TH1D*> > h1DY_v){
    double dL = 1./(h1DY_v[0].size()+2);
    std::vector<TH1D*> hExcess(h1DY_v[0].size());
    for(int j=0;j<h1DY_v[0].size();j++){
        hExcess[j] = (TH1D*) h1DY_v[0][j]->Clone("hExcess");
        for(int i=1;i<h1DY_v.size()-4;i++){
            hExcess[j]->Add(h1DY_v[i][j],-1);
        }
    }
    double ThisMin = 1000;
    double ThisMax = -1000;
    for(int j=0;j<h1DY_v[0].size();j++){
        for(int i= 0;i<h1DY_v[0][j]->GetNbinsX();i++){
            if(hExcess[j]->GetBinContent(i+1)+hExcess[j]->GetBinError(i+1)>ThisMax)ThisMax = hExcess[j]->GetBinContent(i+1)+hExcess[j]->GetBinError(i+1);
            if(hExcess[j]->GetBinContent(i+1)-hExcess[j]->GetBinError(i+1)<ThisMin)ThisMin = hExcess[j]->GetBinContent(i+1)-hExcess[j]->GetBinError(i+1);
        }
    }

    TCanvas *c1DYExcessPi0scaled = new TCanvas(Form("c1DYExcessPi0scaled_%s",h1DY_v[0][0]->GetName()),Form("c1DYExcessPi0scaled_%s",h1DY_v[0][0]->GetName()),5000,2000);
    std::vector<TH1D*> hExcessPi0scaled(h1DY_v[0].size());
    std::vector<TPad*> padYExcessPi0scaled(h1DY_v[0].size()+1);
    double IntPi0 = 0;
    double IntExcess = 0;
    for(int j=0;j<h1DY_v[0].size();j++){
        hExcessPi0scaled[j] = (TH1D*) h1DY_v[4][j]->Clone("hExcessPi0scaled");
        for(int i = 0;i<hExcessPi0scaled[j]->GetNbinsX();i++){
            IntPi0+=hExcessPi0scaled[j]->GetBinContent(i+1);
            IntExcess+=hExcess[j]->GetBinContent(i+1);
        }
    }
    for(int j=0;j<h1DY_v[0].size();j++){
        c1DYExcessPi0scaled->cd();
        padYExcessPi0scaled[j+1] = new TPad(Form("padYExcessPi0scaled_%s_%d",h1DY_v[0][0]->GetName(),j),Form("padYExcessPi0scaled_%s_%d",h1DY_v[0][0]->GetName(),j),(j+1)*dL,0,(j+2)*dL,1);
        padYExcessPi0scaled[j+1]->Draw();
        padYExcessPi0scaled[j+1]->cd();
        padYExcessPi0scaled[j+1]->SetLeftMargin(0.01);
        padYExcessPi0scaled[j+1]->SetRightMargin(0.01);
        padYExcessPi0scaled[j+1]->SetTopMargin(0.25);
        padYExcessPi0scaled[j+1]->SetBottomMargin(0.13);
        hExcess[j]->Draw("e");
        hExcess[j]->GetYaxis()->SetRangeUser(ThisMin-5,ThisMax+5);
        if(IntPi0!=0)hExcessPi0scaled[j]->Scale(IntExcess*1./IntPi0);
        hExcessPi0scaled[j]->Draw("hist same");
        hExcess[j]->Draw("same e");
    }
    c1DYExcessPi0scaled->cd();
    padYExcessPi0scaled[0] = new TPad(Form("padYExcessPi0scaled_%s_axis",h1DY_v[0][0]->GetName()),Form("padYExcessPi0scaled_%s_axis",h1DY_v[0][0]->GetName()),0,0,dL,1);
    padYExcessPi0scaled[0]->Draw();
    padYExcessPi0scaled[0]->cd();
    TGaxis *axis3 = new TGaxis(1,padYExcessPi0scaled[1]->GetBottomMargin(),1,1-padYExcessPi0scaled[1]->GetTopMargin(),ThisMin-5,ThisMax+5,105,"");
    axis3->SetName("axis2");
    axis3->SetLabelSize(0.2);
    axis3->SetLabelOffset(0.1);
    axis3->Draw();

    c1DYExcessPi0scaled->cd();
    TPad *padYaxis = new TPad(Form("p1DY_%s_Xaxis",h1DY_v[0][0]->GetName()),Form("p1DY_%s_Xaxis",h1DY_v[0][0]->GetName()),0,0,1,padYExcessPi0scaled[1]->GetBottomMargin());
    padYaxis->Draw();
    padYaxis->cd();
    TGaxis *axis12 = new TGaxis(dL,1,2*dL,1,h1DY_v[0][0]->GetXaxis()->GetXmin(),h1DY_v[0][0]->GetXaxis()->GetXmax(),105,"");
    axis12->SetName("axis12");
    axis12->SetLabelSize(0.1);
    axis12->SetLabelOffset(0.05);
    axis12->Draw();

    TLatex *lVariable = new TLatex(0.301,0.2,Form("%s for slices of %s", h1DY_v[0][0]->GetXaxis()->GetTitle(), h1DX_v[0][0]->GetXaxis()->GetTitle()));
    lVariable->SetTextSize(0.2);
    lVariable->SetTextAlign(12);
    lVariable->Draw();

    TLatex *lTitle = new TLatex(0.3,0.55,"Excess compared to #pi^{0} mis-ID shape");
    lTitle->SetTextSize(0.4);
    lTitle->SetTextAlign(12);
    lTitle->Draw();

    c1DYExcessPi0scaled->cd();
    TPad *padYaxisTop = new TPad(Form("p1DY_%s_XaxisTop",h1DY_v[0][0]->GetName()),Form("p1DY_%s_XaxisTop",h1DY_v[0][0]->GetName()),0,1-padYExcessPi0scaled[1]->GetTopMargin()+0.01,1,1);
    padYaxisTop->Draw();
    padYaxisTop->cd();
    for(int i=0;i<h1DY_v[0].size();i++){
        TLatex *taxisTopX = new TLatex((i+1)*dL,0,
                                       Form("%4.02f-%4.02f",
                                            h1DX_v[0][0]->GetXaxis()->GetXmin()+i*h1DX_v[0][0]->GetBinWidth(1),
                                            h1DX_v[0][0]->GetXaxis()->GetXmin()+(i+1)*h1DX_v[0][0]->GetBinWidth(1)));
        taxisTopX->SetTextSize(0.12);
        taxisTopX->Draw();
        taxisTopX->PaintLatex(taxisTopX->GetX(),taxisTopX->GetY(),60,taxisTopX->GetTextSize(),taxisTopX->GetTitle());
    }


    c1DYExcessPi0scaled->SaveAs(Form("c1DYExcessPi0scaled_%s.pdf",h1DY_v[0][0]->GetName()));
}
void Plot1DYExcessGamma(std::vector<std::vector<TH1D*> > h1DX_v,std::vector<std::vector<TH1D*> > h1DY_v){
    double dL = 1./(h1DY_v[0].size()+2);
    double ThisMin = 1000;
    double ThisMax = -1000;


    std::vector<TH1D*> hExcess(h1DY_v[0].size());
    for(int j=0;j<h1DY_v[0].size();j++){
        hExcess[j] = (TH1D*) h1DY_v[0][j]->Clone("hExcess");
        for(int i=1;i<h1DY_v.size()-4;i++){
            hExcess[j]->Add(h1DY_v[i][j],-1);
        }
    }
    for(int j=0;j<h1DY_v[0].size();j++){
        for(int i= 0;i<h1DY_v[0][j]->GetNbinsX();i++){
            if(hExcess[j]->GetBinContent(i+1)+hExcess[j]->GetBinError(i+1)>ThisMax)ThisMax = hExcess[j]->GetBinContent(i+1)+hExcess[j]->GetBinError(i+1);
            if(hExcess[j]->GetBinContent(i+1)-hExcess[j]->GetBinError(i+1)<ThisMin)ThisMin = hExcess[j]->GetBinContent(i+1)-hExcess[j]->GetBinError(i+1);
        }
    }


    TCanvas *c1DYExcessDeltascaled = new TCanvas(Form("c1DYExcessDeltascaled_%s",h1DY_v[0][0]->GetName()),Form("c1DYExcessDeltascaled_%s",h1DY_v[0][0]->GetName()),5000,2000);
    std::vector<TH1D*> hExcessDeltascaled(h1DY_v[0].size());
    std::vector<TPad*> padYExcessDeltascaled(h1DY_v[0].size()+1);
    double IntDelta = 0;
    double IntExcess = 0;
    for(int j=0;j<h1DY_v[0].size();j++){
        hExcessDeltascaled[j] = (TH1D*) h1DY_v[3][j]->Clone("hExcessDeltascaled");
        for(int i = 0;i<hExcessDeltascaled[j]->GetNbinsX();i++){
            IntDelta+=hExcessDeltascaled[j]->GetBinContent(i+1);
            IntExcess+=hExcess[j]->GetBinContent(i+1);
        }
    }
    for(int j=0;j<h1DY_v[0].size();j++){
        c1DYExcessDeltascaled->cd();
        padYExcessDeltascaled[j+1] = new TPad(Form("padYExcessDeltascaled_%s_%d",h1DY_v[0][0]->GetName(),j),Form("padYExcessDeltascaled_%s_%d",h1DY_v[0][0]->GetName(),j),(j+1)*dL,0,(j+2)*dL,1);
        padYExcessDeltascaled[j+1]->Draw();
        padYExcessDeltascaled[j+1]->cd();
        padYExcessDeltascaled[j+1]->SetLeftMargin(0.01);
        padYExcessDeltascaled[j+1]->SetRightMargin(0.01);
        padYExcessDeltascaled[j+1]->SetTopMargin(0.25);
        padYExcessDeltascaled[j+1]->SetBottomMargin(0.13);
        hExcess[j]->Draw("e");
        hExcess[j]->GetYaxis()->SetRangeUser(ThisMin-5,ThisMax+5);
        if(IntDelta!=0)hExcessDeltascaled[j]->Scale(IntExcess*1./IntDelta);
        hExcessDeltascaled[j]->Draw("hist same");
        hExcess[j]->Draw("same e");
    }
    c1DYExcessDeltascaled->cd();
    padYExcessDeltascaled[0] = new TPad(Form("padYExcessDeltascaled_%s_axis",h1DY_v[0][0]->GetName()),Form("padYExcessDeltascaled_%s_axis",h1DY_v[0][0]->GetName()),0,0,dL,1);
    padYExcessDeltascaled[0]->Draw();
    padYExcessDeltascaled[0]->cd();
    TGaxis *axis4 = new TGaxis(1,padYExcessDeltascaled[1]->GetBottomMargin(),1,1-padYExcessDeltascaled[1]->GetTopMargin(),ThisMin-5,ThisMax+5,105,"");
    axis4->SetName("axis2");
    axis4->SetLabelSize(0.2);
    axis4->SetLabelOffset(0.1);
    axis4->Draw();

    c1DYExcessDeltascaled->cd();
    TPad *padYaxis = new TPad(Form("p1DY_%s_Xaxis",h1DY_v[0][0]->GetName()),Form("p1DY_%s_Xaxis",h1DY_v[0][0]->GetName()),0,0,1,padYExcessDeltascaled[1]->GetBottomMargin());
    padYaxis->Draw();
    padYaxis->cd();
    TGaxis *axis12 = new TGaxis(dL,1,2*dL,1,h1DY_v[0][0]->GetXaxis()->GetXmin(),h1DY_v[0][0]->GetXaxis()->GetXmax(),105,"");
    axis12->SetName("axis12");
    axis12->SetLabelSize(0.1);
    axis12->SetLabelOffset(0.05);
    axis12->Draw();

    TLatex *lVariable = new TLatex(0.301,0.2,Form("%s for slices of %s", h1DY_v[0][0]->GetXaxis()->GetTitle(), h1DX_v[0][0]->GetXaxis()->GetTitle()));
    lVariable->SetTextSize(0.2);
    lVariable->SetTextAlign(12);
    lVariable->Draw();

    TLatex *lTitle = new TLatex(0.3,0.55,"Excess compared to #Delta #rightarrow N#gamma shape");
    lTitle->SetTextSize(0.4);
    lTitle->SetTextAlign(12);
    lTitle->Draw();

    c1DYExcessDeltascaled->cd();
    TPad *padYaxisTop = new TPad(Form("p1DY_%s_XaxisTop",h1DY_v[0][0]->GetName()),Form("p1DY_%s_XaxisTop",h1DY_v[0][0]->GetName()),0,1-padYExcessDeltascaled[1]->GetTopMargin()+0.01,1,1);
    padYaxisTop->Draw();
    padYaxisTop->cd();
    for(int i=0;i<h1DY_v[0].size();i++){
        TLatex *taxisTopX = new TLatex((i+1)*dL,0,
                                       Form("%4.02f-%4.02f",
                                            h1DX_v[0][0]->GetXaxis()->GetXmin()+i*h1DX_v[0][0]->GetBinWidth(1),
                                            h1DX_v[0][0]->GetXaxis()->GetXmin()+(i+1)*h1DX_v[0][0]->GetBinWidth(1)));
        taxisTopX->SetTextSize(0.12);
        taxisTopX->Draw();
        taxisTopX->PaintLatex(taxisTopX->GetX(),taxisTopX->GetY(),60,taxisTopX->GetTextSize(),taxisTopX->GetTitle());
    }

    c1DYExcessDeltascaled->SaveAs(Form("c1DYExcessDeltascaled_%s.pdf",h1DY_v[0][0]->GetName()));
}
void Plot1DY(std::vector<std::vector<TH1D*> > h1DX_v,std::vector<std::vector<TH1D*> > h1DY_v){
    Plot1DYStack(h1DX_v,h1DY_v);
    Plot1DYExcessBF(h1DX_v,h1DY_v);
    Plot1DYExcessPion(h1DX_v,h1DY_v);
    Plot1DYExcessGamma(h1DX_v,h1DY_v);
}

void Plot2D(TH2D *hdata, TH2D *hdirt, TH2D *hfullOsc, std::vector<TH2D*> hMC){

    TCanvas *c = new TCanvas(Form("c_%s",hdata->GetName()),Form("c_%s",hdata->GetName()),800,800);
    c->Divide(3,4);
    c->cd(1);
    hdata->Draw("colz");
    TH2D *hExcess = (TH2D*)hdata->Clone(Form("%s_Excess",hdata->GetName()));
    c->cd(2);
    hdirt->Draw("colz");
    TH2D *hBG = (TH2D*)hdirt->Clone(Form("%s_BG",hdata->GetName()));
    hExcess->Add(hdirt,-1);

    for(int i=0;i<hMC.size()-3;i++){
        c->cd(i+3);
        hMC[i]->Draw("colz");
        hExcess->Add(hMC[i],-1);
        hBG->Add(hMC[i],1);
    }
    c->cd(hMC.size()+3);
    hfullOsc->Draw("colz");

    c->cd(hMC.size()+4);
    hExcess->SetTitle("Excess");
    hExcess->Draw("colz");

    TH2D *hExcessMinusBF = (TH2D*)hExcess->Clone(Form("%s_ExcessMinusBF",hdata->GetName()));
    hExcessMinusBF->Add(hfullOsc,-1);
    c->cd(hMC.size()+6);
    hExcessMinusBF->SetTitle("Excess - BF");
    hExcessMinusBF->Draw("colz");
    hExcessMinusBF->SaveAs(Form("%s.root",hExcessMinusBF->GetName()));

    c->SaveAs(Form("%s.pdf",c->GetName()));

    TCanvas *cData = new TCanvas(Form("c_%s_data",hdata->GetName()),Form("c_%s_data",hdata->GetName()),800,800);
    hdata->SetTitle("Data");
    hdata->Draw("colz");
    cData->SaveAs(Form("%s.pdf",cData->GetName()));

    TCanvas *cBG = new TCanvas(Form("c_%s_BG",hdata->GetName()),Form("c_%s_BG",hdata->GetName()),800,800);
    hBG->GetXaxis()->SetTitle(Form("%s",hdata->GetXaxis()->GetTitle()));
    hBG->GetYaxis()->SetTitle(Form("%s",hdata->GetYaxis()->GetTitle()));
    hBG->SetTitle("Backgrounds");
    hBG->Draw("colz");
    cBG->SaveAs(Form("%s.pdf",cBG->GetName()));

    TCanvas *cExcess = new TCanvas(Form("c_%s_Excess",hdata->GetName()),Form("c_%s_Excess",hdata->GetName()),800,800);
    hExcess->Draw("colz");
    cExcess->SaveAs(Form("%s.pdf",cExcess->GetName()));
}

double GetChi2(TH2D *h0, TH2D *h1){

    double chi2 = 0;

    for(int i=0;i<h0->GetNbinsX();i++){
        for(int j=0;j<h0->GetNbinsY();j++){
            if(pow(h1->GetBinError(i+1,j+1),2)+pow(h0->GetBinError(i+1,j+1),2) != 0)chi2 += pow(h1->GetBinContent(i+1,j+1)-h0->GetBinContent(i+1,j+1),2)/(pow(h1->GetBinError(i+1,j+1),2)+pow(h0->GetBinError(i+1,j+1),2));
        }
    }

    return chi2;
}

void PlotFull(TH2D *hdata, TH2D *hdirt, TH2D *hfullOsc, std::vector<TH2D*> hMC){
    Plot2D(hdata, hdirt, hfullOsc, hMC);
    std::vector<std::vector<TH1D*> > hX =  Project1DX(hdata, hdirt, hfullOsc, hMC);
    std::vector<std::vector<TH1D*> > hY =  Project1DY(hdata, hdirt, hfullOsc, hMC);
    Plot1DX(hX,hY);
    Plot1DY(hX,hY);
}

void Compare1DXexcess2coherent(TH2D *hdata, TH2D *hdirt, TH2D *hfullOsc, std::vector<TH2D*> hMC, TH2D *hCoherentPi0){
    hdata->SetName(Form("%s_compCoherent",hdata->GetName()));
    hdirt->SetName(Form("%s_compCoherent",hdirt->GetName()));
    hfullOsc->SetName(Form("%s_compCoherent",hfullOsc->GetName()));
    for(int i=0;i<hMC.size();i++){hMC[i]->SetName(Form("%s_compCoherent",hMC[i]->GetName()));}

    std::vector<std::vector<TH1D*> > h1DX_v = Project1DX(hdata,hdirt,hfullOsc,hMC);
    std::vector<TH2D*> hCoherentPi0_v;
    hCoherentPi0_v.push_back(hCoherentPi0);
    std::vector<std::vector<TH1D*> > h1DX_CoherentPi0_v = Project1DX(hCoherentPi0_v);

    double dL = 1./(h1DX_v[0].size()+2);

    std::vector<THStack*> hStack_v(h1DX_v[0].size());
    for(int j=0;j<h1DX_v[0].size();j++){
        hStack_v[j] = new THStack(Form("hStack_%d",j),"");
        for(int i=1;i<h1DX_v.size()-4;i++){
            h1DX_v[i][j]->SetLineColor(1);
            h1DX_v[i][j]->SetLineWidth(0);
            h1DX_v[i][j]->SetFillColor(PlotColors[i-1]);
            hStack_v[j]->Add(h1DX_v[i][j]);
        }
    }

    TCanvas *c1DXExcessCompared2Coherent = new TCanvas(Form("c1DXExcessCompared2Coherent_%s",h1DX_v[0][0]->GetName()),Form("c1DXExcessCompared2Coherent_%s",h1DX_v[0][0]->GetName()),5000,2000);
    std::vector<TH1D*> hExcess(h1DX_v[0].size());
    std::vector<TPad*> padXExcess(h1DX_v[0].size()+1);
    for(int j=0;j<h1DX_v[0].size();j++){
        hExcess[j] = (TH1D*) h1DX_v[0][j]->Clone("hExcess");
        for(int i=1;i<h1DX_v.size()-4;i++){
            hExcess[j]->Add(h1DX_v[i][j],-1);
        }
    }
    double ThisMin = 1000;
    double ThisMax = -1000;
    for(int j=0;j<h1DX_v[0].size();j++){
        for(int i= 0;i<h1DX_v[0][j]->GetNbinsX();i++){
            if(hExcess[j]->GetBinContent(i+1)+hExcess[j]->GetBinError(i+1)>ThisMax)ThisMax = hExcess[j]->GetBinContent(i+1)+hExcess[j]->GetBinError(i+1);
            if(hExcess[j]->GetBinContent(i+1)-hExcess[j]->GetBinError(i+1)<ThisMin)ThisMin = hExcess[j]->GetBinContent(i+1)-hExcess[j]->GetBinError(i+1);
        }
    }
    double IntPi0 = 0;
    double IntExcess = 0;
    double IntBF = 0;
    std::vector<TH1D*> hExcessPi0scaledcoherent(h1DX_v[0].size());
    for(int j=0;j<h1DX_v[0].size();j++){
        hExcessPi0scaledcoherent[j] = (TH1D*) h1DX_CoherentPi0_v[0][j]->Clone("hExcessPi0scaledcoherent");
        for(int i = 0;i<hExcessPi0scaledcoherent[j]->GetNbinsX();i++){
            IntPi0+=hExcessPi0scaledcoherent[j]->GetBinContent(i+1);
            IntExcess+=hExcess[j]->GetBinContent(i+1);
            //IntExcess-=h1DX_v[h1DX_v.size()-1][j]->GetBinContent(i+1);
        }
    }
    for(int j=0;j<h1DX_v[0].size();j++){
        THStack *hStack_coherent = new THStack(Form("hStack_coherent_%d",j),"");
        c1DXExcessCompared2Coherent->cd();
        padXExcess[j+1] = new TPad(Form("padXExcess_%s_%d",h1DX_v[0][0]->GetName(),j),Form("padXExcess_%s_%d",h1DX_v[0][0]->GetName(),j),(j+1)*dL,0,(j+2)*dL,1);
        padXExcess[j+1]->Draw();
        padXExcess[j+1]->cd();
        padXExcess[j+1]->SetLeftMargin(0.01);
        padXExcess[j+1]->SetRightMargin(0.01);

        hExcess[j]->GetYaxis()->SetRangeUser(ThisMin-5,ThisMax+5);
        hExcess[j]->SetLineColor(1);
        hExcess[j]->SetMarkerStyle(20);
        hExcess[j]->SetMarkerSize(0.5);
        hExcess[j]->SetMarkerColor(1);
        hExcess[j]->Draw("e");
        h1DX_CoherentPi0_v[0][j]->SetFillColor(kRed-6);
        h1DX_CoherentPi0_v[0][j]->SetLineColor(kRed-6);
        //h1DX_CoherentPi0_v[0][j]->Scale((IntExcess)/IntPi0);
        h1DX_CoherentPi0_v[0][j]->Scale(0.35);
        //h1DX_CoherentPi0_v[0][j]->Draw("same hist");
        h1DX_v[h1DX_v.size()-1][j]->Draw("same hist");
        hStack_coherent->Add(h1DX_v[h1DX_v.size()-1][j]);
        hStack_coherent->Add(h1DX_CoherentPi0_v[0][j]);
        hStack_coherent->Draw("same hist");
        hExcess[j]->Draw("same e");
    }
    c1DXExcessCompared2Coherent->cd();
    padXExcess[0] = new TPad(Form("padXExcess_%s_axis",h1DX_v[0][0]->GetName()),Form("padXExcess_%s_axis",h1DX_v[0][0]->GetName()),0,0,dL,1);
    padXExcess[0]->Draw();
    padXExcess[0]->cd();
    TGaxis *axis2 = new TGaxis(1,0.13,1,0.87,ThisMin-5,ThisMax+5,105,"");
    axis2->SetName("axis2");
    axis2->SetLabelSize(0.2);
    axis2->SetLabelOffset(0.1);
    axis2->Draw();
    c1DXExcessCompared2Coherent->SaveAs(Form("c1DXExcessCompared2Coherent_%s.pdf",h1DX_v[0][0]->GetName()));



}

void Load1sigmaFile(){
    cCLgrid->cd();
    ifstream OneSigmaFile("cont_fake_oct19_contNunubar_1s.txt");
    ifstream FourSigmaFile("cont_fake_oct19_contNunubar_4s.txt");
    ifstream ThreeSigmaFile("cont_fake_oct19_contNunubar_3s.txt");
    ifstream pc90File("cont_fake_oct19_contNunubar_90.txt");
    ifstream pc95File("cont_fake_oct19_contNunubar_95.txt");
    ifstream pc99File("cont_fake_oct19_contNunubar_99.txt");

    bool goOn = true;
    double stheta,dm2;

    int i=0;
    while(goOn && !OneSigmaFile.eof()){

        OneSigmaFile >> stheta >> dm2;
        if(OneSigmaFile.eof()){goOn=false;break;}
        //std::cout << stheta << "\t" << dm2 << std::endl;
        g1sigma->SetPoint(i,stheta, dm2);
        i++;
        std::vector<double> thisPoint(2);
        thisPoint[0] = stheta;
        thisPoint[1] = dm2;
        OneSigmaContour.push_back(thisPoint);
    }

    goOn = true;i=0;
    while(goOn && !FourSigmaFile.eof()){
        FourSigmaFile >> stheta >> dm2;
        if(FourSigmaFile.eof()){goOn=false;break;}
        g4sigma->SetPoint(i,stheta, dm2);i++;
    }

    goOn = true;i=0;
    while(goOn && !ThreeSigmaFile.eof()){
        ThreeSigmaFile >> stheta >> dm2;
        if(ThreeSigmaFile.eof()){goOn=false;break;}
        g3sigma->SetPoint(i,stheta, dm2);i++;
    }

    goOn = true;i=0;
    while(goOn && !pc90File.eof()){
        pc90File >> stheta >> dm2;
        if(pc90File.eof()){goOn=false;break;}
        g90pc->SetPoint(i,stheta, dm2);i++;
    }

    goOn = true;i=0;
    while(goOn && !pc95File.eof()){
        pc95File >> stheta >> dm2;
        if(pc95File.eof()){goOn=false;break;}
        g95pc->SetPoint(i,stheta, dm2);i++;
    }

    goOn = true;i=0;
    while(goOn && !pc99File.eof()){
        pc99File >> stheta >> dm2;
        if(pc99File.eof()){goOn=false;break;}
        g99pc->SetPoint(i,stheta, dm2);i++;
    }

    g4sigma->SetMarkerStyle(6);
    g4sigma->SetNameTitle("g4sigma",";sin^{2} 2#theta;#Deltam^{2} (eV^{2})");
    g4sigma->Draw("AP");
    g4sigma->GetYaxis()->SetTitleSize();
    g4sigma->GetXaxis()->SetTitleSize();
    g4sigma->GetYaxis()->SetRangeUser(1e-2,1e2);
    g1sigma->SetMarkerStyle(6);
    g1sigma->Draw("same P");
    g3sigma->SetMarkerStyle(6);
    g3sigma->Draw("same P");
    g90pc->SetMarkerStyle(6);
    g90pc->Draw("same P");
    g95pc->SetMarkerStyle(6);
    g95pc->Draw("same P");
    g99pc->SetMarkerStyle(6);
    g99pc->Draw("same P");
}

bool CheckIn1sigma(double stheta = 0.8072, double dm2 = 0.043){

    double diffDm = 1e9;
    double bestDm = 0;
    std::vector<std::pair<double, double> > Closestdm2;
    for(int i=0;i<OneSigmaContour.size();i++){
        if(std::abs(OneSigmaContour[i][1]-dm2) < diffDm){
            diffDm=std::abs(OneSigmaContour[i][1]-dm2);
            bestDm = OneSigmaContour[i][1];
        }
    }

    if(std::abs(bestDm-dm2)/dm2 > 0.05)return false;
    //std::cout  << " relative diff = " << std::abs(bestDm-dm2)/dm2 << std::endl;

    std::vector<double> CorrespondingSinTheta;

    for(int i=0;i<OneSigmaContour.size();i++){
        if(OneSigmaContour[i][1] == bestDm){
            CorrespondingSinTheta.push_back(OneSigmaContour[i][0]);
        }
    }

    if(CorrespondingSinTheta.size() == 1 && stheta < CorrespondingSinTheta[0])return false;
    if(CorrespondingSinTheta.size() == 1 && stheta > CorrespondingSinTheta[0])return true;
    if(CorrespondingSinTheta.size() > 2)return false;

    if(stheta < CorrespondingSinTheta[0] && stheta < CorrespondingSinTheta[1])return false;
    if(stheta > CorrespondingSinTheta[0] && stheta > CorrespondingSinTheta[1])return false;

    return true;
}

std::vector<std::pair<double, double> > FindExamples(int Nt = 60){
    cCLgrid->cd();
    std::vector<std::pair<double, double> > PhaseSpacePoints;
    double st,dm;
    int iOK = 0;
    std::vector<TGraph*> gExamples;
    for(int i = 0;i<Nt;i++){
        st = 0.012*exp((i*100./Nt)/22.);
        if(st > 1){std::cout << "***" << std::endl; continue;}
        dm = 0.038*pow(st,-0.5);
        if(!CheckIn1sigma(st,dm))continue;
        std::pair<double, double> thisPoint;
        thisPoint.first = st;
        thisPoint.second = dm;
        PhaseSpacePoints.push_back(thisPoint);
        TGraph *thisExample = new TGraph();
        thisExample->SetPoint(0,st,dm);
        thisExample->SetMarkerStyle(3);
        thisExample->SetMarkerSize(1.5);
        thisExample->SetMarkerColor(gExamples.size()+1);
        thisExample->Draw("same P");
        gExamples.push_back(thisExample);
    }
    return PhaseSpacePoints;
}

std::vector<std::pair<double, double> > FindExamples2(int Nt = 60){
    cCLgrid->cd();
    std::vector<std::pair<double, double> > PhaseSpacePoints;
    int iOK = 0;

    TGraph *gBestFit = new TGraph();
    gBestFit->SetPoint(0,BFst,BFdm);
    gBestFit->SetMarkerStyle(29);
    gBestFit->SetMarkerSize(1.5);
    gBestFit->SetMarkerColor(1);
    gBestFit->Draw("same P");


    for(int i=0;i<OneSigmaContour.size();i++){
        if(i%(OneSigmaContour.size()/(Nt)) != 0)continue;
        iOK++;
        std::pair<double, double> thisPoint;
        thisPoint.first  = OneSigmaContour[i][0];
        thisPoint.second = OneSigmaContour[i][1];
        PhaseSpacePoints.push_back(thisPoint);
        TGraph *thisExample = new TGraph();
        thisExample->SetPoint(0,thisPoint.first,thisPoint.second);
        thisExample->SetMarkerStyle(20);
        thisExample->SetMarkerSize(1);
        thisExample->SetMarkerColor(iOK);
        thisExample->Draw("same P");

    }
    return PhaseSpacePoints;
}

std::vector<std::pair<double, double> > FindPhaseSpacePoints(int Nt = 60){
    cCLgrid->cd();
    std::vector<std::pair<double, double> > PhaseSpacePoints;
    TGraph *gTry = new TGraph();
    TGraph *gInside = new TGraph();
    double st,dm;
    int iOK = 0;
    for(int i = 0;i<Nt;i++){
        for(int j=0;j<Nt;j++){
            st = 1e-4*exp((i*100./Nt)/10.75);
            dm = 0.01*exp((j*100./Nt)/10.1);

            //if(dm > 1)continue;
            if(st > 7e-3 && dm > 0.1*pow(st,-0.5))continue;
            if(dm < 0.01*pow(st,-0.5))continue;
            if(st > 1){continue;}
            gTry->SetPoint(i*Nt+j,st,dm);

            if(CheckIn1sigma(st,dm)){
                gInside->SetPoint(iOK,st,dm);
                iOK++;
                std::pair<double, double> thisPoint;
                thisPoint.first = st;
                thisPoint.second = dm;
                PhaseSpacePoints.push_back(thisPoint);
            }
        }
    }
    //gTry->SetMarkerStyle(20);
    //gTry->SetMarkerSize(0.25);
    //gTry->SetMarkerColor(2);
    //gTry->Draw("same P");
    gInside->SetMarkerStyle(20);
    gInside->SetMarkerSize(0.25);
    gInside->SetMarkerColor(4);
    gInside->Draw("same P");

    g1sigma->Draw("same P");
    g4sigma->Draw("same P");
    g3sigma->Draw("same P");
    g90pc->Draw("same P");
    g95pc->Draw("same P");
    g99pc->Draw("same P");
    cCLgrid->SetLogx();
    cCLgrid->SetLogy();
    cCLgrid->SaveAs("cCLgrid.pdf");
    return PhaseSpacePoints;
}

void  GetMCSystematicError(){
    TH1D *hNueMC       = (TH1D*)fFitOutput->Get("Nue MC constrained Null");
    TMatrixT<double>* collapsed_error_matrix_constrained_BF = (TMatrixT<double>*)fFitOutput->Get("collapsed_error_matrix_constrained_BF");
    TMatrixT<double>* frac_error_matrix = (TMatrixT<double>*)fFitOutput->Get("frac_error_matrix");

    TCanvas *cMatrix = new TCanvas("cMatrix","cMatrix",1600,800);
    cMatrix->Divide(2,1);
    cMatrix->cd(1);
    collapsed_error_matrix_constrained_BF->Draw("colz");
    cMatrix->cd()->SetLogz();
    cMatrix->cd()->SetTopMargin(0.2);
    cMatrix->cd()->SetBottomMargin(0.2);
    cMatrix->cd()->SetRightMargin(0.2);
    cMatrix->cd()->SetLeftMargin(0.2);
    std::vector<double> frac_error;
    for(int i=0;i<11;i++){
        std::cout << i+1 << " " << (*collapsed_error_matrix_constrained_BF)(i,i) << std::endl;
        frac_error.push_back(pow((*collapsed_error_matrix_constrained_BF)(i,i),2));
    }

    cMatrix->cd(2);
    hNueMC->Draw();
    TH1D *hNueMC_syst = (TH1D*)hNueMC->Clone("hNueMC_syst");
    for(int i=0;i<hNueMC_syst->GetNbinsX();i++){
        std::cout << hNueMC->GetBinContent(i+1)/pow(hNueMC->GetBinError(i+1),2) << std::endl;
        hNueMC_syst->SetBinError(i+1,hNueMC->GetBinContent(i+1)*frac_error[i]);
    }
    hNueMC_syst->Draw("hist");

}

void mBplots(){
    gStyle->SetOptStat(0);
    gStyle->SetPalette(kTemperatureMap);

    //GetMCSystematicError();
    //return;

    Load1sigmaFile();

    std::vector<std::pair<double, double> > PhaseSpacePoints = FindPhaseSpacePoints(200);
    std::vector<std::pair<double, double> > ExampleParameters = FindExamples2(3);

    bool Generate1sigma = false;
    bool PlotExamples = true;
    bool DrawFitLimints = true;
    bool plot1sigmaBand = true;

    TH1D *hNueData     = (TH1D*)fFitOutput->Get("Nue Data");
    TH1D *hNueSignal   = (TH1D*)fFitOutput->Get("Nue Signal");
    TH1D *hNueMC       = (TH1D*)fFitOutput->Get("Nue MC constrained Null");
    TH1D* hNueDataExcess = (TH1D*)hNueData->Clone("hNueDataExcess");
    TH1D* hNueDataExcessStatOnly = (TH1D*)hNueData->Clone("hNueDataExcessStatOnly");
    hNueMC->SetLineColor(2);
    hNueMC->SetLineWidth(3);


    //POTlist[kData]    = 18.7374e20;
    POTlist[kData]    = 18.75e20;
    POTlist[kOtherMC] = 2*2.054e21;
    POTlist[kDirt]    = 2*2.054E21;
    POTlist[kFullOsc] = 2*6.639e19;

    // BF neutrino 2020
    //fullOsc.SetSin22theta(0.807);
    //fullOsc.SetDm2(0.043);

    fullOsc.SetSin22theta(BFst);
    fullOsc.SetDm2(BFdm);

    fullOsc_nuebar.SetSin22theta(BFst);
    fullOsc_nuebar.SetDm2(BFdm);

    // test for Janet
    //fullOsc.SetSin22theta(1e-3);
    //fullOsc.SetDm2(1.32);



    /*data.ChooseOnlyForward();
    OtherMC.ChooseOnlyForward();
    fullOsc.ChooseOnlyForward();
    DirtMC.ChooseOnlyForward();*/

    /*data.ChooseOnlyBackward();
    OtherMC.ChooseOnlyBackward();
    fullOsc.ChooseOnlyBackward();
    DirtMC.ChooseOnlyBackward();*/


    OtherMC.SetScaleFactor(POTlist[kData]/POTlist[kOtherMC]);
    fullOsc.SetScaleFactor(POTlist[kData]/POTlist[kFullOsc]);
    DirtMC.SetScaleFactor(POTlist[kData]/POTlist[kDirt]);

    std::cout << "scaling factor dirt = " << POTlist[kData]/POTlist[kDirt] << std::endl;
    std::cout << "scaling factor other = " << POTlist[kData]/POTlist[kOtherMC] << std::endl;

    data.Loop();
    OtherMC.Loop();
    fullOsc.Loop();
    //fullOsc_nuebar.Loop();
    DirtMC.Loop();


    //GetMCSystematicError();

    TCanvas *cEnuQE = new TCanvas("cEnuQE","cEnuQE",800,600);
    data.hEnuqe->SetMarkerStyle(20);
    data.hEnuqe->SetLineColor(1);

    TH1D *hMCBGstat = (TH1D*)OtherMC.hEnuqe[0]->Clone("hMCBGstat");
    for(int i=0;i<hMCBGstat->GetNbinsX();i++){
        double valueDirt = DirtMC.hEnuqe->GetBinContent(i+1)*POTlist[kDirt]/POTlist[kData];
        double valueOther = 0;
        for(int j=0;j<OtherMC.hEnuqe.size();j++){
            valueOther+=OtherMC.hEnuqe[j]->GetBinContent(i+1)*POTlist[kOtherMC]/POTlist[kData];
        }
        double errtotal = sqrt(valueDirt + valueOther);
        double scaling = POTlist[kDirt]/POTlist[kData];
        if(i > 1)scaling = (valueDirt+valueOther)/(hNueMC->GetBinContent(i-1));
        std::cout << i+1 << " " << scaling << std::endl;
        hMCBGstat->SetBinContent(i+1,((valueDirt+valueOther)/(scaling))/(1000.*hMCBGstat->GetBinWidth(i+1)));
        hMCBGstat->SetBinError(i+1,   (errtotal             /(scaling))/(1000.*hMCBGstat->GetBinWidth(i+1)));

    }

    THStack *hMC_Enuqe = new THStack("hMC_Enuqe","hMC_Enuqe");
    DirtMC.hEnuqe->SetFillColor(kDirtColor);

    for(int j=0;j<OtherMC.hEnuqe[0]->GetNbinsX();j++){
        OtherMC.hEnuqe[0]->SetBinContent(j+1,OtherMC.hEnuqe[0]->GetBinContent(j+1)/(OtherMC.hEnuqe[0]->GetBinWidth(j+1)*1000));
        OtherMC.hEnuqe[0]->SetBinError(j+1,OtherMC.hEnuqe[0]->GetBinError(j+1)/(OtherMC.hEnuqe[0]->GetBinWidth(j+1)*1000));
        DirtMC.hEnuqe->SetBinContent(j+1,DirtMC.hEnuqe->GetBinContent(j+1)/(DirtMC.hEnuqe->GetBinWidth(j+1)*1000));
        DirtMC.hEnuqe->SetBinError(  j+1,DirtMC.hEnuqe->GetBinError(j+1)/(  DirtMC.hEnuqe->GetBinWidth(j+1)*1000));
    }

    hMC_Enuqe->Add(OtherMC.hEnuqe[0]);
    TH1D *hEnuQEBG = (TH1D*)OtherMC.hEnuqe[0]->Clone("hEnuQEBG");

    hMC_Enuqe->Add(DirtMC.hEnuqe);
    hEnuQEBG->Add(DirtMC.hEnuqe,1);
    for(int i=1;i<OtherMC.hEnuqe.size();i++){
        for(int j=0;j<OtherMC.hEnuqe[i]->GetNbinsX();j++){
            OtherMC.hEnuqe[i]->SetBinContent(j+1,OtherMC.hEnuqe[i]->GetBinContent(j+1)/(OtherMC.hEnuqe[i]->GetBinWidth(j+1)*1000));
            OtherMC.hEnuqe[i]->SetBinError(j+1,OtherMC.hEnuqe[i]->GetBinError(j+1)/(OtherMC.hEnuqe[i]->GetBinWidth(j+1)*1000));
        }
        hMC_Enuqe->Add(OtherMC.hEnuqe[i]);
        hEnuQEBG->Add(OtherMC.hEnuqe[i],1);
    }
    //hMC_Enuqe->Add(fullOsc.hEnuqe);


    for(int i=0;i<data.hEnuqe->GetNbinsX();i++){
        //hEnuQEBG->SetBinContent(i+1,hEnuQEBG->GetBinContent(i+1)/(hEnuQEBG->GetBinWidth(i+1)*1000.));
        double content = data.hEnuqe->GetBinContent(i+1);
        data.hEnuqe->SetBinContent(i+1,data.hEnuqe->GetBinContent(i+1)/(data.hEnuqe->GetBinWidth(i+1)*1000.));
        data.hEnuqe->SetBinError(i+1,sqrt(content)/(data.hEnuqe->GetBinWidth(i+1)*1000.));
    }

    data.hEnuqe->Draw("e");
    data.hEnuqe->GetXaxis()->SetTitle("E_{QE} [GeV]");
    data.hEnuqe->GetYaxis()->SetTitle("excess events/MeV");
    hMC_Enuqe->Draw("hist same");
    data.hEnuqe->Draw("same e");
    hMCBGstat->Draw("same e");
    cEnuQE->SaveAs("cEnuQE.pdf");

    //hNueMC->Draw("same");


    for(int i=0;i<hNueData->GetNbinsX();i++){
        //std::cout << hNueDataExcess->GetBinCenter(i+1)+0.5*hNueDataExcess->GetBinWidth(i+1) << " " ;
        double contentExcess = hNueDataExcess->GetBinContent(i+1);
        hNueDataExcess->SetBinContent(i+1,(hNueDataExcess->GetBinContent(i+1)-hNueMC->GetBinContent(i+1))/(hNueDataExcess->GetBinWidth(i+1)*1000.) );
        double contentSignal = hNueSignal->GetBinContent(i+1);
        double dataError = hNueData->GetBinError(i+1);
        double MCerror = hNueMC->GetBinError(i+1);
        hNueSignal->SetBinContent(i+1,hNueSignal->GetBinContent(i+1)/(hNueSignal->GetBinWidth(i+1)*1000.) );
        hNueSignal->SetBinError(i+1,sqrt(contentSignal)/(hNueSignal->GetBinWidth(i+1)*1000.) );
        hNueDataExcess->SetBinError(i+1,sqrt( pow(dataError,2)+pow(MCerror,2) )/(hNueDataExcess->GetBinWidth(i+1)*1000.));

        //double MCstatError = sqrt(hNueMC->GetBinContent(i+1));
        double MCstatError = hMCBGstat->GetBinError(i+2);
        hNueDataExcessStatOnly->SetBinContent(i+1,(contentExcess-hNueMC->GetBinContent(i+1))/(hNueSignal->GetBinWidth(i+1)*1000.) );
        hNueDataExcessStatOnly->SetBinError(i+1,sqrt( pow(dataError,2)+pow(MCstatError,2) )/(hNueDataExcess->GetBinWidth(i+1)*1000.));

    }
    std::cout << std::endl;

    //TCanvas *cEnuQE_Excess = new TCanvas("cEnuQE_Excess","cEnuQE_Excess",800,600);
    TH1D *hEnuqeExcess = (TH1D*)data.hEnuqe->Clone("hEnuqeExcess");
    hEnuqeExcess->Add(hEnuQEBG,-1);

    
    for(int i=0;i<data.hEnuqe->GetNbinsX();i++){
        hEnuqeExcess->SetBinError(i+1,data.hEnuqe->GetBinError(i+1) );
        fullOsc.hEnuqe->SetBinContent(i+1,fullOsc.hEnuqe->GetBinContent(i+1)/(fullOsc.hEnuqe->GetBinWidth(i+1)*1000.));
        fullOsc.hEnuqe->SetBinError(i+1,0);
    }
    hEnuqeExcess->GetXaxis()->SetTitle("E_{QE} [GeV]");
    hEnuqeExcess->GetYaxis()->SetTitle("excess events/MeV");

    hEnuqeExcess->GetYaxis()->SetRangeUser(-1,3);
    hNueDataExcess->SetLineColor(2);
    hNueDataExcess->SetMarkerStyle(20);
    hNueDataExcess->SetMarkerColor(2);
    //hNueDataExcess->Draw("same");
    hNueSignal->SetLineColor(2);
    //hNueSignal->Draw("hist same");
    fullOsc.hEnuqe->SetLineColor(1);
    //fullOsc.hEnuqe->Draw("hist same");
    TH1D *hBFexcess = (TH1D*)fullOsc.hEnuqe->Clone("hBFexcess");

    /*fullOsc.SetSin22theta(0.01);
    fullOsc.SetDm2(0.35);
    fullOsc.Loop();
    fullOsc.hEnuqe->SetLineColor(4);
    for(int i=0;i<data.hEnuqe->GetNbinsX();i++){
        fullOsc.hEnuqe->SetBinContent(i+1,fullOsc.hEnuqe->GetBinContent(i+1)/(fullOsc.hEnuqe->GetBinWidth(i+1)*1000.));
        //fullOsc.hEnuqe->SetBinError(i+1,sqrt(fullOsc.hEnuqe->GetBinContent(i+1))/(fullOsc.hEnuqe->GetBinWidth(i+1)*1000.));
        fullOsc.hEnuqe->SetBinError(i+1,0);
    }
    fullOsc.hEnuqe->Draw("hist same");
    TH1D *h1sigmaPoint = (TH1D*)fullOsc.hEnuqe->Clone("h1sigmaPoint");*/


    //cEnuQE_Excess->SaveAs("cEnuQE_Excess.pdf");

    //fullOsc.hEnuqe->Draw("hist same");

    /*TCanvas *cR = new TCanvas("cR","cR",800,600);
    data.hR->SetLineColor(1);
    data.hR->SetMarkerStyle(20);
    data.hR->Draw("e");
    THStack *hMC_R = new THStack("hMC_R","hMC_R");
    DirtMC.hR->SetFillColor(kDirtColor);
    for(int i=0;i<OtherMC.hEnuqe.size();i++){
        hMC_R->Add(OtherMC.hR_a[i]);
        if(i == OtherMC.kOther)hMC_R->Add(DirtMC.hR);
    }
    hMC_R->Draw("same hist");
    data.hR->Draw("same e");
    cR->SaveAs("cR.pdf");*/

    /*TCanvas *cMCEVNT_EVWT_MC = new TCanvas("cMCEVNT_EVWT_MC","cMCEVNT_EVWT_MC",800,600);
    OtherMC.hMCEVNT_EVWT->Draw("hist");
    cMCEVNT_EVWT_MC->cd()->SetLogy();
    cMCEVNT_EVWT_MC->SaveAs("cMCEVNT_EVWT_MC.pdf");*/

    /*TCanvas *cMCEVNT_EVWT_fullOsc = new TCanvas("cMCEVNT_EVWT_fullOsc","cMCEVNT_EVWT_fullOsc",800,600);
    fullOsc.hMCEVNT_EVWT->Draw("hist");
    cMCEVNT_EVWT_fullOsc->cd()->SetLogy();
    cMCEVNT_EVWT_fullOsc->SaveAs("cMCEVNT_EVWT_fullOsc.pdf");*/
    //PlotFull(data.hEvisCosTheta,DirtMC.hEvisCosTheta,fullOsc.hEvisCosTheta,OtherMC.hEvisCosTheta);
    //PlotFull(data.hEvisCosTheta_09,DirtMC.hEvisCosTheta_09,fullOsc.hEvisCosTheta_09,OtherMC.hEvisCosTheta_09);

    /*PlotFull(data.hEvisPperp,DirtMC.hEvisPperp,fullOsc.hEvisPperp,OtherMC.hEvisPperpComponents);
    PlotFull(data.hEvisSinTheta,DirtMC.hEvisSinTheta,fullOsc.hEvisSinTheta,OtherMC.hEvisSinTheta);
    PlotFull(data.hRnormEvis,DirtMC.hRnormEvis,fullOsc.hRnormEvis,OtherMC.hRnormEvis);
    PlotFull(data.hREvis,DirtMC.hREvis,fullOsc.hREvis,OtherMC.hREvis);
    PlotFull(data.hXEvis,DirtMC.hXEvis,fullOsc.hXEvis,OtherMC.hXEvis);
    PlotFull(data.hYEvis,DirtMC.hYEvis,fullOsc.hYEvis,OtherMC.hYEvis);
    PlotFull(data.hZEvis,DirtMC.hZEvis,fullOsc.hZEvis,OtherMC.hZEvis);
    PlotFull(data.hDistToMeanCerEvis,DirtMC.hDistToMeanCerEvis,fullOsc.hDistToMeanCerEvis,OtherMC.hDistToMeanCerEvis);
    PlotFull(data.hFEvis,DirtMC.hFEvis,fullOsc.hFEvis,OtherMC.hFEvis);
    PlotFull(data.hRendEvis,DirtMC.hRendEvis,fullOsc.hRendEvis,OtherMC.hRendEvis);
    PlotFull(data.hUdotREvis,DirtMC.hUdotREvis,fullOsc.hUdotREvis,OtherMC.hUdotREvis);
    PlotFull(data.hX2Y2Evis,DirtMC.hX2Y2Evis,fullOsc.hX2Y2Evis,OtherMC.hX2Y2Evis);*/

    //data.hEvisPperp->SetName("hEvisPperp_Coherent");
    //PlotFull(data.hEvisPperp,DirtMC.hPperpEvis_Coherent,fullOsc.hPperpEvis_Coherent,OtherMC.hPperpEvis_Coherent);

    //Compare1DXexcess2coherent(data.hEvisPperp,DirtMC.hEvisPperp,fullOsc.hEvisPperp,OtherMC.hEvisPperpComponents,OtherMC.hPperpEvis_Coherent[OtherMC.kpi0]);

    /*TCanvas *cCoherent = new TCanvas("cCoherent","cCoherent",800,600);
    OtherMC.hPperpEvis_Coherent[OtherMC.kpi0]->Draw("colz");
    TH2D *hBGprediction = (TH2D*)DirtMC.hEvisPperp->Clone("hBGprediction");
    for(int i=0;i<OtherMC.hEvisPperpComponents.size();i++){
        hBGprediction->Add(OtherMC.hEvisPperpComponents[i]);
    }
    hBGprediction->Add(fullOsc.hEvisPperp);
    double chi2 = 0;
    double scaling = 0.005;
    int imin = 0;
    const int Ntry = 200;
    double chi2min = 1000;
    OtherMC.hPperpEvis_Coherent[OtherMC.kpi0]->Scale(scaling);
    double chi2aray[Ntry];
    TGraph *gChi2 = new TGraph();
    for(int i=0;i<Ntry;i++){
        hBGprediction->Add(OtherMC.hPperpEvis_Coherent[OtherMC.kpi0]);
        chi2aray[i] = GetChi2(data.hEvisPperp,hBGprediction);
        if(chi2aray[i] < chi2min){chi2min = chi2aray[i]; imin = i;}
    }
    int isigma_m, isigma_p;
    for(int i=0;i<Ntry;i++){
        if(i>0 && chi2aray[i-1] > chi2min+1 && chi2aray[i] <= chi2min+1)isigma_m = i;
        if(i<Ntry-1 && chi2aray[i] <= chi2min+1 && chi2aray[i+1] > chi2min+1)isigma_p = i;
        gChi2->SetPoint(i,(i+1)*scaling,chi2aray[i]-chi2min);
    }
    std::cout << "best fit : i = " << imin << " need to add => " << (imin+1)*scaling << " of pi0 coherent "<< std::endl;
    std::cout << "isigma_m = " << isigma_m << " => " << (isigma_m+1)*scaling << std::endl;
    std::cout << "isigma_p = " << isigma_p << " => " << (isigma_p+1)*scaling << std::endl;
    std::cout << chi2min << std::endl;
    gChi2->SetTitle(";Additional coherent fraction;#Delta #chi^{2}");
    gChi2->Draw("ALP");
    cCoherent->SaveAs("cCoherent.pdf");*/

    TCanvas *cCosTheta = new TCanvas("cCosTheta","cCosTheta",800,600);
    data.hCosTheta->SetMarkerStyle(20);
    data.hCosTheta->SetLineColor(1);

    THStack *hMC_CosTheta = new THStack("hMC_CosTheta","hMC_CosTheta");
    DirtMC.hCosTheta->SetFillColor(kDirtColor);
    DirtMC.hCosTheta->SetLineColor(kDirtColor);

    OtherMC.hCosTheta[0]->SetLineColor(OtherMC.hCosTheta[0]->GetFillColor());
    hMC_CosTheta->Add(OtherMC.hCosTheta[0]);
    TH1D *hCosThetaBG = (TH1D*)OtherMC.hCosTheta[0]->Clone("hCosThetaBG");

    hMC_CosTheta->Add(DirtMC.hCosTheta);
    hCosThetaBG->Add(DirtMC.hCosTheta,1);
    for(int i=1;i<OtherMC.hCosTheta.size()-3;i++){

        OtherMC.hCosTheta[i]->SetLineColor(OtherMC.hCosTheta[i]->GetFillColor());
        hMC_CosTheta->Add(OtherMC.hCosTheta[i]);
        hCosThetaBG->Add(OtherMC.hCosTheta[i],1);
    }

    hMC_CosTheta->Add(fullOsc.hCosTheta);
    //hMC_Enuqe->Add(fullOsc.hEnuqe);

    data.hCosTheta->Draw("e");
    data.hCosTheta->GetXaxis()->SetTitle("cos #theta");
    data.hCosTheta->GetYaxis()->SetTitle("events");
    hMC_CosTheta->Draw("hist same");

    OtherMC.hCosTheta[OtherMC.hCosTheta.size()-3]->SetLineColor(1);
    OtherMC.hCosTheta[OtherMC.hCosTheta.size()-3]->SetLineColor(1);
    OtherMC.hCosTheta[OtherMC.hCosTheta.size()-3]->SetLineStyle(3);
    OtherMC.hCosTheta[OtherMC.hCosTheta.size()-3]->SetLineWidth(1);
    OtherMC.hCosTheta[OtherMC.hCosTheta.size()-3]->SetFillStyle(3004);
    OtherMC.hCosTheta[OtherMC.hCosTheta.size()-3]->SetFillColor(1);
    OtherMC.hCosTheta[OtherMC.hCosTheta.size()-3]->Draw("hist same");


    data.hCosTheta->Draw("same e");
    data.hCosTheta->GetYaxis()->SetRangeUser(0,900);

    TLegend *legStackCosTheta = new TLegend(0.13,0.7,0.7,0.87);
    legStackCosTheta->SetNColumns(3);
    legStackCosTheta->SetTextSize(0.025);
    legStackCosTheta->AddEntry(data.hCosTheta,"data","LP");

    legStackCosTheta->AddEntry(OtherMC.hCosTheta[0],Form("%s",HistNames[0].c_str()),"F");
    legStackCosTheta->AddEntry(DirtMC.hCosTheta,Form("%s",HistNames[1].c_str()),"F");

    for(int i=1;i<OtherMC.hCosTheta.size()-2;i++){
        legStackCosTheta->AddEntry(OtherMC.hCosTheta[i],Form("%s",HistNames[i+1].c_str()),"F");
    }
    legStackCosTheta->Draw();

    TH1D *hCosThetaExcess = (TH1D*)data.hCosTheta->Clone("hCosThetaExcess");
    hCosThetaExcess->Add(hCosThetaBG,-1.);
    hCosThetaExcess->Add(fullOsc.hCosTheta,-1);
    hCosThetaExcess->Draw();
    cCosTheta->SaveAs("cCosTheta.pdf");


    //return;
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    TCanvas *cBestFitSigma = new TCanvas("cBestFitSigma","cBestFitSigma",800,600);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Get Keng's results
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TFile *fFitKeng = TFile::Open("../../Fitter/output/fit_oct19_histNunubar_kengSystematics.root");
    TH1D *hExcessKeng = (TH1D*)fFitKeng->Get("Nue Data");
    TH1D *hBGKeng = (TH1D*)fFitKeng->Get("Nue MC constrained Null");
    for(int i=0;i<hExcessKeng->GetNbinsX();i++){
        double valdata = hExcessKeng->GetBinContent(i+1);
        double errdata = hExcessKeng->GetBinError(i+1);
        double valMC = hBGKeng->GetBinContent(i+1);
        double errMC = hBGKeng->GetBinError(i+1);
        double err = sqrt(errdata*errdata+errMC*errMC);
        double dE = hExcessKeng->GetBinWidth(i+1)*1000.;
        hExcessKeng->SetBinContent(i+1,  (valdata-valMC)/dE);
        hExcessKeng->SetBinError(i+1,  err/dE);
    }

    Float_t Ebins[14] = {0,0.15, 0.2, 0.3, 0.375, 0.475, 0.55, 0.675, 0.8, 0.95, 1.1, 1.3, 1.5, 2};
    TH1D *hExcessXcheat = new TH1D("hExcessXcheat","",sizeof(Ebins)/sizeof(Float_t) - 1,Ebins);
    TH1D *hNueDataExcessStatOnlyXcheat = new TH1D("hNueDataExcessStatOnlyXcheat","",sizeof(Ebins)/sizeof(Float_t) - 1,Ebins);
    for(int i=0;i<hExcessXcheat->GetNbinsX();i++){
        hExcessXcheat->SetBinContent(i+3,hNueDataExcess->GetBinContent(i+1));
        hExcessXcheat->SetBinError(i+3,hNueDataExcess->GetBinError(i+1));

        hNueDataExcessStatOnlyXcheat->SetBinContent(i+3,hNueDataExcessStatOnly->GetBinContent(i+1));
        hNueDataExcessStatOnlyXcheat->SetBinError(i+3,hNueDataExcessStatOnly->GetBinError(i+1));
    }

    hExcessXcheat->SetTitle(";E_{#nu}^{QE} [GeV];excess events/MeV");
    hExcessXcheat->SetLineWidth(2);
    hExcessXcheat->SetMarkerStyle(20);
    hExcessXcheat->SetLineColor(1);
    hExcessXcheat->SetMarkerColor(1);

    hExcessXcheat->Draw("e");
    hExcessXcheat->GetYaxis()->SetRangeUser(-0.5,3);

    hNueDataExcessStatOnlyXcheat->SetLineWidth(6);
    hNueDataExcessStatOnlyXcheat->SetLineColor(kSpring-5);
    hNueDataExcessStatOnlyXcheat->SetMarkerStyle(1);
    hNueDataExcessStatOnlyXcheat->SetMarkerColor(kSpring-5);
    hNueDataExcessStatOnlyXcheat->Draw("same e");

    
    std::cout << "PhaseSpacePoints.size() = "<< PhaseSpacePoints.size() << std::endl;
    hBFexcess->SetLineColor(2);
    hBFexcess->SetLineWidth(2);
    hBFexcess->SetTitle(";E_{#nu}^{QE} [GeV];excess events/MeV");
    hBFexcess->Draw("same C");
    hBFexcess->GetYaxis()->SetRangeUser(-0.5,3);


    TGraphAsymmErrors *g1sigma;
    if(Generate1sigma){
        std::vector<TH1D*> h1sBestFit(PhaseSpacePoints.size());
        for(int i=0;i<PhaseSpacePoints.size();i++){
            std::cout << Form("%2.2f",i*100./PhaseSpacePoints.size()) << std::endl;
            fullOsc.SetSin22theta(PhaseSpacePoints[i].first);
            fullOsc.SetDm2(PhaseSpacePoints[i].second);
            fullOsc.Loop();
            for(int j=0;j<fullOsc.hEnuqe->GetNbinsX();j++){
                fullOsc.hEnuqe->SetBinContent(j+1,fullOsc.hEnuqe->GetBinContent(j+1)/(fullOsc.hEnuqe->GetBinWidth(j+1)*1000.));
            }
            h1sBestFit[i] = fullOsc.hEnuqe;
        }


        g1sigma = new TGraphAsymmErrors();
        double ymin = 1e9;
        double ymax = -1e9;
        double thisBF;
        g1sigma->SetPoint(0,-1,0);
        g1sigma->SetPointError(0,0,0,0,0);
        g1sigma->SetPoint(1,0,0);
        g1sigma->SetPointError(1,0,0,0,0);
        for(int i=0;i<h1sBestFit[0]->GetNbinsX()-1;i++){
            ymin = 1e9;
            ymax = -1e9;
            thisBF = hBFexcess->GetBinContent(i+1);
            for(int j=0;j<h1sBestFit.size();j++){
                if(h1sBestFit[j]->GetBinContent(i+1) > ymax)ymax = h1sBestFit[j]->GetBinContent(i+1);
                if(h1sBestFit[j]->GetBinContent(i+1) < ymin)ymin = h1sBestFit[j]->GetBinContent(i+1);
            }
            std::cout << "bin " << i << " center "<< hBFexcess->GetBinCenter(i+1) << std::endl;
            g1sigma->SetPoint(i+2,hBFexcess->GetBinCenter(i+1),thisBF);
            g1sigma->SetPointError(i+2,0.5*hBFexcess->GetBinWidth(i+1),0.5*hBFexcess->GetBinWidth(i+1),thisBF-ymin,ymax-thisBF);

        }
        g1sigma->SetPoint(     h1sBestFit[0]->GetNbinsX()+1,2,hBFexcess->GetBinContent(hBFexcess->GetNbinsX()));
        g1sigma->SetPointError(h1sBestFit[0]->GetNbinsX()+1,0,0,0,0);
            g1sigma->SetLineColor(2);
            g1sigma->SetLineWidth(2);
            g1sigma->SetFillColorAlpha(1,0.5);
            g1sigma->Draw("same PC E4");
        g1sigma->SetNameTitle("g1sigma","g1sigma;E_{#nu}^{QE} [GeV];events/MeV");
        g1sigma->SaveAs("BF1sigmaBand.root");
    }
    else{
        TFile *f1sigma = TFile::Open("BF1sigmaBand.root");
        g1sigma = (TGraphAsymmErrors*)f1sigma->Get("g1sigma");
    }
    g1sigma->SetLineColor(2);
    g1sigma->SetLineWidth(2);
    g1sigma->SetFillColorAlpha(1,0.5);
    if(plot1sigmaBand)g1sigma->Draw("same PC E4");

    hNueDataExcess->SetLineWidth(2);
    hNueDataExcess->SetMarkerStyle(20);
    hNueDataExcess->SetLineColor(1);
    hNueDataExcess->SetMarkerColor(1);
    //hNueDataExcess->Draw("same");

    hEnuqeExcess->SetLineColor(4);
    hEnuqeExcess->SetMarkerColor(4);
    hEnuqeExcess->SetMarkerStyle(22);
    //hEnuqeExcess->Draw("same");

    //h1sigmaPoint->SetLineWidth(2);
    //h1sigmaPoint->Draw("same C");
    //hBFexcess->Draw("same C");

    hNueDataExcessStatOnly->SetLineWidth(6);
    hNueDataExcessStatOnly->SetLineColor(kSpring-5);
    hNueDataExcessStatOnly->SetMarkerStyle(1);
    hNueDataExcessStatOnly->SetMarkerColor(kSpring-5);
    //hNueDataExcessStatOnly->Draw("same e");


    //hEnuqeExcess->Draw("same");

    TH1D *hEnuqeExcess1stbin = new TH1D("hEnuqeExcess1stbin","hEnuqeExcess1stbin",1,0.15,0.2);
    //hEnuqeExcess1stbin->SetBinContent(1,hExcessKeng->GetBinContent(1));
    //hEnuqeExcess1stbin->SetBinError(1,hExcessKeng->GetBinError(1));
    hEnuqeExcess1stbin->SetBinContent(1,hEnuqeExcess->GetBinContent(2));
    hEnuqeExcess1stbin->SetBinError(1,hEnuqeExcess->GetBinError(2));
    //hEnuqeExcess1stbin->SetLineColor(kSpring-5); // green bars
    hEnuqeExcess1stbin->SetLineColor(4);
    hEnuqeExcess1stbin->SetLineWidth(4);
    hEnuqeExcess1stbin->SetMarkerColor(4);
    hEnuqeExcess1stbin->SetMarkerStyle(22);
    hEnuqeExcess1stbin->Draw("same e");

    TLegend *legBestFitSigma = new TLegend(0.45,0.48,0.87,0.7);
    legBestFitSigma->AddEntry(hNueDataExcess,"data - constr. BG (stat. + syst. errors)","LP");
    legBestFitSigma->AddEntry(hNueDataExcessStatOnly,"statistical errors only","LP");
    legBestFitSigma->AddEntry(hEnuqeExcess1stbin,"data - unconstr. BG (stat. errors only)","LP");
    legBestFitSigma->AddEntry(g1sigma,Form("Best fit (sin^{2} 2#theta, #Delta m^{2}) = (%.3f,%.3f eV^{2})",BFst, BFdm),"FL");
    //legBestFitSigma->AddEntry(h1sigmaPoint,"1#sigma (sin^{2} 2#theta, #Delta m^{2}) = (0.01,0.35 eV^{2})","L");

    //////////////////////////////////////////////////////////////////////
    // drawing examples of fot shapes
    //////////////////////////////////////////////////////////////////////
    if(PlotExamples){
        std::vector<TH1D*> hExamples(ExampleParameters.size());
        for(int i=0;i<ExampleParameters.size();i++){
            fullOsc.SetSin22theta(ExampleParameters[i].first);
            fullOsc.SetDm2(ExampleParameters[i].second);
            std::cout << ExampleParameters[i].first << " " << ExampleParameters[i].second << std::endl;
            fullOsc.Loop();
            for(int j=0;j<fullOsc.hEnuqe->GetNbinsX();j++){
                fullOsc.hEnuqe->SetBinContent(j+1,fullOsc.hEnuqe->GetBinContent(j+1)/(fullOsc.hEnuqe->GetBinWidth(j+1)*1000.));
                fullOsc.hEnuqe->SetBinError(j+1,0);
            }
            hExamples[i] = fullOsc.hEnuqe;
            legBestFitSigma->AddEntry(hExamples[i],Form("(sin^{2} 2#theta, #Delta m^{2}) = (%.3f,%.3f eV^{2})",ExampleParameters[i].first,ExampleParameters[i].second),"L");
        }

        for(int i=0;i<hExamples.size();i++){
            hExamples[i]->SetLineColor(i+1);
            hExamples[i]->SetLineStyle(2);
            hExamples[i]->Draw("same C");

        }

        //hNueDataExcessStatOnly->Draw("same e");
    }



    //////////////////////////////////////////////////////////////////////
    ///drawing the limits of the fit region
    //////////////////////////////////////////////////////////////////////
    if(DrawFitLimints){
        TLine *lLowE = new TLine(0.200,-0.5,0.200,3);
        lLowE->SetLineWidth(1);
        lLowE->SetLineColor(1);
        lLowE->Draw();
        TArrow *arrow1= new TArrow(0.2,2.5,1.250,2.5,0.02,"< >");
        arrow1->SetLineWidth(2);
        arrow1->Draw();
        TLatex *FitRange = new TLatex(0.27,2.65,"Fit range: 200 - 1250 MeV");
        FitRange->SetTextSize(0.03);
        FitRange->Draw();

        TLine *lHighE = new TLine(1.250,-0.5,1.250,3);
        lHighE->SetLineWidth(1);
        lHighE->SetLineColor(1);
        lHighE->Draw();
    }


    //hEnuqeExcess->Draw("same");
    //hNueDataExcess->GetXaxis()->SetRange(0,10);
    //hNueDataExcess->Draw("same e");

    hExcessXcheat->Draw("same e");
    hNueDataExcessStatOnlyXcheat->Draw("same e");

    //hExcessKeng->Draw("same e");


    legBestFitSigma->SetTextSize(0.025);
    legBestFitSigma->Draw();

    TBox *bAxisHide = new TBox(1.9,-0.53,2.1,-0.75);
    bAxisHide->SetFillColor(0);
    bAxisHide->SetFillStyle(1001);
    bAxisHide->SetLineColor(1);
    bAxisHide->Draw();


    TLatex label;
    label.SetTextSize(1.41*hNueDataExcess->GetXaxis()->GetLabelSize());
    label.SetTextFont(hNueDataExcess->GetXaxis()->GetLabelFont());
    label.DrawText(1.98,-0.71,"3");

    cBestFitSigma->Modified();
    cBestFitSigma->Update();

    //hEnuqeExcess->Draw("same");
    hExcessXcheat->Draw("same e");

    if((std::string)fFitOutput->GetName() == (std::string)("../../Fitter/output/fit_oct19_histNunubar_kengSystematics.root"))cBestFitSigma->SaveAs("cBestFitSigma_keng.pdf");
    else cBestFitSigma->SaveAs("cBestFitSigma.pdf");

    cCLgrid->SaveAs("cCLgrid.pdf");

    std::cout << "cCLgrid printed" << std::endl;

    /*TCanvas *cExcessMinusBF = new TCanvas();
    TH1D *hExcessMinusBF = (TH1D*)hExcessXcheat->Clone("hExcessMinusBF");
    hExcessMinusBF->GetYaxis()->SetTitle("excess - global BF (events/MeV)");
    hExcessMinusBF->Add(hBFexcess,-1);
    hExcessMinusBF->Draw();

    std::cout << "hExcessMinusBF drawn" << std::endl;*/

    /*TBox *bAxisHide2 = new TBox(1.9,-0.6,2.1,-0.78);
    bAxisHide2->SetFillColor(0);
    bAxisHide2->SetFillStyle(1001);
    bAxisHide2->SetLineColor(1);
    bAxisHide2->Draw();


    TLatex label2;
    label2.SetTextSize(1.41*hNueDataExcess->GetXaxis()->GetLabelSize());
    label2.SetTextFont(hNueDataExcess->GetXaxis()->GetLabelFont());
    label2.DrawText(1.98,-0.76,"3");*/
    
}
