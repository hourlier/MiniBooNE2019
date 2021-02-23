#include "DataClass.C"
#include "FullOscClass.C"
#include "FullOscClass_nuebar.C"
#include "MCclass.C"
#include "dirtMC.C"

int kDirtColor = kOrange+3;
const int kData    = 0;
const int kOtherMC = 1;
const int kDirt    = 2;
const int kFullOsc = 3;

const double BFst = 0.8072;
const double BFdm = 0.043;

string HistNames[10] = {"Others","dirt","#Delta #rightarrow N#gamma","#pi^{0} mis-ID","#nu_{e} from K^{0}","#nu_{e} from K^{#pm}","#nu_{e} from #mu^{#pm}","#nu_{x}-e elastic scattering","#nu_{#mu}, #bar#nu_{#mu}","#pi^{0} coherent"};
int PlotColors[10] = {kGray,kDirtColor,kYellow+1,kRed-6,kGreen-5,kGreen-6,kGreen-8,1,2,4};
double POTlist[4];

DataClass data;
FullOscClass fullOsc;
FullOscClass_nuebar fullOsc_nuebar;
MCclass OtherMC;
dirtMC DirtMC;

void JanetList(){
    gStyle->SetOptStat(0);
    gStyle->SetPalette(kTemperatureMap);

    POTlist[kData]    = 18.75e20;
    POTlist[kOtherMC] = 2*2.054e21;
    POTlist[kDirt]    = 2*2.054E21;
    POTlist[kFullOsc] = 2*6.639e19;

    fullOsc.SetSin22theta(BFst);
    fullOsc.SetDm2(BFdm);

    OtherMC.SetScaleFactor(POTlist[kData]/POTlist[kOtherMC]);
    fullOsc.SetScaleFactor(POTlist[kData]/POTlist[kFullOsc]);
    DirtMC.SetScaleFactor(POTlist[kData]/POTlist[kDirt]);

    std::cout << "scaling factor dirt = " << POTlist[kData]/POTlist[kDirt] << std::endl;
    std::cout << "scaling factor other = " << POTlist[kData]/POTlist[kOtherMC] << std::endl;

    //data.Loop2();
    OtherMC.Loop2();
    fullOsc.Loop2();
    //fullOsc_nuebar.Loop2();
    DirtMC.Loop2();

}
