#include "ErrorMatrixROOT.h"
#include "Event.h"
#include "Consts.h"
#include "FuncStopwatch.h"
#include "MsgLog.h"

#include <iostream>
#include <iomanip>
#include <memory>
#include <sstream>

#include <stdlib.h>
#include <math.h>

#include "TVectorD.h"
#include "TMatrixD.h"
#include "TMatrixDEigen.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TKey.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TStyle.h"
using namespace std;

ErrorMatrix::ErrorMatrix()
{
  fFracErrorMatrix.clear();
  fFracErrHistID = 83;
  fCVhistID      = 61;
  fNx = 0;
  fNy = 0;
  fPositiveDef = false;
  fAddedToDiag = 0.;
}

ErrorMatrix::ErrorMatrix(const ErrorMatrix &matrix)
{
  fFracErrorMatrix.clear();

  fNx   = matrix.GetDimX();
  fNy   = matrix.GetDimY();

  std::string tmp(matrix.GetName());
  fName = tmp;
  fFracErrorMatrix.resize(fNx);
  for (int i=0;i<fNx; i++) fFracErrorMatrix[i].resize(fNy);

  for (int i=0;i<fNx; i++) {
    for (int j=0;j<fNy; j++) {
      fFracErrorMatrix[i][j] = matrix.GetElement(i,j);
    }
  }
  fAddedToDiag = matrix.GetFactorAddedToDiag();
  fVerbosity = matrix.GetVerbosity();
}

void ErrorMatrix::CopyFrom(const shared_ptr<ErrorMatrix> matrix)
{
  fFracErrorMatrix.clear();

  fNx   = matrix->GetDimX();
  fNy   = matrix->GetDimY();

  std::string tmp(matrix->GetName());
  fName = tmp;
  fFracErrorMatrix.resize(fNx);
  for (int i=0;i<fNx; i++) fFracErrorMatrix[i].resize(fNy);

  for (int i=0;i<fNx; i++) {
    for (int j=0;j<fNy; j++) {
      fFracErrorMatrix[i][j] = matrix->GetElement(i,j);
    }
  }
  fAddedToDiag = matrix->GetFactorAddedToDiag();
  fVerbosity = matrix->GetVerbosity();
}


ErrorMatrix::~ErrorMatrix()
{
}

ErrorMatrix ErrorMatrix::operator+(const ErrorMatrix& m) const
{
  if ( m.GetDimX() != this->GetDimX() &&
       m.GetDimY() != this->GetDimY() ) {
    MsgWarning( "Can't add matrices with different number of elements!" );
    MsgWarning( this->GetName() + " and " + m.GetName() );
    return *this;
  }
  
  ErrorMatrix mat;
  mat.Resize(this->GetDimX(),this->GetDimY());
  mat.SetName(this->GetName());
  mat.SetVerbosity(this->GetVerbosity());
  mat.SetFactorAddedToDiag(this->GetFactorAddedToDiag()+m.GetFactorAddedToDiag());

  for (Int_t i=0;i<fNx;i++) {
    for (Int_t j=0;j<fNy;j++) {
      Double_t sum=0.;
      sum = m.GetElement(i,j) + this->GetElement(i,j);
      mat.SetElement(i,j, sum);
    }
  }
  return mat;
}


void ErrorMatrix::AddBlock2Block(const std::shared_ptr<ErrorMatrix> matrix1, int iBinX1, int iBinY1,  
                                                                int iBinX0, int iBinY0, int nBinsX, int nBinsY)
{
  // Add block of matrix1 from (iBinX1, iBinY1) and (nBinsX, nBinsY) to this matrix's (iBinX0, iBinY0)
  // check initial points
  if(iBinX0<0 || iBinY0<0 || iBinX1<0 || iBinY1<0){
    MsgError("initial points should be larger than 0");
    return;
  }
  if( (iBinX0+nBinsX > this->GetDimX())    || (iBinY0+nBinsY > this->GetDimY())   ||
      (iBinX1+nBinsX > matrix1->GetDimX()) || (iBinY1+nBinsY > matrix1->GetDimY())  ){
    MsgError("final points should be smaller or equal to the matrix dimension");
    MsgError(MsgLog::Form("%d, %d, %d, %d, %d, %d",iBinX1,  iBinY1, iBinX0,  iBinY0,  nBinsX,  nBinsY));
    return;
  }

  for(int iX=0; iX<nBinsX; iX++){
    for(int iY=0; iY<nBinsY; iY++){
      fFracErrorMatrix[iBinX0+iX][iBinY0+iY] += matrix1->GetElement(iBinX1+iX, iBinY1+iY);
    }
  }

}
void ErrorMatrix::FillMatrix(std::string input_file)
{
  fInputFileName = input_file;
  TFile *fIn = new TFile(fInputFileName.c_str());
  if(fIn->IsOpen()!=true){
    MsgFatal("ERROR: Error opening input file ... ("+fInputFileName+")");
    exit(1);
  }

  // Get error matrix first
  TH2D* matrix=(TH2D*) fIn->Get(fName.c_str());
  if(matrix==NULL){
    matrix = (TH2D*) fIn->Get(Form("h%d", fFracErrHistID) ) ;
  }
  if(matrix==NULL){
    MsgError(MsgLog::Form("ERROR: Cannot find TH2D %s h%d in %s", fName.c_str(), fFracErrHistID, fInputFileName.c_str()) );
    fIn->ls();
    exit(1);
  }

  fNx=matrix->GetNbinsX();
  fNy=matrix->GetNbinsY();
  fFracErrorMatrix.resize(fNx);
  for (int i=0;i<fNx;i++) fFracErrorMatrix[i].resize(fNy);

  for (int i=0;i<fNx;i++) {
    for (int j=0;j<fNy;j++) {
      //  if ( (i<fNx/2 && j<fNy/2) || (i>=fNx/2 && j>=fNy/2) )
      fFracErrorMatrix[i][j] = matrix->GetBinContent(i+1,j+1);
      //  else
      // fFracErrorMatrix[i][j] = 0;
    }
  }

  // Get Central Value
  TH1D* hcv=(TH1D*) fIn->Get("bigcv");
  if(hcv==NULL) hcv = (TH1D*) fIn->Get(Form("h%d", fCVhistID) ) ;
  if(hcv) {
    for (Int_t i=0;i<fNx;i++) {
      fCVhist.push_back( Double_t( hcv->GetBinContent(i+1) ) );
    }
  }
  else{
    MsgWarning("CV information is missing");
  }

  fIn->Close();
}

void ErrorMatrix::Invert()
{
  TMatrixD matrix(fNx,fNy);
  for (Int_t i=0;i<fNx;i++)
    for (Int_t j=0;j<fNy;j++)
      matrix(i,j)=fFracErrorMatrix[i][j];

  matrix.Invert();

  for (Int_t i=0;i<fNx;i++)
    for (Int_t j=0;j<fNy;j++)
      fFracErrorMatrix[i][j]=matrix(i,j);
}

Double_t ErrorMatrix::GetDeterminant()
{
  TMatrixD matrix(fNx,fNy);
  for (Int_t i=0;i<fNx;i++)
    for (Int_t j=0;j<fNy;j++)
      matrix(i,j)=fFracErrorMatrix[i][j];
  
  return matrix.Determinant();
}

void ErrorMatrix::MakePosDef(bool dryrun)
{ 
  StartWatch("ErrorMatrix::MakePosDef");
  fPositiveDef = false;
  TMatrixD matrix(fNx,fNy);
  for (Int_t i=0;i<fNx;i++){
    for (Int_t j=0;j<fNy;j++){
      matrix(i,j)=fFracErrorMatrix[i][j];
    }
  }

  Double_t last_min=0.;
  Double_t scale=1.01;
  Int_t counter=0;
  while ( !fPositiveDef ) {
    fPositiveDef = true;
    
    //check if positive definite
    TMatrixDEigen v(matrix);
    TVectorD eig_re = v.GetEigenValuesRe();
    TVectorD eig_im = v.GetEigenValuesIm();
    
    Double_t min=0.      ; // minimum of eigenvalues
    Double_t minDiag = 99999.; // minimum of diagonal terms
    for (Int_t i=0;i<fNx;i++) {
      if (eig_re(i)<0) {
        fPositiveDef = false;
        if (eig_re(i)<min) min=eig_re(i);
      }
      if(matrix(i,i)<minDiag) minDiag = matrix(i,i);
    }

    if ( !fPositiveDef ) {
      if ( min == last_min || (counter%10==0 && counter!=0)) scale *= 2.;
      fAddedToDiag += min*scale;
      if(dryrun) {
        MsgWarning( MsgLog::Form("Matrix %s NOT POS DEF! Minimum eigenvalue is %.3e while minimum diag term is %.3e Nothing Added!", fName.c_str(), min, minDiag));
        return;
      }
      MsgWarning( MsgLog::Form("NOT POS DEF! Adding %.3e * %.3f = %.3e", min, scale, min*scale));
      for (Int_t i=0;i<fNx;i++) matrix(i,i) -= min*scale;
      last_min=min;
    } 
    counter++;
  }

  if(dryrun==false){
    for (Int_t i=0;i<fNx;i++){
      for (Int_t j=0;j<fNy;j++){
        fFracErrorMatrix[i][j]=matrix(i,j);
      }
    }
  }

  StopWatch("ErrorMatrix::MakePosDef");

}

void ErrorMatrix::Resize(Int_t nrows, Int_t ncol)
{
  fFracErrorMatrix.resize(nrows);
  for (Int_t i=0;i<nrows; i++) fFracErrorMatrix[i].resize(ncol);
  fNx = nrows;
  fNy = ncol;
}

void ErrorMatrix::Reset()
{
  for(unsigned int i=0;i<fFracErrorMatrix.size();i++) fill(fFracErrorMatrix[i].begin(), fFracErrorMatrix[i].end(), 0);
}

void ErrorMatrix::Clear()
{
  if (fNx == 0 || fNy ==0) return;
  for (Int_t i=0;i<fNx; i++) 
    for (Int_t j=0;j<fNy; j++) 
      fFracErrorMatrix[i][j] = 0.;
}

void ErrorMatrix::Dump()
{
  ostringstream oss;
  oss << endl;
  oss << "******************************************************************"<<endl;
  oss << "Error matrix for "<< fName<< endl;
  oss << "Number of bins:  "<< fNx  << " * " << fNy <<endl;
  if (fPositiveDef)
    oss << "Matrix was not positive definite. Added "<<fAddedToDiag<<" to diagonal"<<endl;

  for (int i=0;i<fNx;i++) {
    oss << Form("%2d: ", i+1);
    for (int j=0;j<fNy;j++) {
      oss<< setw(12) << std::setprecision(4)<<fFracErrorMatrix[i][j];
    }
    oss<<endl;
  }
  MsgInfo(oss.str());
}

void ErrorMatrix::Draw(string outputName, double zmin, double zmax)
{
  MsgInfo( "\n******************************************************************");
  MsgInfo( "Drawing Error matrix for " + fName );
  if (fPositiveDef)
    MsgWarning(MsgLog::Form("Matrix was not positive definite. Added %.4e to diagonal terms", fAddedToDiag));
  TH2D tempH = TH2D(fName.c_str(), fName.c_str(), fNx, 0.5, fNx+0.5, fNy, 0.5, fNy+0.5);

  for (int i=0;i<fNx;i++) {
    for (int j=0;j<fNy;j++) {
      tempH.SetBinContent(i+1, j+1, fFracErrorMatrix[i][j]);
    }
  }
  TCanvas c1("c1", "c1", 800, 600);
  tempH.Draw("colz");
  if( zmin!=-9999 || zmax!=-9999) tempH.GetZaxis()->SetRangeUser(zmin, zmax);
  gStyle->SetOptStat(0);
  c1.SaveAs(outputName.c_str());
}


void ErrorMatrix::DrawDiag(string outputName, double ymin, double ymax)
{
  MsgInfo( "\n******************************************************************");
  MsgInfo( "Drawing diagonal terms for Error matrix " + fName );
  if (fPositiveDef)
    MsgWarning(MsgLog::Form("Matrix was not positive definite. Added %.4e to diagonal terms", fAddedToDiag));
  if (fNx != fNy){
    MsgWarning(MsgLog::Form("Warning: fNx != fNy: %d != %d, cannot draw diagonal terms", fNx, fNy));
    return;
  }
  TH1D tempH = TH1D(fName.c_str(), fName.c_str(), fNx, 0.5, fNx+0.5);

  for (int i=0;i<fNx;i++) {
      tempH.SetBinContent(i+1, fFracErrorMatrix[i][i]);
  }
  TCanvas c1("c1", "c1", 800, 600);
  tempH.Draw("colz");
  if( ymin!=-9999 || ymax!=-9999) tempH.GetYaxis()->SetRangeUser(ymin, ymax);
  gStyle->SetOptStat(0);
  c1.SaveAs(outputName.c_str());
}
