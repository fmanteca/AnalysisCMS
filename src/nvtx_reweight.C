#include "TCanvas.h"
#include "TTree.h"
#include "TH1D.h"
#include "TFile.h"
#include <iostream>
#include <fstream>



void nvtx_reweight()
{
  TH1::SetDefaultSumw2();
  
  double lumi = 35.867;

  // Opening files and filling input histograms                                                                                                                                      
  TFile* file_data = new TFile("../rootfiles/nominal/DY/01_Data.root");

  TH1D* hist_data_nvtx = (TH1D*)file_data->Get("DY/03_ZWindow/h_nvtx_mm");

  double entries_data = hist_data_nvtx->Integral(-1,-1);
  
  hist_data_nvtx -> Rebin(2);

  cout << "entries_data = " << entries_data << endl;

  TFile* file_MC = new TFile("../rootfiles/nominal/DY/07_ZJets.root");

  TH1D* hist_MC_nvtx = (TH1D*)file_MC->Get("DY/03_ZWindow/h_nvtx_mm");
  
  hist_MC_nvtx->Scale(lumi);

  double entries_MC = hist_MC_nvtx->Integral(-1,-1);
  
  hist_MC_nvtx -> Rebin(2);

  cout << "entries_MC = " << entries_MC << endl; 

  // Drawing the corrected data histogram                                                                                                                                           

  TCanvas* ratio = new TCanvas("ratio", "ratio");

  TH1D*   hist_ratio = new TH1D("hist_ratio", "", 25, 0, 50);

  hist_ratio->Divide(hist_data_nvtx, hist_MC_nvtx);

  hist_ratio->Scale(entries_data/entries_MC);
  hist_ratio->Draw();
  hist_ratio->Print();

  printf("\n");


  for (int i=1; i<=hist_ratio->GetNbinsX(); i++)
    {
            printf(" i=%2d    %5.2f < nvtx < %5.2f    weight = %f\n",                                                                                                            
               i,                                                                                                                                                                    
               hist_ratio->GetBinLowEdge(i),                                                                                                                                         
               hist_ratio->GetBinLowEdge(i) + hist_ratio->GetBinWidth(i),                                                                                                            
               hist_ratio->GetBinContent(i));
    }

  printf("\n");
}


