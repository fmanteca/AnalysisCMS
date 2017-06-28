//------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Usage: root -l -b -q diffxs.C
// Change the difxs histogram information in "Introduce the values: Bin values and its errors"  
// Extract the plots from lxplus to your computer: scp fernanpe@lxplus.cern.ch:/afs/cern.ch/work/f/fernanpe/CMSSW_8_0_5/src/AnalysisCMS/test/mll_xs.png Desktop/
//------------------------------------------------------------------------------------------------------------------------------------------------------------------


#include <iostream>
using namespace std;
#include "TLatex.h"


//Declaration of some style functions from HistogramReader.C

void     DrawLatex          (Font_t         tfont,
			     Float_t        x,
			     Float_t        y,
			     Float_t        tsize,
			     Short_t        align,
			     const char*    text,
			     Bool_t         setndc = true);


void     SetAxis            (TH1*           hist,
			     TString        xtitle,
			     TString        ytitle,
			     Float_t        xoffset,
			     Float_t        yoffset);





void diffxs()
{
  
  gInterpreter->ExecuteMacro("PaperStyle.C");

  // Introduce the values: Bin values and its errors

   const Int_t NBINS_mll = 8;
   Float_t edges_mll[NBINS_mll+1]={20,40,60,80,100,125,150,175,200};
   Float_t values_xs_mll[NBINS_mll]={13.40, 18.47, 17.69, 13.42, 12.63, 8.67, 5.34, 3.64};
   Float_t values_xs_mll_err[NBINS_mll]={0.96, 1.13, 1.05, 0.88, 0.85, 0.70, 0.57, 0.44};


   const Int_t NBINS_mllMC = 8;
   Float_t edges_mllMC[NBINS_mllMC+1]={20,40,60,80,100,125,150,175,200};
   Float_t values_xs_mllMC[NBINS_mllMC]={12.58, 16.67, 15.74, 12.45, 11.40, 7.91, 5.37, 3.80};
   Float_t values_xs_mllMC_err[NBINS_mllMC]={0.80, 0.86, 0.88, 0.74, 0.72, 0.60, 0.46, 0.45};

   const Int_t NBINS_ptll = 6;
   Float_t edges_ptll[NBINS_ptll+1]={30,40,50,60,70,85,120};
   Float_t values_xs_ptll[NBINS_ptll]={17.30, 16.19, 13.94, 10.62, 9.79, 7.77};
   Float_t values_xs_ptll_err[NBINS_ptll]={1.08, 0.93, 0.80, 0.66, 0.68, 1.09};


   const Int_t NBINS_ptllMC = 6;
   Float_t edges_ptllMC[NBINS_ptllMC+1]={30,40,50,60,70,85,120};
   Float_t values_xs_ptllMC[NBINS_ptllMC]={15.53, 14.81, 12.67, 9.84, 9.24, 8.25};
   Float_t values_xs_ptllMC_err[NBINS_ptllMC]={0.91, 0.80, 0.68, 0.57, 0.58, 0.82};

   const Int_t NBINS_ptl1 = 6;
   Float_t edges_ptl1[NBINS_ptl1+1]={20,40,60,80,100,125, 150};
   Float_t values_xs_ptl1[NBINS_ptl1]={10.78, 37.91, 18.06, 8.11, 4.50, 1.10};
   Float_t values_xs_ptl1_err[NBINS_ptl1]={39.53, 2.15, 0.86, 0.49, 0.51, 0.90};


   const Int_t NBINS_ptl1MC = 6;
   Float_t edges_ptl1MC[NBINS_ptl1MC+1]={20,40,60,80,100,125, 150};
   Float_t values_xs_ptl1MC[NBINS_ptl1MC]={32.45, 34.91, 16.50, 7.49, 4.39, 2.03};
   Float_t values_xs_ptl1MC_err[NBINS_ptl1MC]={27.95, 1.83, 0.75, 0.41, 0.41, 0.70};

   const Int_t NBINS_etal1 = 7;
   Float_t edges_etal1[NBINS_etal1+1]={-1.8,-1.2,-0.6,0.,0.6,1.2,1.8,2.4};
   Float_t values_xs_etal1[NBINS_etal1]={9.82, 11.93, 13.12, 13.16, 13.13, 11.41, 8.31};
   Float_t values_xs_etal1_err[NBINS_etal1]={0.86, 0.87, 0.86, 0.85, 0.84, 0.75, 0.59};


   const Int_t NBINS_etal1MC = 7;
   Float_t edges_etal1MC[NBINS_etal1MC+1]={-1.8,-1.2,-0.6,0.,0.6,1.2,1.8,2.4};
   Float_t values_xs_etal1MC[NBINS_etal1MC]={9.71, 11.10, 11.62, 12.01, 11.53, 10.25, 8.30};
   Float_t values_xs_etal1MC_err[NBINS_etal1MC]={0.72, 0.74, 0.71, 0.72, 0.69, 0.63, 0.56};

   const Int_t NBINS_jetpt = 7;
   Float_t edges_jetpt[NBINS_jetpt+1]={20,30,40,50,60,70,85,100};
   Float_t values_xs_jetpt[NBINS_jetpt]={11.43, 18.11, 18.93, 14.19, 11.05, 10.69, 11.83};
   Float_t values_xs_jetpt_err[NBINS_jetpt]={0.94, 1.03, 1.80, 2.47, 3.19, 4.27, 6.92};


   const Int_t NBINS_jetptMC = 7;
   Float_t edges_jetptMC[NBINS_jetptMC+1]={20,30,40,50,60,70,85,100};
   Float_t values_xs_jetptMC[NBINS_jetptMC]={11.39, 17.82, 17.95, 14.08, 9.75, 9.23, 4.64};
   Float_t values_xs_jetptMC_err[NBINS_jetptMC]={0.84, 0.91, 1.37, 1.61, 1.78, 2.40, 2.71};

   // const Int_t NBINS_dphill = 6;
   // Double_t edges_dphill[NBINS_dphill+1]={0., 0.5, 1.0, 1.5, 2.0, 2.5, 3.0};
   // Double_t values_xs_dphill[NBINS_dphill]={99.35, 108.04, 112.63, 112.55, 114.45, 109.76};
   // Double_t values_xs_dphill_err[NBINS_dphill]={7.67, 7.04, 6.94, 7.43, 7.08, 9.18};
  // Fill the histograms

  TH1F* mll = new TH1F("mll","",NBINS_mll, edges_mll);
  TH1F* mllMC = new TH1F("mllMC","",NBINS_mllMC, edges_mllMC);
  TH1F* ptll = new TH1F("ptll","", NBINS_ptll, edges_ptll);
  TH1F* ptllMC = new TH1F("ptllMC","", NBINS_ptllMC, edges_ptllMC);
  TH1F* ptl1 = new TH1F("ptl1","", NBINS_ptl1, edges_ptl1);
  TH1F* ptl1MC = new TH1F("ptl1MC","", NBINS_ptl1MC, edges_ptl1MC);
  TH1F* etal1 = new TH1F("etal1","", NBINS_etal1, edges_etal1);
  TH1F* etal1MC = new TH1F("etal1MC","", NBINS_etal1MC, edges_etal1MC);
  TH1F* jetpt = new TH1F("jetpt","", NBINS_jetpt, edges_jetpt);
  TH1F* jetptMC = new TH1F("jetptMC","", NBINS_jetptMC, edges_jetptMC);
  //  TH1D* dphill = new TH1D("dphill","",NBINS_dphill,edges_dphill[0],edges_dphill[NBINS_dphill]);

  for(int i=1; i<=NBINS_mll; i++){
    mll->SetBinContent(i,values_xs_mll[i-1]);
    mll->SetBinError(i, values_xs_mll_err[i-1]);
  }


  for(int i=1; i<=NBINS_mllMC; i++){
    mllMC->SetBinContent(i,values_xs_mllMC[i-1]);
    mllMC->SetBinError(i, values_xs_mllMC_err[i-1]);
  }

  for(int i=1; i<=NBINS_ptll; i++){
    ptll->SetBinContent(i,values_xs_ptll[i-1]);
    ptll->SetBinError(i,values_xs_ptll_err[i-1]);
  }


  for(int i=1; i<=NBINS_ptllMC; i++){
    ptllMC->SetBinContent(i,values_xs_ptllMC[i-1]);
    ptllMC->SetBinError(i,values_xs_ptllMC_err[i-1]);
  }

  for(int i=1; i<=NBINS_ptl1; i++){
    ptl1->SetBinContent(i,values_xs_ptl1[i-1]);
    ptl1->SetBinError(i,values_xs_ptl1_err[i-1]);
  }


  for(int i=1; i<=NBINS_ptl1MC; i++){
    ptl1MC->SetBinContent(i,values_xs_ptl1MC[i-1]);
    ptl1MC->SetBinError(i,values_xs_ptl1MC_err[i-1]);
  }

  for(int i=1; i<=NBINS_etal1; i++){
    etal1->SetBinContent(i,values_xs_etal1[i-1]);
    etal1->SetBinError(i,values_xs_etal1_err[i-1]);
  }


  for(int i=1; i<=NBINS_etal1MC; i++){
    etal1MC->SetBinContent(i,values_xs_etal1MC[i-1]);
    etal1MC->SetBinError(i,values_xs_etal1MC_err[i-1]);
  }

  for(int i=1; i<=NBINS_jetpt; i++){
    jetpt->SetBinContent(i,values_xs_jetpt[i-1]);
    jetpt->SetBinError(i,values_xs_jetpt_err[i-1]);
  }


  for(int i=1; i<=NBINS_jetptMC; i++){
    jetptMC->SetBinContent(i,values_xs_jetptMC[i-1]);
    jetptMC->SetBinError(i,values_xs_jetptMC_err[i-1]);
  }

  // for(int i=1; i<=NBINS_dphill; i++){
  //   dphill->SetBinContent(i,values_xs_dphill[i-1]);
  //   dphill->SetBinError(i,values_xs_dphill_err[i-1]);
  // }



  // Plot the histograms

  TCanvas* mll_xs = new TCanvas("mll_xs", "WW diff Cross Section (mll)");
  mll->Draw("E1");
  mllMC->Draw("E1,SAME");
  SetAxis(mll, "m_{ll} [GeV]", "XSec / pb", 1.5, 2.5);

  //Titles
  DrawLatex(61, 0.190, 0.945, 0.050, 11, "CMS");
  DrawLatex(52, 0.300, 0.945, 0.030, 11, "Preliminary");
  DrawLatex(42, 0.940, 0.945, 0.050, 31, Form("%.3f fb^{-1} (13TeV)", 35.867));

  mll->SetFillColor  (kGray+1);
  mll->SetFillStyle  (   3345);
  mll->SetLineColor  (2);
  mll->SetLineWidth (2);
  mllMC->SetLineWidth (2);
  mll->SetMarkerColor(kGray+1);
  mll->SetMarkerSize (      0);


  //Legend

  auto legend = new TLegend(0.7, 0.77, 0.87, 0.87);
  legend->AddEntry(mll," Data","l");
  legend->AddEntry(mllMC," MC","l");
  legend->SetBorderSize(1);
  legend->Draw("SAME");



  TCanvas* ptll_xs = new TCanvas("ptll_xs", "WW diff Cross Section (ptll)");
  ptll->Draw("E1");
  ptllMC->Draw("E1,SAME");
  SetAxis(ptll, "p_{T}^{ll} [GeV]", "XSec / pb", 1.5, 2.5);


  //Titles
  DrawLatex(61, 0.190, 0.945, 0.050, 11, "CMS");
  DrawLatex(52, 0.300, 0.945, 0.030, 11, "Preliminary");
  DrawLatex(42, 0.940, 0.945, 0.050, 31, Form("%.3f fb^{-1} (13TeV)", 35.867));

  ptll->SetFillColor  (kGray+1);
  ptll->SetFillStyle  (   3345);
  ptll->SetLineColor  (2);
  ptll->SetLineWidth (2);
  ptllMC->SetLineWidth (2);
  ptll->SetMarkerColor(kGray+1);
  ptll->SetMarkerSize (      0);

  //Legend

  auto legend = new TLegend(0.7, 0.77, 0.87, 0.87);
  legend->AddEntry(mll," Data","l");
  legend->AddEntry(mllMC," MC","l");
  legend->SetBorderSize(1);
  legend->Draw("SAME");


  TCanvas* ptl1_xs = new TCanvas("ptl1_xs", "WW diff Cross Section (ptl1)");
  ptl1->Draw("E1");
  ptl1MC->Draw("E1,SAME");
  SetAxis(ptl1, "leading lepton p_{T} [GeV]", "XSec / pb", 1.5, 2.5);

  //Titles
  DrawLatex(61, 0.190, 0.945, 0.050, 11, "CMS");
  DrawLatex(52, 0.300, 0.945, 0.030, 11, "Preliminary");
  DrawLatex(42, 0.940, 0.945, 0.050, 31, Form("%.3f fb^{-1} (13TeV)", 35.867));

  ptl1->SetFillColor  (kGray+1);
  ptl1->SetFillStyle  (   3345);
  ptl1->SetLineColor  (2);
  ptl1->SetLineWidth (2);
  ptl1MC->SetLineWidth (2);
  ptl1->SetMarkerColor(kGray+1);
  ptl1->SetMarkerSize (      0);

  //Legend

  auto legend = new TLegend(0.7, 0.77, 0.87, 0.87);
  legend->AddEntry(mll," Data","l");
  legend->AddEntry(mllMC," MC","l");
  legend->SetBorderSize(1);
  legend->Draw("SAME");


  TCanvas* etal1_xs = new TCanvas("etal1_xs", "WW diff Cross Section (etal1)");
  etal1->Draw("E1");
  etal1MC->Draw("E1,SAME");
  SetAxis(etal1, "leading lepton #eta [rad]", "XSec / pb", 1.5, 2.5);

  //Titles
  DrawLatex(61, 0.190, 0.945, 0.050, 11, "CMS");
  DrawLatex(52, 0.300, 0.945, 0.030, 11, "Preliminary");
  DrawLatex(42, 0.940, 0.945, 0.050, 31, Form("%.3f fb^{-1} (13TeV)", 35.867));

  etal1->SetFillColor  (kGray+1);
  etal1->SetFillStyle  (   3345);
  etal1->SetLineColor  (2);
  etal1->SetLineWidth (2);
  etal1MC->SetLineWidth (2);
  etal1->SetMarkerColor(kGray+1);
  etal1->SetMarkerSize (      0);

  //Legend

  auto legend = new TLegend(0.76, 0.77, 0.90, 0.87);
  legend->AddEntry(mll," Data","l");
  legend->AddEntry(mllMC," MC","l");
  legend->SetBorderSize(1);
  legend->Draw("SAME");

  TCanvas* jetpt_xs = new TCanvas("jetpt_xs", "WW diff Cross Section (jetpt)");
  jetpt->Draw("E1");
  jetptMC->Draw("E1,SAME");
  SetAxis(jetpt, "jet p_{T} [GeV]", "XSec / pb", 1.5, 2.5);

  //Titles
  DrawLatex(61, 0.190, 0.945, 0.050, 11, "CMS");
  DrawLatex(52, 0.300, 0.945, 0.030, 11, "Preliminary");
  DrawLatex(42, 0.940, 0.945, 0.050, 31, Form("%.3f fb^{-1} (13TeV)", 35.867));

  jetpt->SetFillColor  (kGray+1);
  jetpt->SetFillStyle  (   3345);
  jetpt->SetLineColor  (2);
  jetpt->SetLineWidth (2);
  jetptMC->SetLineWidth (2);
  jetpt->SetMarkerColor(kGray+1);
  jetpt->SetMarkerSize (      0);

  //Legend

  auto legend = new TLegend(0.65, 0.77, 0.83, 0.87);
  legend->AddEntry(mll," Data","l");
  legend->AddEntry(mllMC," MC","l");
  legend->SetBorderSize(1);
  legend->Draw("SAME");
  // TCanvas* dphill_xs = new TCanvas("dphill_xs", "WW diff Cross Section (dphill)");
  // dphill->Draw("E1");
  // SetAxis(dphill, "dphi^{ll} [GeV]", "XSec / pb", 1.5, 2.5);

  // //Titles
  // DrawLatex(61, 0.190, 0.945, 0.050, 11, "CMS");
  // DrawLatex(52, 0.300, 0.945, 0.030, 11, "Preliminary");
  // DrawLatex(42, 0.940, 0.945, 0.050, 31, Form("%.3f fb^{-1} (13TeV)", 35.867));

  // dphill->SetFillColor  (kGray+1);
  // dphill->SetFillStyle  (   3345);
  // dphill->SetLineColor  (3);
  // dphill->SetMarkerColor(kGray+1);
  // dphill->SetMarkerSize (      0);

  
  //Save the plots
  mll_xs->SaveAs("mll_xs.png");
  ptll_xs->SaveAs("ptll_xs.png");
  ptl1_xs->SaveAs("ptl1_xs.png");
  etal1_xs->SaveAs("etal1_xs.png");
  jetpt_xs->SaveAs("jetpt_xs.png");
             
}

  // DrawLatex                                                                                                                                                     
  //------------------------------------------------------------------------------      

  void DrawLatex(Font_t      tfont,
	         Float_t     x,
	         Float_t     y,
		 Float_t     tsize,
		 Short_t     align,
		 const char* text,
		 Bool_t      setndc)
  {
    TLatex* tl = new TLatex(x, y, text);

    tl->SetNDC      (setndc);
    tl->SetTextAlign( align);
    tl->SetTextFont ( tfont);
    tl->SetTextSize ( tsize);

    tl->Draw("same");
  

  }





      void SetAxis(TH1*    hist,
		  TString xtitle,
		  TString ytitle,
		  Float_t xoffset,
	          Float_t yoffset)
      {
	gPad->cd();
	gPad->Update();

	// See https://root.cern.ch/doc/master/classTAttText.html#T4                                                                                                                                     
	Float_t padw = gPad->XtoPixel(gPad->GetX2());
	Float_t padh = gPad->YtoPixel(gPad->GetY1());

	Float_t size = (padw < padh) ? padw : padh;

	size = 20. / size;  // Like this label size is always 20 pixels                                                                                                                                  

	TAxis* xaxis = (TAxis*)hist->GetXaxis();
	TAxis* yaxis = (TAxis*)hist->GetYaxis();

	xaxis->SetTitleOffset(xoffset);
	yaxis->SetTitleOffset(yoffset);

	//  xaxis->SetLabelOffset(5.*xaxis->GetLabelOffset());  // It works for Juan                                                                                                                     
	//  yaxis->SetLabelOffset(3.*yaxis->GetLabelOffset());  // It works for Juan                                                                                                                     

	xaxis->SetLabelSize(size);
	yaxis->SetLabelSize(size);
	xaxis->SetTitleSize(size);
	yaxis->SetTitleSize(size);

	xaxis->SetTitle(xtitle);
	yaxis->SetTitle(ytitle);

	yaxis->CenterTitle();

	gPad->GetFrame()->DrawClone();
	gPad->RedrawAxis();     

      }


