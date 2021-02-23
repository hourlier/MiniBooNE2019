void m_get_prob(string vecfile="null_chi2_200.vec", Double_t chi2BF=0, Double_t chi2NL=0, string outputFigName="temp.png", string title="")
{
  /* 
     constrained chi2 for XXX-1250 for 11.27
                E>200           E>475
    Null       16.55          7.82
     BF         4.79          3.29
  */
  
  //gROOT.ProcessLine(".x ~/bin/root/rootlogon.C");
  ResetColorMap("set1", 5, 21);
  TH1F hchi2("hchi2",title.c_str(),2000,0,200);
  hchi2.GetXaxis()->SetTitle("#chi^{2}");
  ifstream fin(vecfile);
  if(!fin) {
    cout << "there's something wrong with the file " << vecfile << endl; 
    return;
  }
  Int_t N=0;
  Int_t NaboveBF=0;
  Int_t NaboveNL=0;
  Double_t x;
  while (fin>>x) {
    hchi2.Fill(x);
    if (x>=chi2BF) NaboveBF++;
    if (x>=chi2NL) NaboveNL++;
    N++;
    //if (N%100000==0) cout << N << endl;
  }
  fin.close();
  TCanvas c1("c1","",550,500);
  TF1 fchi2("fchi2","[0]*pow(x,[1]/2.-1)*exp(-x/2.)/(TMath::Gamma([1]/2)*pow(2.,[1]/2))",0.,50.);
  Double_t norm =hchi2.GetSumOfWeights()*hchi2.GetBinWidth(1);
  fchi2.SetParameter(0,norm);
  fchi2.SetParLimits(0,norm,norm);
  fchi2.SetParameter(1,hchi2.GetMean());
  hchi2.SetStats(0);
  hchi2.Fit("fchi2");
  double pBF = ROOT::Math::chisquared_cdf_c(chi2BF, fchi2.GetParameter(1) );
  double pNL = ROOT::Math::chisquared_cdf_c(chi2NL, fchi2.GetParameter(1) +2);
  cout << Form("NaboveBF/N = %5d/%5d = %.2f%%\n", NaboveBF, N, double(NaboveBF)/N*100.);
  cout << Form("NaboveNL/N = %5d/%5d = %.2f%%\n", NaboveNL, N, double(NaboveNL)/N*100.);
  cout <<"p(BF) = "<< pBF * 100.  << "%" <<endl;  // pvalue is calcualted by assuming chiSq and gamma
  cout <<"p(NL) = "<< pNL * 100.  << "%" <<endl;;  // pvalue is calcualted by assuming chiSq and gamma
  double yMax = hchi2.GetMaximum();
  cout << yMax << endl;
  TLine lBF(chi2BF, 0, chi2BF, yMax/2);
  TLine lNL(chi2NL, 0, chi2NL, yMax/2);
  lBF.SetLineWidth(2);
  lNL.SetLineWidth(2);
  lBF.SetLineColor(22);
  lNL.SetLineColor(23);
  lBF.Draw();
  lNL.Draw();

  //cout<<"p(NDF-2)="<<(1-TMath::Gamma(0.5*(fchi2.GetParameter(1)-2),0.5*chi2BF))*100<<endl;


  TLegend leg1(0.5,0.65,0.88,0.88);
  leg1.SetTextFont(82);
  leg1.SetFillColor(0);
  leg1.SetBorderSize(0);
  leg1.AddEntry(&hchi2,"Fake exp. @ BF","l");
  leg1.AddEntry(&fchi2,Form("NDF=%5.2f(#chi^{2} dist.)",fchi2.GetParameter(1)),"l");
  leg1.AddEntry(&lBF  ,Form("p(chiSqBF=%.3f) = %5.2f%%",chi2BF, double(NaboveBF)/N*100.), "l");
  leg1.AddEntry(&lNL  ,Form("p(chiSqNL=%.3f) = %5.2f%%",chi2NL, double(NaboveNL)/N*100.), "l");
  leg1.Draw();
  c1.Print(outputFigName.c_str());
  cout << endl << endl;
  
}


void get_prob(){
  // try to get published POT 6.46 results
  //m_get_prob("fit_nu_6.46_200-1250", "fake_data/fake_nu_200_1point/fit_nu_6.46_200-1250.vec", 13.24);

  // get POT 9.38
  //m_get_prob("fit_nu_9.38_200-1250", "fake_data/fake_nu_9.38_200_1point/nu_9.38_E1.02_200_1250_numuC.vec", 10.539561);

  // get POT 9.38 nu mode + 11.27 nubar mode
  //m_get_prob("fit_nunubar_9.38_E1.02_200_1300_numumubarC", "fake_data/fake_nunubar_9.38_11.27_200_1point/nunubar_9.38_11.27_200_numumubarC_1point.vec", 19.284481);


  // BF points for apr18
  //m_get_prob("fake_data/fake_apr18_1point_contNu_NueLastAsConstrain/fake_1point_BF.vec", //fit_nu12.84_nubar11.27_contNunubar.log
  //           9.90320, 24.87871, "figures/apr18/probability_contNu.pdf", "Contour Nu (Nue Last As Constrain)");
  //m_get_prob("fake_data/fake_apr18_1point_contNu_NueLastNotAsConstrain/fake_1point_BF.vec", //fit_nu12.84_nubar11.27_contNunubar.log
  //           10.16541, 25.72971, "figures/apr18/probability_contNu_NueLastNotAsConstrain.pdf", "Cont Nu (Nue Last Not As Constrain)");
//  //m_get_prob("fake_data/fake_apr18_1point_contNunubar_NueLastAsConstrain/fake_1point_BF.vec", 
//  //           19.44454, 47.11417, "figures/apr18/probability_contNunubar.pdf", "Cont Nunubar (Nue Last As Constrain)");
//  //m_get_prob("fake_data/fake_apr18_1point_contNunubar_NueLastNotAsConstrain/fake_1point_BF.vec", 
  //           20.05007, 48.23267, "figures/apr18/probability_contNunubar_NueLastNotAsConstrain.pdf", "Cont Nunubar (Nue Last Not As Constrain)");


  // Null point for apr18
  //m_get_prob("fake_data/fake_apr18_1pointNull_contNu_NueLastAsConstrain/fake_1point_BF.vec", //fit_nu12.84_nubar11.27_contNunubar.log
  //           9.90320, 24.87871, "figures/apr18/probabilityNull_BFVec_contNu.pdf", "Contour Nu (Nue Last As Constrain)");
  
  //m_get_prob("fake_data/fake_apr18_1pointNull_contNu_NueLastNotAsConstrain/fake_1point_BF.vec", //fit_nu12.84_nubar11.27_contNunubar.log
  //           10.16541, 25.72971, "figures/apr18/probabilityNull_BFVec_contNu_NueLastNotAsConstrain.pdf", "Cont Nu (Nue Last Not As Constrain)");

  //m_get_prob("fake_data/fake_apr18_1pointNull_contNunubar_NueLastAsConstrain/fake_1point_BF.vec", 
  //           19.44454, 47.11417, "figures/apr18/probabilityNull_BFVec_contNunubar.pdf", "Cont Nunubar (Nue Last As Constrain)");
  //m_get_prob("fake_data/fake_apr18_1pointNull_contNunubar_NueLastNotAsConstrain/fake_1point_BF.vec", 
  //           20.05007, 48.23267, "figures/apr18/probabilityNull_BFVec_contNunubar_NueLastNotAsConstrain.pdf", "Cont Nunubar (Nue Last Not As Constrain)");

   m_get_prob("fake_data/fake_apr18_1point_contNunubarLSND/fake_1point_BF.vec", //fit_nu12.84_nubar11.27_contNunubar.log
             22.40704, 62.51359, "figures/apr18/probability_BFVec_contNunubarLSND.pdf", "Contour Nunubar LSND");
  




}



