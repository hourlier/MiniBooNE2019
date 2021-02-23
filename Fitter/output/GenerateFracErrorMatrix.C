void doTheThing(std::string fileName){
    TFile *fIN = TFile::Open(Form("fit_oct19_%s.root",fileName.c_str()),"READ");
    TMatrixT<double> *matrix = (TMatrixT<double>*)fIN->Get("frac_error_matrix");

    ofstream fout(Form("frac_error_matrix_%s.txt",fileName.c_str()));
    std::cout << matrix->GetNrows() << std::endl;
    matrix->Draw("colz");
    for(int iY = 0;iY < matrix->GetNrows();iY++){
        for(int iX = 0;iX < matrix->GetNcols();iX++){
            fout << (*matrix)(iX,iY) << " ";
        }
        fout << std::endl;
    }
}

void GenerateFracErrorMatrix(){
    doTheThing("contour_nuonly");
    doTheThing("contNunubar");
}
