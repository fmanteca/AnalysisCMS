#if !defined(MYLIB_CUTSDY_H)
#define MYLIB_CUTSDY_H 1

#include "TString.h"


enum {
  Stop_00_noCuts,
  Stop_01_Has2Leptons,
  Stop_02_BVetoLoose,
  Stop_03_ZWindow,
  Stop_04_ZVeto,
  Stop_05_Ptll,
  Stop_06_mpMet,
  Stop_07_PfMet,
   /* Stop_25_20, */
   /* Stop_third_lepton_veto, */
   /* Stop_mll, */
   /* Stop_met, */
   /* Stop_zveto, */
   /* Stop_mpmet, */
   /* Stop_ptll, */
   /* Stop_bveto, */
   /* Stop_0jet, */
   /* Stop_1jet, */
  Stop_08_dphillmet,
  Stop_09_metopt2l,
  Stop_10_ht,
  /* DY_11_TopControl, */
  /* DY_12_DYControl, */
  /* DY_13_IFCA_Control, */
  /* DY_14_latino, */

  /* DY_mll1, */
  /* DY_mll2, */
  /* DY_mll3, */
  /* DY_mll4, */
  /* DY_mll5, */
  /* DY_mll6, */
  /* DY_ptll1, */
  /* DY_ptll2, */
  /* DY_ptll3, */
  /* DY_ptll4, */
  /* DY_ptll5, */
  /* DY_ptll6, */
  /* DY_dphill1, */
  /* DY_dphill2, */
  /* DY_dphill3, */
  /* DY_dphill4, */
  /* DY_dphill5, */
  /* DY_dphill6, */

  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "Stop/00_noCuts",
  "Stop/01_Has2Leptons",
  "Stop/02_BVetoLoose",
  "Stop/03_ZWindow",
  "Stop/04_ZVeto" ,
  "Stop/05_Ptll"   ,
  "Stop/06_mpMet",
  "Stop/07_PfMet",
  /* "Stop/25_20", */
  /* "Stop/third_lepton_veto", */
  /* "Stop/mll", */
  /* "Stop/met", */
  /* "Stop/zveto", */
  /* "Stop/mpmet", */
  /* "Stop/ptll", */
  /* "Stop/bveto", */
  /* "Stop/0jet", */
  /* "Stop/1jet", */
  "Stop/08_dphillmet" ,
  "Stop/09_metopt2l" ,
  "Stop/10_ht",
  /* "DY/11_TopControl" , */
  /* "DY/12_DYControl", */
  /* "DY/13_IFCA_Control", */
  
  /* "DY/14_latino", */

  /* "DY/mll1", */
  /* "DY/mll2", */
  /* "DY/mll3", */
  /* "DY/mll4", */
  /* "DY/mll5", */
  /* "DY/mll6", */
  /* "DY/ptll1", */
  /* "DY/ptll2", */
  /* "DY/ptll3", */
  /* "DY/ptll4", */
  /* "DY/ptll5", */
  /* "DY/ptll6", */
  /* "DY/dphill1", */
  /* "DY/dphill2", */
  /* "DY/dphill3", */
  /* "DY/dphill4", */
  /* "DY/dphill5", */
  /* "DY/dphill6", */

};

#endif
