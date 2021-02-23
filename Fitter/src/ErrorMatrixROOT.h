#ifndef ERRORMATRIX_H
#define ERRORMATRIX_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include "Rtypes.h"
#include "Consts.h"

class ErrorMatrix {

 public :
  ErrorMatrix();
  ErrorMatrix(const ErrorMatrix &matrix);
  void CopyFrom(const std::shared_ptr<ErrorMatrix> matrix);
  virtual ~ErrorMatrix();

  ErrorMatrix operator+(const ErrorMatrix&) const;

  void SetName(std::string name) {fName = name;};
  std::string GetName() const    {return fName;};

  void  FillMatrix(std::string input_file);
  void  SetFracErrHistID(Int_t id) {fFracErrHistID = id;};

  Int_t GetDimX() const {return fNx;};  
  Int_t GetDimY() const {return fNy;};  
  Double_t GetElement(Int_t i, Int_t j) const {return fFracErrorMatrix[i][j];};
  void SetElement(Int_t i, Int_t j, Double_t val) {fFracErrorMatrix[i][j] = val;};

  void SetCV(std::vector<Double_t> vec) {fCVhist=vec;};
  std::vector<Double_t> GetCV() {return fCVhist;};

  void Resize(Int_t nrows, Int_t ncol); 
  void Reset();
  void Invert();
  void MakePosDef(bool dryrun=false);
  void Clear();

  // Add block of matrix1 from (iBinX1, iBinY1) and (nBinsX, nBinsY) to this matrix's (iBinX0, iBinY0)
  void AddBlock2Block(const std::shared_ptr<ErrorMatrix> matrix1, int iBinX1, int iBinY1,  
                                                                  int iBinX0, int iBinY0, int nBinsX, int nBinsY);

  Double_t GetDeterminant();

  void     SetFactorAddedToDiag(Double_t val) {fAddedToDiag = val;};
  Double_t GetFactorAddedToDiag() const {return fAddedToDiag;};

  void  SetVerbosity(Int_t val) {fVerbosity = val;};
  Int_t GetVerbosity() const    {return fVerbosity;};
  void Dump();
  void Draw(std::string outputName, double zmin = -9999, double zmax = -9999);
  void DrawDiag(std::string outputName, double ymin = -9999, double ymax = -9999);
 private:
  bool fPositiveDef;
  Double_t fAddedToDiag;

  std::string fName;
  std::string fInputFileName;
  Int_t fFracErrHistID;
  Int_t fCVhistID;
  Int_t fVerbosity;

  Int_t fNx;
  Int_t fNy;
  std::vector<std::vector<Double_t> > fFracErrorMatrix;

  std::vector<Double_t> fCVhist;

};
#endif
