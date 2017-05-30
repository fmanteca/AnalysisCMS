#if !defined(MYLIB_CUTSDY_H)
#define MYLIB_CUTSDY_H 1

#include "TString.h"


enum {
  /* DY_00_noCuts, */
  /* DY_01_Has2Leptons, */
  /* DY_02_BVetoLoose, */
  /* DY_03_ZWindow, */
  /* DY_04_ZVeto, */
  /* DY_05_Ptll, */
  /* DY_06_mpMet, */
  /* DY_07_PfMet, */
   /* DY_25_20, */
   /* DY_third_lepton_veto, */
   /* DY_mll, */
   /* DY_met, */
   /* DY_zveto, */
   /* DY_mpmet, */
   /* DY_ptll, */
   /* DY_bveto, */
   /* DY_0jet, */
   /* DY_1jet, */
  /* DY_08_dphillmet, */
  /* DY_09_metopt2l, */
  /* DY_10_ht, */
  /* DY_11_TopControl, */
  /* DY_12_DYControl, */
  /* DY_13_IFCA_Control, */
  /* DY_14_latino, */

  DY_mll1,
  DY_mll2,
  DY_mll3,
  DY_mll4,
  DY_mll5,
  DY_mll6,
  DY_ptll1,
  DY_ptll2,
  DY_ptll3,
  DY_ptll4,
  DY_ptll5,
  DY_ptll6,
  DY_dphill1,
  DY_dphill2,
  DY_dphill3,
  DY_dphill4,
  DY_dphill5,
  DY_dphill6,

  ncut  // This line should be always last
};

const TString scut[ncut] = {
  /* "DY/00_noCuts", */
  /* "DY/01_Has2Leptons", */
  /* "DY/02_BVetoLoose", */
  /* "DY/03_ZWindow", */
  /* "DY/04_ZVeto" , */
  /* "DY/05_Ptll"   , */
  /* "DY/06_mpMet", */
  /* "DY/07_PfMet", */
  /* "DY/25_20", */
  /* "DY/third_lepton_veto", */
  /* "DY/mll", */
  /* "DY/met", */
  /* "DY/zveto", */
  /* "DY/mpmet", */
  /* "DY/ptll", */
  /* "DY/bveto", */
  /* "DY/0jet", */
  /* "DY/1jet", */
  /* "DY/08_dphillmet" , */
  /* "DY/09_metopt2l" , */
  /* "DY/10_ht", */
  /* "DY/11_TopControl" , */
  /* "DY/12_DYControl", */
  /* "DY/13_IFCA_Control", */
  
  /* "DY/14_latino", */

  "DY/mll1",
  "DY/mll2",
  "DY/mll3",
  "DY/mll4",
  "DY/mll5",
  "DY/mll6",
  "DY/ptll1",
  "DY/ptll2",
  "DY/ptll3",
  "DY/ptll4",
  "DY/ptll5",
  "DY/ptll6",
  "DY/dphill1",
  "DY/dphill2",
  "DY/dphill3",
  "DY/dphill4",
  "DY/dphill5",
  "DY/dphill6",

};

#endif
