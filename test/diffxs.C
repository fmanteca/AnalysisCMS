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

   const Int_t NBINS_mll = 7;
   Int_t edges_mll[NBINS_mll+1]={20,40,60,80,100,120, 140, 160};
   Double_t values_xs_mll[NBINS_mll]={90., 120., 100., 80., 50., 25., 10.};
   Double_t values_xs_mll_err[NBINS_mll]={3., 5., 10., 10., 5., 3., 2.};

   const Int_t NBINS_ptll = 9;
   Int_t edges_ptll[NBINS_ptll+1]={20,40,60,80,100,120,140,160,180,200};
   Double_t values_xs_ptll[NBINS_ptll]={140., 120., 100., 90., 70., 60., 40., 30., 20.};
   Double_t values_xs_ptll_err[NBINS_ptll]={10., 10., 10., 9., 8., 6., 5., 2., 2.,};

   
  // Fill the histograms

  TH1D* mll = new TH1D("mll","",NBINS_mll,edges_mll[0],edges_mll[NBINS_mll]);
  TH1D* ptll = new TH1D("ptll","", NBINS_ptll,edges_ptll[0],edges_ptll[NBINS_ptll]);


  for(int i=1; i<=NBINS_mll; i++){
    mll->SetBinContent(i,values_xs_mll[i-1]);
    mll->SetBinError(i, values_xs_mll_err[i-1]);
  }

  for(int i=1; i<=NBINS_ptll; i++){
    ptll->SetBinContent(i,values_xs_ptll[i-1]);
    ptll->SetBinError(i,values_xs_ptll_err[i-1]);
  }



  // Plot the histograms

  TCanvas* mll_xs = new TCanvas("mll_xs", "WW diff Cross Section (mll)");
  mll->Draw("E1");
  SetAxis(mll, "m_{ll} [GeV]", "XSec / pb", 1.5, 2.5);

  //Titles
  DrawLatex(61, 0.190, 0.945, 0.050, 11, "CMS");
  DrawLatex(52, 0.300, 0.945, 0.030, 11, "Preliminary");
  DrawLatex(42, 0.940, 0.945, 0.050, 31, Form("%.3f fb^{-1} (13TeV)", 35.867));

  mll->SetFillColor  (kGray+1);
  mll->SetFillStyle  (   3345);
  mll->SetLineColor  (kGray+1);
  mll->SetMarkerColor(kGray+1);
  mll->SetMarkerSize (      0);

  TCanvas* ptll_xs = new TCanvas("ptll_xs", "WW diff Cross Section (ptll)");
  ptll->Draw("E1");
  SetAxis(ptll, "p_{T}^{ll} [GeV]", "XSec / pb", 1.5, 2.5);

  //Titles
  DrawLatex(61, 0.190, 0.945, 0.050, 11, "CMS");
  DrawLatex(52, 0.300, 0.945, 0.030, 11, "Preliminary");
  DrawLatex(42, 0.940, 0.945, 0.050, 31, Form("%.3f fb^{-1} (13TeV)", 35.867));

  ptll->SetFillColor  (kGray+1);
  ptll->SetFillStyle  (   3345);
  ptll->SetLineColor  (kGray+1);
  ptll->SetMarkerColor(kGray+1);
  ptll->SetMarkerSize (      0);

  
  //Save the plots
  mll_xs->SaveAs("mll_xs.png");
  ptll_xs->SaveAs("ptll_xs.png");
             
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


