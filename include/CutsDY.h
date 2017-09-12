#if !defined(MYLIB_CUTSDY_H)
#define MYLIB_CUTSDY_H 1

#include "TString.h"


enum {
  DY_00_noCuts,
  DY_01_Has2Leptons,
  DY_02_BVetoLoose,
  //  DY_03_ZWindow,
  DY_03_ZVeto,
  DY_04_Ptll,
  DY_05_mpMet,
  DY_06_PfMet,
  /*  DY_25_20, */
  /*  DY_third_lepton_veto, */
  /*  DY_mll, */
  /*  DY_met, */
  /*  DY_zveto, */
  /*  DY_mpmet, */
  /*  DY_ptll, */
  /*  DY_bveto, */
  /*  DY_0jet, */
  /*  DY_1jet, */
  //  DY_08_dphillmet,
  //DY_09_metopt2l,
  //DY_10_ht,
  DY_07_TopControl,
  DY_08_DYControl,
  DY_09_SSControl,
  /* DY_13_IFCA_Control, */
  /* DY_14_latino, */

  /* DY_mll1, */
  /* DY_mll2, */
  /* DY_mll3, */
  /* DY_mll4, */
  /* DY_mll5, */
  /* DY_mll6, */
  /* DY_mll7, */
  /* DY_mll8, */
  /* DY_ptll1, */
  /* DY_ptll2, */
  /* DY_ptll3, */
  /* DY_ptll4, */
  /* DY_ptll5, */
  /* DY_ptll6, */
  /* DY_ptll7, */
  /* DY_ptl11, */
  /* DY_ptl12, */
  /* DY_ptl13, */
  /* DY_ptl14, */
  /* DY_ptl15, */
  /* DY_ptl16, */
  /* DY_ptl17, */
  /* DY_eta11, */
  /* DY_eta12, */
  /* DY_eta13, */
  /* DY_eta14, */
  /* DY_eta15, */
  /* DY_eta16, */
  /* DY_eta17, */
  /* DY_eta18, */
  /* DY_jetpt1, */
  /* DY_jetpt2, */
  /* DY_jetpt3, */
  /* DY_jetpt4, */
  /* DY_jetpt5, */
  /* DY_jetpt6, */
  /* DY_jetpt7, */
  /* DY_Incljet, */
  /* DY_0jet, */
  /* DY_1jet, */
  /* DY_2jet, */
  /* DY_3jet, */
  /* DY_4jet, */
  /* DY_5jet, */


  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "DY/00_noCuts",
  "DY/01_Has2Leptons",
  "DY/02_BVetoLoose",
  //  "DY/03_ZWindow",
  "DY/03_ZVeto" ,
  "DY/04_Ptll"   ,
  "DY/05_mpMet",
  "DY/06_PfMet",
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
  //"DY/08_dphillmet" ,
  //"DY/09_metopt2l" ,
  //"DY/10_ht",
  "DY/07_TopControl" ,
  "DY/08_DYControl",
  "DY/09_SSControl",
  /* "DY/13_IFCA_Control", */
  
  /* "DY/14_latino", */

  /* "DY/mll1", */
  /* "DY/mll2", */
  /* "DY/mll3", */
  /* "DY/mll4", */
  /* "DY/mll5", */
  /* "DY/mll6", */
  /* "DY/mll7", */
  /* "DY/mll8", */
  /* "DY/ptll1", */
  /* "DY/ptll2", */
  /* "DY/ptll3", */
  /* "DY/ptll4", */
  /* "DY/ptll5", */
  /* "DY/ptll6", */
  /* "DY/ptl11", */
  /* "DY/ptl12", */
  /* "DY/ptl13", */
  /* "DY/ptl14", */
  /* "DY/ptl15", */
  /* "DY/ptl16", */
  /* "DY/ptl17", */
  /* "DY/eta1", */
  /* "DY/eta2", */
  /* "DY/eta3", */
  /* "DY/eta4", */
  /* "DY/eta5", */
  /* "DY/eta6", */
  /* "DY/eta7", */
  /* "DY/eta8", */
  /* "DY/jetpt1", */
  /* "DY/jetpt2", */
  /* "DY/jetpt3", */
  /* "DY/jetpt4", */
  /* "DY/jetpt5", */
  /* "DY/jetpt6", */
  /* "DY/jetpt7", */
  /* "DY/Incljet", */
  /* "DY/0jet", */
  /* "DY/1jet", */
  /* "DY/2jet", */
  /* "DY/3jet", */
  /* "DY/4jet", */
  /* "DY/5jet", */

};

#endif
