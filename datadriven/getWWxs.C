#include <iostream>
using namespace std;


// Constants                                                                                                                                                                           
//------------------------------------------------------------------------------                                                                                                       
enum {
  ee,
  mm,
  em,
  ll,
  nchannel  // This line should be always last                                                                                                                                         
};

const TString schannel[nchannel] = {
  "ee",
  "mm",
  "em",
  "ll"
};



// Data members                                                                                                                                                                        
//------------------------------------------------------------------------------                                                                                                       
TH1D*        h1_data        [ll];  // ee, mm, em 
TH1D*        h1_f           [ll];
TH1D*        h1_wz          [ll];  // ee, mm, em                                                                                                                                       
TH1D*        h1_zz          [ll];  // ee, mm, em                                                                                                                                       
TH1D*        h1_tt          [ll];
TH1D*        h1_st          [ll];
TH1D*        h1_ww          [ll];
TH1D*        h1_dy          [ll];
TH1D*        h1_wjets       [ll];
TH1D*        h1_ttv         [ll];
TH1D*        h1_hww         [ll];
TH1D*        h1_wg          [ll];
TH1D*        h1_zg          [ll];
TH1D*        h1_vvv         [ll];
TH1D*        h1_hz          [ll];
TH1D*        h1_wgs         [ll];
TH1D*  h1_tt_mc_signallevel [ll];
TH1D*        h1_datacontrol [ll];     
TH1D*        h1_wzcontrol   [ll]; 
TH1D*        h1_zzcontrol   [ll];
TH1D*        h1_dycontrol   [ll];
TH1D*        h1_dysignal    [ll];

TH1D*        h1_signal1      [ll];
TH1D*        h1_signal2      [ll];

TH1D*        h1s_data        [ll];                                                                                                                            
TH1D*        h1s_f           [ll];
TH1D*        h1s_wz          [ll];  
TH1D*        h1s_zz          [ll];
TH1D*        h1s_tt          [ll];
TH1D*        h1s_st          [ll];
TH1D*        h1s_ww          [ll];
TH1D*        h1s_dy          [ll];
TH1D*        h1s_wjets       [ll];
TH1D*        h1s_ttv         [ll];
TH1D*        h1s_hww         [ll];
TH1D*        h1s_wg          [ll];
TH1D*        h1s_zg          [ll];
TH1D*        h1s_vvv         [ll];
TH1D*        h1s_hz          [ll];
TH1D*        h1s_wgs         [ll];

double       data_yield        [ll];
double       data_yield_err    [ll];
double       mc_yield          [ll];
double       mc_yield_err      [ll];
double       SF                [ll];
double       SF_err            [ll];
double    tt_mc_signallevel    [ll];
double    tt_mc_signallevel_err[ll];
double       top_bkg           [ll];
double       top_bkg_err       [ll];
double        datacontrol      [ll];
double        datacontrol_err  [ll];
double        wzcontrol        [ll]; 
double        wzcontrol_err    [ll];
double        zzcontrol        [ll];
double        zzcontrol_err    [ll];
double         dycontrol       [ll];
double        dycontrol_err    [ll];
double        dysignal         [ll];
double        dysignal_err     [ll];

double        k             [2];
double        k_err         [2];
double        R             [2];
double        R_err         [2];
double        dy_bkg       [ll];
double        dy_bkg_err   [ll];


//entries at signal level
double        sdata        [ll];
double        sdata_err    [ll];
double        sf           [ll];
double        sf_err       [ll];
double        swz          [ll];
double        swz_err      [ll];
double        szz          [ll];
double        szz_err      [ll]; 
double        sst          [ll];
double        sst_err      [ll];
double        swjets       [ll];
double        swjets_err   [ll];
double        sttv         [ll];
double        sttv_err     [ll];
double        shww         [ll];
double        shww_err     [ll];
double        swg          [ll];
double        swg_err      [ll];
double        szg          [ll];
double        szg_err      [ll];
double        svvv         [ll];
double        svvv_err     [ll];
double        shz          [ll];
double        shz_err      [ll];
double        swgs         [ll];
double        swgs_err     [ll];


double        Nem              ;
double        Nem_err          ;
double        WWxs         [ll];
double        WWxs_stat    [ll];
double        num          [ll];
double        num_err      [ll];


double    signal1_fraction;
double    signal2_fraction;
double    signal1_counterRaw[ll];
double    signal2_counterRaw[ll];
double   signal1_efficiency [ll];
double   signal2_efficiency [ll];
double         eff          [ll];     


// Functions                                                                                                                                                                           
//------------------------------------------------------------------------------                                                                                                       

void getWWxs  (TString analysis = "DY", TString Top_control_level = "11_TopControl/1jet", TString WW_signal_level = "10_ZVeto/1jet", double lumi_fb = 12.9, double lumi_fb_err=0.1, TString DY_control_level = "02_DYControl/1jet", double signal1_xs=12.1780, double signal2_xs=0.5905, double signal1_ngen=1967180, double signal2_ngen=481600, double baseW=0.3189026, double BR=0.1086, double BR_err=0.0009) {


  //Cuidado: los MC estan estalados a lumi_fb, pero los fakes y los datos no                                                                                                          

  TFile* file_data = new TFile("../rootfiles/nominal/" + analysis + "/01_Data.root",     "read");
  TFile* file_f    = new TFile("../rootfiles/nominal/" + analysis + "/00_Fakes.root",     "read");
  TFile* file_wz   = new TFile("../rootfiles/nominal/" + analysis + "/02_WZTo3LNu.root", "read");
  TFile* file_zz   = new TFile("../rootfiles/nominal/" + analysis + "/03_VZ.root",       "read");
  TFile* file_tt   = new TFile("../rootfiles/nominal/" + analysis + "/04_TTTo2L2Nu.root","read");
  TFile* file_st   = new TFile("../rootfiles/nominal/" + analysis + "/05_ST.root",       "read");
  TFile* file_ww   = new TFile("../rootfiles/nominal/" + analysis + "/06_WW.root",       "read");
  TFile* file_dy   = new TFile("../rootfiles/nominal/" + analysis + "/07_ZJets.root",    "read");
  TFile* file_wjets= new TFile("../rootfiles/nominal/" + analysis + "/08_WJets.root",    "read");
  TFile* file_ttv  = new TFile("../rootfiles/nominal/" + analysis + "/09_TTV.root",      "read");
  TFile* file_hww  = new TFile("../rootfiles/nominal/" + analysis + "/10_HWW.root",      "read");
  TFile* file_wg   = new TFile("../rootfiles/nominal/" + analysis + "/11_Wg.root",       "read");
  TFile* file_zg   = new TFile("../rootfiles/nominal/" + analysis + "/12_Zg.root",       "read");
  TFile* file_vvv  = new TFile("../rootfiles/nominal/" + analysis + "/13_VVV.root",      "read");
  TFile* file_hz   = new TFile("../rootfiles/nominal/" + analysis + "/14_HZ.root",       "read");
  TFile* file_wgs  = new TFile("../rootfiles/nominal/" + analysis + "/15_WgStar.root",   "read");
  
  //Top BKG
  
  for (int i=ee; i<ll; i++)
    {
      //top
      h1_data[i] = (TH1D*)file_data->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_f   [i] = (TH1D*)file_f   ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_wz  [i] = (TH1D*)file_wz  ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_zz  [i] = (TH1D*)file_zz  ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_tt  [i] = (TH1D*)file_tt  ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_st  [i] = (TH1D*)file_st  ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_ww  [i] = (TH1D*)file_ww  ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_dy  [i] = (TH1D*)file_dy  ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_wjets[i]= (TH1D*)file_wjets->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_ttv [i] = (TH1D*)file_ttv ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_hww [i] = (TH1D*)file_hww ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_wg  [i] = (TH1D*)file_wg  ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_zg  [i] = (TH1D*)file_zg  ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_vvv [i] = (TH1D*)file_vvv ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_hz  [i] = (TH1D*)file_hz  ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_wgs [i] = (TH1D*)file_wgs ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);


      data_yield [i] = h1_data[i] -> GetBinContent(2);



      data_yield_err [i] = h1_data[i] ->   GetBinError(2);

      mc_yield [i] = h1_f[i] -> GetBinContent(2) + (h1_wz[i] -> GetBinContent(2) + h1_zz[i] -> GetBinContent(2) + h1_tt[i] -> GetBinContent(2)
			       + h1_st[i] -> GetBinContent(2) + h1_ww[i] -> GetBinContent(2) + h1_dy[i] -> GetBinContent(2) + h1_wjets[i] -> GetBinContent(2)
			       + h1_ttv[i] -> GetBinContent(2) + h1_hww[i] -> GetBinContent(2) + h1_wg[i] -> GetBinContent(2) + h1_zg[i] -> GetBinContent(2)
			       + h1_vvv[i] -> GetBinContent(2) + h1_hz[i] -> GetBinContent(2) + h1_wgs[i] -> GetBinContent(2))*lumi_fb;

      mc_yield_err [i] = sqrt((h1_f[i] -> GetBinError(2)) * (h1_f[i] -> GetBinError(2)) + ((h1_wz[i] -> GetBinError(2)) * (h1_wz[i] -> GetBinError(2))
			  + (h1_zz[i] -> GetBinError(2))*(h1_zz[i] -> GetBinError(2)) + (h1_tt[i] -> GetBinError(2))*(h1_tt[i] -> GetBinError(2))
			  + (h1_st[i] -> GetBinError(2))*(h1_st[i] -> GetBinError(2)) + (h1_ww[i] -> GetBinError(2))*(h1_ww[i] -> GetBinError(2))
	         	   + (h1_dy[i] -> GetBinError(2))*(h1_dy[i] -> GetBinError(2)) + (h1_wjets[i] -> GetBinError(2))*(h1_wjets[i] -> GetBinError(2))
			   + (h1_ttv[i] -> GetBinError(2))*(h1_ttv[i] -> GetBinError(2)) + (h1_hww[i] -> GetBinError(2))*(h1_hww[i] -> GetBinError(2))
			   + (h1_wg[i] -> GetBinError(2))*(h1_wg[i] -> GetBinError(2)) + (h1_zg[i] -> GetBinError(2))*(h1_zg[i] -> GetBinError(2))
			   + (h1_vvv[i] -> GetBinError(2))*(h1_vvv[i] -> GetBinError(2)) + (h1_hz[i] -> GetBinError(2))*(h1_hz[i] -> GetBinError(2))
			   + (h1_wgs[i] -> GetBinError(2))*(h1_wgs[i] -> GetBinError(2)))*lumi_fb*lumi_fb);



      SF [i] = data_yield[i] / mc_yield[i];
      SF_err [i] = sqrt((data_yield_err[i]/mc_yield[i])*data_yield_err[i]/mc_yield[i]) + (data_yield[i]/(mc_yield[i]*mc_yield[i])*mc_yield_err[i])
	*(data_yield[i]/(mc_yield[i]*mc_yield[i])*mc_yield_err[i]);


      // Top bkg stimation                                                                                                                                                                  


      h1_tt_mc_signallevel [i]  = (TH1D*)file_tt  ->Get(analysis + "/" + WW_signal_level + "/h_counterLum_" + schannel[i]);
      tt_mc_signallevel [i]    = (h1_tt_mc_signallevel[i] -> GetBinContent(2))*lumi_fb;
      tt_mc_signallevel_err [i] = (h1_tt_mc_signallevel[i] -> GetBinError(2))*lumi_fb;

      top_bkg [i]     = tt_mc_signallevel[i] * SF[i];
      top_bkg_err [i] = sqrt(SF[i]*SF[i]*tt_mc_signallevel_err[i]*tt_mc_signallevel_err[i] + tt_mc_signallevel[i]*tt_mc_signallevel[i]*SF_err[i]*SF_err[i]);

      cout << "Canal " << schannel[i] << ": Top bkg = " << top_bkg[i] << "+-" << top_bkg_err[i] << endl;


    }

    
  //DY BKG

  for (int i=ee; i<ll; i++)
    {
      h1_datacontrol[i] = (TH1D*)file_data->Get(analysis + "/" + DY_control_level + "/h_counterLum_" + schannel[i]);
      h1_wzcontrol  [i] = (TH1D*)file_wz  ->Get(analysis + "/" + DY_control_level + "/h_counterLum_" + schannel[i]);
      h1_zzcontrol  [i] = (TH1D*)file_zz  ->Get(analysis + "/" + DY_control_level + "/h_counterLum_" + schannel[i]);
      h1_dycontrol  [i] = (TH1D*)file_dy  ->Get(analysis + "/" + DY_control_level + "/h_counterLum_" + schannel[i]);
      h1_dysignal  [i] = (TH1D*)file_dy  ->Get(analysis + "/" + WW_signal_level + "/h_counterLum_" + schannel[i]);
      

      datacontrol[i]=h1_datacontrol[i]->GetBinContent(2);
      datacontrol_err[i]= h1_datacontrol[i]->GetBinError(2);
      wzcontrol[i]=(h1_wzcontrol[i]->GetBinContent(2))*lumi_fb;
      wzcontrol_err[i]=(h1_wzcontrol[i]->GetBinError(2))*lumi_fb;
      zzcontrol[i]=(h1_zzcontrol[i]->GetBinContent(2))*lumi_fb;
      zzcontrol_err[i]=(h1_zzcontrol[i]->GetBinError(2))*lumi_fb;
      dycontrol[i]=(h1_dycontrol[i]->GetBinContent(2))*lumi_fb;
      dycontrol_err[i]=(h1_dycontrol[i]->GetBinError(2))*lumi_fb;
      dysignal[i]=(h1_dysignal[i]->GetBinContent(2))*lumi_fb;
      dysignal_err[i]=(h1_dysignal[i]->GetBinError(2))*lumi_fb;

    }
  
  
  Nem=datacontrol[em];
  Nem_err=datacontrol_err[em];
  k[ee]=1/2*sqrt(datacontrol[mm]/datacontrol[ee]);
  k_err[ee]=1/2*sqrt((1/sqrt(datacontrol[mm]/datacontrol[ee])*1/datacontrol[ee]*datacontrol[ee])*(1/sqrt(datacontrol[mm]/datacontrol[ee])*1/datacontrol[ee]*datacontrol[ee])*datacontrol_err                          [ee]*datacontrol_err[ee]+1/sqrt(datacontrol[mm]/datacontrol[ee])*1/sqrt(datacontrol[mm]/datacontrol[ee])*datacontrol_err[mm]*datacontrol_err[mm]);
  k[mm]= 1/2*sqrt(datacontrol[ee]/datacontrol[mm]);
  k_err[ee]=1/2*sqrt((1/sqrt(datacontrol[ee]/datacontrol[mm])*1/datacontrol[mm]*datacontrol[mm])*(1/sqrt(datacontrol[ee]/datacontrol[mm])*1/datacontrol[mm]*datacontrol[mm])*datacontrol_err                           [mm]*datacontrol_err[mm]+1/sqrt(datacontrol[ee]/datacontrol[mm])*1/sqrt(datacontrol[ee]/datacontrol[mm])*datacontrol_err[ee]*datacontrol_err[ee]);

  
  for (int i=ee; i<em; i++)
    {

      R[i]=dysignal[i]/dycontrol[i];
      R_err[i]=sqrt((1/dycontrol[i])*(1/dycontrol[i])*dysignal_err[i]*dysignal_err[i]+(dysignal[i]/dycontrol[i]/dycontrol[i])*(dysignal[i]/dycontrol[i]/dycontrol[i])*dycontrol_err[i]*dycontrol_err[i]);
      dy_bkg[i] = (datacontrol[i]-k[i]*Nem-wzcontrol[i]-zzcontrol[i])*R[i];
      dy_bkg_err[i]=sqrt(R[i]*R[i]*datacontrol_err[i]*datacontrol_err[i]+R[i]*R[i]*Nem*Nem*k_err[i]*k_err[i]+R[i]*R[i]*k[i]*k[i]*Nem_err*Nem_err+R[i]*R[i]*wzcontrol_err[i]*wzcontrol_err[i]+R[i]*R[i]*zzcontrol_err[i]*zzcontrol_err[i]+(datacontrol[i]-k[i]*Nem-wzcontrol[i]-zzcontrol[i])*(datacontrol[i]-k[i]*Nem-wzcontrol[i]-zzcontrol[i])*R_err[i]*R_err[i]);

      cout << "Canal " << schannel[i] << ": DY bkg = " << dy_bkg[i] << "+-" << dy_bkg_err[i] << endl;

    }
  


  
  // WW Cross section stimation:

  dy_bkg[em]=dysignal[em];
  dy_bkg_err[em]=dysignal_err[em];
  cout<<"Canal em: DY bkg = " <<dy_bkg[em] << "+-" << dy_bkg_err[em] << endl;
  
  //Get the efficiency

  double signal1_fraction = signal1_xs / (signal1_xs + signal2_xs);
  double signal2_fraction = 1. - signal1_fraction;
  
  TFile* file_signal1   = new TFile("../rootfiles/nominal/" + analysis + "/WWTo2L2Nu.root",            "read");
  TFile* file_signal2   = new TFile("../rootfiles/nominal/" + analysis + "/GluGluWWTo2L2Nu_MCFM.root", "read");
  

  for(int i=ee; i<ll; i++){

    h1_signal1[i]=(TH1D*)file_signal1->Get(analysis + "/" + WW_signal_level + "/h_counterRaw_" + schannel[i]);
    h1_signal2[i]=(TH1D*)file_signal2->Get(analysis + "/" + WW_signal_level + "/h_counterRaw_" + schannel[i]);
    signal1_counterRaw[i]=h1_signal1[i]->GetBinContent(2);
    signal2_counterRaw[i]=h1_signal2[i]->GetBinContent(2);
    
    signal1_efficiency[i]=signal1_counterRaw[i] / signal1_ngen;
    signal2_efficiency[i]=signal2_counterRaw[i] / signal2_ngen;
    eff[i]=signal1_fraction*signal1_efficiency[i] + signal2_fraction*signal2_efficiency[i];
  }

  
  for(int i=ee; i<ll; i++){
    
    h1s_data[i]=(TH1D*)file_data->Get(analysis + "/" + WW_signal_level + "/h_counterLum_" + schannel[i]);
    h1s_f   [i] = (TH1D*)file_f   ->Get(analysis + "/" + WW_signal_level + "/h_counterLum_" + schannel[i]);
    h1s_wz  [i] = (TH1D*)file_wz  ->Get(analysis + "/" + WW_signal_level + "/h_counterLum_" + schannel[i]);
    h1s_zz  [i] = (TH1D*)file_zz  ->Get(analysis + "/" + WW_signal_level + "/h_counterLum_" + schannel[i]);
    h1s_tt  [i] = (TH1D*)file_tt  ->Get(analysis + "/" + WW_signal_level + "/h_counterLum_" + schannel[i]);
    h1s_st  [i] = (TH1D*)file_st  ->Get(analysis + "/" + WW_signal_level + "/h_counterLum_" + schannel[i]);
    h1s_ww  [i] = (TH1D*)file_ww  ->Get(analysis + "/" + WW_signal_level + "/h_counterLum_" + schannel[i]);
    h1s_dy  [i] = (TH1D*)file_dy  ->Get(analysis + "/" + WW_signal_level + "/h_counterLum_" + schannel[i]);
    h1s_wjets[i]= (TH1D*)file_wjets->Get(analysis + "/" + WW_signal_level + "/h_counterLum_" + schannel[i]);
    h1s_ttv [i] = (TH1D*)file_ttv ->Get(analysis + "/" + WW_signal_level + "/h_counterLum_" + schannel[i]);
    h1s_hww [i] = (TH1D*)file_hww ->Get(analysis + "/" + WW_signal_level + "/h_counterLum_" + schannel[i]);
    h1s_wg  [i] = (TH1D*)file_wg  ->Get(analysis + "/" + WW_signal_level + "/h_counterLum_" + schannel[i]);
    h1s_zg  [i] = (TH1D*)file_zg  ->Get(analysis + "/" + WW_signal_level + "/h_counterLum_" + schannel[i]);
    h1s_vvv [i] = (TH1D*)file_vvv ->Get(analysis + "/" + WW_signal_level + "/h_counterLum_" + schannel[i]);
    h1s_hz  [i] = (TH1D*)file_hz  ->Get(analysis + "/" + WW_signal_level + "/h_counterLum_" + schannel[i]);
    h1s_wgs [i] = (TH1D*)file_wgs ->Get(analysis + "/" + WW_signal_level + "/h_counterLum_" + schannel[i]);
    
  
  
    sdata[i]=h1s_data[i]->GetBinContent(2);
    sdata_err[i]=h1s_data[i]->GetBinError(2);
    sf[i]=h1s_f[i]->GetBinContent(2);
    sf_err[i]=h1s_f[i]->GetBinError(2);
    
    swz[i]=(h1s_wz[i]->GetBinContent(2))*lumi_fb;  
    swz_err[i]=(h1s_wz[i]->GetBinError(2))*lumi_fb;
    szz[i]=(h1s_zz[i]->GetBinContent(2))*lumi_fb;
    szz_err[i]=(h1s_zz[i]->GetBinError(2))*lumi_fb;
    sst[i]=(h1s_st[i]->GetBinContent(2))*lumi_fb;
    sst_err[i]=(h1s_st[i]->GetBinError(2))*lumi_fb;
    swjets[i]=(h1s_wjets[i]->GetBinContent(2))*lumi_fb;
    swjets_err[i]=(h1s_wjets[i]->GetBinError(2))*lumi_fb;
    sttv[i]=(h1s_ttv[i]->GetBinContent(2))*lumi_fb;
    sttv_err[i]=(h1s_ttv[i]->GetBinError(2))*lumi_fb;
    shww[i]=(h1s_hww[i]->GetBinContent(2))*lumi_fb;
    shww_err[i]=(h1s_hww[i]->GetBinError(2))*lumi_fb;
    swg[i]=(h1s_wg[i]->GetBinContent(2))*lumi_fb;
    swg_err[i]=(h1s_wg[i]->GetBinError(2))*lumi_fb;
    szg[i]=(h1s_zg[i]->GetBinContent(2))*lumi_fb;
    szg_err[i]=(h1s_zg[i]->GetBinError(2))*lumi_fb;
    svvv[i]=(h1s_vvv[i]->GetBinContent(2))*lumi_fb;
    svvv_err[i]=(h1s_vvv[i]->GetBinError(2))*lumi_fb;
    shz[i]=(h1s_hz[i]->GetBinContent(2))*lumi_fb;
    shz_err[i]=(h1s_hz[i]->GetBinError(2))*lumi_fb;
    swgs[i]=(h1s_wgs[i]->GetBinContent(2))*lumi_fb;
    swgs_err[i]=(h1s_wgs[i]->GetBinError(2))*lumi_fb;
  

    num[i]=(sdata[i]-dy_bkg[i]-top_bkg[i]-sf[i]-swz[i]-szz[i]-sst[i]-swjets[i]-sttv[i]-shww[i]-swg[i]-szg[i]-svvv[i]-shz[i]-swgs[i]);

    WWxs[i]=num[i]/(lumi_fb*eff[i]*3*3*BR*BR);
    
    num_err[i]=sqrt(sdata_err[i]*sdata_err[i]+dy_bkg_err[i]*dy_bkg_err[i]+top_bkg_err[i]*top_bkg_err[i]+sf_err[i]*sf_err[i]+swz_err[i]*swz_err[i]+szz_err[i]*szz_err[i]+sst_err[i]*sst_err[i]+swjets_err[i]*swjets_err[i]+sttv_err[i]*sttv_err[i]+shww_err[i]*shww_err[i]+swg_err[i]*swg_err[i]+szg_err[i]*szg_err[i]+svvv_err[i]*svvv_err[i]+shz_err[i]*shz_err[i]+swgs_err[i]*swgs_err[i]);
    
    WWxs_stat[i]=sqrt(sqrt(sdata[i])*sqrt(sdata[i])/sdata[i]/sdata[i])*WWxs[i];


  

  cout << "Canal " << schannel[i] << ": WW XS = " << WWxs[i]/1000 << "+-" << WWxs_stat[i]/1000 << " pb" << endl;


  }
  
}





