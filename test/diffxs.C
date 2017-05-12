#include <iostream>
using namespace std;
#include "TLatex.h"

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

   const Int_t NBINS_mll = 5;
   Int_t edges_mll[NBINS_mll+1]={0,1,2,3,4,5};
   Double_t values_xs_mll[NBINS_mll]={100., 150., 300., 600., 1000.};
   Double_t values_xs_mll_err[NBINS_mll]={10., 15., 30., 60., 100.};

   const Int_t NBINS_ptll = 5;
   Int_t edges_ptll[NBINS_ptll+1]={0,1,2,3,4,5};
   Double_t values_xs_ptll[NBINS_ptll]={1000., 500., 800., 1500., 300.};
   Double_t values_xs_ptll_err[NBINS_ptll]={120., 400., 100., 50., 50.};

   
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
  SetAxis(mll, "m_{ll} [GeV]", "events", 1.5, 2.5);

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
  SetAxis(ptll, "p_{T}^{ll} [GeV]", "events", 1.5, 2.5);

  //Titles
  DrawLatex(61, 0.190, 0.945, 0.050, 11, "CMS");
  DrawLatex(52, 0.300, 0.945, 0.030, 11, "Preliminary");
  DrawLatex(42, 0.940, 0.945, 0.050, 31, Form("%.3f fb^{-1} (13TeV)", 35.867));

  ptll->SetFillColor  (kGray+1);
  ptll->SetFillStyle  (   3345);
  ptll->SetLineColor  (kGray+1);
  ptll->SetMarkerColor(kGray+1);
  ptll->SetMarkerSize (      0);

  

             
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


