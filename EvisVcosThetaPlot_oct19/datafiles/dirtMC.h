#ifndef dirtMC_h
#define dirtMC_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "TObject.h"
#include "string"
#include "vector"

class dirtMC {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

    TH2D *hEvisCosTheta;
    TH2D *hEvisCosTheta_09;
    TH2D *hEvisSinTheta;
    TH2D *hEvisPperp;
    TH1D *hEnuqe;
    TH1D *hCosTheta;
    TH1D *hR;

    TH2D *hREvis;
    TH2D *hXEvis;
    TH2D *hYEvis;
    TH2D *hZEvis;

    TH2D *hDistToMeanCerEvis;
    TH2D *hFEvis;
    TH2D *hRendEvis;
    TH2D *hUdotREvis;
    TH2D *hX2Y2Evis;
    TH2D *hPperpEvis_Coherent;
    TH2D *hRnormEvis;

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

    double ScaleFactor = 1;
    int _forward = 0;

// Fixed size dimensions of array or collections stored in the TTree if any.
   static constexpr Int_t kMaxel = 1;
   static constexpr Int_t kMaxmu = 1;
   static constexpr Int_t kMaxpiFix = 1;
   static constexpr Int_t kMaxpiNoFix = 1;
   static constexpr Int_t kMaxMCEVNT = 1;
   static constexpr Int_t kMaxMCBEAM = 1;

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
   Float_t         MC_T;
   Float_t         MC_X;
   Float_t         MC_Y;
   Float_t         MC_Z;
   Float_t         dtRF;
   Float_t         dtInstr;
   Float_t         dtCorr;
   vector<float>   *vec_RWM;
   Int_t           ibkgd;
   Int_t           nuchan;
   Int_t           inno;
   Float_t         enugen;
   Float_t         energy;
   Float_t         nuleng;
   Int_t           parid;
   Float_t         wgt;
   Int_t           ispi0;
   Int_t           isdirt;
   Int_t           npi0;
   Int_t           backgroundType;
 //MonteCarloEVNTChunkExt *MCEVNT_;
   UInt_t          MCEVNT_TObject_fUniqueID;
   UInt_t          MCEVNT_TObject_fBits;
   Int_t           MCEVNT_VERSION;
   Int_t           MCEVNT_VAR_FLAGS;
   Int_t           MCEVNT_IEVNT;
   Int_t           MCEVNT_INNO;
   Int_t           MCEVNT_IBKGD;
   Int_t           MCEVNT_NDCY;
   Int_t           MCEVNT_RNDM1;
   Int_t           MCEVNT_RNDM2;
   Int_t           MCEVNT_NFSP;
   vector<int>     MCEVNT_IPFS;
   vector<int>     MCEVNT_NHTTCER;
   vector<int>     MCEVNT_NHTTSCI;
   Int_t           MCEVNT_NPROCTAG;
   vector<int>     MCEVNT_IPROCTAG;
   Int_t           MCEVNT_NHTT;
   vector<int>     MCEVNT_NOTPMT;
   Float_t         MCEVNT_EVWT;
   Float_t         MCEVNT_PNEUX;
   Float_t         MCEVNT_PNEUY;
   Float_t         MCEVNT_PNEUZ;
   Float_t         MCEVNT_PNEUE;
   vector<float>   MCEVNT_VRTX;
   vector<float>   MCEVNT_VRTY;
   vector<float>   MCEVNT_VRTZ;
   vector<float>   MCEVNT_TIME;
   vector<float>   MCEVNT_PFSPX;
   vector<float>   MCEVNT_PFSPY;
   vector<float>   MCEVNT_PFSPZ;
   vector<float>   MCEVNT_PFSPP;
   vector<float>   MCEVNT_TITPMT;
   Float_t         MCEVNT_R;
   Float_t         MCEVNT_X;
   Float_t         MCEVNT_Y;
   Float_t         MCEVNT_Z;
   Float_t         MCEVNT_E;
   Float_t         MCEVNT_wt;
 //MonteCarloBEAMChunkExt *MCBEAM_;
   UInt_t          MCBEAM_TObject_fUniqueID;
   UInt_t          MCBEAM_TObject_fBits;
   Int_t           MCBEAM_VERSION;
   Int_t           MCBEAM_VAR_FLAGS;
   Int_t           MCBEAM_ntp;
   Int_t           MCBEAM_npart;
   vector<int>     MCBEAM_id;
   Float_t         MCBEAM_beamwgt;
   vector<float>   MCBEAM_ini_eng;
   vector<float>   MCBEAM_ini_t;
   vector<float>   MCBEAM_ini_posX;
   vector<float>   MCBEAM_ini_posY;
   vector<float>   MCBEAM_ini_posZ;
   vector<float>   MCBEAM_ini_momX;
   vector<float>   MCBEAM_ini_momY;
   vector<float>   MCBEAM_ini_momZ;
   vector<float>   MCBEAM_fin_momX;
   vector<float>   MCBEAM_fin_momY;
   vector<float>   MCBEAM_fin_momZ;
   vector<float>   MCBEAM_fin_polX;
   vector<float>   MCBEAM_fin_polY;
   vector<float>   MCBEAM_fin_polZ;
   vector<float>   MCBEAM_dtDcy;
   vector<float>   MCBEAM_distDiff;
   Float_t         MCBEAM_dtDcySum;
   Float_t         MCBEAM_ppitot;
   Float_t         MCBEAM_ppiangle;
   Float_t         MCBEAM_pktot;
   Float_t         MCBEAM_pkangle;
   Float_t         MCBEAM_dt_PiNu;
   Float_t         MCBEAM_dt_KaonNu;
   Int_t           MCBEAM_piflag;
   Int_t           MCBEAM_muflag;
   Int_t           MCBEAM_kaonflag;
   Float_t         MCBEAM_pi_E;
   Float_t         MCBEAM_pi_t;
   Float_t         MCBEAM_pi_x;
   Float_t         MCBEAM_pi_y;
   Float_t         MCBEAM_pi_z;
   Float_t         MCBEAM_nu_E;
   Float_t         MCBEAM_nu_t;
   Float_t         MCBEAM_nu_x;
   Float_t         MCBEAM_nu_y;
   Float_t         MCBEAM_nu_z;
   Float_t         MCBEAM_kaon_E;
   Float_t         MCBEAM_kaon_t;
   Float_t         MCBEAM_kaon_x;
   Float_t         MCBEAM_kaon_y;
   Float_t         MCBEAM_kaon_z;
   Float_t         MCBEAM_dist_PiNu;
   Float_t         MCBEAM_dist_KaonNu;

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
   TBranch        *b_MC_T;   //!
   TBranch        *b_MC_X;   //!
   TBranch        *b_MC_Y;   //!
   TBranch        *b_MC_Z;   //!
   TBranch        *b_dtRF;   //!
   TBranch        *b_dtInstr;   //!
   TBranch        *b_dtCorr;   //!
   TBranch        *b_vec_RWM;   //!
   TBranch        *b_ibkgd;   //!
   TBranch        *b_nuchan;   //!
   TBranch        *b_inno;   //!
   TBranch        *b_enugen;   //!
   TBranch        *b_energy;   //!
   TBranch        *b_nuleng;   //!
   TBranch        *b_parid;   //!
   TBranch        *b_wgt;   //!
   TBranch        *b_ispi0;   //!
   TBranch        *b_isdirt;   //!
   TBranch        *b_npi0;   //!
   TBranch        *b_backgroundType;   //!
   TBranch        *b_MCEVNT_TObject_fUniqueID;   //!
   TBranch        *b_MCEVNT_TObject_fBits;   //!
   TBranch        *b_MCEVNT_VERSION;   //!
   TBranch        *b_MCEVNT_VAR_FLAGS;   //!
   TBranch        *b_MCEVNT_IEVNT;   //!
   TBranch        *b_MCEVNT_INNO;   //!
   TBranch        *b_MCEVNT_IBKGD;   //!
   TBranch        *b_MCEVNT_NDCY;   //!
   TBranch        *b_MCEVNT_RNDM1;   //!
   TBranch        *b_MCEVNT_RNDM2;   //!
   TBranch        *b_MCEVNT_NFSP;   //!
   TBranch        *b_MCEVNT_IPFS;   //!
   TBranch        *b_MCEVNT_NHTTCER;   //!
   TBranch        *b_MCEVNT_NHTTSCI;   //!
   TBranch        *b_MCEVNT_NPROCTAG;   //!
   TBranch        *b_MCEVNT_IPROCTAG;   //!
   TBranch        *b_MCEVNT_NHTT;   //!
   TBranch        *b_MCEVNT_NOTPMT;   //!
   TBranch        *b_MCEVNT_EVWT;   //!
   TBranch        *b_MCEVNT_PNEUX;   //!
   TBranch        *b_MCEVNT_PNEUY;   //!
   TBranch        *b_MCEVNT_PNEUZ;   //!
   TBranch        *b_MCEVNT_PNEUE;   //!
   TBranch        *b_MCEVNT_VRTX;   //!
   TBranch        *b_MCEVNT_VRTY;   //!
   TBranch        *b_MCEVNT_VRTZ;   //!
   TBranch        *b_MCEVNT_TIME;   //!
   TBranch        *b_MCEVNT_PFSPX;   //!
   TBranch        *b_MCEVNT_PFSPY;   //!
   TBranch        *b_MCEVNT_PFSPZ;   //!
   TBranch        *b_MCEVNT_PFSPP;   //!
   TBranch        *b_MCEVNT_TITPMT;   //!
   TBranch        *b_MCEVNT_R;   //!
   TBranch        *b_MCEVNT_X;   //!
   TBranch        *b_MCEVNT_Y;   //!
   TBranch        *b_MCEVNT_Z;   //!
   TBranch        *b_MCEVNT_E;   //!
   TBranch        *b_MCEVNT_wt;   //!
   TBranch        *b_MCBEAM_TObject_fUniqueID;   //!
   TBranch        *b_MCBEAM_TObject_fBits;   //!
   TBranch        *b_MCBEAM_VERSION;   //!
   TBranch        *b_MCBEAM_VAR_FLAGS;   //!
   TBranch        *b_MCBEAM_ntp;   //!
   TBranch        *b_MCBEAM_npart;   //!
   TBranch        *b_MCBEAM_id;   //!
   TBranch        *b_MCBEAM_beamwgt;   //!
   TBranch        *b_MCBEAM_ini_eng;   //!
   TBranch        *b_MCBEAM_ini_t;   //!
   TBranch        *b_MCBEAM_ini_posX;   //!
   TBranch        *b_MCBEAM_ini_posY;   //!
   TBranch        *b_MCBEAM_ini_posZ;   //!
   TBranch        *b_MCBEAM_ini_momX;   //!
   TBranch        *b_MCBEAM_ini_momY;   //!
   TBranch        *b_MCBEAM_ini_momZ;   //!
   TBranch        *b_MCBEAM_fin_momX;   //!
   TBranch        *b_MCBEAM_fin_momY;   //!
   TBranch        *b_MCBEAM_fin_momZ;   //!
   TBranch        *b_MCBEAM_fin_polX;   //!
   TBranch        *b_MCBEAM_fin_polY;   //!
   TBranch        *b_MCBEAM_fin_polZ;   //!
   TBranch        *b_MCBEAM_dtDcy;   //!
   TBranch        *b_MCBEAM_distDiff;   //!
   TBranch        *b_MCBEAM_dtDcySum;   //!
   TBranch        *b_MCBEAM_ppitot;   //!
   TBranch        *b_MCBEAM_ppiangle;   //!
   TBranch        *b_MCBEAM_pktot;   //!
   TBranch        *b_MCBEAM_pkangle;   //!
   TBranch        *b_MCBEAM_dt_PiNu;   //!
   TBranch        *b_MCBEAM_dt_KaonNu;   //!
   TBranch        *b_MCBEAM_piflag;   //!
   TBranch        *b_MCBEAM_muflag;   //!
   TBranch        *b_MCBEAM_kaonflag;   //!
   TBranch        *b_MCBEAM_pi_E;   //!
   TBranch        *b_MCBEAM_pi_t;   //!
   TBranch        *b_MCBEAM_pi_x;   //!
   TBranch        *b_MCBEAM_pi_y;   //!
   TBranch        *b_MCBEAM_pi_z;   //!
   TBranch        *b_MCBEAM_nu_E;   //!
   TBranch        *b_MCBEAM_nu_t;   //!
   TBranch        *b_MCBEAM_nu_x;   //!
   TBranch        *b_MCBEAM_nu_y;   //!
   TBranch        *b_MCBEAM_nu_z;   //!
   TBranch        *b_MCBEAM_kaon_E;   //!
   TBranch        *b_MCBEAM_kaon_t;   //!
   TBranch        *b_MCBEAM_kaon_x;   //!
   TBranch        *b_MCBEAM_kaon_y;   //!
   TBranch        *b_MCBEAM_kaon_z;   //!
   TBranch        *b_MCBEAM_dist_PiNu;   //!
   TBranch        *b_MCBEAM_dist_KaonNu;   //!

   dirtMC(TTree *tree=0);
   virtual ~dirtMC();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
    virtual void     Loop2();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
    void SetScaleFactor(double scale){ScaleFactor = scale;}
    void ChooseOnlyForward(){_forward = 1;}
    void ChooseOnlyBackward(){_forward = -1;}
    
};

#endif

#ifdef dirtMC_cxx
dirtMC::dirtMC(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("timing_MC_dirt_may07_2.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("timing_MC_dirt_may07_2.root");
      }
      f->GetObject("TTiming",tree);

   }
   Init(tree);
}

dirtMC::~dirtMC()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t dirtMC::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t dirtMC::LoadTree(Long64_t entry)
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

void dirtMC::Init(TTree *tree)
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
   fChain->SetBranchAddress("MC_T", &MC_T, &b_MC_T);
   fChain->SetBranchAddress("MC_X", &MC_X, &b_MC_X);
   fChain->SetBranchAddress("MC_Y", &MC_Y, &b_MC_Y);
   fChain->SetBranchAddress("MC_Z", &MC_Z, &b_MC_Z);
   fChain->SetBranchAddress("dtRF", &dtRF, &b_dtRF);
   fChain->SetBranchAddress("dtInstr", &dtInstr, &b_dtInstr);
   fChain->SetBranchAddress("dtCorr", &dtCorr, &b_dtCorr);
   fChain->SetBranchAddress("vec_RWM", &vec_RWM, &b_vec_RWM);
   fChain->SetBranchAddress("ibkgd", &ibkgd, &b_ibkgd);
   fChain->SetBranchAddress("nuchan", &nuchan, &b_nuchan);
   fChain->SetBranchAddress("inno", &inno, &b_inno);
   fChain->SetBranchAddress("enugen", &enugen, &b_enugen);
   fChain->SetBranchAddress("energy", &energy, &b_energy);
   fChain->SetBranchAddress("nuleng", &nuleng, &b_nuleng);
   fChain->SetBranchAddress("parid", &parid, &b_parid);
   fChain->SetBranchAddress("wgt", &wgt, &b_wgt);
   fChain->SetBranchAddress("ispi0", &ispi0, &b_ispi0);
   fChain->SetBranchAddress("isdirt", &isdirt, &b_isdirt);
   fChain->SetBranchAddress("npi0", &npi0, &b_npi0);
   fChain->SetBranchAddress("backgroundType", &backgroundType, &b_backgroundType);
   fChain->SetBranchAddress("MCEVNT.TObject.fUniqueID", &MCEVNT_TObject_fUniqueID, &b_MCEVNT_TObject_fUniqueID);
   fChain->SetBranchAddress("MCEVNT.TObject.fBits", &MCEVNT_TObject_fBits, &b_MCEVNT_TObject_fBits);
   fChain->SetBranchAddress("MCEVNT.VERSION", &MCEVNT_VERSION, &b_MCEVNT_VERSION);
   fChain->SetBranchAddress("MCEVNT.VAR_FLAGS", &MCEVNT_VAR_FLAGS, &b_MCEVNT_VAR_FLAGS);
   fChain->SetBranchAddress("MCEVNT.IEVNT", &MCEVNT_IEVNT, &b_MCEVNT_IEVNT);
   fChain->SetBranchAddress("MCEVNT.INNO", &MCEVNT_INNO, &b_MCEVNT_INNO);
   fChain->SetBranchAddress("MCEVNT.IBKGD", &MCEVNT_IBKGD, &b_MCEVNT_IBKGD);
   fChain->SetBranchAddress("MCEVNT.NDCY", &MCEVNT_NDCY, &b_MCEVNT_NDCY);
   fChain->SetBranchAddress("MCEVNT.RNDM1", &MCEVNT_RNDM1, &b_MCEVNT_RNDM1);
   fChain->SetBranchAddress("MCEVNT.RNDM2", &MCEVNT_RNDM2, &b_MCEVNT_RNDM2);
   fChain->SetBranchAddress("MCEVNT.NFSP", &MCEVNT_NFSP, &b_MCEVNT_NFSP);
   fChain->SetBranchAddress("MCEVNT.IPFS", &MCEVNT_IPFS, &b_MCEVNT_IPFS);
   fChain->SetBranchAddress("MCEVNT.NHTTCER", &MCEVNT_NHTTCER, &b_MCEVNT_NHTTCER);
   fChain->SetBranchAddress("MCEVNT.NHTTSCI", &MCEVNT_NHTTSCI, &b_MCEVNT_NHTTSCI);
   fChain->SetBranchAddress("MCEVNT.NPROCTAG", &MCEVNT_NPROCTAG, &b_MCEVNT_NPROCTAG);
   fChain->SetBranchAddress("MCEVNT.IPROCTAG", &MCEVNT_IPROCTAG, &b_MCEVNT_IPROCTAG);
   fChain->SetBranchAddress("MCEVNT.NHTT", &MCEVNT_NHTT, &b_MCEVNT_NHTT);
   fChain->SetBranchAddress("MCEVNT.NOTPMT", &MCEVNT_NOTPMT, &b_MCEVNT_NOTPMT);
   fChain->SetBranchAddress("MCEVNT.EVWT", &MCEVNT_EVWT, &b_MCEVNT_EVWT);
   fChain->SetBranchAddress("MCEVNT.PNEUX", &MCEVNT_PNEUX, &b_MCEVNT_PNEUX);
   fChain->SetBranchAddress("MCEVNT.PNEUY", &MCEVNT_PNEUY, &b_MCEVNT_PNEUY);
   fChain->SetBranchAddress("MCEVNT.PNEUZ", &MCEVNT_PNEUZ, &b_MCEVNT_PNEUZ);
   fChain->SetBranchAddress("MCEVNT.PNEUE", &MCEVNT_PNEUE, &b_MCEVNT_PNEUE);
   fChain->SetBranchAddress("MCEVNT.VRTX", &MCEVNT_VRTX, &b_MCEVNT_VRTX);
   fChain->SetBranchAddress("MCEVNT.VRTY", &MCEVNT_VRTY, &b_MCEVNT_VRTY);
   fChain->SetBranchAddress("MCEVNT.VRTZ", &MCEVNT_VRTZ, &b_MCEVNT_VRTZ);
   fChain->SetBranchAddress("MCEVNT.TIME", &MCEVNT_TIME, &b_MCEVNT_TIME);
   fChain->SetBranchAddress("MCEVNT.PFSPX", &MCEVNT_PFSPX, &b_MCEVNT_PFSPX);
   fChain->SetBranchAddress("MCEVNT.PFSPY", &MCEVNT_PFSPY, &b_MCEVNT_PFSPY);
   fChain->SetBranchAddress("MCEVNT.PFSPZ", &MCEVNT_PFSPZ, &b_MCEVNT_PFSPZ);
   fChain->SetBranchAddress("MCEVNT.PFSPP", &MCEVNT_PFSPP, &b_MCEVNT_PFSPP);
   fChain->SetBranchAddress("MCEVNT.TITPMT", &MCEVNT_TITPMT, &b_MCEVNT_TITPMT);
   fChain->SetBranchAddress("MCEVNT.R", &MCEVNT_R, &b_MCEVNT_R);
   fChain->SetBranchAddress("MCEVNT.X", &MCEVNT_X, &b_MCEVNT_X);
   fChain->SetBranchAddress("MCEVNT.Y", &MCEVNT_Y, &b_MCEVNT_Y);
   fChain->SetBranchAddress("MCEVNT.Z", &MCEVNT_Z, &b_MCEVNT_Z);
   fChain->SetBranchAddress("MCEVNT.E", &MCEVNT_E, &b_MCEVNT_E);
   fChain->SetBranchAddress("MCEVNT.wt", &MCEVNT_wt, &b_MCEVNT_wt);
   fChain->SetBranchAddress("MCBEAM.TObject.fUniqueID", &MCBEAM_TObject_fUniqueID, &b_MCBEAM_TObject_fUniqueID);
   fChain->SetBranchAddress("MCBEAM.TObject.fBits", &MCBEAM_TObject_fBits, &b_MCBEAM_TObject_fBits);
   fChain->SetBranchAddress("MCBEAM.VERSION", &MCBEAM_VERSION, &b_MCBEAM_VERSION);
   fChain->SetBranchAddress("MCBEAM.VAR_FLAGS", &MCBEAM_VAR_FLAGS, &b_MCBEAM_VAR_FLAGS);
   fChain->SetBranchAddress("MCBEAM.ntp", &MCBEAM_ntp, &b_MCBEAM_ntp);
   fChain->SetBranchAddress("MCBEAM.npart", &MCBEAM_npart, &b_MCBEAM_npart);
   fChain->SetBranchAddress("MCBEAM.id", &MCBEAM_id, &b_MCBEAM_id);
   fChain->SetBranchAddress("MCBEAM.beamwgt", &MCBEAM_beamwgt, &b_MCBEAM_beamwgt);
   fChain->SetBranchAddress("MCBEAM.ini_eng", &MCBEAM_ini_eng, &b_MCBEAM_ini_eng);
   fChain->SetBranchAddress("MCBEAM.ini_t", &MCBEAM_ini_t, &b_MCBEAM_ini_t);
   fChain->SetBranchAddress("MCBEAM.ini_posX", &MCBEAM_ini_posX, &b_MCBEAM_ini_posX);
   fChain->SetBranchAddress("MCBEAM.ini_posY", &MCBEAM_ini_posY, &b_MCBEAM_ini_posY);
   fChain->SetBranchAddress("MCBEAM.ini_posZ", &MCBEAM_ini_posZ, &b_MCBEAM_ini_posZ);
   fChain->SetBranchAddress("MCBEAM.ini_momX", &MCBEAM_ini_momX, &b_MCBEAM_ini_momX);
   fChain->SetBranchAddress("MCBEAM.ini_momY", &MCBEAM_ini_momY, &b_MCBEAM_ini_momY);
   fChain->SetBranchAddress("MCBEAM.ini_momZ", &MCBEAM_ini_momZ, &b_MCBEAM_ini_momZ);
   fChain->SetBranchAddress("MCBEAM.fin_momX", &MCBEAM_fin_momX, &b_MCBEAM_fin_momX);
   fChain->SetBranchAddress("MCBEAM.fin_momY", &MCBEAM_fin_momY, &b_MCBEAM_fin_momY);
   fChain->SetBranchAddress("MCBEAM.fin_momZ", &MCBEAM_fin_momZ, &b_MCBEAM_fin_momZ);
   fChain->SetBranchAddress("MCBEAM.fin_polX", &MCBEAM_fin_polX, &b_MCBEAM_fin_polX);
   fChain->SetBranchAddress("MCBEAM.fin_polY", &MCBEAM_fin_polY, &b_MCBEAM_fin_polY);
   fChain->SetBranchAddress("MCBEAM.fin_polZ", &MCBEAM_fin_polZ, &b_MCBEAM_fin_polZ);
   fChain->SetBranchAddress("MCBEAM.dtDcy", &MCBEAM_dtDcy, &b_MCBEAM_dtDcy);
   fChain->SetBranchAddress("MCBEAM.distDiff", &MCBEAM_distDiff, &b_MCBEAM_distDiff);
   fChain->SetBranchAddress("MCBEAM.dtDcySum", &MCBEAM_dtDcySum, &b_MCBEAM_dtDcySum);
   fChain->SetBranchAddress("MCBEAM.ppitot", &MCBEAM_ppitot, &b_MCBEAM_ppitot);
   fChain->SetBranchAddress("MCBEAM.ppiangle", &MCBEAM_ppiangle, &b_MCBEAM_ppiangle);
   fChain->SetBranchAddress("MCBEAM.pktot", &MCBEAM_pktot, &b_MCBEAM_pktot);
   fChain->SetBranchAddress("MCBEAM.pkangle", &MCBEAM_pkangle, &b_MCBEAM_pkangle);
   fChain->SetBranchAddress("MCBEAM.dt_PiNu", &MCBEAM_dt_PiNu, &b_MCBEAM_dt_PiNu);
   fChain->SetBranchAddress("MCBEAM.dt_KaonNu", &MCBEAM_dt_KaonNu, &b_MCBEAM_dt_KaonNu);
   fChain->SetBranchAddress("MCBEAM.piflag", &MCBEAM_piflag, &b_MCBEAM_piflag);
   fChain->SetBranchAddress("MCBEAM.muflag", &MCBEAM_muflag, &b_MCBEAM_muflag);
   fChain->SetBranchAddress("MCBEAM.kaonflag", &MCBEAM_kaonflag, &b_MCBEAM_kaonflag);
   fChain->SetBranchAddress("MCBEAM.pi_E", &MCBEAM_pi_E, &b_MCBEAM_pi_E);
   fChain->SetBranchAddress("MCBEAM.pi_t", &MCBEAM_pi_t, &b_MCBEAM_pi_t);
   fChain->SetBranchAddress("MCBEAM.pi_x", &MCBEAM_pi_x, &b_MCBEAM_pi_x);
   fChain->SetBranchAddress("MCBEAM.pi_y", &MCBEAM_pi_y, &b_MCBEAM_pi_y);
   fChain->SetBranchAddress("MCBEAM.pi_z", &MCBEAM_pi_z, &b_MCBEAM_pi_z);
   fChain->SetBranchAddress("MCBEAM.nu_E", &MCBEAM_nu_E, &b_MCBEAM_nu_E);
   fChain->SetBranchAddress("MCBEAM.nu_t", &MCBEAM_nu_t, &b_MCBEAM_nu_t);
   fChain->SetBranchAddress("MCBEAM.nu_x", &MCBEAM_nu_x, &b_MCBEAM_nu_x);
   fChain->SetBranchAddress("MCBEAM.nu_y", &MCBEAM_nu_y, &b_MCBEAM_nu_y);
   fChain->SetBranchAddress("MCBEAM.nu_z", &MCBEAM_nu_z, &b_MCBEAM_nu_z);
   fChain->SetBranchAddress("MCBEAM.kaon_E", &MCBEAM_kaon_E, &b_MCBEAM_kaon_E);
   fChain->SetBranchAddress("MCBEAM.kaon_t", &MCBEAM_kaon_t, &b_MCBEAM_kaon_t);
   fChain->SetBranchAddress("MCBEAM.kaon_x", &MCBEAM_kaon_x, &b_MCBEAM_kaon_x);
   fChain->SetBranchAddress("MCBEAM.kaon_y", &MCBEAM_kaon_y, &b_MCBEAM_kaon_y);
   fChain->SetBranchAddress("MCBEAM.kaon_z", &MCBEAM_kaon_z, &b_MCBEAM_kaon_z);
   fChain->SetBranchAddress("MCBEAM.dist_PiNu", &MCBEAM_dist_PiNu, &b_MCBEAM_dist_PiNu);
   fChain->SetBranchAddress("MCBEAM.dist_KaonNu", &MCBEAM_dist_KaonNu, &b_MCBEAM_dist_KaonNu);
   Notify();
}

Bool_t dirtMC::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void dirtMC::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t dirtMC::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef dirtMC_cxx
