void doTheThing(std::string fileName){
    TFile *fIN = TFile::Open(Form("fit_oct19_%s.root",fileName.c_str()),"READ");
    TH2F *hChi2 = (TH2F*)fIN->Get("chi2");
    TCanvas *cChi2 = new TCanvas(Form("cChi2_%s",fileName.c_str()),Form("cChi2_%s",fileName.c_str()),800,600);
    hChi2->Draw("colz");
    cChi2->SetLogx();
    cChi2->SetLogy();
    cChi2->SetLogz();

    ofstream fout(Form("Likelihood_surface_%s.txt",fileName.c_str()));
    fout << "dm2    sintheta    -2ln(L)" << std::endl;
    for(int i=0;i<hChi2->GetNbinsX();i++){
        for(int j=0;j<hChi2->GetNbinsY();j++){
            double dm2 = hChi2->GetYaxis()->GetBinCenter(j+1);
            double sintheta = hChi2->GetXaxis()->GetBinCenter(i+1);
            double likelihood = hChi2->GetBinContent(i+1,j+1);
            fout << dm2 << " " << sintheta << " " << likelihood << std::endl;
        }
    }
}
void GenerateLikelihoodSurface(){
    doTheThing("contour_nuonly");
    doTheThing("contNunubar");
}
