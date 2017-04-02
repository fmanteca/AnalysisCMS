#!/bin/bash

if [ $# -lt 1 ] ; then
    echo "  "
    echo "  ./merge.sh ../rootfiles/<systematic>/<analysis>"
    echo "  ./merge.sh ../minitrees/<systematic>/<analysis>"
    echo "  "
    exit -1
fi

FOLDER="$1"

pushd $FOLDER


#hadd -f -k 00_Fakes.root     fakeW*.root
hadd -f -k 01_Data.root      Dou*.root Sin*.root Muon*.root
hadd -f -k 02_WZTo3LNu.root  WZTo3LNu.root
hadd -f -k 03_VZ.root        ZZTo2L2Q*.root WZTo2L2Q__part*.root
hadd -f -k 04_Top.root       TTTo2L2*.root ST_tW_antitop.root ST_tW_top.root
#hadd -f -k 05_ST.root        ST_tW_antitop.root ST_tW_top.root
hadd -f -k 06_WW.root        WWTo2L2Nu.root GluGluWW*.root
hadd -f -k 07_ZJets.root     DYJetsToLL_M-10to50.root DYJetsToTT*.root #DYJetsToLL_M-50*.root
hadd -f -k 08_WJets.root     WJetsToLNu.root
hadd -f -k 09_TTV.root       TTWJetsToLNu*.root TTWJetsToQQ.root TTZjets.root TTZToQQ.root TTZToLLNuNu_M-10.root 
hadd -f -k 10_HWW.root       GluGluHToWWTo2L2NuAMCNLO_M125.root VBFHToWWTo2L2Nu_M125.root GluGluHToTauTau_M125.root VBFHToTauTau_M125.root
hadd -f -k 11_Vg.root        Wg_MADGRAPHMLM.root Zg*.root
#hadd -f -k 12_Zg.root        Zg*.root
hadd -f -k 13_VVV.root       WWW.root WWZ.root WZZ.root ZZZ.root
hadd -f -k 14_HZ.root        HZJ_HToWWTo2L2Nu*.root GluGluZH_HToWWTo2L2Nu*.root
hadd -f -k 15_WgStar.root    WgStarLNuEE.root WgStarLNuMuMu.root


popd
