#include "Util.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <memory>
#include "ErrorMatrixROOT.h"
#include "FitMaster.h"
#include "MsgLog.h"

#include "TH2F.h"
#include "TString.h"
#include "TList.h"
#include "TObjString.h"
#include "TObjArray.h"
#include "TDecompChol.h"
#include "TRandom3.h"
#include "TMatrixD.h"

#include "TXMLNode.h"
#include "TXMLAttr.h"

using namespace std;

void Util::CorrectMSstat(ErrorMatrix* matrix, 
			 std::vector<Int_t> bin_mask, 
			 std::vector<Double_t> cv, 
			 std::vector<Double_t> ms_cv, 
			 Double_t ms_to_data_pot)
{
  /*
    This function corrects the diagonal elements of the fractional
    error matrix to account for the differences between the 
    central value in the MC and in the Multisims. Also it accounts
    for potentially different POT used to generate multisims.
    It applies correction only to bins set by bin_mask
  */
  MsgInfo("Util::Correcting diagonal elements of the matrix.");
  MsgInfo("Util::Multisim cv:");
  ostringstream oss;
  oss<<"      ";
  for (unsigned int i=0; i<ms_cv.size(); i++) oss<<setw(10)<<ms_cv[i];
  MsgInfo(oss.str());
  oss.str("");

  MsgInfo("Util::MC cv:");
  oss<<"      ";
  for (unsigned int i=0; i<cv.size(); i++) oss<<setw(10)<<cv[i];
  MsgInfo(oss.str());
  oss.str("");
  unsigned int nbin = ms_cv.size();
  for (unsigned int i=0; i<nbin; i++) {
    Double_t mii = (cv[i]/ms_cv[i])*(cv[i]/ms_cv[i])*matrix->GetElement(i,i);
    matrix->SetElement(i,i,mii);
  }
}

void Util::AddMonteCarloStatError(FitMaster* fit_master)
{
  std::shared_ptr<ErrorMatrix> mcstat = std::make_shared<ErrorMatrix>();
  Int_t dim = fit_master->fSumw2Vector.size();
  mcstat->Resize(dim,dim);
  mcstat->SetName("MC stat");
  for (int i=0; i<dim; i++) {
    Double_t m_ii = fit_master->fSumw2Vector[i];
    if ( fit_master->fEventVector[i] != 0. ) 
      m_ii /= (fit_master->fEventVector[i]*fit_master->fEventVector[i]);
    else 
      m_ii = 0.;

    mcstat->SetElement(i,i,m_ii);
  }
  fit_master->AddFracErrorMatrix(mcstat);
}

Double_t* Util::GetHistogramAxis(Int_t grid_points, Double_t *grid_vector)
{
  //code form CombinedFit/kumacs/smoothgrid_exec.C

  Double_t* histo_vector = new Double_t[grid_points+1];

  Double_t last_point, interval=0.;

  histo_vector[0] = grid_vector[0] - (grid_vector[1] - grid_vector[0])/2.0;
  last_point = grid_vector[0];

  for( int i = 1; i < grid_points; i++ ){
    interval = (grid_vector[i] - last_point)/2.0;
    histo_vector[i] = last_point + interval;
    last_point = grid_vector[i];
  }

  histo_vector[grid_points] = last_point + interval;

  return histo_vector;
}

std::map<std::string, std::string> Util::GetAttributeMap(TXMLNode* node)
{
  std::map<std::string, std::string> attr_map;

  TList* alist=node->GetAttributes();
  for (Int_t i=0; i< alist->GetSize();i++) {
    TXMLAttr* attr=dynamic_cast<TXMLAttr*> (alist->At(i));
    attr_map[attr->GetName()] = attr->GetValue();
  }

  return attr_map;
}

std::list<TXMLNode*> Util::GetSubNodeList(TXMLNode*)
{
  std::list<TXMLNode*> sub_node_list;

  return sub_node_list;
}

std::vector<Int_t> Util::MakeVectorI(std::string s, char* delim)
{
  std::vector<Int_t> vec;

  TString ts(s);
  TObjArray* ts_arr  = ts.Tokenize(delim);
  TIter iVec(ts_arr);
  while (TObjString* os=(TObjString*)iVec()) vec.push_back((os->GetString()).Atoi());

  return vec;
}

std::vector<Double_t> Util::MakeVectorD(std::string s, char* delim)
{
  std::vector<Double_t> vec;

  TString ts(s);
  TObjArray* ts_arr  = ts.Tokenize(delim);
  TIter iVec(ts_arr);
  while (TObjString* os=(TObjString*)iVec()) vec.push_back((os->GetString()).Atof());

  return vec;
}

std::vector<std::string> Util::MakeVectorS(std::string s, char* delim)
{
  std::vector<std::string> vec;
  
  TString ts(s);
  TObjArray* ts_arr  = ts.Tokenize(delim);
  TIter iVec(ts_arr);
  while (TObjString* os=(TObjString*)iVec()) {
    TString stripped=(os->GetString()).Strip(TString::kBoth,' ');
    vec.push_back(stripped.Data());
  }
  
  return vec;
}

bool Util::atob(const std::string s)
{
  if (s == "true")   return true;  
  if (s == "false")  return false;
  if (s == "kTRUE")  return true;  
  if (s == "kFALSE") return false;
  if (s == "TRUE")   return true;  
  if (s == "FALSE")  return false;
  if (s == "True")   return true;
  if (s == "False")  return false;
  MsgWarning("not recognized");
  return false;  // by default invalid strings are false
}

ErrorMatrix* Util::GetRebinnedMatrix(const ErrorMatrix& mat, std::vector<Double_t> event_vec, std::vector<Int_t> rebin_vec)
{
  ErrorMatrix* rebinned_matrix=new ErrorMatrix();

  Int_t n_bins_new=0;
  for (unsigned int i=0;i<rebin_vec.size();i++) if (rebin_vec[i]>0) n_bins_new++;

  std::vector<std::vector<Double_t> > new_matrix;
  new_matrix.resize(n_bins_new);
  std::vector<Double_t > new_event_vec(n_bins_new);

  for (int i=0; i<n_bins_new; i++) new_matrix[i].resize(n_bins_new);

  for (Int_t i=0;i<mat.GetDimX();i++) {
    Int_t new_i = rebin_vec[i]-1;
    for (Int_t j=0;j<mat.GetDimY();j++) {
      Int_t new_j = rebin_vec[j]-1;
      if (rebin_vec[i] < 0 || rebin_vec[j] < 0) {
        MsgWarning("Util::Can't rebin! Invalid value in rebin vector.");
        return 0;
      }
     
      if (rebin_vec[i] != 0 && rebin_vec[j] !=0 ) {
        new_matrix[new_i][new_j] += mat.GetElement(i,j)*event_vec[i]*event_vec[j];
      }
    }
    if (rebin_vec[i] != 0 ) {
      new_event_vec[new_i] += event_vec[i];
    }    
  }

  rebinned_matrix->SetName(mat.GetName());
  rebinned_matrix->SetFactorAddedToDiag(mat.GetFactorAddedToDiag());
  rebinned_matrix->Resize(n_bins_new, n_bins_new);
  for (Int_t i=0;i<n_bins_new;i++) {
    for (Int_t j=0;j<n_bins_new;j++) {
      rebinned_matrix->SetElement(i,j,new_matrix[i][j]/(new_event_vec[i]*new_event_vec[j]));
    }
  }

  
  
  return rebinned_matrix;
}

std::vector<Double_t>  Util::GetOneFakeData(UInt_t random_seed, std::vector<Double_t> cv, const ErrorMatrix& errmat){
  //returns a vector of n fake data vectors drawn around a cv according to matrix errmat
  std::vector<Double_t> fake_data;

  const int dim=errmat.GetDimX();
  fake_data.resize(dim);
  
  //first decompose matrix
  TMatrixD mat(dim,dim);
  
  for (Int_t i=0;i<dim;i++) {
    fake_data[i]=0.;
    for (Int_t j=0;j<dim;j++) {
      mat(i,j)=errmat.GetElement(i,j);
    }
  }
  //Cholesky decomposition fails for sin22theta=0 since a part of a matrix is zero.
  //Either use very small sin22theta or rewrite this part (like CombinedFit_SystFluctuation.F)
  TDecompChol dc(mat);
  if (!dc.Decompose()) {
    MsgError("Can't decompose matrix and generate fake data");
    return fake_data;
  }

  Double_t* rands=new Double_t[dim];
  TRandom3* rnd=new TRandom3();
  rnd->SetSeed(random_seed);
  for(int j=0;j<dim;j++) 
    rands[j] = rnd->Gaus(0,1);
   
  for(int j=0;j<dim;j++){
    double row=0.;
    for(int k=0;k<dim;k++){
      row += dc.GetU()(k,j)*rands[k];
    }
    fake_data[j] = row + cv[j];
    //if negative set to 0
    if (fake_data[j]<0) {
      MsgWarning(MsgLog::Form("Warning:: Fake data in bin %d is negative. Setting it to 0", j));
      fake_data[j]=0;
    }
  }
  MsgLog::Flush();
  delete [] rands;
  delete rnd;
  return fake_data;


}

std::vector<std::vector<Double_t> > Util::GetFakeData(UInt_t random_seed, UInt_t n, std::vector<Double_t> cv, const ErrorMatrix& errmat)
{
  //returns a vector of n fake data vectors drawn around a cv according to matrix errmat

  std::vector<std::vector<Double_t> > all_fake_data;
  std::vector<Double_t> fake_data;
  for (UInt_t t=0; t<n; t++) {
    if(random_seed == 0)
      fake_data = GetOneFakeData(0, cv, errmat);
    else
      fake_data = GetOneFakeData(random_seed + t, cv, errmat);

    all_fake_data.push_back(fake_data);
  }
  MsgLog::Flush();
  return all_fake_data;
}

