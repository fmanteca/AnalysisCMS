#if !defined(MYLIB_CUTSDY_H)
#define MYLIB_CUTSDY_H 1

#include "TString.h"


enum {
  DY_00_noCuts,
  DY_01_Has2Leptons,  
  DY_02_BVetoLoose,  
  DY_03_ZWindow,
  DY_04_ZVeto,
  DY_05_Ptll,
  DY_06_mpMet,
  DY_07_PfMet,
  DY_08_dphillmet,
  DY_09_metopt2l,
  DY_10_ht,
  DY_11_TopControl,
  DY_12_DYControl,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "DY/00_noCuts",
  "DY/01_Has2Leptons",  
  "DY/02_BVetoLoose",  
  "DY/03_ZWindow",
  "DY/04_ZVeto" ,
  "DY/05_Ptll"   ,
  "DY/06_mpMet",
  "DY/07_PfMet",
  "DY/08_dphillmet" ,
  "DY/09_metopt2l" ,
  "DY/10_ht",
  "DY/11_TopControl" ,
  "DY/12_DYControl",  

};

#endif
