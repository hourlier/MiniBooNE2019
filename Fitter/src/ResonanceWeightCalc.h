#ifndef ResonanceWeightCalc_H
#define ResonanceWeightCalc_H
#include <memory>

#include "WeightCalc.h"


class ResonanceWeightCalc : public WeightCalc {

 public :
  ResonanceWeightCalc();
  ~ResonanceWeightCalc();

  Double_t     GetWeight(Event &event);
  Double_t     GetWeight(Double_t L, Double_t E, int inno);
  bool         ParametersChanged();
  void         SetNeutrinoResonance(    bool neutrino_resonance     = true);  // if (anti)neutrino is set to not have resonance, then 
  void         SetAntineutrinoResonance(bool antineutrino_resonance = true);  // normal 2v oscillation is applied
  bool         GetNeutrinoResonance(){return m_neutrino_resonance;};
  bool         GetAntineutrinoResonance(){return m_antineutrino_resonance;};

  std::shared_ptr<WeightCalc>  Clone();   

 private:
  bool m_neutrino_resonance;
  bool m_antineutrino_resonance;
  bool checkedResonanceType;
};

#endif
