#define AnalysisDY_cxx
#include "../include/AnalysisDY.h"


//------------------------------------------------------------------------------
// AnalysisDY
//------------------------------------------------------------------------------
AnalysisDY::AnalysisDY(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{
  SetSaveMinitree(false);
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
        
     // bool pass_2l = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
     // pass_2l &= (std_vector_lepton_pt->at(0) > 25.);
     // pass_2l &= (std_vector_lepton_pt->at(1) > 20.);
     // pass_2l &= (std_vector_lepton_pt->at(2) < 10.);
     // pass_2l &= (mll>20.);

    

    // // Fill histograms. WW selection: CMS AN-15-325, pag.7 with pt1 > 25 (trigger)
    // //--------------------------------------------------------------------------
    
    // //No cuts
    // //---------------------------------------------------------------------------
    //        FillLevelHistograms(DY_00_noCuts, true);    

    
    //Has 2 Leptons                                                                                                                           
    // //---------------------------------------------------------------------------     
    //FillLevelHistograms(DY_01_Has2Leptons, pass_2l);

    // //if (_saveminitree && pass_2l) minitree->Fill();

    
    // // B Veto                                                                                                                                
    // //---------------------------------------------------------------------------    
    // pass_2l &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_2l &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_2l &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_2l &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_2l &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_2l &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_2l &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_2l &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_2l &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_2l &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // FillLevelHistograms(DY_02_BVetoLoose, pass_2l);

    
    // // Z Window region
    // //---------------------------------------------------------------------------  
    //    bool pass_Zwindow = fabs(mll - Z_MASS) < 15.;
    //    FillLevelHistograms(DY_03_ZWindow, pass_2l && pass_Zwindow);


    // // Z peak Veto
    // //---------------------------------------------------------------------------    
    //  pass_2l &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    //FillLevelHistograms(DY_04_ZVeto, pass_2l);



    // //Ptll cut           
    // //---------------------------------------------------------------------------                         
    //pass_2l &= (ptll>30.);
    //pass_2l &= (_channel == em || ptll > 45.);
    //FillLevelHistograms(DY_05_Ptll, pass_2l);


    // // mpMET cut
    // //---------------------------------------------------------------------------    
    //pass_2l &= (mpmet > 20.);
    //FillLevelHistograms(DY_06_mpMet, pass_2l);


    // // MET cut                                                                                                           
    // //---------------------------------------------------------------------------                                                            
    //pass_2l &= (MET.Et() > 20.);
    //pass_2l &= (_channel == em || MET.Et() > 55.);
    //FillLevelHistograms(DY_07_PfMet, pass_2l);
    //if (pass_2l && _channel==em && _njet==0) EventDump();

    //Gui_syncro
    // bool pass_gui = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_gui &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_gui &= (std_vector_lepton_pt->at(1) > 20.);

    // FillLevelHistograms(DY_25_20, pass_gui);
    // pass_gui &= (std_vector_lepton_pt->at(2) < 10.);
    // FillLevelHistograms(DY_third_lepton_veto, pass_gui);
    // pass_gui &= (mll>12.);
    // FillLevelHistograms(DY_mll, pass_gui);
    // pass_gui &= (metPfType1 > 20.);
    // FillLevelHistograms(DY_met, pass_gui);
    // pass_gui &= (_channel == em || fabs(mll - Z_MASS) > 15.);
    // FillLevelHistograms(DY_zveto, pass_gui);
    // pass_gui &= (mpmet > 20.);
    // FillLevelHistograms(DY_mpmet, pass_gui);
    // pass_gui &= (ptll>30.); 
    // FillLevelHistograms(DY_ptll, pass_gui);
    //  pass_gui &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    //  pass_gui &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    //  pass_gui &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    //  pass_gui &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    //  pass_gui &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    //  pass_gui &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    //  pass_gui &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    //  pass_gui &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    //  pass_gui &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    //  pass_gui &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );

    //  FillLevelHistograms(DY_bveto, pass_gui); 
    //  if (pass_gui && _channel== mm) EventDump();
    // bool pass_0j = pass_gui;
    // pass_0j &= (njet==0);

    // FillLevelHistograms(DY_0jet, pass_0j);
    // pass_gui &= (njet==1);
    // FillLevelHistograms(DY_1jet, pass_gui);
    

    // // Cortes adicionales en variables: dphillmet, MET/ptll                                                            
    // //---------------------------------------------------------------------------                             
       // pass_2l &= (_dphillmet>2.14);
       // pass_2l &= (_channel == em || _dphillmet > 2.37);
       // FillLevelHistograms(DY_08_dphillmet, pass_2l);

       // pass_2l &= (MET.Et()/_pt2l < 1.8);
       // pass_2l &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
       // FillLevelHistograms(DY_09_metopt2l, pass_2l);


    // // // // Corte en Ht para el canal em                                                           
    // // // //---------------------------------------------------------------------------                             
    //       pass_2l &= (_ht<195.);
    //  pass_2l &= (_channel == em ||  _ht < 184.);
    //  FillLevelHistograms(DY_10_ht, pass_2l);


    // // // //top-enriched control region (CMS AN-16-182, pag.57, adapted to WW selection)
    // // // //---------------------------------------------------------------------------                        
    // bool pass_tcontrol = (Lepton1.flavour * Lepton2.flavour < 0);
    // pass_tcontrol &= (Lepton1.v.Pt() > 25.);
    // pass_tcontrol &= (Lepton2.v.Pt() > 20.);
    // pass_tcontrol &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_tcontrol &= (mll > 12.);
    // pass_tcontrol &= (ptll > 30.);
    // pass_tcontrol &= (_channel == em || ptll > 45.);
    // pass_tcontrol &= (MET.Et() > 20.);
    // pass_tcontrol &= (_channel == em || MET.Et() > 45.);
    // pass_tcontrol &= (mpmet > 20.);
    // pass_tcontrol &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_tcontrol &= (_njet==0 ? _nbjet20cmvav2l > 0 : _nbjet30cmvav2l > 0);
    // FillLevelHistograms(DY_11_TopControl, pass_tcontrol);    


    // // // //DY-Control region
    // // // //---------------------------------------------------------------------------

    // bool pass_dycontrol = (Lepton1.flavour * Lepton2.flavour < 0);
    // pass_dycontrol &= (Lepton1.v.Pt() > 25.);
    // pass_dycontrol &= (Lepton2.v.Pt() > 20.);
    // pass_dycontrol &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_dycontrol &= (mll > 12.);
    // pass_dycontrol &= (ptll>30.);
    // pass_dycontrol &= (mpmet > 20.);
    // pass_dycontrol &= (MET.Et() > 20.);
    // FillLevelHistograms(DY_12_DYControl, pass_dycontrol);        


    // // // // IFCA-syncro region
    // // // //----------------------------------------------------------------------------

    // bool pass_ifca = (Lepton1.flavour * Lepton2.flavour < 0);
    // pass_ifca &= (Lepton1.v.Pt() > 25.);
    // pass_ifca &= (Lepton2.v.Pt() > 20.);
    // pass_ifca &= (_nlepton == 2);
    // pass_ifca &= (mll > 20.);
    // pass_ifca &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // FillLevelHistograms(DY_13_IFCA_Control, pass_ifca);        
    
    // //plotsConfiguration synchro
    
    // bool pass_latino = (std_vector_lepton_pt->at(0) > 25.);
    // pass_latino &= (std_vector_lepton_pt->at(1)>25.);
    // pass_latino &= (std_vector_lepton_pt->at(2)<10.); 
    // pass_latino &= (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) == -13*11);
    // pass_latino &= (mll > 20.);                                                                                                                      
    // pass_latino &= (metPfType1 > 20.);                                                                                                              
    // pass_latino &= (mpmet > 20.);                                                                                                                    
    // pass_latino &= (ptll > 30.);	 
    // pass_latino &= (std_vector_jet_pt->at(0) < 30.);                                                                                              
    // pass_latino &= ( std_vector_jet_pt->at(0) < 15. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );                                       
    // pass_latino &= ( std_vector_jet_pt->at(1) < 15. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );                                                          
    // pass_latino &= ( std_vector_jet_pt->at(2) < 15. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );                                                          
    // pass_latino &= ( std_vector_jet_pt->at(3) < 15. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );                                                          
    // pass_latino &= ( std_vector_jet_pt->at(4) < 15. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );                                                          
    // pass_latino &= ( std_vector_jet_pt->at(5) < 15. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );                                                          
    // pass_latino &= ( std_vector_jet_pt->at(6) < 15. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );                                                          
    // pass_latino &= ( std_vector_jet_pt->at(7) < 15. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );                                                 
    // pass_latino &= ( std_vector_jet_pt->at(8) < 15. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );                                                       
    // pass_latino &= ( std_vector_jet_pt->at(9) < 15. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // FillLevelHistograms(DY_14_latino, pass_latino);
    








    //Diff XS:
    
    bool pass_mll1 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    pass_mll1 &= (std_vector_lepton_pt->at(0) > 25.);
    pass_mll1 &= (std_vector_lepton_pt->at(1) > 20.);
    pass_mll1 &= (std_vector_lepton_pt->at(2) < 10.);
    pass_mll1 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    pass_mll1 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    pass_mll1 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    pass_mll1 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    pass_mll1 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    pass_mll1 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    pass_mll1 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    pass_mll1 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    pass_mll1 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    pass_mll1 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    pass_mll1 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass_mll1 &= (ptll>30.);
    pass_mll1 &= (_channel == em || ptll > 45.);
    pass_mll1 &= (mpmet > 20.);
    pass_mll1 &= (MET.Et() > 20.);
    pass_mll1 &= (_channel == em || MET.Et() > 55.);
    pass_mll1 &= (_dphillmet>2.14);
    pass_mll1 &= (_channel == em || _dphillmet > 2.37);
    pass_mll1 &= (MET.Et()/_pt2l < 1.8);
    pass_mll1 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    pass_mll1 &= (_ht<195.);
    pass_mll1 &= (_channel == em ||  _ht < 184.);
    pass_mll1 &= (mll>20. && mll<40.);
    FillLevelHistograms(DY_mll1, pass_mll1);        




    bool pass_mll2 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    pass_mll2 &= (std_vector_lepton_pt->at(0) > 25.);
    pass_mll2 &= (std_vector_lepton_pt->at(1) > 20.);
    pass_mll2 &= (std_vector_lepton_pt->at(2) < 10.);
    pass_mll2 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    pass_mll2 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    pass_mll2 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    pass_mll2 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    pass_mll2 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    pass_mll2 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    pass_mll2 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    pass_mll2 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    pass_mll2 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    pass_mll2 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    pass_mll2 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass_mll2 &= (ptll>30.);
    pass_mll2 &= (_channel == em || ptll > 45.);
    pass_mll2 &= (mpmet > 20.);
    pass_mll2 &= (MET.Et() > 20.);
    pass_mll2 &= (_channel == em || MET.Et() > 55.);
    pass_mll2 &= (_dphillmet>2.14);
    pass_mll2 &= (_channel == em || _dphillmet > 2.37);
    pass_mll2 &= (MET.Et()/_pt2l < 1.8);
    pass_mll2 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    pass_mll2 &= (_ht<195.);
    pass_mll2 &= (_channel == em ||  _ht < 184.);
    pass_mll2 &= (mll>40. && mll<60.);
    FillLevelHistograms(DY_mll2, pass_mll2);        
    

    
    bool pass_mll3 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    pass_mll3 &= (std_vector_lepton_pt->at(0) > 25.);
    pass_mll3 &= (std_vector_lepton_pt->at(1) > 20.);
    pass_mll3 &= (std_vector_lepton_pt->at(2) < 10.);
    pass_mll3 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    pass_mll3 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    pass_mll3 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    pass_mll3 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    pass_mll3 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    pass_mll3 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    pass_mll3 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    pass_mll3 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    pass_mll3 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    pass_mll3 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    pass_mll3 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass_mll3 &= (ptll>30.);
    pass_mll3 &= (_channel == em || ptll > 45.);
    pass_mll3 &= (mpmet > 20.);
    pass_mll3 &= (MET.Et() > 20.);
    pass_mll3 &= (_channel == em || MET.Et() > 55.);
    pass_mll3 &= (_dphillmet>2.14);
    pass_mll3 &= (_channel == em || _dphillmet > 2.37);
    pass_mll3 &= (MET.Et()/_pt2l < 1.8);
    pass_mll3 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    pass_mll3 &= (_ht<195.);
    pass_mll3 &= (_channel == em ||  _ht < 184.);
    pass_mll3 &= (mll>60. && mll<80.);
    FillLevelHistograms(DY_mll3, pass_mll3);        




    bool pass_mll4 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    pass_mll4 &= (std_vector_lepton_pt->at(0) > 25.);
    pass_mll4 &= (std_vector_lepton_pt->at(1) > 20.);
    pass_mll4 &= (std_vector_lepton_pt->at(2) < 10.);
    pass_mll4 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    pass_mll4 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    pass_mll4 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    pass_mll4 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    pass_mll4 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    pass_mll4 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    pass_mll4 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    pass_mll4 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    pass_mll4 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    pass_mll4 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    pass_mll4 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass_mll4 &= (ptll>30.);
    pass_mll4 &= (_channel == em || ptll > 45.);
    pass_mll4 &= (mpmet > 20.);
    pass_mll4 &= (MET.Et() > 20.);
    pass_mll4 &= (_channel == em || MET.Et() > 55.);
    pass_mll4 &= (_dphillmet>2.14);
    pass_mll4 &= (_channel == em || _dphillmet > 2.37);
    pass_mll4 &= (MET.Et()/_pt2l < 1.8);
    pass_mll4 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    pass_mll4 &= (_ht<195.);
    pass_mll4 &= (_channel == em ||  _ht < 184.);
    pass_mll4 &= (mll>80. && mll<100.);
    FillLevelHistograms(DY_mll4, pass_mll4);        





    bool pass_mll5 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    pass_mll5 &= (std_vector_lepton_pt->at(0) > 25.);
    pass_mll5 &= (std_vector_lepton_pt->at(1) > 20.);
    pass_mll5 &= (std_vector_lepton_pt->at(2) < 10.);
    pass_mll5 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    pass_mll5 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    pass_mll5 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    pass_mll5 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    pass_mll5 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    pass_mll5 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    pass_mll5 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    pass_mll5 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    pass_mll5 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    pass_mll5 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    pass_mll5 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass_mll5 &= (ptll>30.);
    pass_mll5 &= (_channel == em || ptll > 45.);
    pass_mll5 &= (mpmet > 20.);
    pass_mll5 &= (MET.Et() > 20.);
    pass_mll5 &= (_channel == em || MET.Et() > 55.);
    pass_mll5 &= (_dphillmet>2.14);
    pass_mll5 &= (_channel == em || _dphillmet > 2.37);
    pass_mll5 &= (MET.Et()/_pt2l < 1.8);
    pass_mll5 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    pass_mll5 &= (_ht<195.);
    pass_mll5 &= (_channel == em ||  _ht < 184.);
    pass_mll5 &= (mll>100. && mll<140.);
    FillLevelHistograms(DY_mll5, pass_mll5);        






    bool pass_mll6 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    pass_mll6 &= (std_vector_lepton_pt->at(0) > 25.);
    pass_mll6 &= (std_vector_lepton_pt->at(1) > 20.);
    pass_mll6 &= (std_vector_lepton_pt->at(2) < 10.);
    pass_mll6 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    pass_mll6 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    pass_mll6 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    pass_mll6 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    pass_mll6 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    pass_mll6 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    pass_mll6 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    pass_mll6 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    pass_mll6 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    pass_mll6 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    pass_mll6 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass_mll6 &= (ptll>30.);
    pass_mll6 &= (_channel == em || ptll > 45.);
    pass_mll6 &= (mpmet > 20.);
    pass_mll6 &= (MET.Et() > 20.);
    pass_mll6 &= (_channel == em || MET.Et() > 55.);
    pass_mll6 &= (_dphillmet>2.14);
    pass_mll6 &= (_channel == em || _dphillmet > 2.37);
    pass_mll6 &= (MET.Et()/_pt2l < 1.8);
    pass_mll6 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    pass_mll6 &= (_ht<195.);
    pass_mll6 &= (_channel == em ||  _ht < 184.);
    pass_mll6 &= (mll>140. && mll<180.);
    FillLevelHistograms(DY_mll6, pass_mll6);        
    





    bool pass_ptll1 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    pass_ptll1 &= (std_vector_lepton_pt->at(0) > 25.);
    pass_ptll1 &= (std_vector_lepton_pt->at(1) > 20.);
    pass_ptll1 &= (std_vector_lepton_pt->at(2) < 10.);
    pass_ptll1 &= (mll > 20.);
    pass_ptll1 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    pass_ptll1 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    pass_ptll1 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    pass_ptll1 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    pass_ptll1 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    pass_ptll1 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    pass_ptll1 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    pass_ptll1 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    pass_ptll1 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    pass_ptll1 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    pass_ptll1 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass_ptll1 &= (mpmet > 20.);
    pass_ptll1 &= (MET.Et() > 20.);
    pass_ptll1 &= (_channel == em || MET.Et() > 55.);
    pass_ptll1 &= (_dphillmet>2.14);
    pass_ptll1 &= (_channel == em || _dphillmet > 2.37);
    pass_ptll1 &= (MET.Et()/_pt2l < 1.8);
    pass_ptll1 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    pass_ptll1 &= (_ht<195.);
    pass_ptll1 &= (_channel == em ||  _ht < 184.);
    pass_ptll1 &= (ptll>30. && ptll<40.);
    FillLevelHistograms(DY_ptll1, pass_ptll1);        








    bool pass_ptll2 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    pass_ptll2 &= (std_vector_lepton_pt->at(0) > 25.);
    pass_ptll2 &= (std_vector_lepton_pt->at(1) > 20.);
    pass_ptll2 &= (std_vector_lepton_pt->at(2) < 10.);
    pass_ptll2 &= (mll > 20.);
    pass_ptll2 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    pass_ptll2 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    pass_ptll2 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    pass_ptll2 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    pass_ptll2 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    pass_ptll2 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    pass_ptll2 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    pass_ptll2 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    pass_ptll2 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    pass_ptll2 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    pass_ptll2 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass_ptll2 &= (mpmet > 20.);
    pass_ptll2 &= (MET.Et() > 20.);
    pass_ptll2 &= (_channel == em || MET.Et() > 55.);
    pass_ptll2 &= (_dphillmet>2.14);
    pass_ptll2 &= (_channel == em || _dphillmet > 2.37);
    pass_ptll2 &= (MET.Et()/_pt2l < 1.8);
    pass_ptll2 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    pass_ptll2 &= (_ht<195.);
    pass_ptll2 &= (_channel == em ||  _ht < 184.);
    pass_ptll2 &= (ptll>40. && ptll<50.);
    FillLevelHistograms(DY_ptll2, pass_ptll2);        







    bool pass_ptll3 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    pass_ptll3 &= (std_vector_lepton_pt->at(0) > 25.);
    pass_ptll3 &= (std_vector_lepton_pt->at(1) > 20.);
    pass_ptll3 &= (std_vector_lepton_pt->at(2) < 10.);
    pass_ptll3 &= (mll > 20.);
    pass_ptll3 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    pass_ptll3 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    pass_ptll3 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    pass_ptll3 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    pass_ptll3 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    pass_ptll3 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    pass_ptll3 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    pass_ptll3 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    pass_ptll3 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    pass_ptll3 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    pass_ptll3 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass_ptll3 &= (mpmet > 20.);
    pass_ptll3 &= (MET.Et() > 20.);
    pass_ptll3 &= (_channel == em || MET.Et() > 55.);
    pass_ptll3 &= (_dphillmet>2.14);
    pass_ptll3 &= (_channel == em || _dphillmet > 2.37);
    pass_ptll3 &= (MET.Et()/_pt2l < 1.8);
    pass_ptll3 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    pass_ptll3 &= (_ht<195.);
    pass_ptll3 &= (_channel == em ||  _ht < 184.);
    pass_ptll3 &= (ptll>50. && ptll<60.);
    FillLevelHistograms(DY_ptll3, pass_ptll3);        
    




    bool pass_ptll4 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    pass_ptll4 &= (std_vector_lepton_pt->at(0) > 25.);
    pass_ptll4 &= (std_vector_lepton_pt->at(1) > 20.);
    pass_ptll4 &= (std_vector_lepton_pt->at(2) < 10.);
    pass_ptll4 &= (mll > 20.);
    pass_ptll4 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    pass_ptll4 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    pass_ptll4 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    pass_ptll4 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    pass_ptll4 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    pass_ptll4 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    pass_ptll4 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    pass_ptll4 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    pass_ptll4 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    pass_ptll4 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    pass_ptll4 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass_ptll4 &= (mpmet > 20.);
    pass_ptll4 &= (MET.Et() > 20.);
    pass_ptll4 &= (_channel == em || MET.Et() > 55.);
    pass_ptll4 &= (_dphillmet>2.14);
    pass_ptll4 &= (_channel == em || _dphillmet > 2.37);
    pass_ptll4 &= (MET.Et()/_pt2l < 1.8);
    pass_ptll4 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    pass_ptll4 &= (_ht<195.);
    pass_ptll4 &= (_channel == em ||  _ht < 184.);
    pass_ptll4 &= (ptll>60. && ptll<70.);
    FillLevelHistograms(DY_ptll4, pass_ptll4);        







    bool pass_ptll5 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    pass_ptll5 &= (std_vector_lepton_pt->at(0) > 25.);
    pass_ptll5 &= (std_vector_lepton_pt->at(1) > 20.);
    pass_ptll5 &= (std_vector_lepton_pt->at(2) < 10.);
    pass_ptll5 &= (mll > 20.);
    pass_ptll5 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    pass_ptll5 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    pass_ptll5 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    pass_ptll5 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    pass_ptll5 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    pass_ptll5 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    pass_ptll5 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    pass_ptll5 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    pass_ptll5 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    pass_ptll5 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    pass_ptll5 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass_ptll5 &= (mpmet > 20.);
    pass_ptll5 &= (MET.Et() > 20.);
    pass_ptll5 &= (_channel == em || MET.Et() > 55.);
    pass_ptll5 &= (_dphillmet>2.14);
    pass_ptll5 &= (_channel == em || _dphillmet > 2.37);
    pass_ptll5 &= (MET.Et()/_pt2l < 1.8);
    pass_ptll5 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    pass_ptll5 &= (_ht<195.);
    pass_ptll5 &= (_channel == em ||  _ht < 184.);
    pass_ptll5 &= (ptll>70. && ptll<90.);
    FillLevelHistograms(DY_ptll5, pass_ptll5);        







    bool pass_ptll6 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    pass_ptll6 &= (std_vector_lepton_pt->at(0) > 25.);
    pass_ptll6 &= (std_vector_lepton_pt->at(1) > 20.);
    pass_ptll6 &= (std_vector_lepton_pt->at(2) < 10.);
    pass_ptll6 &= (mll > 20.);
    pass_ptll6 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    pass_ptll6 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    pass_ptll6 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    pass_ptll6 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    pass_ptll6 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    pass_ptll6 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    pass_ptll6 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    pass_ptll6 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    pass_ptll6 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    pass_ptll6 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    pass_ptll6 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass_ptll6 &= (mpmet > 20.);
    pass_ptll6 &= (MET.Et() > 20.);
    pass_ptll6 &= (_channel == em || MET.Et() > 55.);
    pass_ptll6 &= (_dphillmet>2.14);
    pass_ptll6 &= (_channel == em || _dphillmet > 2.37);
    pass_ptll6 &= (MET.Et()/_pt2l < 1.8);
    pass_ptll6 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    pass_ptll6 &= (_ht<195.);
    pass_ptll6 &= (_channel == em ||  _ht < 184.);
    pass_ptll6 &= (ptll>90. && ptll<120.);
    FillLevelHistograms(DY_ptll6, pass_ptll6);        
    
    







    bool pass_dphill1 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    pass_dphill1 &= (std_vector_lepton_pt->at(0) > 25.);
    pass_dphill1 &= (std_vector_lepton_pt->at(1) > 20.);
    pass_dphill1 &= (std_vector_lepton_pt->at(2) < 10.);
    pass_dphill1 &= (mll>20.);
    pass_dphill1 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    pass_dphill1 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    pass_dphill1 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    pass_dphill1 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    pass_dphill1 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    pass_dphill1 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    pass_dphill1 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    pass_dphill1 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    pass_dphill1 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    pass_dphill1 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    pass_dphill1 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass_dphill1 &= (ptll>30.);
    pass_dphill1 &= (_channel == em || ptll > 45.);
    pass_dphill1 &= (mpmet > 20.);
    pass_dphill1 &= (MET.Et() > 20.);
    pass_dphill1 &= (_channel == em || MET.Et() > 55.);
    pass_dphill1 &= (_dphillmet>2.14);
    pass_dphill1 &= (_channel == em || _dphillmet > 2.37);
    pass_dphill1 &= (MET.Et()/_pt2l < 1.8);
    pass_dphill1 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    pass_dphill1 &= (_ht<195.);
    pass_dphill1 &= (_channel == em ||  _ht < 184.);
    pass_dphill1 &= (dphill > 0. && dphill < 0.5);
    FillLevelHistograms(DY_dphill1, pass_dphill1);        





    bool pass_dphill2 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    pass_dphill2 &= (std_vector_lepton_pt->at(0) > 25.);
    pass_dphill2 &= (std_vector_lepton_pt->at(1) > 20.);
    pass_dphill2 &= (std_vector_lepton_pt->at(2) < 10.);
    pass_dphill2 &= (mll>20.);
    pass_dphill2 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    pass_dphill2 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    pass_dphill2 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    pass_dphill2 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    pass_dphill2 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    pass_dphill2 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    pass_dphill2 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    pass_dphill2 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    pass_dphill2 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    pass_dphill2 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    pass_dphill2 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass_dphill2 &= (ptll>30.);
    pass_dphill2 &= (_channel == em || ptll > 45.);
    pass_dphill2 &= (mpmet > 20.);
    pass_dphill2 &= (MET.Et() > 20.);
    pass_dphill2 &= (_channel == em || MET.Et() > 55.);
    pass_dphill2 &= (_dphillmet>2.14);
    pass_dphill2 &= (_channel == em || _dphillmet > 2.37);
    pass_dphill2 &= (MET.Et()/_pt2l < 1.8);
    pass_dphill2 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    pass_dphill2 &= (_ht<195.);
    pass_dphill2 &= (_channel == em ||  _ht < 184.);
    pass_dphill2 &= (dphill > 0.5 && dphill < 1.0);
    FillLevelHistograms(DY_dphill2, pass_dphill2);        





    bool pass_dphill3 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    pass_dphill3 &= (std_vector_lepton_pt->at(0) > 25.);
    pass_dphill3 &= (std_vector_lepton_pt->at(1) > 20.);
    pass_dphill3 &= (std_vector_lepton_pt->at(2) < 10.);
    pass_dphill3 &= (mll>20.);
    pass_dphill3 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    pass_dphill3 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    pass_dphill3 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    pass_dphill3 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    pass_dphill3 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    pass_dphill3 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    pass_dphill3 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    pass_dphill3 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    pass_dphill3 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    pass_dphill3 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    pass_dphill3 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass_dphill3 &= (ptll>30.);
    pass_dphill3 &= (_channel == em || ptll > 45.);
    pass_dphill3 &= (mpmet > 20.);
    pass_dphill3 &= (MET.Et() > 20.);
    pass_dphill3 &= (_channel == em || MET.Et() > 55.);
    pass_dphill3 &= (_dphillmet>2.14);
    pass_dphill3 &= (_channel == em || _dphillmet > 2.37);
    pass_dphill3 &= (MET.Et()/_pt2l < 1.8);
    pass_dphill3 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    pass_dphill3 &= (_ht<195.);
    pass_dphill3 &= (_channel == em ||  _ht < 184.);
    pass_dphill3 &= (dphill > 1. && dphill < 1.5);
    FillLevelHistograms(DY_dphill3, pass_dphill3);







    bool pass_dphill4 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    pass_dphill4 &= (std_vector_lepton_pt->at(0) > 25.);
    pass_dphill4 &= (std_vector_lepton_pt->at(1) > 20.);
    pass_dphill4 &= (std_vector_lepton_pt->at(2) < 10.);
    pass_dphill4 &= (mll>20.);
    pass_dphill4 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    pass_dphill4 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    pass_dphill4 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    pass_dphill4 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    pass_dphill4 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    pass_dphill4 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    pass_dphill4 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    pass_dphill4 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    pass_dphill4 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    pass_dphill4 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    pass_dphill4 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass_dphill4 &= (ptll>30.);
    pass_dphill4 &= (_channel == em || ptll > 45.);
    pass_dphill4 &= (mpmet > 20.);
    pass_dphill4 &= (MET.Et() > 20.);
    pass_dphill4 &= (_channel == em || MET.Et() > 55.);
    pass_dphill4 &= (_dphillmet>2.14);
    pass_dphill4 &= (_channel == em || _dphillmet > 2.37);
    pass_dphill4 &= (MET.Et()/_pt2l < 1.8);
    pass_dphill4 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    pass_dphill4 &= (_ht<195.);
    pass_dphill4 &= (_channel == em ||  _ht < 184.);
    pass_dphill4 &= (dphill > 1.5 && dphill < 2.0);
    FillLevelHistograms(DY_dphill4, pass_dphill4);        







    bool pass_dphill5 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    pass_dphill5 &= (std_vector_lepton_pt->at(0) > 25.);
    pass_dphill5 &= (std_vector_lepton_pt->at(1) > 20.);
    pass_dphill5 &= (std_vector_lepton_pt->at(2) < 10.);
    pass_dphill5 &= (mll>20.);
    pass_dphill5 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    pass_dphill5 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    pass_dphill5 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    pass_dphill5 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    pass_dphill5 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    pass_dphill5 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    pass_dphill5 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    pass_dphill5 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    pass_dphill5 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    pass_dphill5 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    pass_dphill5 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass_dphill5 &= (ptll>30.);
    pass_dphill5 &= (_channel == em || ptll > 45.);
    pass_dphill5 &= (mpmet > 20.);
    pass_dphill5 &= (MET.Et() > 20.);
    pass_dphill5 &= (_channel == em || MET.Et() > 55.);
    pass_dphill5 &= (_dphillmet>2.14);
    pass_dphill5 &= (_channel == em || _dphillmet > 2.37);
    pass_dphill5 &= (MET.Et()/_pt2l < 1.8);
    pass_dphill5 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    pass_dphill5 &= (_ht<195.);
    pass_dphill5 &= (_channel == em ||  _ht < 184.);
    pass_dphill5 &= (dphill > 2.0 && dphill < 2.5);
    FillLevelHistograms(DY_dphill5, pass_dphill5);       






    bool pass_dphill6 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    pass_dphill6 &= (std_vector_lepton_pt->at(0) > 25.);
    pass_dphill6 &= (std_vector_lepton_pt->at(1) > 20.);
    pass_dphill6 &= (std_vector_lepton_pt->at(2) < 10.);
    pass_dphill6 &= (mll>20.);
    pass_dphill6 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    pass_dphill6 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    pass_dphill6 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    pass_dphill6 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    pass_dphill6 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    pass_dphill6 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    pass_dphill6 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    pass_dphill6 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    pass_dphill6 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    pass_dphill6 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    pass_dphill6 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass_dphill6 &= (ptll>30.);
    pass_dphill6 &= (_channel == em || ptll > 45.);
    pass_dphill6 &= (mpmet > 20.);
    pass_dphill6 &= (MET.Et() > 20.);
    pass_dphill6 &= (_channel == em || MET.Et() > 55.);
    pass_dphill6 &= (_dphillmet>2.14);
    pass_dphill6 &= (_channel == em || _dphillmet > 2.37);
    pass_dphill6 &= (MET.Et()/_pt2l < 1.8);
    pass_dphill6 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    pass_dphill6 &= (_ht<195.);
    pass_dphill6 &= (_channel == em ||  _ht < 184.);
    pass_dphill6 &= (dphill > 2.5 && dphill < 3.0);
    FillLevelHistograms(DY_dphill6, pass_dphill6);        
    
    
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
