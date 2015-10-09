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
#include "TImage.h"
#include <map>
#include "TKey.h"
#include "TClass.h"


class DirectoryFinder
{
private:
    std::map<std::string, TDirectory*> ptdMap;
    TDirectory* findDirectory( TDirectory *target, std::string& s);
public:
    TDirectory* operator()(TDirectory *target, std::string& s);
} dfRef, dfVal, dfRefn, dfValn;

void drawRatio(const char *file1, const char *file2,
	       const char *histname, const char *histtitle, const char *xtitle,  const char *ytitle, const char *outfile, const char *normToHist = "none"){
		    



  bool normed = false;
  if(strcmp(normToHist,"none")) normed = true;

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

   Int_t marker1 = 6;
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

    //split directory off histName 
  std::string histName = std::string(histname);
    int slashLoc = histName.rfind("/");
    std::string histDir = histName.substr(0, slashLoc);
    if(slashLoc < histName.size() - 1) histName = histName.substr(slashLoc + 1, histName.size());

    std::string histNamen;
    std::string histDirn;
    if(normed){
      histNamen = std::string(normToHist);
      slashLoc = histNamen.rfind("/");
      histDirn = histNamen.substr(0, slashLoc);
      if(slashLoc < histNamen.size() - 1) histNamen = histNamen.substr(slashLoc + 1, histNamen.size());
    }

    //Get objects from TFiles
    TDirectory *TD1 = dfRef(f1, histDir);
    TDirectory *TD2 = dfVal(f2, histDir);

    TDirectory *TD1n;
     TDirectory *TD2n;
     if(normed){
       TD1n = dfRefn(f1, histDirn); 
       TD2n = dfValn(f2, histDirn);
     }
  
    TH1F *h1;
    TH1F *h2;

    TH1F *h1n;
    TH1F *h2n;

    double scale1, scale2;

    if(normed){
    if(TD1n) 
    {
      h1n = (TH1F*)TD1n->Get(histNamen.c_str());
      if(h1n){
	h1n = (TH1F*)h1n->Clone();
	scale1 = 1/h1n->Integral();
	cout << "Event count 1: " << h1n->Integral() << endl;
	      } else {
	cout << "Failed to load " << normToHist << " from " << file2 << "!" << endl;
	return;
      }
    }
    else 
    {
	std::cout << "Cannot find directory \"" << histDirn << "\" in file \"" << f1->GetName() << "\"" << std::endl;
	return;
    }

    if(TD2n) 
    {
      h2n = (TH1F*)TD2n->Get(histNamen.c_str());
      if(h2n){
	h2n = (TH1F*)h2n->Clone();
	scale2 = 1/h2n->Integral();
	cout << "Event count 2: " <<  h2n->Integral() << endl;
	} else {
	cout << "Failed to load " << normToHist << " from " << file2 << "!";
	return;
      }
    }
    else 
    {
	std::cout << "Cannot find directory \"" << histDirn << "\" in file \"" << f2->GetName() << "\"" << std::endl;
	return;
    }
    }

    if(TD1) 
    {
      h1 = (TH1F*)TD1->Get(histName.c_str());
      if(h1){
	h1 = (TH1F*)h1->Clone();
	h1->Sumw2();
	if(normed) h1->Scale(scale1);
      } else {
	cout << "Failed to load " << histname << " from " << file2 << "!" << endl;
	return;
      }
    }
    else 
    {
	std::cout << "Cannot find directory \"" << histDir << "\" in file \"" << f1->GetName() << "\"" << std::endl;
	return;
    }

    if(TD2) 
    {
      h2 = (TH1F*)TD2->Get(histName.c_str());
      if(h2){
	h2 = (TH1F*)h2->Clone();
	h2->Sumw2();
	if(normed) h2->Scale(scale2);
	
      } else {
	cout << "Failed to load " << histname << " from " << file2 << "!";
	return;
      }
    }
    else 
    {
	std::cout << "Cannot find directory \"" << histDir << "\" in file \"" << f2->GetName() << "\"" << std::endl;
	return;
    }


 cout << "Done opening root files" << endl;

 if(!h1->Divide(h2)) cout << "Division operation failed" << endl;


 int startplot = h1->FindFirstBinAbove(0.00005);

 int endplot = 400; h1->FindLastBinAbove(0.00005);

 float yrange = h1->GetBinContent(h1->GetMaximumBin());

 float ymin = 100;
 for(int bIdx = 1; bIdx < h1->GetNbinsX(); bIdx++){
   float test = h1->GetBinContent(bIdx);

   if(test > 0.2 && test < ymin) ymin = test;
 }

 yrange = (yrange > 5 ? 5 : yrange);

 float extra = (yrange - ymin)/10;

 cout << "Ymax: " << yrange << endl;
 cout << "Ymin: " << ymin << endl;
 cout << "View: " << extra << endl;

  //Start Drawing

 gStyle->SetOptTitle(1);

  h1->SetName(histname);
  h1->SetTitle(histtitle);
  h1->SetMaximum(yrange+extra);
  h1->SetMinimum(ymin-extra <= 0 ? 0.1 : ymin-extra);

  h1->SetLineColor(prof1_1);
  h1->SetMarkerColor(prof1_1);
  h1->SetMarkerStyle(marker1);

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

  gPad->Update();
  
  c->Modified();
  c->cd();
  c->SetSelected(c);
  
  TImage *img = TImage::Create();
  
  img->FromPad(c);
  img->WriteImage(outfile);
  

}

TDirectory* DirectoryFinder::operator()(TDirectory *target, std::string& s)
{
    if(ptdMap.find(s) == ptdMap.end()) return (ptdMap[s] = findDirectory(target, s));
    else                               return ptdMap[s];
}

TDirectory* DirectoryFinder::findDirectory( TDirectory *target, std::string& s)
{
    TDirectory *retval = 0;

    // loop over all keys in this directory                                                                                                                                                                                                  
    TIter nextkey(target->GetListOfKeys());
    TKey *key, *oldkey=0;
    while((key = (TKey*)nextkey()))
    {
	//keep only the highest cycle number for each key                                                                                                                                                                                    
	if (oldkey && !strcmp(oldkey->GetName(),key->GetName())) continue;

	// read object from file                                                                                                                                                                                                             
//	target->cd();
	TObject *obj = key->ReadObj();
//	obj->Print();

	if(obj->IsA()->InheritsFrom(TDirectory::Class()))
	{
	    // it's a subdirectory                                                                                                                                                                                                           
	    //std::cout << "Found subdirectory " << obj->GetName() << std::endl;

	    if(strcmp(s.c_str(), obj->GetName()) == 0) return (TDirectory*)obj;

	    if((retval = findDirectory((TDirectory*)obj, s))) break;

	}
	else
	{
	    break;
	}
    }

    return retval;
}
