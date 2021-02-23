#include "ErrorMatrix.h"
#include "Event.h"
#include "Consts.h"

#include <iostream>
#include <iomanip>

#include <stdlib.h>
#include "cfortran.h"
#include <packlib.h>
#include <math.h>

#include "TVectorD.h"
#include "TMatrixD.h"
#include "TMatrixDEigen.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"

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

ErrorMatrix::~ErrorMatrix()
{
}

ErrorMatrix ErrorMatrix::operator+(const ErrorMatrix& m) const
{
  if ( m.GetDimX() != this->GetDimX() &&
       m.GetDimY() != this->GetDimY() ) {
    cout << "Can't add matrices with different number of elements!" <<endl;
    cout <<this->GetName()<<" and "<<m.GetName()<<endl;
    return *this;
  }
  
  //  ErrorMatrix* mat=new ErrorMatrix();
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

void ErrorMatrix::FillMatrix(std::string input_file)
{
  fInputFileName = input_file;

  if (fInputFileName.find("root") < fInputFileName.length() ) {
    cout <<"ErrorMatrix::Loading matrix from root file!"<<endl;
    TFile f(fInputFileName.c_str());
    TH2D* matrix=dynamic_cast<TH2D*> (f.Get(fName.c_str()));
    fNx=matrix->GetNbinsX();
    fNy=matrix->GetNbinsY();
    fFracErrorMatrix.resize(fNx);
    for (int i=0;i<fNx;i++) fFracErrorMatrix[i].resize(fNy);
  
    for (int i=0;i<fNx;i++) {
      for (int j=0;j<fNy;j++) {
        //	if ( (i<fNx/2 && j<fNy/2) || (i>=fNx/2 && j>=fNy/2) )
        fFracErrorMatrix[i][j] = matrix->GetBinContent(i+1,j+1);
        //	else
        // fFracErrorMatrix[i][j] = 0;
      }
    }
    TH1D* hcv=dynamic_cast<TH1D*> (f.Get("bigcv"));
    if ( hcv ) {
      for (Int_t i=0;i<fNx;i++) {
        fCVhist.push_back( Double_t( hcv->GetBinContent(i+1) ) );
      }
      
    } else {
      cout<<"CV information is missing"<<endl;
    }
  } else {
    std::string fIndir  = "INPUT";
    int lrec=1024;
    int istat=0;
    
    char* infile = const_cast<char*> (fInputFileName.c_str());
    HROPEN(20, strdup(fIndir.c_str()), infile, "P", lrec, istat);
    if (istat != 0) {
      cout << "ErrorMatrix::ERROR: Error opening input file ... ("<<fInputFileName<<")" << endl;
      exit(1);
    }

    HRIN(fFracErrHistID, 999999, 0);
    int ierr=0;
    Int_t nevents;

    HCDIR(strdup(("//"+fIndir).c_str()), " ");
    char chtitle[80];
    float fXmin, fXmax, fYmin, fYmax;
    Int_t nwt, loc;
    HGIVE(fFracErrHistID, chtitle, fNx, fXmin, fXmax, fNy, fYmin, fYmax, nwt, loc);  

    fFracErrorMatrix.resize(fNx);
    for (int i=0;i<fNx;i++) fFracErrorMatrix[i].resize(fNy);
    
    for (int i=0;i<fNx;i++) {
      for (int j=0;j<fNy;j++) {
        fFracErrorMatrix[i][j] = Double_t( HIJ(fFracErrHistID, i+1, j+1) );
      }
    }

    //try to get CV from histogram 61
    HRIN(fCVhistID, 999999, 0);
    if (HEXIST (fCVhistID)) {
      for (Int_t i=0;i<fNx;i++) {
        fCVhist.push_back( Double_t( HI(fCVhistID,i+1) ) );
      }
    } else {
      cout<<"CV information is missing"<<endl;
    }

    HREND(strdup((""+fIndir).c_str()));
    char* dummy="";
    HSCR(fFracErrHistID,0, dummy);
    HSCR(fCVhistID,0, dummy);
  }
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

void ErrorMatrix::MakePosDef()
{ 
  fPositiveDef = false;
  TMatrixD matrix(fNx,fNy);
  for (Int_t i=0;i<fNx;i++)
    for (Int_t j=0;j<fNy;j++)
      matrix(i,j)=fFracErrorMatrix[i][j];

  Double_t last_min=0.;
  Double_t scale=1.01;
  Int_t counter=0;
  while ( !fPositiveDef ) {
    fPositiveDef = true;
    
    //check if positive definite
    TMatrixDEigen v(matrix);
    TVectorD eig_re = v.GetEigenValuesRe();
    TVectorD eig_im = v.GetEigenValuesIm();
    
    Double_t min=0.;
    for (Int_t i=0;i<fNx;i++) {
      //cout <<eig_re(i)<<endl;
      if (eig_re(i)<0) {
        fPositiveDef = false;
        if (eig_re(i)<min) min=eig_re(i);
      }
    }

    if ( !fPositiveDef ) {
      if ( min == last_min || (counter%10==0 && counter!=0)) scale *= 2.;
      fAddedToDiag += min*scale;
      cout <<"NOT POS DEF! Adding "<<min<<"*"<<scale<<"="<<min*scale<<endl;
      for (Int_t i=0;i<fNx;i++) matrix(i,i) -= min*scale;
      last_min=min;
    } 
    counter++;
  }
  for (Int_t i=0;i<fNx;i++)
    for (Int_t j=0;j<fNy;j++)
      fFracErrorMatrix[i][j]=matrix(i,j);
}

void ErrorMatrix::Resize(Int_t nrows, Int_t ncol)
{
  fFracErrorMatrix.resize(nrows);
  for (Int_t i=0;i<nrows; i++) fFracErrorMatrix[i].resize(ncol);
  fNx = nrows;
  fNy = ncol;
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
  cout << endl;
  cout << "******************************************************************"<<endl;
  cout << "Error matrix for "<< fName<< endl;
  cout << "Number of bins:  "<< fNx  << " * " << fNy <<endl;
  cout << "Verbosity:       "<< fVerbosity<<endl;
  if (fPositiveDef)
    cout << "Matrix was not positive definite. Added "<<fAddedToDiag<<" to diagonal"<<endl;

  if (fVerbosity < 4) return;

  cout << " Printing matrix "<<endl;
  for (int i=0;i<fNx;i++) {
    cout <<i<<": ";
    for (int j=0;j<fNy;j++) {
      cout.width(14);
      cout<<std::setprecision(8)<<fFracErrorMatrix[i][j];
    }
    cout<<endl;
    }
}


void ErrorMatrix::Draw(string outputName, double zmin, double zmax)
{
  cout << endl;
  cout << "******************************************************************"<<endl;
  cout << "Drawing Error matrix for "<< fName<< endl;
  if (fPositiveDef)
    cout << "Matrix was not positive definite. Added "<<fAddedToDiag<<" to diagonal"<<endl;
  TH2D tempH = TH2D(fName.c_str(), fName.c_str(), fNx, 0.5, fNx+0.5, fNy, 0.5, fNy+0.5);

  for (int i=0;i<fNx;i++) {
    for (int j=0;j<fNy;j++) {
      tempH.SetBinContent(i+1, j+1, fFracErrorMatrix[i][j]);
    }
  }
  TCanvas c1("c1", "c1", 800, 600);
  tempH.Draw("colz");
  if( zmin!=-9999 || zmax!=-9999) tempH.GetZaxis()->SetRangeUser(zmin, zmax);
  c1.SaveAs(outputName.c_str());
}


void ErrorMatrix::DrawDiag(string outputName, double ymin, double ymax)
{
  cout << endl;
  cout << "******************************************************************"<<endl;
  cout << "Drawing Error matrix for "<< fName<< endl;
  if (fPositiveDef)
    cout << "Matrix was not positive definite. Added "<<fAddedToDiag<<" to diagonal"<<endl;
  if (fNx != fNy){
    cout << "Warning: fNx != fNy: " << fNx << "!=" << fNy <<", cannot draw diagonal!";
    return;
  }
  TH1D tempH = TH1D(fName.c_str(), fName.c_str(), fNx, 0.5, fNx+0.5);

  for (int i=0;i<fNx;i++) {
      tempH.SetBinContent(i+1, fFracErrorMatrix[i][i]);
  }
  TCanvas c1("c1", "c1", 800, 600);
  tempH.Draw("colz");
  if( ymin!=-9999 || ymax!=-9999) tempH.GetYaxis()->SetRangeUser(ymin, ymax);
  c1.SaveAs(outputName.c_str());
}
