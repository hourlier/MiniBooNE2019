#ifndef DataClass_h
#define DataClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "TObject.h"
#include "string"
#include "vector"

class DataClass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.
   static constexpr Int_t kMaxel = 1;
   static constexpr Int_t kMaxmu = 1;
   static constexpr Int_t kMaxpiFix = 1;
   static constexpr Int_t kMaxpiNoFix = 1;

    TH2D *hEvisCosTheta;
    TH2D *hEvisCosTheta_09;
    TH2D *hEvisPperp;
    TH2D *hEvisSinTheta;
    TH1D *hEnuqe;
    TH1D *hR;
    TH1D *hCosTheta;
    TH2D *hREvis;
    TH2D *hXEvis;
    TH2D *hYEvis;
    TH2D *hZEvis;
    TH2D *hRnormEvis;

    TH2D *hDistToMeanCerEvis;
    TH2D *hFEvis;
    TH2D *hRendEvis;
    TH2D *hUdotREvis;
    TH2D *hX2Y2Evis;

    Float_t Ebins[14] = {0,0.15, 0.2, 0.3, 0.375, 0.475, 0.55, 0.675, 0.8, 0.95, 1.1, 1.3, 1.5, 3};

    int NbinsEnuqe = 20;
    double Enuqemin = 150;
    double Enuqemax = 3000;
    double pperp;
    double me = 0.511;

    int NbinsXpperp = 20;
    double EvisMin = 100;
    double Evismax = 1300;
    int Nbinspperp = 20;
    double pperpMin = 0;
    double pperpMax = 25;

    int NbinsR = 20;
    double Rmin = 0;
    double Rmax = 600;

    int _forward = 0;

   // Declaration of leaf types
 //OneTrackChunkExt *el_;
   UInt_t          el_TObject_fUniqueID;
   UInt_t          el_TObject_fBits;
   Int_t           el_iterations;
   Int_t           el_trackType;
   Float_t         el_X;
   Float_t         el_Y;
   Float_t         el_Z;
   Float_t         el_UX;
   Float_t         el_UY;
   Float_t         el_UZ;
   Float_t         el_T;
   Float_t         el_E;
   Float_t         el_distToMeanCer;
   Float_t         el_fluxScale;
   Float_t         el_relativeSci;
   Float_t         el_F;
   Float_t         el_R;
   Float_t         el_REnd;
   Float_t         el_UdotR;
   Float_t         el_NormUdotR;
   Float_t         el_RtoWallF;
   Float_t         el_RtoWallB;
   Float_t         el_TransR;
   Float_t         el_cosTheta;
   Float_t         el_thetaZ;
   Float_t         el_EnuQE;
   Float_t         el_EnuQEOrig;
   Float_t         el_EnuQERyan;
   Float_t         el_EnuQ2;
   Float_t         el_Qpaper;
   Float_t         el_RtoWallCutLine;
   Float_t         el_XDiff;
   Float_t         el_YDiff;
   Float_t         el_TrackBeamRadius;
   Float_t         el_XODiff;
   Float_t         el_YODiff;
   Float_t         el_dtZ;
   Float_t         el_dtR;
   Float_t         el_dtEDrive;
   Float_t         el_dtCorr;
   Float_t         el_T_RWMZ;
   Float_t         el_T_RWMR;
   Float_t         el_T_RWMZ_Bunch;
   Float_t         el_T_RWMR_Bunch;
   Int_t           el_NBKZ;
   Int_t           el_NBKR;
   Bool_t          el_MCMode;
   Float_t         el_MC_dZ;
   Float_t         el_MC_dtRec;
   Float_t         el_MC_dtZ;
   Float_t         el_MC_dtDcy;
 //OneTrackChunkExt *mu_;
   UInt_t          mu_TObject_fUniqueID;
   UInt_t          mu_TObject_fBits;
   Int_t           mu_iterations;
   Int_t           mu_trackType;
   Float_t         mu_X;
   Float_t         mu_Y;
   Float_t         mu_Z;
   Float_t         mu_UX;
   Float_t         mu_UY;
   Float_t         mu_UZ;
   Float_t         mu_T;
   Float_t         mu_E;
   Float_t         mu_distToMeanCer;
   Float_t         mu_fluxScale;
   Float_t         mu_relativeSci;
   Float_t         mu_F;
   Float_t         mu_R;
   Float_t         mu_REnd;
   Float_t         mu_UdotR;
   Float_t         mu_NormUdotR;
   Float_t         mu_RtoWallF;
   Float_t         mu_RtoWallB;
   Float_t         mu_TransR;
   Float_t         mu_cosTheta;
   Float_t         mu_thetaZ;
   Float_t         mu_EnuQE;
   Float_t         mu_EnuQEOrig;
   Float_t         mu_EnuQERyan;
   Float_t         mu_EnuQ2;
   Float_t         mu_Qpaper;
   Float_t         mu_RtoWallCutLine;
   Float_t         mu_XDiff;
   Float_t         mu_YDiff;
   Float_t         mu_TrackBeamRadius;
   Float_t         mu_XODiff;
   Float_t         mu_YODiff;
   Float_t         mu_dtZ;
   Float_t         mu_dtR;
   Float_t         mu_dtEDrive;
   Float_t         mu_dtCorr;
   Float_t         mu_T_RWMZ;
   Float_t         mu_T_RWMR;
   Float_t         mu_T_RWMZ_Bunch;
   Float_t         mu_T_RWMR_Bunch;
   Int_t           mu_NBKZ;
   Int_t           mu_NBKR;
   Bool_t          mu_MCMode;
   Float_t         mu_MC_dZ;
   Float_t         mu_MC_dtRec;
   Float_t         mu_MC_dtZ;
   Float_t         mu_MC_dtDcy;
 //TwoTrackChunkExt *piFix_;
   UInt_t          piFix_TObject_fUniqueID;
   UInt_t          piFix_TObject_fBits;
   Int_t           piFix_trackType1;
   Int_t           piFix_trackType2;
   Float_t         piFix_X;
   Float_t         piFix_Y;
   Float_t         piFix_Z;
   Float_t         piFix_T;
   Float_t         piFix_M;
   Float_t         piFix_CosOp;
   Float_t         piFix_UX1;
   Float_t         piFix_UY1;
   Float_t         piFix_UZ1;
   Float_t         piFix_E1;
   Float_t         piFix_distToMeanCer1;
   Float_t         piFix_S1;
   Float_t         piFix_UX2;
   Float_t         piFix_UY2;
   Float_t         piFix_UZ2;
   Float_t         piFix_E2;
   Float_t         piFix_distToMeanCer2;
   Float_t         piFix_S2;
   Float_t         piFix_fluxScale;
   Float_t         piFix_relativeSci;
   Float_t         piFix_Fqqq;
   Float_t         piFix_Fttt;
   Float_t         piFix_F;
   Bool_t          piFix_fixedMass;
   Float_t         piFix_pi0mom;
   Float_t         piFix_R;
   Float_t         piFix_REnd1;
   Float_t         piFix_REnd2;
   Float_t         piFix_UdotR1;
   Float_t         piFix_UdotR2;
   Float_t         piFix_NormUdotR1;
   Float_t         piFix_NormUdotR2;
   Float_t         piFix_RtoWallF;
   Float_t         piFix_RtoWallB;
   Float_t         piFix_TransR;
   Float_t         piFix_cosTheta;
   Float_t         piFix_thetaZ;
   Float_t         piFix_EnuQE;
   Float_t         piFix_EnuQEOrig;
   Float_t         piFix_EnuQERyan;
   Float_t         piFix_EnuQ2;
   Float_t         piFix_Qpaper;
   Float_t         piFix_XDiff;
   Float_t         piFix_YDiff;
   Float_t         piFix_TrackBeamRadius;
   Float_t         piFix_XODiff;
   Float_t         piFix_YODiff;
   Float_t         piFix_dtZ;
   Float_t         piFix_dtR;
   Float_t         piFix_dtEDrive;
   Float_t         piFix_dtCorr;
   Float_t         piFix_T_RWMZ;
   Float_t         piFix_T_RWMR;
   Float_t         piFix_T_RWMZ_Bunch;
   Float_t         piFix_T_RWMR_Bunch;
   Int_t           piFix_NBKZ;
   Int_t           piFix_NBKR;
   Bool_t          piFix_MCMode;
   Float_t         piFix_MC_dtRec;
   Float_t         piFix_MC_dZ;
   Float_t         piFix_MC_dtZ;
 //TwoTrackChunkExt *piNoFix_;
   UInt_t          piNoFix_TObject_fUniqueID;
   UInt_t          piNoFix_TObject_fBits;
   Int_t           piNoFix_trackType1;
   Int_t           piNoFix_trackType2;
   Float_t         piNoFix_X;
   Float_t         piNoFix_Y;
   Float_t         piNoFix_Z;
   Float_t         piNoFix_T;
   Float_t         piNoFix_M;
   Float_t         piNoFix_CosOp;
   Float_t         piNoFix_UX1;
   Float_t         piNoFix_UY1;
   Float_t         piNoFix_UZ1;
   Float_t         piNoFix_E1;
   Float_t         piNoFix_distToMeanCer1;
   Float_t         piNoFix_S1;
   Float_t         piNoFix_UX2;
   Float_t         piNoFix_UY2;
   Float_t         piNoFix_UZ2;
   Float_t         piNoFix_E2;
   Float_t         piNoFix_distToMeanCer2;
   Float_t         piNoFix_S2;
   Float_t         piNoFix_fluxScale;
   Float_t         piNoFix_relativeSci;
   Float_t         piNoFix_Fqqq;
   Float_t         piNoFix_Fttt;
   Float_t         piNoFix_F;
   Bool_t          piNoFix_fixedMass;
   Float_t         piNoFix_pi0mom;
   Float_t         piNoFix_R;
   Float_t         piNoFix_REnd1;
   Float_t         piNoFix_REnd2;
   Float_t         piNoFix_UdotR1;
   Float_t         piNoFix_UdotR2;
   Float_t         piNoFix_NormUdotR1;
   Float_t         piNoFix_NormUdotR2;
   Float_t         piNoFix_RtoWallF;
   Float_t         piNoFix_RtoWallB;
   Float_t         piNoFix_TransR;
   Float_t         piNoFix_cosTheta;
   Float_t         piNoFix_thetaZ;
   Float_t         piNoFix_EnuQE;
   Float_t         piNoFix_EnuQEOrig;
   Float_t         piNoFix_EnuQERyan;
   Float_t         piNoFix_EnuQ2;
   Float_t         piNoFix_Qpaper;
   Float_t         piNoFix_XDiff;
   Float_t         piNoFix_YDiff;
   Float_t         piNoFix_TrackBeamRadius;
   Float_t         piNoFix_XODiff;
   Float_t         piNoFix_YODiff;
   Float_t         piNoFix_dtZ;
   Float_t         piNoFix_dtR;
   Float_t         piNoFix_dtEDrive;
   Float_t         piNoFix_dtCorr;
   Float_t         piNoFix_T_RWMZ;
   Float_t         piNoFix_T_RWMR;
   Float_t         piNoFix_T_RWMZ_Bunch;
   Float_t         piNoFix_T_RWMR_Bunch;
   Int_t           piNoFix_NBKZ;
   Int_t           piNoFix_NBKR;
   Bool_t          piNoFix_MCMode;
   Float_t         piNoFix_MC_dtRec;
   Float_t         piNoFix_MC_dZ;
   Float_t         piNoFix_MC_dtZ;
   Int_t           runno;
   Int_t           subrunno;
   Int_t           eventno;
   Int_t           tankhits;
   Int_t           vetohits;
   Int_t           particleMode;
   string          *particleName;
   Float_t         MW875H;
   Float_t         MW875V;
   Float_t         MW876H;
   Float_t         MW876V;
   Float_t         dHorG;
   Float_t         dVerG;
   Float_t         HorAngle;
   Float_t         VerAngle;
   Float_t         XProj;
   Float_t         YProj;
   Float_t         XBeam;
   Float_t         YBeam;
   Float_t         dtBunchOffset;
   Float_t         dtRWMRawTime;
   Float_t         tRWM1544;
   Float_t         tRWM1545;
   Float_t         tRWM1546;
   Float_t         tRWM1549;
   Float_t         tRWM1552;
   Float_t         GMST;
   Float_t         GMSTDays;
   Float_t         GMST24;
   Int_t           SECS;
   Int_t           MSECS;
   Float_t         MC_T;
   Float_t         MC_X;
   Float_t         MC_Y;
   Float_t         MC_Z;
   Float_t         dtRF;
   Float_t         dtInstr;
   Float_t         dtCorr;
   vector<float>   *vec_RWM;

   // List of branches
   TBranch        *b_el_TObject_fUniqueID;   //!
   TBranch        *b_el_TObject_fBits;   //!
   TBranch        *b_el_iterations;   //!
   TBranch        *b_el_trackType;   //!
   TBranch        *b_el_X;   //!
   TBranch        *b_el_Y;   //!
   TBranch        *b_el_Z;   //!
   TBranch        *b_el_UX;   //!
   TBranch        *b_el_UY;   //!
   TBranch        *b_el_UZ;   //!
   TBranch        *b_el_T;   //!
   TBranch        *b_el_E;   //!
   TBranch        *b_el_distToMeanCer;   //!
   TBranch        *b_el_fluxScale;   //!
   TBranch        *b_el_relativeSci;   //!
   TBranch        *b_el_F;   //!
   TBranch        *b_el_R;   //!
   TBranch        *b_el_REnd;   //!
   TBranch        *b_el_UdotR;   //!
   TBranch        *b_el_NormUdotR;   //!
   TBranch        *b_el_RtoWallF;   //!
   TBranch        *b_el_RtoWallB;   //!
   TBranch        *b_el_TransR;   //!
   TBranch        *b_el_cosTheta;   //!
   TBranch        *b_el_thetaZ;   //!
   TBranch        *b_el_EnuQE;   //!
   TBranch        *b_el_EnuQEOrig;   //!
   TBranch        *b_el_EnuQERyan;   //!
   TBranch        *b_el_EnuQ2;   //!
   TBranch        *b_el_Qpaper;   //!
   TBranch        *b_el_RtoWallCutLine;   //!
   TBranch        *b_el_XDiff;   //!
   TBranch        *b_el_YDiff;   //!
   TBranch        *b_el_TrackBeamRadius;   //!
   TBranch        *b_el_XODiff;   //!
   TBranch        *b_el_YODiff;   //!
   TBranch        *b_el_dtZ;   //!
   TBranch        *b_el_dtR;   //!
   TBranch        *b_el_dtEDrive;   //!
   TBranch        *b_el_dtCorr;   //!
   TBranch        *b_el_T_RWMZ;   //!
   TBranch        *b_el_T_RWMR;   //!
   TBranch        *b_el_T_RWMZ_Bunch;   //!
   TBranch        *b_el_T_RWMR_Bunch;   //!
   TBranch        *b_el_NBKZ;   //!
   TBranch        *b_el_NBKR;   //!
   TBranch        *b_el_MCMode;   //!
   TBranch        *b_el_MC_dZ;   //!
   TBranch        *b_el_MC_dtRec;   //!
   TBranch        *b_el_MC_dtZ;   //!
   TBranch        *b_el_MC_dtDcy;   //!
   TBranch        *b_mu_TObject_fUniqueID;   //!
   TBranch        *b_mu_TObject_fBits;   //!
   TBranch        *b_mu_iterations;   //!
   TBranch        *b_mu_trackType;   //!
   TBranch        *b_mu_X;   //!
   TBranch        *b_mu_Y;   //!
   TBranch        *b_mu_Z;   //!
   TBranch        *b_mu_UX;   //!
   TBranch        *b_mu_UY;   //!
   TBranch        *b_mu_UZ;   //!
   TBranch        *b_mu_T;   //!
   TBranch        *b_mu_E;   //!
   TBranch        *b_mu_distToMeanCer;   //!
   TBranch        *b_mu_fluxScale;   //!
   TBranch        *b_mu_relativeSci;   //!
   TBranch        *b_mu_F;   //!
   TBranch        *b_mu_R;   //!
   TBranch        *b_mu_REnd;   //!
   TBranch        *b_mu_UdotR;   //!
   TBranch        *b_mu_NormUdotR;   //!
   TBranch        *b_mu_RtoWallF;   //!
   TBranch        *b_mu_RtoWallB;   //!
   TBranch        *b_mu_TransR;   //!
   TBranch        *b_mu_cosTheta;   //!
   TBranch        *b_mu_thetaZ;   //!
   TBranch        *b_mu_EnuQE;   //!
   TBranch        *b_mu_EnuQEOrig;   //!
   TBranch        *b_mu_EnuQERyan;   //!
   TBranch        *b_mu_EnuQ2;   //!
   TBranch        *b_mu_Qpaper;   //!
   TBranch        *b_mu_RtoWallCutLine;   //!
   TBranch        *b_mu_XDiff;   //!
   TBranch        *b_mu_YDiff;   //!
   TBranch        *b_mu_TrackBeamRadius;   //!
   TBranch        *b_mu_XODiff;   //!
   TBranch        *b_mu_YODiff;   //!
   TBranch        *b_mu_dtZ;   //!
   TBranch        *b_mu_dtR;   //!
   TBranch        *b_mu_dtEDrive;   //!
   TBranch        *b_mu_dtCorr;   //!
   TBranch        *b_mu_T_RWMZ;   //!
   TBranch        *b_mu_T_RWMR;   //!
   TBranch        *b_mu_T_RWMZ_Bunch;   //!
   TBranch        *b_mu_T_RWMR_Bunch;   //!
   TBranch        *b_mu_NBKZ;   //!
   TBranch        *b_mu_NBKR;   //!
   TBranch        *b_mu_MCMode;   //!
   TBranch        *b_mu_MC_dZ;   //!
   TBranch        *b_mu_MC_dtRec;   //!
   TBranch        *b_mu_MC_dtZ;   //!
   TBranch        *b_mu_MC_dtDcy;   //!
   TBranch        *b_piFix_TObject_fUniqueID;   //!
   TBranch        *b_piFix_TObject_fBits;   //!
   TBranch        *b_piFix_trackType1;   //!
   TBranch        *b_piFix_trackType2;   //!
   TBranch        *b_piFix_X;   //!
   TBranch        *b_piFix_Y;   //!
   TBranch        *b_piFix_Z;   //!
   TBranch        *b_piFix_T;   //!
   TBranch        *b_piFix_M;   //!
   TBranch        *b_piFix_CosOp;   //!
   TBranch        *b_piFix_UX1;   //!
   TBranch        *b_piFix_UY1;   //!
   TBranch        *b_piFix_UZ1;   //!
   TBranch        *b_piFix_E1;   //!
   TBranch        *b_piFix_distToMeanCer1;   //!
   TBranch        *b_piFix_S1;   //!
   TBranch        *b_piFix_UX2;   //!
   TBranch        *b_piFix_UY2;   //!
   TBranch        *b_piFix_UZ2;   //!
   TBranch        *b_piFix_E2;   //!
   TBranch        *b_piFix_distToMeanCer2;   //!
   TBranch        *b_piFix_S2;   //!
   TBranch        *b_piFix_fluxScale;   //!
   TBranch        *b_piFix_relativeSci;   //!
   TBranch        *b_piFix_Fqqq;   //!
   TBranch        *b_piFix_Fttt;   //!
   TBranch        *b_piFix_F;   //!
   TBranch        *b_piFix_fixedMass;   //!
   TBranch        *b_piFix_pi0mom;   //!
   TBranch        *b_piFix_R;   //!
   TBranch        *b_piFix_REnd1;   //!
   TBranch        *b_piFix_REnd2;   //!
   TBranch        *b_piFix_UdotR1;   //!
   TBranch        *b_piFix_UdotR2;   //!
   TBranch        *b_piFix_NormUdotR1;   //!
   TBranch        *b_piFix_NormUdotR2;   //!
   TBranch        *b_piFix_RtoWallF;   //!
   TBranch        *b_piFix_RtoWallB;   //!
   TBranch        *b_piFix_TransR;   //!
   TBranch        *b_piFix_cosTheta;   //!
   TBranch        *b_piFix_thetaZ;   //!
   TBranch        *b_piFix_EnuQE;   //!
   TBranch        *b_piFix_EnuQEOrig;   //!
   TBranch        *b_piFix_EnuQERyan;   //!
   TBranch        *b_piFix_EnuQ2;   //!
   TBranch        *b_piFix_Qpaper;   //!
   TBranch        *b_piFix_XDiff;   //!
   TBranch        *b_piFix_YDiff;   //!
   TBranch        *b_piFix_TrackBeamRadius;   //!
   TBranch        *b_piFix_XODiff;   //!
   TBranch        *b_piFix_YODiff;   //!
   TBranch        *b_piFix_dtZ;   //!
   TBranch        *b_piFix_dtR;   //!
   TBranch        *b_piFix_dtEDrive;   //!
   TBranch        *b_piFix_dtCorr;   //!
   TBranch        *b_piFix_T_RWMZ;   //!
   TBranch        *b_piFix_T_RWMR;   //!
   TBranch        *b_piFix_T_RWMZ_Bunch;   //!
   TBranch        *b_piFix_T_RWMR_Bunch;   //!
   TBranch        *b_piFix_NBKZ;   //!
   TBranch        *b_piFix_NBKR;   //!
   TBranch        *b_piFix_MCMode;   //!
   TBranch        *b_piFix_MC_dtRec;   //!
   TBranch        *b_piFix_MC_dZ;   //!
   TBranch        *b_piFix_MC_dtZ;   //!
   TBranch        *b_piNoFix_TObject_fUniqueID;   //!
   TBranch        *b_piNoFix_TObject_fBits;   //!
   TBranch        *b_piNoFix_trackType1;   //!
   TBranch        *b_piNoFix_trackType2;   //!
   TBranch        *b_piNoFix_X;   //!
   TBranch        *b_piNoFix_Y;   //!
   TBranch        *b_piNoFix_Z;   //!
   TBranch        *b_piNoFix_T;   //!
   TBranch        *b_piNoFix_M;   //!
   TBranch        *b_piNoFix_CosOp;   //!
   TBranch        *b_piNoFix_UX1;   //!
   TBranch        *b_piNoFix_UY1;   //!
   TBranch        *b_piNoFix_UZ1;   //!
   TBranch        *b_piNoFix_E1;   //!
   TBranch        *b_piNoFix_distToMeanCer1;   //!
   TBranch        *b_piNoFix_S1;   //!
   TBranch        *b_piNoFix_UX2;   //!
   TBranch        *b_piNoFix_UY2;   //!
   TBranch        *b_piNoFix_UZ2;   //!
   TBranch        *b_piNoFix_E2;   //!
   TBranch        *b_piNoFix_distToMeanCer2;   //!
   TBranch        *b_piNoFix_S2;   //!
   TBranch        *b_piNoFix_fluxScale;   //!
   TBranch        *b_piNoFix_relativeSci;   //!
   TBranch        *b_piNoFix_Fqqq;   //!
   TBranch        *b_piNoFix_Fttt;   //!
   TBranch        *b_piNoFix_F;   //!
   TBranch        *b_piNoFix_fixedMass;   //!
   TBranch        *b_piNoFix_pi0mom;   //!
   TBranch        *b_piNoFix_R;   //!
   TBranch        *b_piNoFix_REnd1;   //!
   TBranch        *b_piNoFix_REnd2;   //!
   TBranch        *b_piNoFix_UdotR1;   //!
   TBranch        *b_piNoFix_UdotR2;   //!
   TBranch        *b_piNoFix_NormUdotR1;   //!
   TBranch        *b_piNoFix_NormUdotR2;   //!
   TBranch        *b_piNoFix_RtoWallF;   //!
   TBranch        *b_piNoFix_RtoWallB;   //!
   TBranch        *b_piNoFix_TransR;   //!
   TBranch        *b_piNoFix_cosTheta;   //!
   TBranch        *b_piNoFix_thetaZ;   //!
   TBranch        *b_piNoFix_EnuQE;   //!
   TBranch        *b_piNoFix_EnuQEOrig;   //!
   TBranch        *b_piNoFix_EnuQERyan;   //!
   TBranch        *b_piNoFix_EnuQ2;   //!
   TBranch        *b_piNoFix_Qpaper;   //!
   TBranch        *b_piNoFix_XDiff;   //!
   TBranch        *b_piNoFix_YDiff;   //!
   TBranch        *b_piNoFix_TrackBeamRadius;   //!
   TBranch        *b_piNoFix_XODiff;   //!
   TBranch        *b_piNoFix_YODiff;   //!
   TBranch        *b_piNoFix_dtZ;   //!
   TBranch        *b_piNoFix_dtR;   //!
   TBranch        *b_piNoFix_dtEDrive;   //!
   TBranch        *b_piNoFix_dtCorr;   //!
   TBranch        *b_piNoFix_T_RWMZ;   //!
   TBranch        *b_piNoFix_T_RWMR;   //!
   TBranch        *b_piNoFix_T_RWMZ_Bunch;   //!
   TBranch        *b_piNoFix_T_RWMR_Bunch;   //!
   TBranch        *b_piNoFix_NBKZ;   //!
   TBranch        *b_piNoFix_NBKR;   //!
   TBranch        *b_piNoFix_MCMode;   //!
   TBranch        *b_piNoFix_MC_dtRec;   //!
   TBranch        *b_piNoFix_MC_dZ;   //!
   TBranch        *b_piNoFix_MC_dtZ;   //!
   TBranch        *b_runno;   //!
   TBranch        *b_subrunno;   //!
   TBranch        *b_eventno;   //!
   TBranch        *b_tankhits;   //!
   TBranch        *b_vetohits;   //!
   TBranch        *b_particleMode;   //!
   TBranch        *b_particleName;   //!
   TBranch        *b_MW875H;   //!
   TBranch        *b_MW875V;   //!
   TBranch        *b_MW876H;   //!
   TBranch        *b_MW876V;   //!
   TBranch        *b_dHorG;   //!
   TBranch        *b_dVerG;   //!
   TBranch        *b_HorAngle;   //!
   TBranch        *b_VerAngle;   //!
   TBranch        *b_XProj;   //!
   TBranch        *b_YProj;   //!
   TBranch        *b_XBeam;   //!
   TBranch        *b_YBeam;   //!
   TBranch        *b_dtBunchOffset;   //!
   TBranch        *b_dtRWMRawTime;   //!
   TBranch        *b_tRWM1544;   //!
   TBranch        *b_tRWM1545;   //!
   TBranch        *b_tRWM1546;   //!
   TBranch        *b_tRWM1549;   //!
   TBranch        *b_tRWM1552;   //!
   TBranch        *b_GMST;   //!
   TBranch        *b_GMSTDays;   //!
   TBranch        *b_GMST24;   //!
   TBranch        *b_SECS;   //!
   TBranch        *b_MSECS;   //!
   TBranch        *b_MC_T;   //!
   TBranch        *b_MC_X;   //!
   TBranch        *b_MC_Y;   //!
   TBranch        *b_MC_Z;   //!
   TBranch        *b_dtRF;   //!
   TBranch        *b_dtInstr;   //!
   TBranch        *b_dtCorr;   //!
   TBranch        *b_vec_RWM;   //!

   DataClass(TTree *tree=0);
   virtual ~DataClass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
    virtual void     Loop2();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
    void ChooseOnlyForward(){_forward = 1;}
    void ChooseOnlyBackward(){_forward = -1;}
};

#endif

#ifdef DataClass_cxx
DataClass::DataClass(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("timing2.0_data_nue_07to19.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("timing2.0_data_nue_07to19.root");
      }
      f->GetObject("TTiming",tree);

   }
   Init(tree);
}

DataClass::~DataClass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t DataClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t DataClass::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void DataClass::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   particleName = 0;
   vec_RWM = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("el.TObject.fUniqueID", &el_TObject_fUniqueID, &b_el_TObject_fUniqueID);
   fChain->SetBranchAddress("el.TObject.fBits", &el_TObject_fBits, &b_el_TObject_fBits);
   fChain->SetBranchAddress("el.iterations", &el_iterations, &b_el_iterations);
   fChain->SetBranchAddress("el.trackType", &el_trackType, &b_el_trackType);
   fChain->SetBranchAddress("el.X", &el_X, &b_el_X);
   fChain->SetBranchAddress("el.Y", &el_Y, &b_el_Y);
   fChain->SetBranchAddress("el.Z", &el_Z, &b_el_Z);
   fChain->SetBranchAddress("el.UX", &el_UX, &b_el_UX);
   fChain->SetBranchAddress("el.UY", &el_UY, &b_el_UY);
   fChain->SetBranchAddress("el.UZ", &el_UZ, &b_el_UZ);
   fChain->SetBranchAddress("el.T", &el_T, &b_el_T);
   fChain->SetBranchAddress("el.E", &el_E, &b_el_E);
   fChain->SetBranchAddress("el.distToMeanCer", &el_distToMeanCer, &b_el_distToMeanCer);
   fChain->SetBranchAddress("el.fluxScale", &el_fluxScale, &b_el_fluxScale);
   fChain->SetBranchAddress("el.relativeSci", &el_relativeSci, &b_el_relativeSci);
   fChain->SetBranchAddress("el.F", &el_F, &b_el_F);
   fChain->SetBranchAddress("el.R", &el_R, &b_el_R);
   fChain->SetBranchAddress("el.REnd", &el_REnd, &b_el_REnd);
   fChain->SetBranchAddress("el.UdotR", &el_UdotR, &b_el_UdotR);
   fChain->SetBranchAddress("el.NormUdotR", &el_NormUdotR, &b_el_NormUdotR);
   fChain->SetBranchAddress("el.RtoWallF", &el_RtoWallF, &b_el_RtoWallF);
   fChain->SetBranchAddress("el.RtoWallB", &el_RtoWallB, &b_el_RtoWallB);
   fChain->SetBranchAddress("el.TransR", &el_TransR, &b_el_TransR);
   fChain->SetBranchAddress("el.cosTheta", &el_cosTheta, &b_el_cosTheta);
   fChain->SetBranchAddress("el.thetaZ", &el_thetaZ, &b_el_thetaZ);
   fChain->SetBranchAddress("el.EnuQE", &el_EnuQE, &b_el_EnuQE);
   fChain->SetBranchAddress("el.EnuQEOrig", &el_EnuQEOrig, &b_el_EnuQEOrig);
   fChain->SetBranchAddress("el.EnuQERyan", &el_EnuQERyan, &b_el_EnuQERyan);
   fChain->SetBranchAddress("el.EnuQ2", &el_EnuQ2, &b_el_EnuQ2);
   fChain->SetBranchAddress("el.Qpaper", &el_Qpaper, &b_el_Qpaper);
   fChain->SetBranchAddress("el.RtoWallCutLine", &el_RtoWallCutLine, &b_el_RtoWallCutLine);
   fChain->SetBranchAddress("el.XDiff", &el_XDiff, &b_el_XDiff);
   fChain->SetBranchAddress("el.YDiff", &el_YDiff, &b_el_YDiff);
   fChain->SetBranchAddress("el.TrackBeamRadius", &el_TrackBeamRadius, &b_el_TrackBeamRadius);
   fChain->SetBranchAddress("el.XODiff", &el_XODiff, &b_el_XODiff);
   fChain->SetBranchAddress("el.YODiff", &el_YODiff, &b_el_YODiff);
   fChain->SetBranchAddress("el.dtZ", &el_dtZ, &b_el_dtZ);
   fChain->SetBranchAddress("el.dtR", &el_dtR, &b_el_dtR);
   fChain->SetBranchAddress("el.dtEDrive", &el_dtEDrive, &b_el_dtEDrive);
   fChain->SetBranchAddress("el.dtCorr", &el_dtCorr, &b_el_dtCorr);
   fChain->SetBranchAddress("el.T_RWMZ", &el_T_RWMZ, &b_el_T_RWMZ);
   fChain->SetBranchAddress("el.T_RWMR", &el_T_RWMR, &b_el_T_RWMR);
   fChain->SetBranchAddress("el.T_RWMZ_Bunch", &el_T_RWMZ_Bunch, &b_el_T_RWMZ_Bunch);
   fChain->SetBranchAddress("el.T_RWMR_Bunch", &el_T_RWMR_Bunch, &b_el_T_RWMR_Bunch);
   fChain->SetBranchAddress("el.NBKZ", &el_NBKZ, &b_el_NBKZ);
   fChain->SetBranchAddress("el.NBKR", &el_NBKR, &b_el_NBKR);
   fChain->SetBranchAddress("el.MCMode", &el_MCMode, &b_el_MCMode);
   fChain->SetBranchAddress("el.MC_dZ", &el_MC_dZ, &b_el_MC_dZ);
   fChain->SetBranchAddress("el.MC_dtRec", &el_MC_dtRec, &b_el_MC_dtRec);
   fChain->SetBranchAddress("el.MC_dtZ", &el_MC_dtZ, &b_el_MC_dtZ);
   fChain->SetBranchAddress("el.MC_dtDcy", &el_MC_dtDcy, &b_el_MC_dtDcy);
   fChain->SetBranchAddress("mu.TObject.fUniqueID", &mu_TObject_fUniqueID, &b_mu_TObject_fUniqueID);
   fChain->SetBranchAddress("mu.TObject.fBits", &mu_TObject_fBits, &b_mu_TObject_fBits);
   fChain->SetBranchAddress("mu.iterations", &mu_iterations, &b_mu_iterations);
   fChain->SetBranchAddress("mu.trackType", &mu_trackType, &b_mu_trackType);
   fChain->SetBranchAddress("mu.X", &mu_X, &b_mu_X);
   fChain->SetBranchAddress("mu.Y", &mu_Y, &b_mu_Y);
   fChain->SetBranchAddress("mu.Z", &mu_Z, &b_mu_Z);
   fChain->SetBranchAddress("mu.UX", &mu_UX, &b_mu_UX);
   fChain->SetBranchAddress("mu.UY", &mu_UY, &b_mu_UY);
   fChain->SetBranchAddress("mu.UZ", &mu_UZ, &b_mu_UZ);
   fChain->SetBranchAddress("mu.T", &mu_T, &b_mu_T);
   fChain->SetBranchAddress("mu.E", &mu_E, &b_mu_E);
   fChain->SetBranchAddress("mu.distToMeanCer", &mu_distToMeanCer, &b_mu_distToMeanCer);
   fChain->SetBranchAddress("mu.fluxScale", &mu_fluxScale, &b_mu_fluxScale);
   fChain->SetBranchAddress("mu.relativeSci", &mu_relativeSci, &b_mu_relativeSci);
   fChain->SetBranchAddress("mu.F", &mu_F, &b_mu_F);
   fChain->SetBranchAddress("mu.R", &mu_R, &b_mu_R);
   fChain->SetBranchAddress("mu.REnd", &mu_REnd, &b_mu_REnd);
   fChain->SetBranchAddress("mu.UdotR", &mu_UdotR, &b_mu_UdotR);
   fChain->SetBranchAddress("mu.NormUdotR", &mu_NormUdotR, &b_mu_NormUdotR);
   fChain->SetBranchAddress("mu.RtoWallF", &mu_RtoWallF, &b_mu_RtoWallF);
   fChain->SetBranchAddress("mu.RtoWallB", &mu_RtoWallB, &b_mu_RtoWallB);
   fChain->SetBranchAddress("mu.TransR", &mu_TransR, &b_mu_TransR);
   fChain->SetBranchAddress("mu.cosTheta", &mu_cosTheta, &b_mu_cosTheta);
   fChain->SetBranchAddress("mu.thetaZ", &mu_thetaZ, &b_mu_thetaZ);
   fChain->SetBranchAddress("mu.EnuQE", &mu_EnuQE, &b_mu_EnuQE);
   fChain->SetBranchAddress("mu.EnuQEOrig", &mu_EnuQEOrig, &b_mu_EnuQEOrig);
   fChain->SetBranchAddress("mu.EnuQERyan", &mu_EnuQERyan, &b_mu_EnuQERyan);
   fChain->SetBranchAddress("mu.EnuQ2", &mu_EnuQ2, &b_mu_EnuQ2);
   fChain->SetBranchAddress("mu.Qpaper", &mu_Qpaper, &b_mu_Qpaper);
   fChain->SetBranchAddress("mu.RtoWallCutLine", &mu_RtoWallCutLine, &b_mu_RtoWallCutLine);
   fChain->SetBranchAddress("mu.XDiff", &mu_XDiff, &b_mu_XDiff);
   fChain->SetBranchAddress("mu.YDiff", &mu_YDiff, &b_mu_YDiff);
   fChain->SetBranchAddress("mu.TrackBeamRadius", &mu_TrackBeamRadius, &b_mu_TrackBeamRadius);
   fChain->SetBranchAddress("mu.XODiff", &mu_XODiff, &b_mu_XODiff);
   fChain->SetBranchAddress("mu.YODiff", &mu_YODiff, &b_mu_YODiff);
   fChain->SetBranchAddress("mu.dtZ", &mu_dtZ, &b_mu_dtZ);
   fChain->SetBranchAddress("mu.dtR", &mu_dtR, &b_mu_dtR);
   fChain->SetBranchAddress("mu.dtEDrive", &mu_dtEDrive, &b_mu_dtEDrive);
   fChain->SetBranchAddress("mu.dtCorr", &mu_dtCorr, &b_mu_dtCorr);
   fChain->SetBranchAddress("mu.T_RWMZ", &mu_T_RWMZ, &b_mu_T_RWMZ);
   fChain->SetBranchAddress("mu.T_RWMR", &mu_T_RWMR, &b_mu_T_RWMR);
   fChain->SetBranchAddress("mu.T_RWMZ_Bunch", &mu_T_RWMZ_Bunch, &b_mu_T_RWMZ_Bunch);
   fChain->SetBranchAddress("mu.T_RWMR_Bunch", &mu_T_RWMR_Bunch, &b_mu_T_RWMR_Bunch);
   fChain->SetBranchAddress("mu.NBKZ", &mu_NBKZ, &b_mu_NBKZ);
   fChain->SetBranchAddress("mu.NBKR", &mu_NBKR, &b_mu_NBKR);
   fChain->SetBranchAddress("mu.MCMode", &mu_MCMode, &b_mu_MCMode);
   fChain->SetBranchAddress("mu.MC_dZ", &mu_MC_dZ, &b_mu_MC_dZ);
   fChain->SetBranchAddress("mu.MC_dtRec", &mu_MC_dtRec, &b_mu_MC_dtRec);
   fChain->SetBranchAddress("mu.MC_dtZ", &mu_MC_dtZ, &b_mu_MC_dtZ);
   fChain->SetBranchAddress("mu.MC_dtDcy", &mu_MC_dtDcy, &b_mu_MC_dtDcy);
   fChain->SetBranchAddress("piFix.TObject.fUniqueID", &piFix_TObject_fUniqueID, &b_piFix_TObject_fUniqueID);
   fChain->SetBranchAddress("piFix.TObject.fBits", &piFix_TObject_fBits, &b_piFix_TObject_fBits);
   fChain->SetBranchAddress("piFix.trackType1", &piFix_trackType1, &b_piFix_trackType1);
   fChain->SetBranchAddress("piFix.trackType2", &piFix_trackType2, &b_piFix_trackType2);
   fChain->SetBranchAddress("piFix.X", &piFix_X, &b_piFix_X);
   fChain->SetBranchAddress("piFix.Y", &piFix_Y, &b_piFix_Y);
   fChain->SetBranchAddress("piFix.Z", &piFix_Z, &b_piFix_Z);
   fChain->SetBranchAddress("piFix.T", &piFix_T, &b_piFix_T);
   fChain->SetBranchAddress("piFix.M", &piFix_M, &b_piFix_M);
   fChain->SetBranchAddress("piFix.CosOp", &piFix_CosOp, &b_piFix_CosOp);
   fChain->SetBranchAddress("piFix.UX1", &piFix_UX1, &b_piFix_UX1);
   fChain->SetBranchAddress("piFix.UY1", &piFix_UY1, &b_piFix_UY1);
   fChain->SetBranchAddress("piFix.UZ1", &piFix_UZ1, &b_piFix_UZ1);
   fChain->SetBranchAddress("piFix.E1", &piFix_E1, &b_piFix_E1);
   fChain->SetBranchAddress("piFix.distToMeanCer1", &piFix_distToMeanCer1, &b_piFix_distToMeanCer1);
   fChain->SetBranchAddress("piFix.S1", &piFix_S1, &b_piFix_S1);
   fChain->SetBranchAddress("piFix.UX2", &piFix_UX2, &b_piFix_UX2);
   fChain->SetBranchAddress("piFix.UY2", &piFix_UY2, &b_piFix_UY2);
   fChain->SetBranchAddress("piFix.UZ2", &piFix_UZ2, &b_piFix_UZ2);
   fChain->SetBranchAddress("piFix.E2", &piFix_E2, &b_piFix_E2);
   fChain->SetBranchAddress("piFix.distToMeanCer2", &piFix_distToMeanCer2, &b_piFix_distToMeanCer2);
   fChain->SetBranchAddress("piFix.S2", &piFix_S2, &b_piFix_S2);
   fChain->SetBranchAddress("piFix.fluxScale", &piFix_fluxScale, &b_piFix_fluxScale);
   fChain->SetBranchAddress("piFix.relativeSci", &piFix_relativeSci, &b_piFix_relativeSci);
   fChain->SetBranchAddress("piFix.Fqqq", &piFix_Fqqq, &b_piFix_Fqqq);
   fChain->SetBranchAddress("piFix.Fttt", &piFix_Fttt, &b_piFix_Fttt);
   fChain->SetBranchAddress("piFix.F", &piFix_F, &b_piFix_F);
   fChain->SetBranchAddress("piFix.fixedMass", &piFix_fixedMass, &b_piFix_fixedMass);
   fChain->SetBranchAddress("piFix.pi0mom", &piFix_pi0mom, &b_piFix_pi0mom);
   fChain->SetBranchAddress("piFix.R", &piFix_R, &b_piFix_R);
   fChain->SetBranchAddress("piFix.REnd1", &piFix_REnd1, &b_piFix_REnd1);
   fChain->SetBranchAddress("piFix.REnd2", &piFix_REnd2, &b_piFix_REnd2);
   fChain->SetBranchAddress("piFix.UdotR1", &piFix_UdotR1, &b_piFix_UdotR1);
   fChain->SetBranchAddress("piFix.UdotR2", &piFix_UdotR2, &b_piFix_UdotR2);
   fChain->SetBranchAddress("piFix.NormUdotR1", &piFix_NormUdotR1, &b_piFix_NormUdotR1);
   fChain->SetBranchAddress("piFix.NormUdotR2", &piFix_NormUdotR2, &b_piFix_NormUdotR2);
   fChain->SetBranchAddress("piFix.RtoWallF", &piFix_RtoWallF, &b_piFix_RtoWallF);
   fChain->SetBranchAddress("piFix.RtoWallB", &piFix_RtoWallB, &b_piFix_RtoWallB);
   fChain->SetBranchAddress("piFix.TransR", &piFix_TransR, &b_piFix_TransR);
   fChain->SetBranchAddress("piFix.cosTheta", &piFix_cosTheta, &b_piFix_cosTheta);
   fChain->SetBranchAddress("piFix.thetaZ", &piFix_thetaZ, &b_piFix_thetaZ);
   fChain->SetBranchAddress("piFix.EnuQE", &piFix_EnuQE, &b_piFix_EnuQE);
   fChain->SetBranchAddress("piFix.EnuQEOrig", &piFix_EnuQEOrig, &b_piFix_EnuQEOrig);
   fChain->SetBranchAddress("piFix.EnuQERyan", &piFix_EnuQERyan, &b_piFix_EnuQERyan);
   fChain->SetBranchAddress("piFix.EnuQ2", &piFix_EnuQ2, &b_piFix_EnuQ2);
   fChain->SetBranchAddress("piFix.Qpaper", &piFix_Qpaper, &b_piFix_Qpaper);
   fChain->SetBranchAddress("piFix.XDiff", &piFix_XDiff, &b_piFix_XDiff);
   fChain->SetBranchAddress("piFix.YDiff", &piFix_YDiff, &b_piFix_YDiff);
   fChain->SetBranchAddress("piFix.TrackBeamRadius", &piFix_TrackBeamRadius, &b_piFix_TrackBeamRadius);
   fChain->SetBranchAddress("piFix.XODiff", &piFix_XODiff, &b_piFix_XODiff);
   fChain->SetBranchAddress("piFix.YODiff", &piFix_YODiff, &b_piFix_YODiff);
   fChain->SetBranchAddress("piFix.dtZ", &piFix_dtZ, &b_piFix_dtZ);
   fChain->SetBranchAddress("piFix.dtR", &piFix_dtR, &b_piFix_dtR);
   fChain->SetBranchAddress("piFix.dtEDrive", &piFix_dtEDrive, &b_piFix_dtEDrive);
   fChain->SetBranchAddress("piFix.dtCorr", &piFix_dtCorr, &b_piFix_dtCorr);
   fChain->SetBranchAddress("piFix.T_RWMZ", &piFix_T_RWMZ, &b_piFix_T_RWMZ);
   fChain->SetBranchAddress("piFix.T_RWMR", &piFix_T_RWMR, &b_piFix_T_RWMR);
   fChain->SetBranchAddress("piFix.T_RWMZ_Bunch", &piFix_T_RWMZ_Bunch, &b_piFix_T_RWMZ_Bunch);
   fChain->SetBranchAddress("piFix.T_RWMR_Bunch", &piFix_T_RWMR_Bunch, &b_piFix_T_RWMR_Bunch);
   fChain->SetBranchAddress("piFix.NBKZ", &piFix_NBKZ, &b_piFix_NBKZ);
   fChain->SetBranchAddress("piFix.NBKR", &piFix_NBKR, &b_piFix_NBKR);
   fChain->SetBranchAddress("piFix.MCMode", &piFix_MCMode, &b_piFix_MCMode);
   fChain->SetBranchAddress("piFix.MC_dtRec", &piFix_MC_dtRec, &b_piFix_MC_dtRec);
   fChain->SetBranchAddress("piFix.MC_dZ", &piFix_MC_dZ, &b_piFix_MC_dZ);
   fChain->SetBranchAddress("piFix.MC_dtZ", &piFix_MC_dtZ, &b_piFix_MC_dtZ);
   fChain->SetBranchAddress("piNoFix.TObject.fUniqueID", &piNoFix_TObject_fUniqueID, &b_piNoFix_TObject_fUniqueID);
   fChain->SetBranchAddress("piNoFix.TObject.fBits", &piNoFix_TObject_fBits, &b_piNoFix_TObject_fBits);
   fChain->SetBranchAddress("piNoFix.trackType1", &piNoFix_trackType1, &b_piNoFix_trackType1);
   fChain->SetBranchAddress("piNoFix.trackType2", &piNoFix_trackType2, &b_piNoFix_trackType2);
   fChain->SetBranchAddress("piNoFix.X", &piNoFix_X, &b_piNoFix_X);
   fChain->SetBranchAddress("piNoFix.Y", &piNoFix_Y, &b_piNoFix_Y);
   fChain->SetBranchAddress("piNoFix.Z", &piNoFix_Z, &b_piNoFix_Z);
   fChain->SetBranchAddress("piNoFix.T", &piNoFix_T, &b_piNoFix_T);
   fChain->SetBranchAddress("piNoFix.M", &piNoFix_M, &b_piNoFix_M);
   fChain->SetBranchAddress("piNoFix.CosOp", &piNoFix_CosOp, &b_piNoFix_CosOp);
   fChain->SetBranchAddress("piNoFix.UX1", &piNoFix_UX1, &b_piNoFix_UX1);
   fChain->SetBranchAddress("piNoFix.UY1", &piNoFix_UY1, &b_piNoFix_UY1);
   fChain->SetBranchAddress("piNoFix.UZ1", &piNoFix_UZ1, &b_piNoFix_UZ1);
   fChain->SetBranchAddress("piNoFix.E1", &piNoFix_E1, &b_piNoFix_E1);
   fChain->SetBranchAddress("piNoFix.distToMeanCer1", &piNoFix_distToMeanCer1, &b_piNoFix_distToMeanCer1);
   fChain->SetBranchAddress("piNoFix.S1", &piNoFix_S1, &b_piNoFix_S1);
   fChain->SetBranchAddress("piNoFix.UX2", &piNoFix_UX2, &b_piNoFix_UX2);
   fChain->SetBranchAddress("piNoFix.UY2", &piNoFix_UY2, &b_piNoFix_UY2);
   fChain->SetBranchAddress("piNoFix.UZ2", &piNoFix_UZ2, &b_piNoFix_UZ2);
   fChain->SetBranchAddress("piNoFix.E2", &piNoFix_E2, &b_piNoFix_E2);
   fChain->SetBranchAddress("piNoFix.distToMeanCer2", &piNoFix_distToMeanCer2, &b_piNoFix_distToMeanCer2);
   fChain->SetBranchAddress("piNoFix.S2", &piNoFix_S2, &b_piNoFix_S2);
   fChain->SetBranchAddress("piNoFix.fluxScale", &piNoFix_fluxScale, &b_piNoFix_fluxScale);
   fChain->SetBranchAddress("piNoFix.relativeSci", &piNoFix_relativeSci, &b_piNoFix_relativeSci);
   fChain->SetBranchAddress("piNoFix.Fqqq", &piNoFix_Fqqq, &b_piNoFix_Fqqq);
   fChain->SetBranchAddress("piNoFix.Fttt", &piNoFix_Fttt, &b_piNoFix_Fttt);
   fChain->SetBranchAddress("piNoFix.F", &piNoFix_F, &b_piNoFix_F);
   fChain->SetBranchAddress("piNoFix.fixedMass", &piNoFix_fixedMass, &b_piNoFix_fixedMass);
   fChain->SetBranchAddress("piNoFix.pi0mom", &piNoFix_pi0mom, &b_piNoFix_pi0mom);
   fChain->SetBranchAddress("piNoFix.R", &piNoFix_R, &b_piNoFix_R);
   fChain->SetBranchAddress("piNoFix.REnd1", &piNoFix_REnd1, &b_piNoFix_REnd1);
   fChain->SetBranchAddress("piNoFix.REnd2", &piNoFix_REnd2, &b_piNoFix_REnd2);
   fChain->SetBranchAddress("piNoFix.UdotR1", &piNoFix_UdotR1, &b_piNoFix_UdotR1);
   fChain->SetBranchAddress("piNoFix.UdotR2", &piNoFix_UdotR2, &b_piNoFix_UdotR2);
   fChain->SetBranchAddress("piNoFix.NormUdotR1", &piNoFix_NormUdotR1, &b_piNoFix_NormUdotR1);
   fChain->SetBranchAddress("piNoFix.NormUdotR2", &piNoFix_NormUdotR2, &b_piNoFix_NormUdotR2);
   fChain->SetBranchAddress("piNoFix.RtoWallF", &piNoFix_RtoWallF, &b_piNoFix_RtoWallF);
   fChain->SetBranchAddress("piNoFix.RtoWallB", &piNoFix_RtoWallB, &b_piNoFix_RtoWallB);
   fChain->SetBranchAddress("piNoFix.TransR", &piNoFix_TransR, &b_piNoFix_TransR);
   fChain->SetBranchAddress("piNoFix.cosTheta", &piNoFix_cosTheta, &b_piNoFix_cosTheta);
   fChain->SetBranchAddress("piNoFix.thetaZ", &piNoFix_thetaZ, &b_piNoFix_thetaZ);
   fChain->SetBranchAddress("piNoFix.EnuQE", &piNoFix_EnuQE, &b_piNoFix_EnuQE);
   fChain->SetBranchAddress("piNoFix.EnuQEOrig", &piNoFix_EnuQEOrig, &b_piNoFix_EnuQEOrig);
   fChain->SetBranchAddress("piNoFix.EnuQERyan", &piNoFix_EnuQERyan, &b_piNoFix_EnuQERyan);
   fChain->SetBranchAddress("piNoFix.EnuQ2", &piNoFix_EnuQ2, &b_piNoFix_EnuQ2);
   fChain->SetBranchAddress("piNoFix.Qpaper", &piNoFix_Qpaper, &b_piNoFix_Qpaper);
   fChain->SetBranchAddress("piNoFix.XDiff", &piNoFix_XDiff, &b_piNoFix_XDiff);
   fChain->SetBranchAddress("piNoFix.YDiff", &piNoFix_YDiff, &b_piNoFix_YDiff);
   fChain->SetBranchAddress("piNoFix.TrackBeamRadius", &piNoFix_TrackBeamRadius, &b_piNoFix_TrackBeamRadius);
   fChain->SetBranchAddress("piNoFix.XODiff", &piNoFix_XODiff, &b_piNoFix_XODiff);
   fChain->SetBranchAddress("piNoFix.YODiff", &piNoFix_YODiff, &b_piNoFix_YODiff);
   fChain->SetBranchAddress("piNoFix.dtZ", &piNoFix_dtZ, &b_piNoFix_dtZ);
   fChain->SetBranchAddress("piNoFix.dtR", &piNoFix_dtR, &b_piNoFix_dtR);
   fChain->SetBranchAddress("piNoFix.dtEDrive", &piNoFix_dtEDrive, &b_piNoFix_dtEDrive);
   fChain->SetBranchAddress("piNoFix.dtCorr", &piNoFix_dtCorr, &b_piNoFix_dtCorr);
   fChain->SetBranchAddress("piNoFix.T_RWMZ", &piNoFix_T_RWMZ, &b_piNoFix_T_RWMZ);
   fChain->SetBranchAddress("piNoFix.T_RWMR", &piNoFix_T_RWMR, &b_piNoFix_T_RWMR);
   fChain->SetBranchAddress("piNoFix.T_RWMZ_Bunch", &piNoFix_T_RWMZ_Bunch, &b_piNoFix_T_RWMZ_Bunch);
   fChain->SetBranchAddress("piNoFix.T_RWMR_Bunch", &piNoFix_T_RWMR_Bunch, &b_piNoFix_T_RWMR_Bunch);
   fChain->SetBranchAddress("piNoFix.NBKZ", &piNoFix_NBKZ, &b_piNoFix_NBKZ);
   fChain->SetBranchAddress("piNoFix.NBKR", &piNoFix_NBKR, &b_piNoFix_NBKR);
   fChain->SetBranchAddress("piNoFix.MCMode", &piNoFix_MCMode, &b_piNoFix_MCMode);
   fChain->SetBranchAddress("piNoFix.MC_dtRec", &piNoFix_MC_dtRec, &b_piNoFix_MC_dtRec);
   fChain->SetBranchAddress("piNoFix.MC_dZ", &piNoFix_MC_dZ, &b_piNoFix_MC_dZ);
   fChain->SetBranchAddress("piNoFix.MC_dtZ", &piNoFix_MC_dtZ, &b_piNoFix_MC_dtZ);
   fChain->SetBranchAddress("runno", &runno, &b_runno);
   fChain->SetBranchAddress("subrunno", &subrunno, &b_subrunno);
   fChain->SetBranchAddress("eventno", &eventno, &b_eventno);
   fChain->SetBranchAddress("tankhits", &tankhits, &b_tankhits);
   fChain->SetBranchAddress("vetohits", &vetohits, &b_vetohits);
   fChain->SetBranchAddress("particleMode", &particleMode, &b_particleMode);
   fChain->SetBranchAddress("particleName", &particleName, &b_particleName);
   fChain->SetBranchAddress("MW875H", &MW875H, &b_MW875H);
   fChain->SetBranchAddress("MW875V", &MW875V, &b_MW875V);
   fChain->SetBranchAddress("MW876H", &MW876H, &b_MW876H);
   fChain->SetBranchAddress("MW876V", &MW876V, &b_MW876V);
   fChain->SetBranchAddress("dHorG", &dHorG, &b_dHorG);
   fChain->SetBranchAddress("dVerG", &dVerG, &b_dVerG);
   fChain->SetBranchAddress("HorAngle", &HorAngle, &b_HorAngle);
   fChain->SetBranchAddress("VerAngle", &VerAngle, &b_VerAngle);
   fChain->SetBranchAddress("XProj", &XProj, &b_XProj);
   fChain->SetBranchAddress("YProj", &YProj, &b_YProj);
   fChain->SetBranchAddress("XBeam", &XBeam, &b_XBeam);
   fChain->SetBranchAddress("YBeam", &YBeam, &b_YBeam);
   fChain->SetBranchAddress("dtBunchOffset", &dtBunchOffset, &b_dtBunchOffset);
   fChain->SetBranchAddress("dtRWMRawTime", &dtRWMRawTime, &b_dtRWMRawTime);
   fChain->SetBranchAddress("tRWM1544", &tRWM1544, &b_tRWM1544);
   fChain->SetBranchAddress("tRWM1545", &tRWM1545, &b_tRWM1545);
   fChain->SetBranchAddress("tRWM1546", &tRWM1546, &b_tRWM1546);
   fChain->SetBranchAddress("tRWM1549", &tRWM1549, &b_tRWM1549);
   fChain->SetBranchAddress("tRWM1552", &tRWM1552, &b_tRWM1552);
   fChain->SetBranchAddress("GMST", &GMST, &b_GMST);
   fChain->SetBranchAddress("GMSTDays", &GMSTDays, &b_GMSTDays);
   fChain->SetBranchAddress("GMST24", &GMST24, &b_GMST24);
   fChain->SetBranchAddress("SECS", &SECS, &b_SECS);
   fChain->SetBranchAddress("MSECS", &MSECS, &b_MSECS);
   fChain->SetBranchAddress("MC_T", &MC_T, &b_MC_T);
   fChain->SetBranchAddress("MC_X", &MC_X, &b_MC_X);
   fChain->SetBranchAddress("MC_Y", &MC_Y, &b_MC_Y);
   fChain->SetBranchAddress("MC_Z", &MC_Z, &b_MC_Z);
   fChain->SetBranchAddress("dtRF", &dtRF, &b_dtRF);
   fChain->SetBranchAddress("dtInstr", &dtInstr, &b_dtInstr);
   fChain->SetBranchAddress("dtCorr", &dtCorr, &b_dtCorr);
   fChain->SetBranchAddress("vec_RWM", &vec_RWM, &b_vec_RWM);
   Notify();
}

Bool_t DataClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void DataClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t DataClass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef DataClass_cxx
