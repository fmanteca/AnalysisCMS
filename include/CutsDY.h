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
  DY_08_dphillmet,
  DY_09_metopt2l,
  DY_10_ht,
  DY_11_TopControl,
  DY_12_DYControl,
  DY_13_IFCA_Control,
  /* DY_14_latino, */
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
  "DY/08_dphillmet" ,
  "DY/09_metopt2l" ,
  "DY/10_ht",
  "DY/11_TopControl" ,
  "DY/12_DYControl",
  "DY/13_IFCA_Control",
  /* "DY/14_latino", */

};

#endif
