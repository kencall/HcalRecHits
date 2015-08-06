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

void multiDrawComp2(const char *file1, const char *dataset1, const char *file2, const char* dataset2,
		    const char *profname1, const char *id1, const char *profname2, const char *id2, const char *histtitle, const char *xtitle,  const char *ytitle, const char *outfile,
		    const char *file3 = "none", const char *dataset3 = "none", const char *file4 ="none", const char *dataset4 = "none", const char *profNum = "double"){

  bool single = false;

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha

   Int_t prof1_1 = 1;
   Int_t prof2_1 = 12;

   //Int_t prof1_1 = 214;
   //Int_t prof2_1 = 216;

   Int_t prof1_2 = 2;
   Int_t prof2_2 = 208;

   Int_t prof1_3 = 3;
   Int_t prof2_3 = 84;

   //Int_t prof1_3 = 210;
   //Int_t prof2_3 = 212;

   Int_t prof1_4 = 222;
   Int_t prof2_4 = 224;

   Int_t marker1 = 20;
   Int_t marker2 = 25;
   Int_t marker3 = 26;
   Int_t marker4 = 28;

   /*
   Int_t line1 = 1;
   Int_t fill1 = 18;

   Int_t line2 = 2;
   Int_t fill2 = 0;

   Int_t line3 = 4;
   Int_t fill3 = 0;

   Int_t line4 = 32;
   Int_t fill4 = 0;
   */


   int filenum = 2;
   if(strcmp(file3,"none")&&strcmp(dataset3,"none")){
     filenum = 3;
   }

   if(strcmp(file4,"none")&&strcmp(dataset4,"none")){
     filenum = 4;
   }

   if(!strcmp(profNum,"single")) single = true;

   cout << "Files: " << filenum << endl;

  TCanvas *c = new TCanvas();
  c->SetGridx();
  c->SetGridy();
  gStyle->SetOptStat(0);

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

  TProfile *p1_1 = (TProfile*)f1->Get(profname1);
  if(!p1_1){
    cout << "Failed to load " << profname1 << " from " << file1 << "!";
    return;
  }
  TProfile *p2_1;
  if(!single) p2_1 = (TProfile*)f1->Get(profname2);

  f2->cd("");

  TProfile *p1_2 = (TProfile*)f2->Get(profname1);
  TProfile *p2_2;
  if(!single) p2_2 = (TProfile*)f2->Get(profname2);

  TProfile *p1_3;
  TProfile *p2_3;

  TProfile *p1_4;
  TProfile *p2_4;

  TFile *f3;
  TFile *f4;

  cout << "Try to open additional files. filenum = " << filenum << endl;

  if(filenum >= 3){
    cout << "Trying to open: " << file3 << endl;
    f3 = new TFile(file3);
    cout << "Created new TFile object" << endl;
    if(f3->IsZombie()){
      cout << "Root file: " << file3 << " not found!" << endl;
      return;
    }
    cout << "Successfully opened " << file3 << endl;
    f3->cd();
    p1_3 = (TProfile*)f3->Get(profname1);
    if(p1_3) cout << "Pointer to p1_3 object" << endl;
    if(!single) p2_3 = (TProfile*)f3->Get(profname2);
    if(p2_3) cout << "Pointer to p2_3 object" << endl;
  }

  cout << "Checking parameters for 4th dataset." << endl;

 if(filenum >= 4){
   cout << "Attempting to open fourth file: " << file4 << endl;
   
    f4 = new TFile(file4);
    if(f4->IsZombie()){
      cout << "Root file: " << file4 << " not found!" << endl;
      return;
    }
    f4->cd();
    p1_4 = (TProfile*)f4->Get(profname1);
    if(!single) p2_4 = (TProfile*)f4->Get(profname2);
 }

 cout << "Done opening root files" << endl;

 cout << "Loading starting bins" << endl;

 cout << "Loading p1_1 named: " << profname1 << endl;
 int start1_1 = p1_1->FindFirstBinAbove(0.00005);
 int start2_1;
 if(!single){
   start2_1 = p2_1->FindFirstBinAbove(0.00005);
   cout << "Loading p2_1" << endl;
 }
 cout << "Loading p1_2" << endl;
 int start1_2 = p1_2->FindFirstBinAbove(0.00005);
 int start2_2;
 if(!single){
   start2_2 = p2_2->FindFirstBinAbove(0.00005);
   cout << "Loading p2_2" << endl;
 }
 int start1_3;
 int start2_3;
 int start1_4;
 int start2_4;

 int startplot;

 cout << "Done loading starting bins for datasets 1&2" << endl;

 startplot = (start1_1 < start1_2 ? start1_1 : start1_2);
 if(!single){
   startplot = (startplot < start2_1 ? startplot : start2_1);
   startplot = (startplot < start2_2 ? startplot : start2_2);
 }

 if(!single) startplot = (startplot < start2_2 ? startplot : start2_2);

 if(filenum >= 3){
   cout << "Looking for lower bin in 3rd datset" << endl;
   start1_3 = p1_3->FindFirstBinAbove(0.00005);
   cout << "Loaded first bin from first profile." << endl;
   if(!single) start2_3 = p2_3->FindFirstBinAbove(0.00005);
   cout << "Loaded first bin from second profile." << endl;
   startplot = (startplot < start1_3 ? startplot : start1_3);
   if(!single) startplot = (startplot < start2_3 ? startplot : start2_3);
 }

 if(filenum >= 4){
   cout << "Looking for lower bin in 4th dataset" << endl;
   start1_4 = p1_4->FindFirstBinAbove(0.00005);
   if(!single) start2_4 = p2_4->FindFirstBinAbove(0.00005);
   startplot = (startplot < start1_4 ? startplot : start1_4);
   if(!single) startplot = (startplot < start2_4 ? startplot : start2_4);
 }

 cout << "Begining search for upper bin" << endl;

 int end1_1 = p1_1->FindLastBinAbove(0.00005);
 int end1_2 = p1_2->FindLastBinAbove(0.00005);
 int end1_3;
 int end1_4;
 int end2_1;
 int end2_2;
 if(!single) end2_1 = p2_1->FindLastBinAbove(0.00005);
 if(!single) end2_2 = p2_2->FindLastBinAbove(0.00005);
 int end2_3;
 int end2_4;

 int endplot;

 cout << "Done loading ending bins for datasets 1&2" << endl;

 endplot = (end1_1 > end1_2 ? end1_1 : end1_2);
 if(!single){
   endplot = (endplot > end2_1 ? endplot : end2_1);
   endplot = (endplot > end2_2 ? endplot : end2_2);
 }

 cout << "Upper bin search complete for first two datasets." << endl;

 if(filenum >= 3){
   cout << "Looking for upper bin in 3rd dataset" << endl;
   end1_3 = p1_3->FindLastBinAbove(0.00005);
   if(!single) end2_3 = p2_3->FindLastBinAbove(0.00005);
   endplot = (endplot > end1_3 ? endplot : end1_3);
   if(!single) endplot = (endplot > end2_3 ? endplot : end2_3);
 }

 if(filenum >= 4){
   end1_4 = p1_4->FindLastBinAbove(0.00005);
   if(!single) end2_4 = p2_4->FindLastBinAbove(0.00005);
   endplot = (endplot > end1_4 ? endplot : end1_4);
   if(!single) endplot = (endplot > end2_4 ? endplot : end2_4);
 }

 int plotspace = (endplot - startplot) / 10;

 cout << "Start Bin: " << startplot << " End Bin : " << endplot << endl;

 startplot = (startplot - plotspace > 0 ? startplot - plotspace : 1);
 endplot = endplot + plotspace;

 cout << "Start Bin: " << startplot << " End Bin : " << endplot << endl;

 

  float yrange;

  if(!single) {
    yrange = (p1_1->GetBinContent(p1_1->GetMaximumBin()) > p2_1->GetBinContent(p2_1->GetMaximumBin()) ? p1_1->GetBinContent(p1_1->GetMaximumBin()) : p2_1->GetBinContent(p2_1->GetMaximumBin()));
  }else{
    yrange = (p1_1->GetBinContent(p1_1->GetMaximumBin()) > p1_2->GetBinContent(p1_2->GetMaximumBin()) ? p1_1->GetBinContent(p1_1->GetMaximumBin()) : p1_2->GetBinContent(p1_2->GetMaximumBin()));
  }
  yrange = (yrange > p1_2->GetBinContent(p1_2->GetMaximumBin()) ? yrange : p1_2->GetBinContent(p1_2->GetMaximumBin()));
  if(!single) yrange = (yrange > p2_2->GetBinContent(p2_2->GetMaximumBin()) ? yrange : p2_2->GetBinContent(p2_2->GetMaximumBin()));

  if(filenum >= 3){
    yrange = (yrange > p1_3->GetBinContent(p1_3->GetMaximumBin()) ? yrange : p1_3->GetBinContent(p1_3->GetMaximumBin()));
    if(!single) yrange = (yrange > p2_3->GetBinContent(p2_3->GetMaximumBin()) ? yrange : p2_3->GetBinContent(p2_3->GetMaximumBin()));
  }

  if(filenum >= 4){
    yrange = (yrange > p1_4->GetBinContent(p1_4->GetMaximumBin()) ? yrange : p1_4->GetBinContent(p1_4->GetMaximumBin()));
    if(!single) yrange = (yrange > p2_4->GetBinContent(p2_4->GetMaximumBin()) ? yrange : p2_4->GetBinContent(p2_4->GetMaximumBin()));
  }

  //Start Drawing
  char profname[30] = {0};
  strcat(profname, dataset1);
  strcat(profname, " ");
  strcat(profname, id1);

  p1_1->SetName(profname);
  p1_1->SetTitle(histtitle);
  p1_1->SetMaximum(yrange*1.3);

  p1_1->SetLineColor(prof1_1);
  p1_1->SetMarkerColor(prof1_1);
  p1_1->SetMarkerStyle(marker1);

  p1_1->GetXaxis()->SetTitle(xtitle);
  p1_1->GetXaxis()->SetLabelFont(42);
  p1_1->GetXaxis()->SetLabelSize(0.035);
  p1_1->GetXaxis()->SetTitleSize(0.045);
  p1_1->GetXaxis()->SetTitleFont(42);
  p1_1->GetYaxis()->SetTitle(ytitle);
  p1_1->GetYaxis()->SetLabelFont(42);
  p1_1->GetYaxis()->SetLabelSize(0.035);
  p1_1->GetYaxis()->SetTitleSize(0.045);
  p1_1->GetYaxis()->SetTitleFont(42);
  p1_1->GetXaxis()->SetRange(startplot,endplot);
  p1_1->GetZaxis()->SetLabelFont(42);
  p1_1->GetZaxis()->SetLabelSize(0.035);
  p1_1->GetZaxis()->SetTitleSize(0.035);
  p1_1->GetZaxis()->SetTitleFont(42);
  p1_1->Draw("");  

  gPad->Update();

  if(!single){ 
  profname[0] = '\0';
  strcat(profname, dataset1);
  strcat(profname, " ");
  strcat(profname, id2);

  p2_1->SetName(profname);
  p2_1->SetTitle(histtitle);
  p2_1->SetMaximum(yrange*1.3);

  p2_1->SetLineColor(prof2_1);
  p2_1->SetMarkerColor(prof2_1);
  p2_1->SetMarkerStyle(marker1);
  p2_1->SetMarkerSize(.7);

  p2_1->GetXaxis()->SetTitle(xtitle);
  p2_1->GetXaxis()->SetLabelFont(42);
  p2_1->GetXaxis()->SetLabelSize(0.035);
  p2_1->GetXaxis()->SetTitleSize(0.045);
  p2_1->GetXaxis()->SetTitleFont(42);
  p2_1->GetYaxis()->SetTitle(ytitle);
  p2_1->GetYaxis()->SetLabelFont(42);
  p2_1->GetYaxis()->SetLabelSize(0.035);
  p2_1->GetYaxis()->SetTitleSize(0.045);
  p2_1->GetYaxis()->SetTitleFont(42);
  p2_1->GetXaxis()->SetRange(startplot,endplot);
  p2_1->GetZaxis()->SetLabelFont(42);
  p2_1->GetZaxis()->SetLabelSize(0.035);
  p2_1->GetZaxis()->SetTitleSize(0.035);
  p2_1->GetZaxis()->SetTitleFont(42);
  p2_1->Draw("sames");  

  gPad->Update();
  } //Second profile

  //Now draw the second dataset

  profname[0] = '\0';
  strcat(profname, dataset2);
  strcat(profname, " ");
  strcat(profname, id1);

  p1_2->SetName(profname);
  p1_2->SetTitle(histtitle);
  p1_2->SetMaximum(yrange*1.3);

  p1_2->SetLineColor(prof1_2);
  p1_2->SetMarkerColor(prof1_2);
  p1_2->SetMarkerStyle(marker2);

  p1_2->GetXaxis()->SetTitle(xtitle);
  p1_2->GetXaxis()->SetLabelFont(42);
  p1_2->GetXaxis()->SetLabelSize(0.035);
  p1_2->GetXaxis()->SetTitleSize(0.045);
  p1_2->GetXaxis()->SetTitleFont(42);
  p1_2->GetYaxis()->SetTitle(ytitle);
  p1_2->GetYaxis()->SetLabelFont(42);
  p1_2->GetYaxis()->SetLabelSize(0.035);
  p1_2->GetYaxis()->SetTitleSize(0.045);
  p1_2->GetYaxis()->SetTitleFont(42);
  p1_2->GetXaxis()->SetRange(startplot,endplot);
  p1_2->GetZaxis()->SetLabelFont(42);
  p1_2->GetZaxis()->SetLabelSize(0.035);
  p1_2->GetZaxis()->SetTitleSize(0.035);
  p1_2->GetZaxis()->SetTitleFont(42);
  p1_2->Draw("sames");  

  gPad->Update();

  if(!single){
  profname[0] = '\0';
  strcat(profname, dataset2);
  strcat(profname, " ");
  strcat(profname, id2);

  p2_2->SetName(profname);
  p2_2->SetTitle(histtitle);
  p2_2->SetMaximum(yrange*1.3);

  p2_2->SetLineColor(prof2_2);
  p2_2->SetMarkerColor(prof2_2);
  p2_2->SetMarkerStyle(marker2);
  p2_2->SetMarkerSize(.7);

  p2_2->GetXaxis()->SetTitle(xtitle);
  p2_2->GetXaxis()->SetLabelFont(42);
  p2_2->GetXaxis()->SetLabelSize(0.035);
  p2_2->GetXaxis()->SetTitleSize(0.045);
  p2_2->GetXaxis()->SetTitleFont(42);
  p2_2->GetYaxis()->SetTitle(ytitle);
  p2_2->GetYaxis()->SetLabelFont(42);
  p2_2->GetYaxis()->SetLabelSize(0.035);
  p2_2->GetYaxis()->SetTitleSize(0.045);
  p2_2->GetYaxis()->SetTitleFont(42);
  p2_2->GetXaxis()->SetRange(startplot,endplot);
  p2_2->GetZaxis()->SetLabelFont(42);
  p2_2->GetZaxis()->SetLabelSize(0.035);
  p2_2->GetZaxis()->SetTitleSize(0.035);
  p2_2->GetZaxis()->SetTitleFont(42);
  p2_2->Draw("sames");  

  gPad->Update();
  } //Second profile


  //Now we draw the third dataset
  if(filenum >= 3){
    profname[0] = '\0';
    strcat(profname, dataset3);
    strcat(profname, " ");
    strcat(profname, id1);

    p1_3->SetName(profname);
    p1_3->SetTitle(histtitle);
    p1_3->SetMaximum(yrange*1.3);

    p1_3->SetLineColor(prof1_3);
    p1_3->SetMarkerColor(prof1_3);
    p1_3->SetMarkerStyle(marker3);

    p1_3->GetXaxis()->SetTitle(xtitle);
    p1_3->GetXaxis()->SetLabelFont(42);
    p1_3->GetXaxis()->SetLabelSize(0.035);
    p1_3->GetXaxis()->SetTitleSize(0.045);
    p1_3->GetXaxis()->SetTitleFont(42);
    p1_3->GetYaxis()->SetTitle(ytitle);
    p1_3->GetYaxis()->SetLabelFont(42);
    p1_3->GetYaxis()->SetLabelSize(0.035);
    p1_3->GetYaxis()->SetTitleSize(0.045);
    p1_3->GetYaxis()->SetTitleFont(42);
    p1_3->GetXaxis()->SetRange(startplot,endplot);
    p1_3->GetZaxis()->SetLabelFont(42);
    p1_3->GetZaxis()->SetLabelSize(0.035);
    p1_3->GetZaxis()->SetTitleSize(0.035);
    p1_3->GetZaxis()->SetTitleFont(42);
    p1_3->Draw("sames");  

    gPad->Update();
    
    if(!single){ 
    profname[0] = '\0';
    strcat(profname, dataset3);
    strcat(profname, " ");
    strcat(profname, id2);

    p2_3->SetName(profname);
    p2_3->SetTitle(histtitle);
    p2_3->SetMaximum(yrange*1.3);
    
    p2_3->SetLineColor(prof2_3);
    p2_3->SetMarkerColor(prof2_3);
    p2_3->SetMarkerStyle(marker3);
    p2_3->SetMarkerSize(.7);
    
    p2_3->GetXaxis()->SetTitle(xtitle);
    p2_3->GetXaxis()->SetLabelFont(42);
    p2_3->GetXaxis()->SetLabelSize(0.035);
    p2_3->GetXaxis()->SetTitleSize(0.045);
    p2_3->GetXaxis()->SetTitleFont(42);
    p2_3->GetYaxis()->SetTitle(ytitle);
    p2_3->GetYaxis()->SetLabelFont(42);
    p2_3->GetYaxis()->SetLabelSize(0.035);
    p2_3->GetYaxis()->SetTitleSize(0.045);
    p2_3->GetYaxis()->SetTitleFont(42);
    p2_3->GetXaxis()->SetRange(startplot,endplot);
    p2_3->GetZaxis()->SetLabelFont(42);
    p2_3->GetZaxis()->SetLabelSize(0.035);
    p2_3->GetZaxis()->SetTitleSize(0.035);
    p2_3->GetZaxis()->SetTitleFont(42);
    p2_3->Draw("sames");  

    gPad->Update();
    }//Second Profile
  }

  //Now we draw the fourth dataset
  if(filenum >= 4){
    profname[0] = '\0';
    strcat(profname, dataset4);
    strcat(profname, " ");
    strcat(profname, id1);

    p1_4->SetName(profname);
    p1_4->SetTitle(histtitle);
    p1_4->SetMaximum(yrange*1.3);

    p1_4->SetLineColor(prof1_4);
    p1_4->SetMarkerColor(prof1_4);
    p1_4->SetMarkerStyle(marker4);

    p1_4->GetXaxis()->SetTitle(xtitle);
    p1_4->GetXaxis()->SetLabelFont(42);
    p1_4->GetXaxis()->SetLabelSize(0.035);
    p1_4->GetXaxis()->SetTitleSize(0.045);
    p1_4->GetXaxis()->SetTitleFont(42);
    p1_4->GetYaxis()->SetTitle(ytitle);
    p1_4->GetYaxis()->SetLabelFont(42);
    p1_4->GetYaxis()->SetLabelSize(0.035);
    p1_4->GetYaxis()->SetTitleSize(0.045);
    p1_4->GetYaxis()->SetTitleFont(42);
    p1_4->GetXaxis()->SetRange(startplot,endplot);
    p1_4->GetZaxis()->SetLabelFont(42);
    p1_4->GetZaxis()->SetLabelSize(0.035);
    p1_4->GetZaxis()->SetTitleSize(0.035);
    p1_4->GetZaxis()->SetTitleFont(42);
    p1_4->Draw("sames");  

    gPad->Update();
    
    if(!single) {
    profname[0] = '\0';
    strcat(profname, dataset4);
    strcat(profname, " ");
    strcat(profname, id2);

    p2_4->SetName(profname);
    p2_4->SetTitle(histtitle);
    p2_4->SetMaximum(yrange*1.3);
    
    p2_4->SetLineColor(prof2_4);
    p2_4->SetMarkerColor(prof2_4);
    p2_4->SetMarkerStyle(marker4);
    p2_4->SetMarkerSize(.7);
    
    p2_4->GetXaxis()->SetTitle(xtitle);
    p2_4->GetXaxis()->SetLabelFont(42);
    p2_4->GetXaxis()->SetLabelSize(0.035);
    p2_4->GetXaxis()->SetTitleSize(0.045);
    p2_4->GetXaxis()->SetTitleFont(42);
    p2_4->GetYaxis()->SetTitle(ytitle);
    p2_4->GetYaxis()->SetLabelFont(42);
    p2_4->GetYaxis()->SetLabelSize(0.035);
    p2_4->GetYaxis()->SetTitleSize(0.045);
    p2_4->GetYaxis()->SetTitleFont(42);
    p2_4->GetXaxis()->SetRange(startplot,endplot);
    p2_4->GetZaxis()->SetLabelFont(42);
    p2_4->GetZaxis()->SetLabelSize(0.035);
    p2_4->GetZaxis()->SetTitleSize(0.035);
    p2_4->GetZaxis()->SetTitleFont(42);
    p2_4->Draw("sames");  

    gPad->Update();
    }//Second Profile
  }

 
  TLegend *leg = new TLegend(0.5043103,0.6666667,0.8821839,0.8776371,NULL,"brNDC");
  leg->SetBorderSize(1);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);
  leg->AddEntry(p1_1,p1_1->GetName(),"lp");
  if(!single) leg->AddEntry(p2_1,p2_1->GetName(),"lp");
  leg->AddEntry(p1_2,p1_2->GetName(),"lp");
  if(!single) leg->AddEntry(p2_2,p2_2->GetName(),"lp");

  if(filenum >= 3){
    leg->AddEntry(p1_3,p1_3->GetName(),"lp");
    if(!single) leg->AddEntry(p2_3,p2_3->GetName(),"lp");
  }

  if(filenum >= 4){
    leg->AddEntry(p1_4,p1_4->GetName(),"lp");
    if(!single) leg->AddEntry(p2_4,p2_4->GetName(),"lp");
  }
  
  leg->Draw();
  
  c->Modified();
  c->cd();
  c->SetSelected(c);
  
  TImage *img = TImage::Create();
  
  img->FromPad(c);
  img->WriteImage(outfile);
  

}
