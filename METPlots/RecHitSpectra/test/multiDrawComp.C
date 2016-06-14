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
#include <iostream>

void multiDrawComp(const char *file1, const char *dataset1, const char *file2, const char* dataset2,
	      const char *histname, const char *histtitle, const char *xtitle,  const char *ytitle, const char *outfile,
		   const char *file3 = "none", const char *dataset3 = "none", 
		const char *file4 ="none", const char *dataset4 = "none", const char *sbox = "True"){

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


   int filenum = 2;
   if(strcmp(file3,"none")&&strcmp(dataset3,"none")){
     filenum = 3;
   }

   if(strcmp(file4,"none")&&strcmp(dataset4,"none")){
     filenum = 4;
   }   

  TCanvas *c = new TCanvas();
  c->SetLogy();
  c->SetGridx();
  c->SetGridy();

  TFile *f1 = new TFile(file1);
  if(f1->IsZombie()){
    cout << "Root file: " << file1 << " not found!" << endl;
    return;
  }

  TFile *f2 = new TFile(file2);
  if(f2->IsZombie()){
    cout << "Root file: " << file2 << " not found!" << endl;
    return;
  }

  f1->cd("");

  TH1F *h1 = (TH1F*)f1->Get(histname);

  f2->cd("");

  TH1F *h2 = (TH1F*)f2->Get(histname);

  TH1F *h3;
  TH1F *h4;

  TFile *f3;
  TFile *f4;

  if(filenum >= 3){
    f3 = new TFile(file3);
    if(f3->IsZombie()){
      cout << "Root file: " << file3 << " not found!" << endl;
      return;
    }
    f3->cd();
    h3 = (TH1F*)f3->Get(histname);
  }

 if(filenum >= 4){
    f4 = new TFile(file4);
    if(f4->IsZombie()){
      cout << "Root file: " << file4 << " not found!" << endl;
      return;
    }
    f4->cd();
    h4 = (TH1F*)f4->Get(histname);
  }

 int start1 = h1->FindFirstBinAbove(0.00005);
 int start2 = h2->FindFirstBinAbove(0.00005);
 int start3;
 int start4;

 int startplot = (start1 < start2 ? start1 : start2);
 if(filenum >= 3){
   start3 = h3->FindFirstBinAbove(0.00005);
   startplot = (startplot < start3 ? startplot : start3);
 }

 if(filenum >= 4){
   start4 = h4->FindFirstBinAbove(0.00005);
   startplot = (startplot < start4 ? startplot : start4);
 }

 int end1 = h1->FindLastBinAbove(0.00005);
 int end2 = h2->FindLastBinAbove(0.00005);
 int end3;
 int end4;

 int endplot = (end1 > end2 ? end1 : end2);
 if(filenum >= 3){
   end3 = h3->FindLastBinAbove(0.00005);
   endplot = (endplot > end3 ? endplot : end3);
 }

 if(filenum >= 4){
   end4 = h4->FindFirstBinAbove(0.00005);
   endplot = (endplot > end4 ? endplot : end4);
 }

 int plotspace = (endplot - startplot) / 4;

 //cout << "Start Bin: " << startplot << " End Bin : " << endplot << endl;

 startplot = (startplot - plotspace > 0 ? startplot - plotspace : 1);
 endplot = endplot + plotspace;

 //cout << "Start Bin: " << startplot << " End Bin : " << endplot << endl;

 

  float yrange;

  if(h1->GetBinContent(h1->GetMaximumBin()) > h2->GetBinContent(h2->GetMaximumBin())){
    yrange = h1->GetBinContent(h1->GetMaximumBin());
  }else{
    yrange = h2->GetBinContent(h2->GetMaximumBin());
  }

  if(filenum >= 3) yrange = (yrange > h3->GetBinContent(h3->GetMaximumBin()) ? yrange : h3->GetBinContent(h3->GetMaximumBin()));
  if(filenum >= 4) yrange = (yrange > h4->GetBinContent(h4->GetMaximumBin()) ? yrange : h4->GetBinContent(h4->GetMaximumBin()));

  //Start Drawing
  h1->SetName(dataset1);
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
   TPaveStats *st2;
   TPaveStats *st3;
   TPaveStats *st4;

   gPad->Update();
   if(drawSBox){
     st1 = (TPaveStats*)h1->FindObject("stats");
     if(!st1){
       cout << "Failed to load statbox!" << endl;
       return;
     }
   }

   float y1 = st1->GetY1NDC();
   float y2 = st1->GetY2NDC();
   float stHeight = y2 - y1;
   float stSpacing = stHeight / 6;

   h2->SetName(dataset2);

   ci = TColor::GetColor("#ffcccc");
   h2->SetFillColor(fill2);
   h2->SetFillStyle(3001);
   h2->SetLineColor(line2);
   h2->SetLineWidth(3);
   h2->GetXaxis()->SetLabelFont(42);
   h2->GetXaxis()->SetLabelSize(0.035);
   h2->GetXaxis()->SetTitleSize(0.035);
   h2->GetXaxis()->SetTitleFont(42);
   h2->GetYaxis()->SetLabelFont(42);
   h2->GetYaxis()->SetLabelSize(0.035);
   h2->GetYaxis()->SetTitleSize(0.035);
   h2->GetYaxis()->SetTitleFont(42);
   h2->GetZaxis()->SetLabelFont(42);
   h2->GetZaxis()->SetLabelSize(0.035);
   h2->GetZaxis()->SetTitleSize(0.035);
   h2->GetZaxis()->SetTitleFont(42);
   h2->Draw("sames");

   gPad->Update();
   if(drawSBox){
     st2 = (TPaveStats*)h2->FindObject("stats");
     st2->SetY1NDC(y1-stSpacing-stHeight);
     st2->SetY2NDC(y1-stSpacing);

     st3;
     st4;
   }

   if(filenum >= 3){
     if(drawSBox){
       y1 = st2->GetY1NDC();
       y2 = st2->GetY2NDC();
     }
     stHeight = y2 - y1;
     stSpacing = stHeight / 6;

     h3->SetName(dataset3);

     ci = TColor::GetColor("#ffcccc");
     h3->SetFillColor(fill3);
     h3->SetFillStyle(3001);
     h3->SetLineColor(line3);
     h3->SetLineWidth(2);
     h3->GetXaxis()->SetLabelFont(42);
     h3->GetXaxis()->SetLabelSize(0.035);
     h3->GetXaxis()->SetTitleSize(0.035);
     h3->GetXaxis()->SetTitleFont(42);
     h3->GetYaxis()->SetLabelFont(42);
     h3->GetYaxis()->SetLabelSize(0.035);
     h3->GetYaxis()->SetTitleSize(0.035);
     h3->GetYaxis()->SetTitleFont(42);
     h3->GetZaxis()->SetLabelFont(42);
     h3->GetZaxis()->SetLabelSize(0.035);
     h3->GetZaxis()->SetTitleSize(0.035);
     h3->GetZaxis()->SetTitleFont(42);
     h3->Draw("sames");

     gPad->Update();
     if(drawSBox){
       st3 = (TPaveStats*)h3->FindObject("stats");
       st3->SetY1NDC(y1-stSpacing-stHeight);
       st3->SetY2NDC(y1-stSpacing);
     }
   }

   if(filenum >= 4){
     if(drawSBox){
       y1 = st3->GetY1NDC();
       y2 = st3->GetY2NDC();
       stHeight = y2 - y1;
       stSpacing = stHeight / 6;
     }

     h4->SetName(dataset4);

     ci = TColor::GetColor("#ffcccc");
     h4->SetFillColor(fill4);
     h4->SetFillStyle(3001);
     h4->SetLineColor(line4);
     h4->SetLineWidth(2);
     h4->GetXaxis()->SetLabelFont(42);
     h4->GetXaxis()->SetLabelSize(0.035);
     h4->GetXaxis()->SetTitleSize(0.035);
     h4->GetXaxis()->SetTitleFont(42);
     h4->GetYaxis()->SetLabelFont(42);
     h4->GetYaxis()->SetLabelSize(0.035);
     h4->GetYaxis()->SetTitleSize(0.035);
     h4->GetYaxis()->SetTitleFont(42);
     h4->GetZaxis()->SetLabelFont(42);
     h4->GetZaxis()->SetLabelSize(0.035);
     h4->GetZaxis()->SetTitleSize(0.035);
     h4->GetZaxis()->SetTitleFont(42);
     h4->Draw("sames");

     gPad->Update();
     if(drawSBox){
       st4 = (TPaveStats*)h4->FindObject("stats");
       st4->SetY1NDC(y1-stSpacing-stHeight);
       st4->SetY2NDC(y1-stSpacing);
     }
   }

   TLegend *leg = new TLegend(0.3913858,0.6656347,0.7696629,0.876161,NULL,"brNDC");
   leg->SetBorderSize(1);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("NULL","Datasets","h");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry(dataset1,dataset1,"lpf");

   ci = TColor::GetColor("#ccccff");
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#000099");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(2);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(1);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry(dataset2,dataset2,"lpf");

   ci = TColor::GetColor("#ffcccc");
   entry->SetFillColor(ci);
   entry->SetFillStyle(3001);
   entry->SetLineColor(2);
   entry->SetLineStyle(2);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(1);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);

   if(filenum >= 3){
     entry=leg->AddEntry(dataset3,dataset3,"lpf");

     ci = TColor::GetColor("#ffcccc");
     entry->SetFillColor(ci);
     entry->SetFillStyle(3001);
     entry->SetLineColor(2);
     entry->SetLineStyle(2);
     entry->SetLineWidth(1);
     entry->SetMarkerColor(1);
     entry->SetMarkerStyle(1);
     entry->SetMarkerSize(1);
     entry->SetTextFont(42);
   }

   if(filenum >=4){
     entry=leg->AddEntry(dataset4,dataset4,"lpf");

     ci = TColor::GetColor("#ffcccc");
     entry->SetFillColor(ci);
     entry->SetFillStyle(3001);
     entry->SetLineColor(2);
     entry->SetLineStyle(2);
     entry->SetLineWidth(1);
     entry->SetMarkerColor(1);
     entry->SetMarkerStyle(1);
     entry->SetMarkerSize(1);
     entry->SetTextFont(42);
   }

   leg->Draw();
   c->Modified();
   c->cd();
   c->SetSelected(c);

   TImage *img = TImage::Create();

   img->FromPad(c);
   img->WriteImage(outfile);


}
