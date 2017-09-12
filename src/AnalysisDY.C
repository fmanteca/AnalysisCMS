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

      //      for (int i=ee; i<=ll; i++) {
      for (int i=SF; i<=ll; i++) {
	
	TString suffix = "_" + schannel[i];

	DefineHistograms(i, j, k, suffix);
      }
    }
  }

  root_output->cd();
  

  // Loop over events
  //----------------------------------------------------------------------------
    for (Long64_t jentry=0; jentry<_nentries; jentry++) {

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

     // if      (_nelectron == 2) _channel = ee;
     // else if (_nelectron == 1) _channel = em;
     // else if (_nelectron == 0) _channel = mm;

    if (_nelectron == 0 || _nelectron == 2) _channel = SF;
    else if (_nelectron == 1) _channel = em;

    
    _m2l  = mll;
    _pt2l = ptll;
        
     bool pass_2l = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
     pass_2l &= (std_vector_lepton_pt->at(0) > 25.);
     pass_2l &= (std_vector_lepton_pt->at(1) > 20.);
     pass_2l &= (std_vector_lepton_pt->at(2) < 10.);
     pass_2l &= (mll>20.);

    

    // Fill histograms. WW selection: CMS AN-15-325, pag.7 with pt1 > 25 (trigger)
    //--------------------------------------------------------------------------
    
    // No cuts
    // ---------------------------------------------------------------------------
           FillLevelHistograms(DY_00_noCuts, true);    

    
     //    Has 2 Leptons                                                                                                                           
    //---------------------------------------------------------------------------     
    FillLevelHistograms(DY_01_Has2Leptons, pass_2l);
    //    if (_saveminitree && pass_2l) minitree->Fill();

    
    // B Veto                                                                                                                                
    //---------------------------------------------------------------------------    
    pass_2l &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    pass_2l &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    pass_2l &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    pass_2l &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    pass_2l &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    pass_2l &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    pass_2l &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    pass_2l &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    pass_2l &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    pass_2l &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    FillLevelHistograms(DY_02_BVetoLoose, pass_2l);

    
    // // Z Window region
    // //---------------------------------------------------------------------------  
    // bool pass_Zwindow = fabs(mll - Z_MASS) < 15.;
    // FillLevelHistograms(DY_03_ZWindow, pass_2l && pass_Zwindow);


    // Z peak Veto
    //---------------------------------------------------------------------------    
    pass_2l &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    FillLevelHistograms(DY_03_ZVeto, pass_2l);



    //Ptll cut           
    //---------------------------------------------------------------------------                         
    pass_2l &= (ptll>30.);
    pass_2l &= (_channel == em || ptll > 45.);
    FillLevelHistograms(DY_04_Ptll, pass_2l);


    // mpMET cut
    //---------------------------------------------------------------------------    
    pass_2l &= (mpmet > 20.);
    FillLevelHistograms(DY_05_mpMet, pass_2l);


    // MET cut                                                                                                           
    //---------------------------------------------------------------------------                                                            
    pass_2l &= (metPfType1 > 20.);
    pass_2l &= (_channel == em || metPfType1 > 55.);
    FillLevelHistograms(DY_06_PfMet, pass_2l);
    

    // // Cortes adicionales en variables: dphillmet, MET/ptll                                                            
    // // ---------------------------------------------------------------------------                             
    // pass_2l &= (_dphillmet>2.14);
    // pass_2l &= (_channel == em || _dphillmet > 2.37);
    // FillLevelHistograms(DY_08_dphillmet, pass_2l);

    // pass_2l &= (MET.Et()/_pt2l < 1.8);
    // pass_2l &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // FillLevelHistograms(DY_09_metopt2l, pass_2l);


    // // // // Corte en Ht                                                            
    // // // //---------------------------------------------------------------------------                             
    // pass_2l &= (_ht<195.);
    // pass_2l &= (_channel == em ||  _ht < 184.);
    // FillLevelHistograms(DY_10_ht, pass_2l);


    // // //top-enriched control region (CMS AN-16-182, pag.57, adapted to WW selection)
    // // //---------------------------------------------------------------------------                        
    bool pass_tcontrol = (Lepton1.flavour * Lepton2.flavour < 0);
    pass_tcontrol &= (Lepton1.v.Pt() > 25.);
    pass_tcontrol &= (Lepton2.v.Pt() > 20.);
    pass_tcontrol &= (std_vector_lepton_pt->at(2) < 10.);
    pass_tcontrol &= (mll > 12.);
    pass_tcontrol &= (ptll > 30.);
    pass_tcontrol &= (_channel == em || ptll > 45.);
    pass_tcontrol &= (MET.Et() > 20.);
    pass_tcontrol &= (_channel == em || MET.Et() > 55.);
    pass_tcontrol &= (mpmet > 20.);
    pass_tcontrol &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass_tcontrol &= (_njet==0 ? _nbjet20cmvav2l > 0 : _nbjet30cmvav2l > 0);
    FillLevelHistograms(DY_07_TopControl, pass_tcontrol);    


    // // //DY-Control region
    // // //---------------------------------------------------------------------------

    bool pass_dycontrol = (Lepton1.flavour * Lepton2.flavour < 0);
    pass_dycontrol &= (Lepton1.v.Pt() > 25.);
    pass_dycontrol &= (Lepton2.v.Pt() > 20.);
    pass_dycontrol &= (std_vector_lepton_pt->at(2) < 10.);
    pass_dycontrol &= (mll > 12.);
    pass_dycontrol &= (ptll>30.);
    pass_dycontrol &= (mpmet > 20.);
    pass_dycontrol &= (metPfType1 > 20.);
    FillLevelHistograms(DY_08_DYControl, pass_dycontrol);        

    //SS Control
    //--------------------------------------------------------------------------------
    
    bool pass_sscontrol = (Lepton1.flavour * Lepton2.flavour > 0);
    pass_sscontrol &= (std_vector_lepton_pt->at(0) > 25.);
    pass_sscontrol &= (std_vector_lepton_pt->at(1) > 20.);
    pass_sscontrol &= (std_vector_lepton_pt->at(2) < 10.);
    pass_sscontrol &= (mll>20);
    pass_sscontrol &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    pass_sscontrol &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    pass_sscontrol &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    pass_sscontrol &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    pass_sscontrol &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    pass_sscontrol &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    pass_sscontrol &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    pass_sscontrol &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    pass_sscontrol &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    pass_sscontrol &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    pass_sscontrol &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass_sscontrol &= (ptll>30.);
    pass_sscontrol &= (_channel == em || ptll > 45.);
    pass_sscontrol &= (mpmet > 20.);
    pass_sscontrol &= (metPfType1 > 20.);
    pass_sscontrol &= (_channel == em || metPfType1 > 55.);
    FillLevelHistograms(DY_09_SSControl, pass_sscontrol);
    








    // // //Diff XS:
    
    // bool pass_mll1 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_mll1 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_mll1 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_mll1 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_mll1 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_mll1 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_mll1 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_mll1 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_mll1 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_mll1 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_mll1 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_mll1 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_mll1 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_mll1 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_mll1 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_mll1 &= (ptll>30.);
    // pass_mll1 &= (_channel == em || ptll > 45.);
    // pass_mll1 &= (mpmet > 20.);
    // pass_mll1 &= (MET.Et() > 20.);
    // pass_mll1 &= (_channel == em || MET.Et() > 55.);
    // pass_mll1 &= (_dphillmet>2.14);
    // pass_mll1 &= (_channel == em || _dphillmet > 2.37);
    // pass_mll1 &= (MET.Et()/_pt2l < 1.8);
    // pass_mll1 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_mll1 &= (_ht<195.);
    // pass_mll1 &= (_channel == em ||  _ht < 184.);
    // pass_mll1 &= (mll>20. && mll<40.);
    // FillLevelHistograms(DY_mll1, pass_mll1);        




    // bool pass_mll2 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_mll2 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_mll2 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_mll2 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_mll2 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_mll2 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_mll2 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_mll2 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_mll2 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_mll2 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_mll2 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_mll2 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_mll2 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_mll2 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_mll2 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_mll2 &= (ptll>30.);
    // pass_mll2 &= (_channel == em || ptll > 45.);
    // pass_mll2 &= (mpmet > 20.);
    // pass_mll2 &= (MET.Et() > 20.);
    // pass_mll2 &= (_channel == em || MET.Et() > 55.);
    // pass_mll2 &= (_dphillmet>2.14);
    // pass_mll2 &= (_channel == em || _dphillmet > 2.37);
    // pass_mll2 &= (MET.Et()/_pt2l < 1.8);
    // pass_mll2 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_mll2 &= (_ht<195.);
    // pass_mll2 &= (_channel == em ||  _ht < 184.);
    // pass_mll2 &= (mll>40. && mll<60.);
    // FillLevelHistograms(DY_mll2, pass_mll2);        
    

    
    // bool pass_mll3 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_mll3 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_mll3 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_mll3 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_mll3 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_mll3 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_mll3 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_mll3 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_mll3 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_mll3 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_mll3 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_mll3 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_mll3 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_mll3 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_mll3 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_mll3 &= (ptll>30.);
    // pass_mll3 &= (_channel == em || ptll > 45.);
    // pass_mll3 &= (mpmet > 20.);
    // pass_mll3 &= (MET.Et() > 20.);
    // pass_mll3 &= (_channel == em || MET.Et() > 55.);
    // pass_mll3 &= (_dphillmet>2.14);
    // pass_mll3 &= (_channel == em || _dphillmet > 2.37);
    // pass_mll3 &= (MET.Et()/_pt2l < 1.8);
    // pass_mll3 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_mll3 &= (_ht<195.);
    // pass_mll3 &= (_channel == em ||  _ht < 184.);
    // pass_mll3 &= (mll>60. && mll<80.);
    // FillLevelHistograms(DY_mll3, pass_mll3);        




    // bool pass_mll4 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_mll4 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_mll4 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_mll4 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_mll4 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_mll4 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_mll4 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_mll4 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_mll4 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_mll4 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_mll4 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_mll4 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_mll4 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_mll4 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_mll4 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_mll4 &= (ptll>30.);
    // pass_mll4 &= (_channel == em || ptll > 45.);
    // pass_mll4 &= (mpmet > 20.);
    // pass_mll4 &= (MET.Et() > 20.);
    // pass_mll4 &= (_channel == em || MET.Et() > 55.);
    // pass_mll4 &= (_dphillmet>2.14);
    // pass_mll4 &= (_channel == em || _dphillmet > 2.37);
    // pass_mll4 &= (MET.Et()/_pt2l < 1.8);
    // pass_mll4 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_mll4 &= (_ht<195.);
    // pass_mll4 &= (_channel == em ||  _ht < 184.);
    // pass_mll4 &= (mll>80. && mll<100.);
    // FillLevelHistograms(DY_mll4, pass_mll4);        





    // bool pass_mll5 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_mll5 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_mll5 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_mll5 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_mll5 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_mll5 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_mll5 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_mll5 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_mll5 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_mll5 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_mll5 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_mll5 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_mll5 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_mll5 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_mll5 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_mll5 &= (ptll>30.);
    // pass_mll5 &= (_channel == em || ptll > 45.);
    // pass_mll5 &= (mpmet > 20.);
    // pass_mll5 &= (MET.Et() > 20.);
    // pass_mll5 &= (_channel == em || MET.Et() > 55.);
    // pass_mll5 &= (_dphillmet>2.14);
    // pass_mll5 &= (_channel == em || _dphillmet > 2.37);
    // pass_mll5 &= (MET.Et()/_pt2l < 1.8);
    // pass_mll5 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_mll5 &= (_ht<195.);
    // pass_mll5 &= (_channel == em ||  _ht < 184.);
    // pass_mll5 &= (mll>100. && mll<125.);
    // FillLevelHistograms(DY_mll5, pass_mll5);        






    // bool pass_mll6 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_mll6 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_mll6 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_mll6 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_mll6 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_mll6 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_mll6 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_mll6 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_mll6 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_mll6 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_mll6 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_mll6 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_mll6 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_mll6 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_mll6 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_mll6 &= (ptll>30.);
    // pass_mll6 &= (_channel == em || ptll > 45.);
    // pass_mll6 &= (mpmet > 20.);
    // pass_mll6 &= (MET.Et() > 20.);
    // pass_mll6 &= (_channel == em || MET.Et() > 55.);
    // pass_mll6 &= (_dphillmet>2.14);
    // pass_mll6 &= (_channel == em || _dphillmet > 2.37);
    // pass_mll6 &= (MET.Et()/_pt2l < 1.8);
    // pass_mll6 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_mll6 &= (_ht<195.);
    // pass_mll6 &= (_channel == em ||  _ht < 184.);
    // pass_mll6 &= (mll>125. && mll<150.);
    // FillLevelHistograms(DY_mll6, pass_mll6);        







    // bool pass_mll7 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_mll7 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_mll7 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_mll7 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_mll7 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_mll7 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_mll7 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_mll7 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_mll7 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_mll7 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_mll7 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_mll7 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_mll7 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_mll7 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_mll7 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_mll7 &= (ptll>30.);
    // pass_mll7 &= (_channel == em || ptll > 45.);
    // pass_mll7 &= (mpmet > 20.);
    // pass_mll7 &= (MET.Et() > 20.);
    // pass_mll7 &= (_channel == em || MET.Et() > 55.);
    // pass_mll7 &= (_dphillmet>2.14);
    // pass_mll7 &= (_channel == em || _dphillmet > 2.37);
    // pass_mll7 &= (MET.Et()/_pt2l < 1.8);
    // pass_mll7 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_mll7 &= (_ht<195.);
    // pass_mll7 &= (_channel == em ||  _ht < 184.);
    // pass_mll7 &= (mll>150. && mll<175.);
    // FillLevelHistograms(DY_mll7, pass_mll7);        







    // bool pass_mll8 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_mll8 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_mll8 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_mll8 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_mll8 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_mll8 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_mll8 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_mll8 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_mll8 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_mll8 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_mll8 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_mll8 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_mll8 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_mll8 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_mll8 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_mll8 &= (ptll>30.);
    // pass_mll8 &= (_channel == em || ptll > 45.);
    // pass_mll8 &= (mpmet > 20.);
    // pass_mll8 &= (MET.Et() > 20.);
    // pass_mll8 &= (_channel == em || MET.Et() > 55.);
    // pass_mll8 &= (_dphillmet>2.14);
    // pass_mll8 &= (_channel == em || _dphillmet > 2.37);
    // pass_mll8 &= (MET.Et()/_pt2l < 1.8);
    // pass_mll8 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_mll8 &= (_ht<195.);
    // pass_mll8 &= (_channel == em ||  _ht < 184.);
    // pass_mll8 &= (mll>175. && mll<200.);
    // FillLevelHistograms(DY_mll8, pass_mll8);        
    



 

    // bool pass_ptll1 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_ptll1 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_ptll1 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_ptll1 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_ptll1 &= (mll > 20.);
    // pass_ptll1 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_ptll1 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_ptll1 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_ptll1 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_ptll1 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_ptll1 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_ptll1 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_ptll1 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_ptll1 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_ptll1 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_ptll1 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_ptll1 &= (mpmet > 20.);
    // pass_ptll1 &= (MET.Et() > 20.);
    // pass_ptll1 &= (_channel == em || MET.Et() > 55.);
    // pass_ptll1 &= (_dphillmet>2.14);
    // pass_ptll1 &= (_channel == em || _dphillmet > 2.37);
    // pass_ptll1 &= (MET.Et()/_pt2l < 1.8);
    // pass_ptll1 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_ptll1 &= (_ht<195.);
    // pass_ptll1 &= (_channel == em ||  _ht < 184.);
    // pass_ptll1 &= (ptll>30. && ptll<40.);
    // FillLevelHistograms(DY_ptll1, pass_ptll1);        








    // bool pass_ptll2 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_ptll2 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_ptll2 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_ptll2 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_ptll2 &= (mll > 20.);
    // pass_ptll2 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_ptll2 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_ptll2 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_ptll2 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_ptll2 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_ptll2 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_ptll2 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_ptll2 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_ptll2 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_ptll2 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_ptll2 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_ptll2 &= (mpmet > 20.);
    // pass_ptll2 &= (MET.Et() > 20.);
    // pass_ptll2 &= (_channel == em || MET.Et() > 55.);
    // pass_ptll2 &= (_dphillmet>2.14);
    // pass_ptll2 &= (_channel == em || _dphillmet > 2.37);
    // pass_ptll2 &= (MET.Et()/_pt2l < 1.8);
    // pass_ptll2 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_ptll2 &= (_ht<195.);
    // pass_ptll2 &= (_channel == em ||  _ht < 184.);
    // pass_ptll2 &= (ptll>40. && ptll<50.);
    // FillLevelHistograms(DY_ptll2, pass_ptll2);        







    // bool pass_ptll3 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_ptll3 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_ptll3 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_ptll3 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_ptll3 &= (mll > 20.);
    // pass_ptll3 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_ptll3 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_ptll3 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_ptll3 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_ptll3 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_ptll3 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_ptll3 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_ptll3 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_ptll3 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_ptll3 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_ptll3 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_ptll3 &= (mpmet > 20.);
    // pass_ptll3 &= (MET.Et() > 20.);
    // pass_ptll3 &= (_channel == em || MET.Et() > 55.);
    // pass_ptll3 &= (_dphillmet>2.14);
    // pass_ptll3 &= (_channel == em || _dphillmet > 2.37);
    // pass_ptll3 &= (MET.Et()/_pt2l < 1.8);
    // pass_ptll3 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_ptll3 &= (_ht<195.);
    // pass_ptll3 &= (_channel == em ||  _ht < 184.);
    // pass_ptll3 &= (ptll>50. && ptll<60.);
    // FillLevelHistograms(DY_ptll3, pass_ptll3);        
    




    // bool pass_ptll4 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_ptll4 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_ptll4 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_ptll4 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_ptll4 &= (mll > 20.);
    // pass_ptll4 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_ptll4 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_ptll4 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_ptll4 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_ptll4 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_ptll4 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_ptll4 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_ptll4 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_ptll4 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_ptll4 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_ptll4 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_ptll4 &= (mpmet > 20.);
    // pass_ptll4 &= (MET.Et() > 20.);
    // pass_ptll4 &= (_channel == em || MET.Et() > 55.);
    // pass_ptll4 &= (_dphillmet>2.14);
    // pass_ptll4 &= (_channel == em || _dphillmet > 2.37);
    // pass_ptll4 &= (MET.Et()/_pt2l < 1.8);
    // pass_ptll4 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_ptll4 &= (_ht<195.);
    // pass_ptll4 &= (_channel == em ||  _ht < 184.);
    // pass_ptll4 &= (ptll>60. && ptll<70.);
    // FillLevelHistograms(DY_ptll4, pass_ptll4);        







    // bool pass_ptll5 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_ptll5 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_ptll5 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_ptll5 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_ptll5 &= (mll > 20.);
    // pass_ptll5 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_ptll5 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_ptll5 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_ptll5 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_ptll5 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_ptll5 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_ptll5 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_ptll5 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_ptll5 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_ptll5 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_ptll5 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_ptll5 &= (mpmet > 20.);
    // pass_ptll5 &= (MET.Et() > 20.);
    // pass_ptll5 &= (_channel == em || MET.Et() > 55.);
    // pass_ptll5 &= (_dphillmet>2.14);
    // pass_ptll5 &= (_channel == em || _dphillmet > 2.37);
    // pass_ptll5 &= (MET.Et()/_pt2l < 1.8);
    // pass_ptll5 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_ptll5 &= (_ht<195.);
    // pass_ptll5 &= (_channel == em ||  _ht < 184.);
    // pass_ptll5 &= (ptll>70. && ptll<85.);
    // FillLevelHistograms(DY_ptll5, pass_ptll5);        







    // bool pass_ptll6 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_ptll6 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_ptll6 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_ptll6 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_ptll6 &= (mll > 20.);
    // pass_ptll6 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_ptll6 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_ptll6 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_ptll6 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_ptll6 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_ptll6 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_ptll6 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_ptll6 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_ptll6 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_ptll6 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_ptll6 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_ptll6 &= (mpmet > 20.);
    // pass_ptll6 &= (MET.Et() > 20.);
    // pass_ptll6 &= (_channel == em || MET.Et() > 55.);
    // pass_ptll6 &= (_dphillmet>2.14);
    // pass_ptll6 &= (_channel == em || _dphillmet > 2.37);
    // pass_ptll6 &= (MET.Et()/_pt2l < 1.8);
    // pass_ptll6 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_ptll6 &= (_ht<195.);
    // pass_ptll6 &= (_channel == em ||  _ht < 184.);
    // pass_ptll6 &= (ptll>85. && ptll<120.);
    // FillLevelHistograms(DY_ptll6, pass_ptll6);        
    




    // bool pass_ptll7 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_ptll7 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_ptll7 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_ptll7 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_ptll7 &= (mll > 20.);
    // pass_ptll7 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_ptll7 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_ptll7 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_ptll7 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_ptll7 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_ptll7 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_ptll7 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_ptll7 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_ptll7 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_ptll7 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_ptll7 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_ptll7 &= (mpmet > 20.);
    // pass_ptll7 &= (MET.Et() > 20.);
    // pass_ptll7 &= (_channel == em || MET.Et() > 55.);
    // pass_ptll7 &= (_dphillmet>2.14);
    // pass_ptll7 &= (_channel == em || _dphillmet > 2.37);
    // pass_ptll7 &= (MET.Et()/_pt2l < 1.8);
    // pass_ptll7 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_ptll7 &= (_ht<195.);
    // pass_ptll7 &= (_channel == em ||  _ht < 184.);
    // pass_ptll7 &= (ptll>120. && ptll<150.);
    // FillLevelHistograms(DY_ptll7, pass_ptll7);        
    
    







    // bool pass_ptl11 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_ptl11 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_ptl11 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_ptl11 &= (mll>20.);
    // pass_ptl11 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_ptl11 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_ptl11 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_ptl11 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_ptl11 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_ptl11 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_ptl11 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_ptl11 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_ptl11 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_ptl11 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_ptl11 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_ptl11 &= (ptll>30.);
    // pass_ptl11 &= (_channel == em || ptll > 45.);
    // pass_ptl11 &= (mpmet > 20.);
    // pass_ptl11 &= (MET.Et() > 20.);
    // pass_ptl11 &= (_channel == em || MET.Et() > 55.);
    // pass_ptl11 &= (_dphillmet>2.14);
    // pass_ptl11 &= (_channel == em || _dphillmet > 2.37);
    // pass_ptl11 &= (MET.Et()/_pt2l < 1.8);
    // pass_ptl11 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_ptl11 &= (_ht<195.);
    // pass_ptl11 &= (_channel == em ||  _ht < 184.);
    // pass_ptl11 &= (std_vector_lepton_pt->at(0) > 20. && std_vector_lepton_pt->at(0) < 40.);
    // FillLevelHistograms(DY_ptl11, pass_ptl11);        





    // bool pass_ptl12 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_ptl12 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_ptl12 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_ptl12 &= (mll>20.);
    // pass_ptl12 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_ptl12 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_ptl12 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_ptl12 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_ptl12 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_ptl12 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_ptl12 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_ptl12 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_ptl12 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_ptl12 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_ptl12 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_ptl12 &= (ptll>30.);
    // pass_ptl12 &= (_channel == em || ptll > 45.);
    // pass_ptl12 &= (mpmet > 20.);
    // pass_ptl12 &= (MET.Et() > 20.);
    // pass_ptl12 &= (_channel == em || MET.Et() > 55.);
    // pass_ptl12 &= (_dphillmet>2.14);
    // pass_ptl12 &= (_channel == em || _dphillmet > 2.37);
    // pass_ptl12 &= (MET.Et()/_pt2l < 1.8);
    // pass_ptl12 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_ptl12 &= (_ht<195.);
    // pass_ptl12 &= (_channel == em ||  _ht < 184.);
    // pass_ptl12 &= (std_vector_lepton_pt->at(0) > 40. && std_vector_lepton_pt->at(0) < 60.);
    // FillLevelHistograms(DY_ptl12, pass_ptl12);        





    // bool pass_ptl13 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_ptl13 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_ptl13 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_ptl13 &= (mll>20.);
    // pass_ptl13 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_ptl13 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_ptl13 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_ptl13 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_ptl13 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_ptl13 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_ptl13 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_ptl13 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_ptl13 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_ptl13 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_ptl13 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_ptl13 &= (ptll>30.);
    // pass_ptl13 &= (_channel == em || ptll > 45.);
    // pass_ptl13 &= (mpmet > 20.);
    // pass_ptl13 &= (MET.Et() > 20.);
    // pass_ptl13 &= (_channel == em || MET.Et() > 55.);
    // pass_ptl13 &= (_dphillmet>2.14);
    // pass_ptl13 &= (_channel == em || _dphillmet > 2.37);
    // pass_ptl13 &= (MET.Et()/_pt2l < 1.8);
    // pass_ptl13 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_ptl13 &= (_ht<195.);
    // pass_ptl13 &= (_channel == em ||  _ht < 184.);
    // pass_ptl13 &= (std_vector_lepton_pt->at(0) > 60. && std_vector_lepton_pt->at(0) < 80.);
    // FillLevelHistograms(DY_ptl13, pass_ptl13);







    // bool pass_ptl14 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_ptl14 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_ptl14 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_ptl14 &= (mll>20.);
    // pass_ptl14 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_ptl14 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_ptl14 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_ptl14 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_ptl14 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_ptl14 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_ptl14 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_ptl14 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_ptl14 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_ptl14 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_ptl14 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_ptl14 &= (ptll>30.);
    // pass_ptl14 &= (_channel == em || ptll > 45.);
    // pass_ptl14 &= (mpmet > 20.);
    // pass_ptl14 &= (MET.Et() > 20.);
    // pass_ptl14 &= (_channel == em || MET.Et() > 55.);
    // pass_ptl14 &= (_dphillmet>2.14);
    // pass_ptl14 &= (_channel == em || _dphillmet > 2.37);
    // pass_ptl14 &= (MET.Et()/_pt2l < 1.8);
    // pass_ptl14 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_ptl14 &= (_ht<195.);
    // pass_ptl14 &= (_channel == em ||  _ht < 184.);
    // pass_ptl14 &= (std_vector_lepton_pt->at(0) > 80. && std_vector_lepton_pt->at(0) < 100.);
    // FillLevelHistograms(DY_ptl14, pass_ptl14);        







    // bool pass_ptl15 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_ptl15 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_ptl15 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_ptl15 &= (mll>20.);
    // pass_ptl15 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_ptl15 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_ptl15 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_ptl15 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_ptl15 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_ptl15 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_ptl15 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_ptl15 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_ptl15 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_ptl15 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_ptl15 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_ptl15 &= (ptll>30.);
    // pass_ptl15 &= (_channel == em || ptll > 45.);
    // pass_ptl15 &= (mpmet > 20.);
    // pass_ptl15 &= (MET.Et() > 20.);
    // pass_ptl15 &= (_channel == em || MET.Et() > 55.);
    // pass_ptl15 &= (_dphillmet>2.14);
    // pass_ptl15 &= (_channel == em || _dphillmet > 2.37);
    // pass_ptl15 &= (MET.Et()/_pt2l < 1.8);
    // pass_ptl15 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_ptl15 &= (_ht<195.);
    // pass_ptl15 &= (_channel == em ||  _ht < 184.);
    // pass_ptl15 &= (std_vector_lepton_pt->at(0) > 100. && std_vector_lepton_pt->at(0) < 125.);
    // FillLevelHistograms(DY_ptl15, pass_ptl15);       






    // bool pass_ptl16 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_ptl16 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_ptl16 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_ptl16 &= (mll>20.);
    // pass_ptl16 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_ptl16 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_ptl16 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_ptl16 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_ptl16 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_ptl16 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_ptl16 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_ptl16 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_ptl16 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_ptl16 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_ptl16 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_ptl16 &= (ptll>30.);
    // pass_ptl16 &= (_channel == em || ptll > 45.);
    // pass_ptl16 &= (mpmet > 20.);
    // pass_ptl16 &= (MET.Et() > 20.);
    // pass_ptl16 &= (_channel == em || MET.Et() > 55.);
    // pass_ptl16 &= (_dphillmet>2.14);
    // pass_ptl16 &= (_channel == em || _dphillmet > 2.37);
    // pass_ptl16 &= (MET.Et()/_pt2l < 1.8);
    // pass_ptl16 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_ptl16 &= (_ht<195.);
    // pass_ptl16 &= (_channel == em ||  _ht < 184.);
    // pass_ptl16 &= (std_vector_lepton_pt->at(0) > 125. && std_vector_lepton_pt->at(0) < 150.);
    // FillLevelHistograms(DY_ptl16, pass_ptl16);        



    // bool pass_ptl17 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_ptl17 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_ptl17 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_ptl17 &= (mll>20.);
    // pass_ptl17 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_ptl17 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_ptl17 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_ptl17 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_ptl17 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_ptl17 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_ptl17 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_ptl17 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_ptl17 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_ptl17 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_ptl17 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_ptl17 &= (ptll>30.);
    // pass_ptl17 &= (_channel == em || ptll > 45.);
    // pass_ptl17 &= (mpmet > 20.);
    // pass_ptl17 &= (MET.Et() > 20.);
    // pass_ptl17 &= (_channel == em || MET.Et() > 55.);
    // pass_ptl17 &= (_dphillmet>2.14);
    // pass_ptl17 &= (_channel == em || _dphillmet > 2.37);
    // pass_ptl17 &= (MET.Et()/_pt2l < 1.8);
    // pass_ptl17 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_ptl17 &= (_ht<195.);
    // pass_ptl17 &= (_channel == em ||  _ht < 184.);
    // pass_ptl17 &= (std_vector_lepton_pt->at(0) > 150. && std_vector_lepton_pt->at(0) < 200.);
    // FillLevelHistograms(DY_ptl17, pass_ptl17);        
    



    // bool pass_eta11 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_eta11 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_eta11 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_eta11 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_eta11 &= (mll>20.);
    // pass_eta11 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_eta11 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_eta11 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_eta11 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_eta11 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_eta11 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_eta11 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_eta11 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_eta11 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_eta11 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_eta11 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_eta11 &= (ptll>30.);
    // pass_eta11 &= (_channel == em || ptll > 45.);
    // pass_eta11 &= (mpmet > 20.);
    // pass_eta11 &= (MET.Et() > 20.);
    // pass_eta11 &= (_channel == em || MET.Et() > 55.);
    // pass_eta11 &= (_dphillmet>2.14);
    // pass_eta11 &= (_channel == em || _dphillmet > 2.37);
    // pass_eta11 &= (MET.Et()/_pt2l < 1.8);
    // pass_eta11 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_eta11 &= (_ht<195.);
    // pass_eta11 &= (_channel == em ||  _ht < 184.);
    // pass_eta11 &= (std_vector_lepton_eta->at(0)>-2.4 && std_vector_lepton_eta->at(0)<-1.8);
    // FillLevelHistograms(DY_eta11, pass_eta11);        



    // bool pass_eta12 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_eta12 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_eta12 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_eta12 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_eta12 &= (mll>20.);
    // pass_eta12 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_eta12 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_eta12 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_eta12 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_eta12 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_eta12 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_eta12 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_eta12 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_eta12 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_eta12 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_eta12 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_eta12 &= (ptll>30.);
    // pass_eta12 &= (_channel == em || ptll > 45.);
    // pass_eta12 &= (mpmet > 20.);
    // pass_eta12 &= (MET.Et() > 20.);
    // pass_eta12 &= (_channel == em || MET.Et() > 55.);
    // pass_eta12 &= (_dphillmet>2.14);
    // pass_eta12 &= (_channel == em || _dphillmet > 2.37);
    // pass_eta12 &= (MET.Et()/_pt2l < 1.8);
    // pass_eta12 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_eta12 &= (_ht<195.);
    // pass_eta12 &= (_channel == em ||  _ht < 184.);
    // pass_eta12 &= (std_vector_lepton_eta->at(0)>-1.8 && std_vector_lepton_eta->at(0)<-1.2);
    // FillLevelHistograms(DY_eta12, pass_eta12);        



    // bool pass_eta13 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_eta13 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_eta13 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_eta13 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_eta13 &= (mll>20.);
    // pass_eta13 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_eta13 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_eta13 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_eta13 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_eta13 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_eta13 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_eta13 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_eta13 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_eta13 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_eta13 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_eta13 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_eta13 &= (ptll>30.);
    // pass_eta13 &= (_channel == em || ptll > 45.);
    // pass_eta13 &= (mpmet > 20.);
    // pass_eta13 &= (MET.Et() > 20.);
    // pass_eta13 &= (_channel == em || MET.Et() > 55.);
    // pass_eta13 &= (_dphillmet>2.14);
    // pass_eta13 &= (_channel == em || _dphillmet > 2.37);
    // pass_eta13 &= (MET.Et()/_pt2l < 1.8);
    // pass_eta13 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_eta13 &= (_ht<195.);
    // pass_eta13 &= (_channel == em ||  _ht < 184.);
    // pass_eta13 &= (std_vector_lepton_eta->at(0)>-1.2 && std_vector_lepton_eta->at(0)<-0.6);
    // FillLevelHistograms(DY_eta13, pass_eta13);        



    // bool pass_eta14 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_eta14 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_eta14 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_eta14 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_eta14 &= (mll>20.);
    // pass_eta14 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_eta14 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_eta14 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_eta14 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_eta14 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_eta14 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_eta14 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_eta14 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_eta14 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_eta14 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_eta14 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_eta14 &= (ptll>30.);
    // pass_eta14 &= (_channel == em || ptll > 45.);
    // pass_eta14 &= (mpmet > 20.);
    // pass_eta14 &= (MET.Et() > 20.);
    // pass_eta14 &= (_channel == em || MET.Et() > 55.);
    // pass_eta14 &= (_dphillmet>2.14);
    // pass_eta14 &= (_channel == em || _dphillmet > 2.37);
    // pass_eta14 &= (MET.Et()/_pt2l < 1.8);
    // pass_eta14 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_eta14 &= (_ht<195.);
    // pass_eta14 &= (_channel == em ||  _ht < 184.);
    // pass_eta14 &= (std_vector_lepton_eta->at(0)>-0.6 && std_vector_lepton_eta->at(0)<0.);
    // FillLevelHistograms(DY_eta14, pass_eta14);        





    // bool pass_eta15 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_eta15 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_eta15 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_eta15 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_eta15 &= (mll>20.);
    // pass_eta15 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_eta15 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_eta15 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_eta15 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_eta15 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_eta15 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_eta15 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_eta15 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_eta15 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_eta15 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_eta15 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_eta15 &= (ptll>30.);
    // pass_eta15 &= (_channel == em || ptll > 45.);
    // pass_eta15 &= (mpmet > 20.);
    // pass_eta15 &= (MET.Et() > 20.);
    // pass_eta15 &= (_channel == em || MET.Et() > 55.);
    // pass_eta15 &= (_dphillmet>2.14);
    // pass_eta15 &= (_channel == em || _dphillmet > 2.37);
    // pass_eta15 &= (MET.Et()/_pt2l < 1.8);
    // pass_eta15 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_eta15 &= (_ht<195.);
    // pass_eta15 &= (_channel == em ||  _ht < 184.);
    // pass_eta15 &= (std_vector_lepton_eta->at(0)>0. && std_vector_lepton_eta->at(0)<0.6);
    // FillLevelHistograms(DY_eta15, pass_eta15);        




    // bool pass_eta16 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_eta16 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_eta16 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_eta16 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_eta16 &= (mll>20.);
    // pass_eta16 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_eta16 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_eta16 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_eta16 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_eta16 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_eta16 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_eta16 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_eta16 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_eta16 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_eta16 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_eta16 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_eta16 &= (ptll>30.);
    // pass_eta16 &= (_channel == em || ptll > 45.);
    // pass_eta16 &= (mpmet > 20.);
    // pass_eta16 &= (MET.Et() > 20.);
    // pass_eta16 &= (_channel == em || MET.Et() > 55.);
    // pass_eta16 &= (_dphillmet>2.14);
    // pass_eta16 &= (_channel == em || _dphillmet > 2.37);
    // pass_eta16 &= (MET.Et()/_pt2l < 1.8);
    // pass_eta16 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_eta16 &= (_ht<195.);
    // pass_eta16 &= (_channel == em ||  _ht < 184.);
    // pass_eta16 &= (std_vector_lepton_eta->at(0)>0.6 && std_vector_lepton_eta->at(0)<1.2);
    // FillLevelHistograms(DY_eta16, pass_eta16);        



    // bool pass_eta17 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_eta17 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_eta17 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_eta17 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_eta17 &= (mll>20.);
    // pass_eta17 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_eta17 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_eta17 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_eta17 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_eta17 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_eta17 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_eta17 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_eta17 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_eta17 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_eta17 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_eta17 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_eta17 &= (ptll>30.);
    // pass_eta17 &= (_channel == em || ptll > 45.);
    // pass_eta17 &= (mpmet > 20.);
    // pass_eta17 &= (MET.Et() > 20.);
    // pass_eta17 &= (_channel == em || MET.Et() > 55.);
    // pass_eta17 &= (_dphillmet>2.14);
    // pass_eta17 &= (_channel == em || _dphillmet > 2.37);
    // pass_eta17 &= (MET.Et()/_pt2l < 1.8);
    // pass_eta17 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_eta17 &= (_ht<195.);
    // pass_eta17 &= (_channel == em ||  _ht < 184.);
    // pass_eta17 &= (std_vector_lepton_eta->at(0)>1.2 && std_vector_lepton_eta->at(0)<1.8);
    // FillLevelHistograms(DY_eta17, pass_eta17);        





    // bool pass_eta18 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_eta18 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_eta18 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_eta18 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_eta18 &= (mll>20.);
    // pass_eta18 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_eta18 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_eta18 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_eta18 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_eta18 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_eta18 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_eta18 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_eta18 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_eta18 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_eta18 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_eta18 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_eta18 &= (ptll>30.);
    // pass_eta18 &= (_channel == em || ptll > 45.);
    // pass_eta18 &= (mpmet > 20.);
    // pass_eta18 &= (MET.Et() > 20.);
    // pass_eta18 &= (_channel == em || MET.Et() > 55.);
    // pass_eta18 &= (_dphillmet>2.14);
    // pass_eta18 &= (_channel == em || _dphillmet > 2.37);
    // pass_eta18 &= (MET.Et()/_pt2l < 1.8);
    // pass_eta18 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_eta18 &= (_ht<195.);
    // pass_eta18 &= (_channel == em ||  _ht < 184.);
    // pass_eta18 &= (std_vector_lepton_eta->at(0)>1.8 && std_vector_lepton_eta->at(0)<2.4);
    // FillLevelHistograms(DY_eta18, pass_eta18);        





    // bool pass_jetpt1 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_jetpt1 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_jetpt1 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_jetpt1 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_jetpt1 &= (mll>20.);
    // pass_jetpt1 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_jetpt1 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_jetpt1 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_jetpt1 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_jetpt1 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_jetpt1 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_jetpt1 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_jetpt1 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_jetpt1 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_jetpt1 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_jetpt1 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_jetpt1 &= (ptll>30.);
    // pass_jetpt1 &= (_channel == em || ptll > 45.);
    // pass_jetpt1 &= (mpmet > 20.);
    // pass_jetpt1 &= (MET.Et() > 20.);
    // pass_jetpt1 &= (_channel == em || MET.Et() > 55.);
    // pass_jetpt1 &= (_dphillmet>2.14);
    // pass_jetpt1 &= (_channel == em || _dphillmet > 2.37);
    // pass_jetpt1 &= (MET.Et()/_pt2l < 1.8);
    // pass_jetpt1 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_jetpt1 &= (_ht<195.);
    // pass_jetpt1 &= (_channel == em ||  _ht < 184.);
    // pass_jetpt1 &= (std_vector_jet_pt->at(0)>20 && std_vector_jet_pt->at(0)<30);
    // FillLevelHistograms(DY_jetpt1, pass_jetpt1);        







    // bool pass_jetpt2 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_jetpt2 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_jetpt2 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_jetpt2 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_jetpt2 &= (mll>20.);
    // pass_jetpt2 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_jetpt2 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_jetpt2 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_jetpt2 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_jetpt2 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_jetpt2 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_jetpt2 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_jetpt2 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_jetpt2 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_jetpt2 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_jetpt2 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_jetpt2 &= (ptll>30.);
    // pass_jetpt2 &= (_channel == em || ptll > 45.);
    // pass_jetpt2 &= (mpmet > 20.);
    // pass_jetpt2 &= (MET.Et() > 20.);
    // pass_jetpt2 &= (_channel == em || MET.Et() > 55.);
    // pass_jetpt2 &= (_dphillmet>2.14);
    // pass_jetpt2 &= (_channel == em || _dphillmet > 2.37);
    // pass_jetpt2 &= (MET.Et()/_pt2l < 1.8);
    // pass_jetpt2 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_jetpt2 &= (_ht<195.);
    // pass_jetpt2 &= (_channel == em ||  _ht < 184.);
    // pass_jetpt2 &= (std_vector_jet_pt->at(0)>30 && std_vector_jet_pt->at(0)<40);
    // FillLevelHistograms(DY_jetpt2, pass_jetpt2);        





    // bool pass_jetpt3 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_jetpt3 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_jetpt3 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_jetpt3 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_jetpt3 &= (mll>20.);
    // pass_jetpt3 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_jetpt3 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_jetpt3 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_jetpt3 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_jetpt3 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_jetpt3 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_jetpt3 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_jetpt3 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_jetpt3 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_jetpt3 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_jetpt3 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_jetpt3 &= (ptll>30.);
    // pass_jetpt3 &= (_channel == em || ptll > 45.);
    // pass_jetpt3 &= (mpmet > 20.);
    // pass_jetpt3 &= (MET.Et() > 20.);
    // pass_jetpt3 &= (_channel == em || MET.Et() > 55.);
    // pass_jetpt3 &= (_dphillmet>2.14);
    // pass_jetpt3 &= (_channel == em || _dphillmet > 2.37);
    // pass_jetpt3 &= (MET.Et()/_pt2l < 1.8);
    // pass_jetpt3 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_jetpt3 &= (_ht<195.);
    // pass_jetpt3 &= (_channel == em ||  _ht < 184.);
    // pass_jetpt3 &= (std_vector_jet_pt->at(0)>40 && std_vector_jet_pt->at(0)<50);
    // FillLevelHistograms(DY_jetpt3, pass_jetpt3);        






    // bool pass_jetpt4 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_jetpt4 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_jetpt4 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_jetpt4 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_jetpt4 &= (mll>20.);
    // pass_jetpt4 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_jetpt4 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_jetpt4 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_jetpt4 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_jetpt4 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_jetpt4 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_jetpt4 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_jetpt4 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_jetpt4 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_jetpt4 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_jetpt4 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_jetpt4 &= (ptll>30.);
    // pass_jetpt4 &= (_channel == em || ptll > 45.);
    // pass_jetpt4 &= (mpmet > 20.);
    // pass_jetpt4 &= (MET.Et() > 20.);
    // pass_jetpt4 &= (_channel == em || MET.Et() > 55.);
    // pass_jetpt4 &= (_dphillmet>2.14);
    // pass_jetpt4 &= (_channel == em || _dphillmet > 2.37);
    // pass_jetpt4 &= (MET.Et()/_pt2l < 1.8);
    // pass_jetpt4 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_jetpt4 &= (_ht<195.);
    // pass_jetpt4 &= (_channel == em ||  _ht < 184.);
    // pass_jetpt4 &= (std_vector_jet_pt->at(0)>50 && std_vector_jet_pt->at(0)<60);
    // FillLevelHistograms(DY_jetpt4, pass_jetpt4);        






    // bool pass_jetpt5 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_jetpt5 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_jetpt5 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_jetpt5 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_jetpt5 &= (mll>20.);
    // pass_jetpt5 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_jetpt5 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_jetpt5 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_jetpt5 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_jetpt5 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_jetpt5 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_jetpt5 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_jetpt5 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_jetpt5 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_jetpt5 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_jetpt5 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_jetpt5 &= (ptll>30.);
    // pass_jetpt5 &= (_channel == em || ptll > 45.);
    // pass_jetpt5 &= (mpmet > 20.);
    // pass_jetpt5 &= (MET.Et() > 20.);
    // pass_jetpt5 &= (_channel == em || MET.Et() > 55.);
    // pass_jetpt5 &= (_dphillmet>2.14);
    // pass_jetpt5 &= (_channel == em || _dphillmet > 2.37);
    // pass_jetpt5 &= (MET.Et()/_pt2l < 1.8);
    // pass_jetpt5 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_jetpt5 &= (_ht<195.);
    // pass_jetpt5 &= (_channel == em ||  _ht < 184.);
    // pass_jetpt5 &= (std_vector_jet_pt->at(0)>60 && std_vector_jet_pt->at(0)<70);
    // FillLevelHistograms(DY_jetpt5, pass_jetpt5);        






    // bool pass_jetpt6 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_jetpt6 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_jetpt6 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_jetpt6 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_jetpt6 &= (mll>20.);
    // pass_jetpt6 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_jetpt6 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_jetpt6 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_jetpt6 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_jetpt6 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_jetpt6 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_jetpt6 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_jetpt6 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_jetpt6 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_jetpt6 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_jetpt6 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_jetpt6 &= (ptll>30.);
    // pass_jetpt6 &= (_channel == em || ptll > 45.);
    // pass_jetpt6 &= (mpmet > 20.);
    // pass_jetpt6 &= (MET.Et() > 20.);
    // pass_jetpt6 &= (_channel == em || MET.Et() > 55.);
    // pass_jetpt6 &= (_dphillmet>2.14);
    // pass_jetpt6 &= (_channel == em || _dphillmet > 2.37);
    // pass_jetpt6 &= (MET.Et()/_pt2l < 1.8);
    // pass_jetpt6 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_jetpt6 &= (_ht<195.);
    // pass_jetpt6 &= (_channel == em ||  _ht < 184.);
    // pass_jetpt6 &= (std_vector_jet_pt->at(0)>70 && std_vector_jet_pt->at(0)<85);
    // FillLevelHistograms(DY_jetpt6, pass_jetpt6);        





    // bool pass_jetpt7 = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_jetpt7 &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_jetpt7 &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_jetpt7 &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_jetpt7 &= (mll>20.);
    // pass_jetpt7 &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_jetpt7 &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_jetpt7 &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_jetpt7 &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_jetpt7 &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_jetpt7 &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_jetpt7 &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_jetpt7 &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_jetpt7 &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_jetpt7 &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_jetpt7 &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_jetpt7 &= (ptll>30.);
    // pass_jetpt7 &= (_channel == em || ptll > 45.);
    // pass_jetpt7 &= (mpmet > 20.);
    // pass_jetpt7 &= (MET.Et() > 20.);
    // pass_jetpt7 &= (_channel == em || MET.Et() > 55.);
    // pass_jetpt7 &= (_dphillmet>2.14);
    // pass_jetpt7 &= (_channel == em || _dphillmet > 2.37);
    // pass_jetpt7 &= (MET.Et()/_pt2l < 1.8);
    // pass_jetpt7 &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_jetpt7 &= (_ht<195.);
    // pass_jetpt7 &= (_channel == em ||  _ht < 184.);
    // pass_jetpt7 &= (std_vector_jet_pt->at(0)>85 && std_vector_jet_pt->at(0)<100);
    // FillLevelHistograms(DY_jetpt7, pass_jetpt7);        

    // bool pass_Incljet = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_Incljet &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_Incljet &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_Incljet &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_Incljet &= (mll>20.); 
    // pass_Incljet &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_Incljet &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_Incljet &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_Incljet &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_Incljet &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_Incljet &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_Incljet &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_Incljet &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_Incljet &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_Incljet &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_Incljet &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_Incljet &= (ptll>30.);
    // pass_Incljet &= (_channel == em || ptll > 45.);
    // pass_Incljet &= (mpmet > 20.);
    // pass_Incljet &= (MET.Et() > 20.);
    // pass_Incljet &= (_channel == em || MET.Et() > 55.);
    // pass_Incljet &= (_dphillmet>2.14);
    // pass_Incljet &= (_channel == em || _dphillmet > 2.37);
    // pass_Incljet &= (MET.Et()/_pt2l < 1.8);
    // pass_Incljet &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_Incljet &= (_ht<195.);
    // pass_Incljet &= (_channel == em ||  _ht < 184.);
    // pass_Incljet &= (njet==2);
    // FillLevelHistograms(DY_Incljet, pass_Incljet);        


    // bool pass_0jet = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_0jet &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_0jet &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_0jet &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_0jet &= (mll>20.); 
    // pass_0jet &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_0jet &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_0jet &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_0jet &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_0jet &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_0jet &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_0jet &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_0jet &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_0jet &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_0jet &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_0jet &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_0jet &= (ptll>30.);
    // pass_0jet &= (_channel == em || ptll > 45.);
    // pass_0jet &= (mpmet > 20.);
    // pass_0jet &= (MET.Et() > 20.);
    // pass_0jet &= (_channel == em || MET.Et() > 55.);
    // pass_0jet &= (_dphillmet>2.14);
    // pass_0jet &= (_channel == em || _dphillmet > 2.37);
    // pass_0jet &= (MET.Et()/_pt2l < 1.8);
    // pass_0jet &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_0jet &= (_ht<195.);
    // pass_0jet &= (_channel == em ||  _ht < 184.);
    // pass_0jet &= (njet==0);
    // FillLevelHistograms(DY_0jet, pass_0jet);        


    // bool pass_1jet = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_1jet &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_1jet &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_1jet &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_1jet &= (mll>20.); 
    // pass_1jet &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_1jet &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_1jet &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_1jet &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_1jet &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_1jet &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_1jet &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_1jet &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_1jet &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_1jet &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_1jet &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_1jet &= (ptll>30.);
    // pass_1jet &= (_channel == em || ptll > 45.);
    // pass_1jet &= (mpmet > 20.);
    // pass_1jet &= (MET.Et() > 20.);
    // pass_1jet &= (_channel == em || MET.Et() > 55.);
    // pass_1jet &= (_dphillmet>2.14);
    // pass_1jet &= (_channel == em || _dphillmet > 2.37);
    // pass_1jet &= (MET.Et()/_pt2l < 1.8);
    // pass_1jet &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_1jet &= (_ht<195.);
    // pass_1jet &= (_channel == em ||  _ht < 184.);
    // pass_1jet &= (njet==1);
    // FillLevelHistograms(DY_1jet, pass_1jet);        


    // bool pass_2jet = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_2jet &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_2jet &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_2jet &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_2jet &= (mll>20.); 
    // pass_2jet &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_2jet &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_2jet &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_2jet &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_2jet &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_2jet &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_2jet &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_2jet &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_2jet &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_2jet &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_2jet &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_2jet &= (ptll>30.);
    // pass_2jet &= (_channel == em || ptll > 45.);
    // pass_2jet &= (mpmet > 20.);
    // pass_2jet &= (MET.Et() > 20.);
    // pass_2jet &= (_channel == em || MET.Et() > 55.);
    // pass_2jet &= (_dphillmet>2.14);
    // pass_2jet &= (_channel == em || _dphillmet > 2.37);
    // pass_2jet &= (MET.Et()/_pt2l < 1.8);
    // pass_2jet &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_2jet &= (_ht<195.);
    // pass_2jet &= (_channel == em ||  _ht < 184.);
    // pass_2jet &= (njet==2);
    // FillLevelHistograms(DY_2jet, pass_2jet);        


    // bool pass_3jet = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_3jet &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_3jet &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_3jet &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_3jet &= (mll>20.); 
    // pass_3jet &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_3jet &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_3jet &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_3jet &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_3jet &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_3jet &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_3jet &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_3jet &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_3jet &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_3jet &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_3jet &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_3jet &= (ptll>30.);
    // pass_3jet &= (_channel == em || ptll > 45.);
    // pass_3jet &= (mpmet > 20.);
    // pass_3jet &= (MET.Et() > 20.);
    // pass_3jet &= (_channel == em || MET.Et() > 55.);
    // pass_3jet &= (_dphillmet>2.14);
    // pass_3jet &= (_channel == em || _dphillmet > 2.37);
    // pass_3jet &= (MET.Et()/_pt2l < 1.8);
    // pass_3jet &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_3jet &= (_ht<195.);
    // pass_3jet &= (_channel == em ||  _ht < 184.);
    // pass_3jet &= (njet==3);
    // FillLevelHistograms(DY_3jet, pass_3jet);        


    // bool pass_4jet = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_4jet &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_4jet &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_4jet &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_4jet &= (mll>20.); 
    // pass_4jet &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_4jet &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_4jet &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_4jet &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_4jet &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_4jet &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_4jet &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_4jet &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_4jet &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_4jet &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_4jet &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_4jet &= (ptll>30.);
    // pass_4jet &= (_channel == em || ptll > 45.);
    // pass_4jet &= (mpmet > 20.);
    // pass_4jet &= (MET.Et() > 20.);
    // pass_4jet &= (_channel == em || MET.Et() > 55.);
    // pass_4jet &= (_dphillmet>2.14);
    // pass_4jet &= (_channel == em || _dphillmet > 2.37);
    // pass_4jet &= (MET.Et()/_pt2l < 1.8);
    // pass_4jet &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_4jet &= (_ht<195.);
    // pass_4jet &= (_channel == em ||  _ht < 184.);
    // pass_4jet &= (njet==4);
    // FillLevelHistograms(DY_4jet, pass_4jet);        


    // bool pass_5jet = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    // pass_5jet &= (std_vector_lepton_pt->at(0) > 25.);
    // pass_5jet &= (std_vector_lepton_pt->at(1) > 20.);
    // pass_5jet &= (std_vector_lepton_pt->at(2) < 10.);
    // pass_5jet &= (mll>20.); 
    // pass_5jet &= ( std_vector_jet_pt->at(0) < 20. || std_vector_jet_csvv2ivf->at(0) < 0.5426 );
    // pass_5jet &= ( std_vector_jet_pt->at(1) < 20. || std_vector_jet_csvv2ivf->at(1) < 0.5426 );
    // pass_5jet &= ( std_vector_jet_pt->at(2) < 20. || std_vector_jet_csvv2ivf->at(2) < 0.5426 );
    // pass_5jet &= ( std_vector_jet_pt->at(3) < 20. || std_vector_jet_csvv2ivf->at(3) < 0.5426 );
    // pass_5jet &= ( std_vector_jet_pt->at(4) < 20. || std_vector_jet_csvv2ivf->at(4) < 0.5426 );
    // pass_5jet &= ( std_vector_jet_pt->at(5) < 20. || std_vector_jet_csvv2ivf->at(5) < 0.5426 );
    // pass_5jet &= ( std_vector_jet_pt->at(6) < 20. || std_vector_jet_csvv2ivf->at(6) < 0.5426 );
    // pass_5jet &= ( std_vector_jet_pt->at(7) < 20. || std_vector_jet_csvv2ivf->at(7) < 0.5426 );
    // pass_5jet &= ( std_vector_jet_pt->at(8) < 20. || std_vector_jet_csvv2ivf->at(8) < 0.5426 );
    // pass_5jet &= ( std_vector_jet_pt->at(9) < 20. || std_vector_jet_csvv2ivf->at(9) < 0.5426 );
    // pass_5jet &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    // pass_5jet &= (ptll>30.);
    // pass_5jet &= (_channel == em || ptll > 45.);
    // pass_5jet &= (mpmet > 20.);
    // pass_5jet &= (MET.Et() > 20.);
    // pass_5jet &= (_channel == em || MET.Et() > 55.);
    // pass_5jet &= (_dphillmet>2.14);
    // pass_5jet &= (_channel == em || _dphillmet > 2.37);
    // pass_5jet &= (MET.Et()/_pt2l < 1.8);
    // pass_5jet &= (_channel == em || (MET.Et()/_pt2l > 0.76 && MET.Et()/_pt2l<1.6));
    // pass_5jet &= (_ht<195.);
    // pass_5jet &= (_channel == em ||  _ht < 184.);
    // pass_5jet &= (njet==5);
    // FillLevelHistograms(DY_5jet, pass_5jet);        

    
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
