#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <map>
#include <list>
#include <string>
#include "Rtypes.h"

class FitMaster;
class Sample;
class ErrorMatrix;
class TXMLNode;
class TXMLAttr;

namespace Util {

  void CorrectMSstat(ErrorMatrix *matrix, 
		     std::vector<Int_t> bin_mask,
		     std::vector<Double_t> cv, 
		     std::vector<Double_t> ms_cv, 
		     Double_t ms_to_data_pot);

  void AddMonteCarloStatError(FitMaster* fit_master);

  Double_t* GetHistogramAxis(Int_t grid_points, Double_t *grid_vector);
  ErrorMatrix* GetRebinnedMatrix(const ErrorMatrix& mat, std::vector<Double_t> event_vec, std::vector<Int_t> rebin_vec);
 
  std::vector<std::vector<Double_t> > GetFakeData(UInt_t random_seed, UInt_t n, std::vector<Double_t> cv, const ErrorMatrix& mat);
  std::vector<Double_t>               GetOneFakeData(UInt_t random_seed, std::vector<Double_t> cv, const ErrorMatrix& mat);

  std::map<std::string, std::string> GetAttributeMap(TXMLNode*);
  std::list<TXMLNode* >              GetSubNodeList(TXMLNode*);
  std::vector<Int_t>                 MakeVectorI(std::string, char* delim= (char*) ",");
  std::vector<Double_t>              MakeVectorD(std::string, char* delim= (char*) ",");
  std::vector<std::string>           MakeVectorS(std::string, char* delim= (char*) ",");
  bool                               atob(const std::string s);
};
#endif
