#ifndef CONSTS_H
#define CONSTS_H

#include <string>
#include "Rtypes.h"

namespace Consts
{

  enum {
    kOverflow   = -1,
    kUnderflow  = -2
  };

  enum ParticleCode {
    kFirst   = 1,
    kPiPlus  = 1,
    kPiMinus = 2,
    kKPlus   = 3,
    kKMinus  = 4,
    kK0L     = 5,
    kMuPlus  = 6,
    kMuMinus = 7,
    kUnknown = 8,
    kLast    = 8
  };
  
  namespace Background {
    enum Type {
      kFirst   = 1,
      kPiPlus  = 1,
      kPiMinus = 2,
      kKPlus   = 3,
      kKMinus  = 4,
      kK0L     = 5,
      kMuPlus  = 6,
      kMuMinus = 7,
      kIsDirt  = 8,
      kIsPi0   = 9,
      kIsDelta = 10,
      kUnknown = 11,
      kLast    = 11
    };
  };

  enum EUpdateType { kFullUpdate, kScaleUpdate, kNoUpdate };

  extern ParticleCode ParticleIDtoEnum(Int_t pid);
  extern Int_t EnumToParticleID(ParticleCode pid_enum);
  extern std::string GetParticleName(ParticleCode pid_enum);
  extern std::string GetParticleName(Int_t pid);
  extern Int_t ParticleIter(ParticleCode pid_enum);
  extern Int_t BackgroundIter(Background::Type bkg_enum);
  extern ParticleCode ParticleIterToEnum(Int_t piter);
  extern Background::Type BackgroundIterToEnum(Int_t biter);
  extern std::string GetBackgroundName(Background::Type pid_enum);

  const static Double_t kChi2ConvergeCrit=0.2;
};
#endif
