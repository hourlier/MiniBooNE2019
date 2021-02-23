#include "Consts.h"
#include <map>

namespace Consts {

  ParticleCode ParticleIDtoEnum(Int_t pid)
  {
    Consts::ParticleCode pid_enum;
    switch (pid) 
    {
      case 8:
      pid_enum=Consts::kPiPlus;
      break;
      case 9:
      pid_enum=Consts::kPiMinus;
      break;
      case 10:
      pid_enum=Consts::kK0L;
      break;
      case 11:
      pid_enum=Consts::kKPlus; 
      break;
      case 12:
      pid_enum=Consts::kKMinus;
      break;
      case 5:
      pid_enum=Consts::kMuPlus;
      break;
      case 6:
      pid_enum=Consts::kMuMinus;
      break;
      default:
      pid_enum=Consts::kUnknown;
      break;
    }
    return pid_enum;
  }

  std::map<Consts::ParticleCode, Int_t > GetEnumToPIDMap()
  {
    std::map<Consts::ParticleCode, Int_t > enumtopid;
    enumtopid[Consts::kPiPlus]=8;
    enumtopid[Consts::kPiMinus]=9;
    enumtopid[Consts::kK0L]=10;
    enumtopid[Consts::kKPlus]=11;
    enumtopid[Consts::kKMinus]=12;
    enumtopid[Consts::kMuPlus]=5;
    enumtopid[Consts::kMuMinus]=6;
    enumtopid[Consts::kUnknown]=13;
    return enumtopid;
  }

  static std::map<Consts::ParticleCode, Int_t > etopid=GetEnumToPIDMap();

  Int_t EnumToParticleID(Consts::ParticleCode pid_enum)
  {
    return etopid[pid_enum];
  }
  /*
  Int_t EnumToParticleID(Consts::ParticleCode pid_enum)
  {
    Int_t pid;
    switch (pid_enum)
      {
      case Consts::kPiPlus:
	pid=8;
	break;
      case Consts::kPiMinus:
	pid=9;
	break;
      case Consts::kK0L:
	pid=10;
	break;
      case Consts::kKPlus:
	pid=11; 
	break;
      case Consts::kKMinus:
	pid=12;
	break;
      case Consts::kMuPlus:
	pid=5;
	break;
      case Consts::kMuMinus:
	pid=6;
	break;
      case Consts::kUnknown:
	pid=13;
	break;
      default:
	pid=0;
	break;
      }
    return pid;
  }
  */

  std::string GetParticleName(Consts::ParticleCode pid_enum)
  {
    std::string pname;
    
    switch (pid_enum) 
    {
      case Consts::kPiPlus:
      pname="pi+";
      break;
      case Consts::kPiMinus:
      pname="pi-";
      break;
      case Consts::kKPlus:
      pname="k+";
      break;
      case Consts::kKMinus:
      pname="k-";
      break;
      case Consts::kK0L:
      pname="k0l";
      break;
      case Consts::kMuPlus:
      pname="mu+";
      break;
      case Consts::kMuMinus:
      pname="mu-";
      break;
      
      default:
      pname="unknown";
      break;
    }
    return pname;
  }

  std::string GetBackgroundName(Consts::Background::Type pid_enum)
  {
    std::string pname;
    
    switch (pid_enum) 
    {
      case Consts::Background::kPiPlus:
      pname="pi+";
      break;
      case Consts::Background::kPiMinus:
      pname="pi-";
      break;
      case Consts::Background::kKPlus:
      pname="k+";
      break;
      case Consts::Background::kKMinus:
      pname="k-";
      break;
      case Consts::Background::kK0L:
      pname="k0l";
      break;
      case Consts::Background::kMuPlus:
      pname="mu+";
      break;
      case Consts::Background::kMuMinus:
      pname="mu-";
      break;
      case Consts::Background::kIsDirt:
      pname="Dirt";
      break;
      case Consts::Background::kIsPi0:
      pname="Pi0";
      break;
      case Consts::Background::kIsDelta:
      pname="Delta";
      break;
      
      default:
      pname="unknown";
      break;
    }
    return pname;
  }
  
  std::string GetParticleName(Int_t pid){
    ParticleCode pcode=ParticleIDtoEnum(pid);
    return GetParticleName(pcode);
  }

  Int_t ParticleIter(Consts::ParticleCode pid_enum)
  {
    Int_t pid;
    switch (pid_enum)
    {
      case Consts::kPiPlus:
      pid=1;
      break;
      case Consts::kPiMinus:
      pid=2;
      break;
      case Consts::kK0L:
      pid=3;
      break;
      case Consts::kKPlus:
      pid=4; 
      break;
      case Consts::kKMinus:
      pid=5;
      break;
      case Consts::kMuPlus:
      pid=6;
      break;
      case Consts::kMuMinus:
      pid=7;
      break;
      case Consts::kUnknown:
      pid=8;
      break;
      default:
      pid=8;
      break;
    }
    return pid;
  }

  ParticleCode ParticleIterToEnum(Int_t piter)
  {
    Consts::ParticleCode pid_enum;
    switch (piter) 
    {
      case 1:
      pid_enum=Consts::kPiPlus;
      break;
      case 2:
      pid_enum=Consts::kPiMinus;
      break;
      case 3:
      pid_enum=Consts::kK0L;
      break;
      case 4:
      pid_enum=Consts::kKPlus; 
      break;
      case 5:
      pid_enum=Consts::kKMinus;
      break;
      case 6:
      pid_enum=Consts::kMuPlus;
      break;
      case 7:
      pid_enum=Consts::kMuMinus;
      break;
      case 8:
      pid_enum=Consts::kUnknown;
      break;
      default:
      pid_enum=Consts::kUnknown;
      break;
    }
    return pid_enum;
  }

  Int_t BackgroundIter(Background::Type bkg_enum)
  {
    Int_t pid;
    switch (bkg_enum)
    {
      case Consts::Background::kPiPlus:
      pid=1;
      break;
      case Consts::Background::kPiMinus:
      pid=2;
      break;
      case Consts::Background::kK0L:
      pid=3;
      break;
      case Consts::Background::kKPlus:
      pid=4; 
      break;
      case Consts::Background::kKMinus:
      pid=5;
      break;
      case Consts::Background::kMuPlus:
      pid=6;
      break;
      case Consts::Background::kMuMinus:
      pid=7;
      break;
      case Consts::Background::kIsDirt:
      pid=8;
      break;
      case Consts::Background::kIsPi0:
      pid=9;
      break;
      case Consts::Background::kIsDelta:
      pid=10;
      break;
      case Consts::Background::kUnknown:
      pid=11;
      break;
      default:
      pid=11;
      break;
    }
    return pid;
  }

  const static Background::Type bkg_enum[11]={
    Consts::Background::kPiPlus,
    Consts::Background::kPiMinus,
    Consts::Background::kK0L,
    Consts::Background::kKPlus,
    Consts::Background::kKMinus,
    Consts::Background::kMuPlus,
    Consts::Background::kMuMinus,
    Consts::Background::kIsDirt,
    Consts::Background::kIsPi0,
    Consts::Background::kIsDelta,
    Consts::Background::kUnknown};
    Background::Type BackgroundIterToEnum(Int_t biter)
    {
      return bkg_enum[biter-1];
    }
  /*
  Background::Type BackgroundIterToEnum(Int_t biter)
  {
    Background::Type bkg_enum;
    switch (biter) 
      {
      case 1:
	bkg_enum=Consts::Background::kPiPlus;
	break;
      case 2:
	bkg_enum=Consts::Background::kPiMinus;
	break;
      case 3:
	bkg_enum=Consts::Background::kK0L;
	break;
      case 4:
	bkg_enum=Consts::Background::kKPlus; 
	break;
      case 5:
	bkg_enum=Consts::Background::kKMinus;
	break;
      case 6:
	bkg_enum=Consts::Background::kMuPlus;
	break;
      case 7:
	bkg_enum=Consts::Background::kMuMinus;
	break;
      case 8:
	bkg_enum=Consts::Background::kIsDirt;
	break;
      case 9:
	bkg_enum=Consts::Background::kIsPi0;
	break;
      case 10:
	bkg_enum=Consts::Background::kIsDelta;
	break;
      case 11:
	bkg_enum=Consts::Background::kUnknown;
	break;
      default:
	bkg_enum=Consts::Background::kUnknown;
	break;
      }
    return bkg_enum;
  }
  */
}
