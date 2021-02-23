bool nuonly = false;

void GenerateFile(TH1D *h, std::string fileName){
    if(nuonly)fileName+="_nuonly";
    std::cout << fileName << " : ";
    ofstream fout(Form("%s.txt",fileName.c_str()));
    for(int i=0;i<h->GetNbinsX();i++){
        std::cout << Form("%.3f",h->GetBinContent(i+1)) << "\t";
        fout << Form("%.3f",h->GetBinContent(i+1)) << "\t";
    }
    std::cout << std::endl;
}
void ReadFitOutputFile(){

    TFile *fIN;
    if(!nuonly)fIN = TFile::Open("fit_oct19_histNunubar.root");
    if(nuonly)fIN = TFile::Open("fit_oct19_hist_nuonly.root");

    TH1D *hNueData     = (TH1D*)fIN->Get("Nue Data");
    TH1D *hNueMC       = (TH1D*)fIN->Get("Nue MC constrained Null");
    TH1D *hNumuData    = (TH1D*)fIN->Get("Numu Data");
    TH1D *hNumuMC      = (TH1D*)fIN->Get("Numu MC");

    TH1D *hNueSignal     = (TH1D*)fIN->Get("Nue Signal");

    TH1D *hNuebarMC;
    TH1D *hNumubarMC;
    TH1D *hNuebarData;
    TH1D *hNumubarData;

    if(!nuonly){
        hNuebarMC    = (TH1D*)fIN->Get("Nuebar MC");
        hNumubarMC   = (TH1D*)fIN->Get("Numubar MC");
        hNuebarData  = (TH1D*)fIN->Get("Nuebar Data");
        hNumubarData = (TH1D*)fIN->Get("Numubar Data");
    }

    GenerateFile(hNueData,"NueData_binContent");
    GenerateFile(hNueMC,"NueBG_binContent");
    GenerateFile(hNumuData,"NumuData_binContent");
    GenerateFile(hNumuMC,"NumuMC_binContent");

    if(!nuonly){

        GenerateFile(hNuebarData,"NuebarData_binContent");
        GenerateFile(hNuebarMC,"NuebarBG_binContent");
        GenerateFile(hNumubarData,"NumubarData_binContent");
        GenerateFile(hNumubarMC,"NumubarMC_binContent");
    }

    TCanvas *cDrawExcess = new TCanvas("cDrawExcess","cDrawExcess",800,600);
    TH1D* hNueDataExcess = (TH1D*)hNueData->Clone("hNueDataExcess");
    double tmpErrorsData[hNueData->GetNbinsX()];
    double tmpErrorsMC[hNueData->GetNbinsX()];
    double binBoundaries[hNueData->GetNbinsX()+1];
    binBoundaries[0] = 200;
    for(int i=0;i<hNueData->GetNbinsX();i++){
        tmpErrorsData[i] = sqrt(hNueDataExcess->GetBinContent(i+1));
        tmpErrorsMC[i] = sqrt(hNueMC->GetBinError(i+1));

        hNueDataExcess->SetBinContent(i+1,(hNueDataExcess->GetBinContent(i+1)-hNueMC->GetBinContent(i+1))/(hNueDataExcess->GetBinWidth(i+1)*1000.) );
        hNueDataExcess->SetBinError(i+1,sqrt(hNueData->GetBinContent(i+1))/(hNueDataExcess->GetBinWidth(i+1)*1000.));

        binBoundaries[i+1] = (hNueDataExcess->GetBinLowEdge(i+1)+hNueData->GetBinWidth(i+1))*1000.;
    }

    std::cout << "Bin boundaries : "; for(int i=0;i<hNueData->GetNbinsX()+1;i++){std::cout << binBoundaries[i] << "\t" ;} std::cout << std::endl;
    std::cout << "Excess : ";
    for(int i=0;i<hNueDataExcess->GetNbinsX();i++){
        std::cout << hNueDataExcess->GetBinContent(i+1) << "\t";
    }
    std::cout << std::endl;
    std::cout << "Error : ";
    for(int i=0;i<hNueDataExcess->GetNbinsX();i++){
        std::cout << hNueDataExcess->GetBinError(i+1) << "\t";
    }
    std::cout << std::endl;
    //hNueData->Draw("e");
    //hNueMC->Draw("same e");
    //hNueDataExcess->Add(hNueMC,-1);
    hNueDataExcess->Draw("e");
    //GenerateFile(hNueDataExcess,"Nue Excess");
    //hNueSignal->SetLineColor(2);
    //hNueSignal->SetLineWidth(2);
    //hNueSignal->Draw("same e");
}
