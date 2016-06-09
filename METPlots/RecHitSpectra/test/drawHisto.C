#include "TROOT.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TPaveStats.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TList.h"
#include "TLegendEntry.h"
#include "string.h"
#include "TImage.h"
#include <iostream>

void drawHisto(const char *file, const char *histname1, const char *desc1,
		const char *histtitle, const char *xtitle,  const char *ytitle, const char *outfile, const char *sbox = "True"){

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   Int_t line1 = 1;
   Int_t fill1 = 18;

   Int_t line2 = 2;
   Int_t fill2 = 0;

   Int_t line3 = 4;
   Int_t fill3 = 0;

   Int_t line4 = 32;
   Int_t fill4 = 0;

   bool drawSBox = true;

   if(strcmp(sbox,"True")){
     gStyle->SetOptStat(0);
     drawSBox = false;
   }


  TCanvas *c = new TCanvas();
  c->SetLogy();
  c->SetGridx();
  c->SetGridy();

  TFile *f1 = new TFile(file);
  if(f1->IsZombie()){
    cout << "Root file: " << file << " not found!" << endl;
    return;
  }

  TH1F *h1 = (TH1F*)f1->Get(histname1);

 int startplot = h1->FindFirstBinAbove(0.00005);

 int endplot = h1->FindLastBinAbove(0.00005);

 int plotspace = (endplot - startplot) / 4;

 //cout << "Start Bin: " << startplot << " End Bin : " << endplot << endl;

 startplot = (startplot - plotspace > 0 ? startplot - plotspace : 1);
 endplot = endplot + plotspace;

 //cout << "Start Bin: " << startplot << " End Bin : " << endplot << endl;

 

  float yrange;

  yrange = h1->GetBinContent(h1->GetMaximumBin());

  h1->SetName(desc1);
  h1->SetTitle(histtitle);
  h1->SetMaximum(yrange*1.3);
   ci = TColor::GetColor("#ccccff");
   h1->SetFillColor(fill1);

   ci = TColor::GetColor("#000099");
   h1->SetLineColor(line1);
   h1->SetLineWidth(2);
   h1->GetXaxis()->SetTitle(xtitle);
   h1->GetXaxis()->SetLabelFont(42);
   h1->GetXaxis()->SetLabelSize(0.035);
   h1->GetXaxis()->SetTitleSize(0.045);
   h1->GetXaxis()->SetTitleFont(42);
   h1->GetYaxis()->SetTitle(ytitle);
   h1->GetYaxis()->SetLabelFont(42);
   h1->GetYaxis()->SetLabelSize(0.035);
   h1->GetYaxis()->SetTitleSize(0.045);
   h1->GetYaxis()->SetTitleFont(42);
   h1->GetXaxis()->SetRange(startplot,endplot);
   h1->GetZaxis()->SetLabelFont(42);
   h1->GetZaxis()->SetLabelSize(0.035);
   h1->GetZaxis()->SetTitleSize(0.035);
   h1->GetZaxis()->SetTitleFont(42);
   h1->Draw("");

   TPaveStats *st1;

   gPad->Update();
   if(drawSBox){
     st1 = (TPaveStats*)h1->FindObject("stats");
     if(!st1){
       cout << "Failed to load statbox!" << endl;
       return;
     }
   }

   c->Modified();
   c->cd();
   c->SetSelected(c);

   TImage *img = TImage::Create();

   img->FromPad(c);
   img->WriteImage(outfile);


}
