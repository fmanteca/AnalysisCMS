#define AnalysisDY_cxx
#include "../include/AnalysisDY.h"


//------------------------------------------------------------------------------
// AnalysisDY
//------------------------------------------------------------------------------
AnalysisDY::AnalysisDY(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{
  SetSaveMinitree(true);
}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisDY::Loop(TString analysis, TString filename, float luminosity)
{
  if (fChain == 0) return;

  Setup(analysis, filename, luminosity);


  // Define histograms
  //----------------------------------------------------------------------------
  for (int j=0; j<ncut; j++) {

    for (int k=0; k<=njetbin; k++) {

      TString sbin = (k < njetbin) ? Form("/%djet", k) : "";

      TString directory = scut[j] + sbin;

      root_output->cd();

      if (k < njetbin) gDirectory->mkdir(directory);

      root_output->cd(directory);

      for (int i=ee; i<=ll; i++) {
	
	TString suffix = "_" + schannel[i];

	DefineHistograms(i, j, k, suffix);
      }
    }
  }

  root_output->cd();
  

  // Loop over events
  //----------------------------------------------------------------------------
    for (Long64_t jentry=0; jentry<_nentries; jentry++) {
  // for (Long64_t jentry=0; jentry<10000; jentry++) {

    Long64_t ientry = LoadTree(jentry);
   
    if (ientry < 0) break;

    fChain->GetEntry(jentry);

    PrintProgress(jentry, _nentries);

    EventSetup();


    // Analysis
    //--------------------------------------------------------------------------

    _nelectron = 0;

    if (abs(Lepton1.flavour) == ELECTRON_FLAVOUR) _nelectron++;
    if (abs(Lepton2.flavour) == ELECTRON_FLAVOUR) _nelectron++;

     if      (_nelectron == 2) _channel = ee;
    else if (_nelectron == 1) _channel = em;
    else if (_nelectron == 0) _channel = mm;

     //if (_nelectron == 0 || _nelectron == 2) _channel = SF;
     //else if (_nelectron == 1) _channel = em;

    
    _m2l  = mll;
    _pt2l = ptll;
    
    bool pass_2l = (Lepton1.flavour * Lepton2.flavour < 0);
    pass_2l &= (Lepton1.v.Pt() > 25.);
    pass_2l &= (Lepton2.v.Pt() > 25.);
    pass_2l &= (std_vector_lepton_pt->at(2) < 10.);
    pass_2l &= (mll>20.);



    // Fill histograms. WW selection: CMS AN-15-325, pag.7 with pt1 > 25 (trigger)
    //--------------------------------------------------------------------------
    
    //No cuts
    //---------------------------------------------------------------------------
    FillLevelHistograms(DY_00_noCuts, true);    

    
    //Has 2 Leptons                                                                                                                           
    //---------------------------------------------------------------------------     
    FillLevelHistograms(DY_01_Has2Leptons, pass_2l);

    //if (_saveminitree && pass_2l) minitree->Fill();

    
    // B Veto                                                                                                                                
    //---------------------------------------------------------------------------    
    pass_2l &= (_nbjet15csvv2l == 0);
    FillLevelHistograms(DY_02_BVetoLoose, pass_2l);

    
    // Z Window region
    //---------------------------------------------------------------------------  
    bool pass_Zwindow = fabs(_m2l - Z_MASS) < 15.;
    FillLevelHistograms(DY_03_ZWindow, pass_2l && pass_Zwindow);


    // Z peak Veto
    //---------------------------------------------------------------------------    
    pass_2l &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    FillLevelHistograms(DY_04_ZVeto, pass_2l);



    //Ptll cut           
    //---------------------------------------------------------------------------                         
    pass_2l &= (ptll>30.);
    pass_2l &= (_channel == em || ptll > 45.);
    FillLevelHistograms(DY_05_Ptll, pass_2l);


    // mpMET cut
    //---------------------------------------------------------------------------    
    pass_2l &= (mpmet > 20.);
    FillLevelHistograms(DY_06_mpMet, pass_2l);


    // MET cut                                                                                                           
    //---------------------------------------------------------------------------                                                            
    pass_2l &= (MET.Et() > 20.);
    pass_2l &= (_channel == em || MET.Et() > 55.);
    FillLevelHistograms(DY_07_PfMet, pass_2l);
    if (pass_2l && _channel==em && _njet==0) EventDump();

    //Gui_syncro
    // bool pass_gui = (Lepton1.flavour * Lepton2.flavour < 0);
    // pass_gui &= (Lepton1.v.Pt() > 25.);
    // pass_gui &= (Lepton2.v.Pt() > 20.);

    // FillLevelHistograms(DY_25_20, pass_gui);
    // pass_gui &= (std_vector_lepton_pt->at(2) < 10.);
    // FillLevelHistograms(DY_third_lepton_veto, pass_gui);
    // pass_gui &= (mll>12.);
    // FillLevelHistograms(DY_mll, pass_gui);
    // pass_gui &= (MET.Et() > 20.);
    // FillLevelHistograms(DY_met, pass_gui);
    // pass_gui &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // FillLevelHistograms(DY_zveto, pass_gui);
    // pass_gui &= (mpmet > 20.);
    // FillLevelHistograms(DY_mpmet, pass_gui);
    // pass_gui &= (ptll>30.); 
    // FillLevelHistograms(DY_ptll, pass_gui);
    // pass_gui &= (_nbjet20csvv2l == 0);
    // FillLevelHistograms(DY_bveto, pass_gui); 
    // if (pass_gui && _channel==em) EventDump();
    // bool pass_0j = pass_gui;
    // pass_0j &= (_njet==0);

    // FillLevelHistograms(DY_0jet, pass_0j);
    // pass_gui &= (_njet==1);
    // FillLevelHistograms(DY_1jet, pass_gui);
    

    // Cortes adicionales en variables: dphillmet, MET/ptll                                                            
    //---------------------------------------------------------------------------                             
     pass_2l &= (_dphillmet>2.14);
     pass_2l &= (_channel == em || _dphillmet > 2.37);
     FillLevelHistograms(DY_08_dphillmet, pass_2l);

     pass_2l &= (MET.Et()/_pt2l < 1.8);
     pass_2l &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
     FillLevelHistograms(DY_09_metopt2l, pass_2l);


    // // Corte en Ht para el canal em                                                           
    // //---------------------------------------------------------------------------                             
     pass_2l &= (_ht<195.);
     pass_2l &= (_channel == em ||  _ht < 184.);
     FillLevelHistograms(DY_10_ht, pass_2l);


    // //top-enriched control region (CMS AN-16-182, pag.57, adapted to WW selection)
    // //---------------------------------------------------------------------------                        
    bool pass_tcontrol = (Lepton1.flavour * Lepton2.flavour < 0);
    pass_tcontrol &= (Lepton1.v.Pt() > 25.);
    pass_tcontrol &= (Lepton2.v.Pt() > 20.);
    pass_tcontrol &= (std_vector_lepton_pt->at(2) < 10.);
    pass_tcontrol &= (mll > 12.);
    pass_tcontrol &= (ptll > 30.);
    pass_tcontrol &= (_channel == em || ptll > 45.);
    pass_tcontrol &= (MET.Et() > 20.);
    pass_tcontrol &= (_channel == em || MET.Et() > 45.);
    pass_tcontrol &= (mpmet > 20.);
    pass_tcontrol &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass_tcontrol &= (_njet==0 ? _nbjet20cmvav2l > 0 : _nbjet30cmvav2l > 0);
    FillLevelHistograms(DY_11_TopControl, pass_tcontrol);    


    // //DY-Control region
    // //---------------------------------------------------------------------------

    bool pass_dycontrol = (Lepton1.flavour * Lepton2.flavour < 0);
    pass_dycontrol &= (Lepton1.v.Pt() > 25.);
    pass_dycontrol &= (Lepton2.v.Pt() > 20.);
    pass_dycontrol &= (std_vector_lepton_pt->at(2) < 10.);
    pass_dycontrol &= (mll > 12.);
    pass_dycontrol &= (ptll>30.);
    pass_dycontrol &= (mpmet > 20.);
    pass_dycontrol &= (MET.Et() > 20.);
    FillLevelHistograms(DY_12_DYControl, pass_dycontrol);        


    // // IFCA-syncro region
    // //----------------------------------------------------------------------------

    bool pass_ifca = (Lepton1.flavour * Lepton2.flavour < 0);
    pass_ifca &= (Lepton1.v.Pt() > 25.);
    pass_ifca &= (Lepton2.v.Pt() > 20.);
    pass_ifca &= (_nlepton == 2);
    pass_ifca &= (mll > 20.);
    pass_ifca &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    FillLevelHistograms(DY_13_IFCA_Control, pass_ifca);        
    
  }




  EndJob();
}


//------------------------------------------------------------------------------
// FillAnalysisHistograms
//------------------------------------------------------------------------------
void AnalysisDY::FillAnalysisHistograms(int ichannel,
					int icut,
					int ijet)
{
  if (ichannel != ll) FillAnalysisHistograms(ll, icut, ijet);
}


//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisDY::FillLevelHistograms(int  icut,
				     bool pass)
{
  if (!pass) return;

  FillHistograms(_channel, icut, _jetbin);
  FillHistograms(_channel, icut, njetbin);

    FillAnalysisHistograms(_channel, icut, _jetbin);
    FillAnalysisHistograms(_channel, icut, njetbin);
}
