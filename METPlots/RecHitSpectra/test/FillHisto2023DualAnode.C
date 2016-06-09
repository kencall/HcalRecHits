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
#include "TTreeReader.h"
#include "TChain.h"
#include "TProfile.h"
#include <cmath>
#include "TProfile2D.h"
#include "TH2D.h"


void FillHisto2023DualAnode(const char *infile, const char *outfile, const char *flag = "single", const char *oflag = "none"){

  float towerWidth[42] = {0,
			  -0.000+0.087, 
			  -0.087+0.174,
			  -0.174+0.261, 
			  -0.261+0.348, 
			  -0.348+0.435, 
			  -0.435+0.522,
			  -0.522+0.609,
			  -0.609+0.696,
			  -0.696+0.783,
			  -0.783+0.870,
			  -0.879+0.957,
			  -0.957+1.044,
			  -1.044+1.131,
			  -1.131+1.218,
			  -1.218+1.305,
			  -1.305+1.392,
			  -1.392+1.479,
			  -1.479+1.566,
			  -1.566+1.653,
			  -1.653+1.740,
			  -1.740+1.830,
			  -1.830+1.930,
			  -1.930+2.043,
			  -2.043+2.172,
			  -2.172+2.322,
			  -2.322+2.500,
			  -2.500+2.650,
			  -2.650+3.000,
			  -2.853+2.964,
			  -2.964+3.139,
			  -3.139+3.314,
			  -3.314+3.489,
			  -3.489+3.664,
			  -3.664+3.839,
			  -3.839+4.013,
			  -4.013+4.191,
			  -4.191+4.363,
			  -4.363+4.538,
			  -4.538+4.716,
			  -4.716+4.889,
			  -4.889+5.191};

  float towerCenter[42] = {0,
			  (0.000+0.087)/2, 
			  (0.087+0.174)/2,
			  (0.174+0.261)/2, 
			  (0.261+0.348)/2, 
			  (0.348+0.435)/2, 
			  (0.435+0.522)/2,
			  (0.522+0.609)/2,
			  (0.609+0.696)/2,
			  (0.696+0.783)/2,
			  (0.783+0.870)/2,
			  (0.879+0.957)/2,
			  (0.957+1.044)/2,
			  (1.044+1.131)/2,
			  (1.131+1.218)/2,
			  (1.218+1.305)/2,
			  (1.305+1.392)/2,
			  (1.392+1.479)/2,
			  (1.479+1.566)/2,
			  (1.566+1.653)/2,
			  (1.653+1.740)/2,
			  (1.740+1.830)/2,
			  (1.830+1.930)/2,
			  (1.930+2.043)/2,
			  (2.043+2.172)/2,
			  (2.172+2.322)/2,
			  (2.322+2.500)/2,
			  (2.500+2.650)/2,
			  (2.650+3.000)/2,
			  (2.853+2.964)/2,
			  (2.964+3.139)/2,
			  (3.139+3.314)/2,
			  (3.314+3.489)/2,
			  (3.489+3.664)/2,
			  (3.664+3.839)/2,
			  (3.839+4.013)/2,
			  (4.013+4.191)/2,
			  (4.191+4.363)/2,
			  (4.363+4.538)/2,
			  (4.538+4.716)/2,
			  (4.716+4.889)/2,
			  (4.889+5.191)/2};


  TTreeReader *tReader;

  bool byLumi = false;

  if(!strcmp(oflag,"byLumi")) byLumi = true;

  if(!strcmp(flag,"single")){

    TFile *f1 = new TFile(infile);
    if(f1->IsZombie()){
      cout << "Root file: " << infile << " not found!" << endl;
      return;
    }
    tReader = new TTreeReader("RecHitTree",f1);

  } else if(!strcmp(flag,"list")){

    ifstream in;
    in.open(infile);
    if(!in.is_open()){
      cout << "Cannot open list file: " << infile << endl;
      return;  
    }

    TChain *chain = new TChain("RecHitTree");
    
    string line;
    while(in.good()){
      if(!std::getline(in,line)) break; // We read a line from the file
      if(!chain->Add(line.c_str())){
	cout << "Problem loading tree from " << line << endl;
      }else{
	cout << "Adding file: " << line << "..." << endl;
      }
      
    }
    
    in.close();
    
    tReader = new TTreeReader((TTree *)chain);
    cout << "Finished loading files." << endl;
  } else {
    cout << "Unknown option: " << flag << endl;
    return;
  }

  TFile *f2 = new TFile(outfile, "NEW");
  if(f2->IsZombie()){
    cout << "Root file: " << outfile << " cannot be created!" << endl;
    return;
  }
      
   //Define the values to be calculated

  bool debug = false;
  int bin = 19;

  int nEvents = 0;

  int nRecHitsHB = 0;
  int nRecHitsHE = 0;
  int nRecHitsHF = 0;
//  int nRecHitsHO = 0;

  int nRecHitsHB1 = 0;
  int nRecHitsHB2 = 0;
  int nRecHitsHB3 = 0;
  int nRecHitsHB4 = 0;

// We wont produce any HE rechits right now.
//  int nRecHitsHE1 = 0;
//  int nRecHitsHE2 = 0;
//  int nRecHitsHE3 = 0;

  int nRecHitsHF1 = 0;
  int nRecHitsHF2 = 0;
  int nRecHitsHF3 = 0;
  int nRecHitsHF4 = 0;

  double recHitSum_ieta_plus[86];
  double recHitSum_ieta_minus[86];

  double recHitSumAllHF1_ieta_plus[86];
  double recHitSumAllHF1_ieta_minus[86];
  double recHitSumAllHF2_ieta_plus[86];
  double recHitSumAllHF2_ieta_minus[86];
  double recHitSumAllHF3_ieta_plus[86];
  double recHitSumAllHF3_ieta_minus[86];
  double recHitSumAllHF4_ieta_plus[86];
  double recHitSumAllHF4_ieta_minus[86];

  double recHitSumCut_ieta_plus[86];
  double recHitSumCut_ieta_minus[86];

  for(int i = 0; i < 86; i++){
    recHitSum_ieta_plus[i]=0;
    recHitSum_ieta_minus[i]=0;

    recHitSumAllHF1_ieta_plus[i]=0;
    recHitSumAllHF1_ieta_minus[i]=0;
    recHitSumAllHF2_ieta_plus[i]=0;
    recHitSumAllHF2_ieta_minus[i]=0;
    recHitSumAllHF3_ieta_plus[i]=0;
    recHitSumAllHF3_ieta_minus[i]=0;
    recHitSumAllHF4_ieta_plus[i]=0;
    recHitSumAllHF4_ieta_minus[i]=0;

    recHitSumCut_ieta_plus[i]=0;
    recHitSumCut_ieta_minus[i]=0;

  }

  //Define the branchs to be read
  //TTreeReaderValue<Int_t> numRecHits(*tReader, "nRecHits");

  TTreeReaderValue<Int_t> eventID(*tReader, "event");
  TTreeReaderValue<Int_t> lumiID(*tReader, "lumi");
  TTreeReaderValue<Int_t> runID(*tReader, "run");  

  TTreeReaderValue<std::vector<float>> readRecHitsHB_En(*tReader, "recHitHB_En");
  //TTreeReaderValue<std::vector<float>> readRecHitsHB_EnRAW(*tReader, "recHitHB_EnRAW");
  TTreeReaderValue<std::vector<int>> readRecHitsHB_ieta(*tReader, "recHitHB_ieta");
  TTreeReaderValue<std::vector<int>> readRecHitsHB_iphi(*tReader, "recHitHB_iphi");
  TTreeReaderValue<std::vector<int>> readRecHitsHB_depth(*tReader, "recHitHB_depth");
  TTreeReaderValue<std::vector<float>> readRecHitsHB_time(*tReader, "recHitHB_time");

  //TTreeReaderValue<std::vector<float>> readRecHitsHE1_En(*tReader, "recHitHE1_En");
  //TTreeReaderValue<std::vector<float>> readRecHitsHE1_EnRAW(*tReader, "recHitHE1_EnRAW");
  //TTreeReaderValue<std::vector<int>> readRecHitsHE1_ieta(*tReader, "recHitHE1_ieta");
  //TTreeReaderValue<std::vector<int>> readRecHitsHE1_iphi(*tReader, "recHitHE1_iphi");
  //TTreeReaderValue<std::vector<float>> readRecHitsHE1_time(*tReader, "recHitHE1_time");

  TTreeReaderValue<std::vector<float>> readRecHitsHF_En(*tReader, "recHitHF_En");
  TTreeReaderValue<std::vector<int>> readRecHitsHF_ieta(*tReader, "recHitHF_ieta");
  TTreeReaderValue<std::vector<int>> readRecHitsHF_iphi(*tReader, "recHitHF_iphi");
  TTreeReaderValue<std::vector<int>> readRecHitsHF_depth(*tReader, "recHitHF_depth");
  TTreeReaderValue<std::vector<float>> readRecHitsHF_time(*tReader, "recHitHF_time");

  //TTreeReaderValue<std::vector<float>> readRecHitsHO_En(*tReader, "recHitHO_En");
  //TTreeReaderValue<std::vector<int>> readRecHitsHO_ieta(*tReader, "recHitHO_ieta");
  //TTreeReaderValue<std::vector<int>> readRecHitsHO_iphi(*tReader, "recHitHO_iphi");
  //TTreeReaderValue<std::vector<float>> readRecHitsHO_time(*tReader, "recHitHO_time");

  f2->cd();

  //Useful definitions
  Double_t ieta_partitions[9] = {-21,-16,-12,-6,0,6,12,16,21};

  //Define the Histograms to be filled

  TH1D *h_nRecHits = new TH1D("nRecHits","nRecHits",760,0,3800);

  TH1D *h_nRecHitsHB = new TH1D("nRecHitsHB","nRecHitsHB",760,0,3800);
  //TH1D *h_nRecHitsHE = new TH1D("nRecHitsHE","nRecHitsHE",760,0,3800);
  TH1D *h_nRecHitsHF = new TH1D("nRecHitsHF","nRecHitsHF",760,0,3800);
  //TH1D *h_nRecHitsHO = new TH1D("nRecHitsHO","nRecHitsHO",760,0,3800);

  TH1D *h_nRecHitsHB1 = new TH1D("nRecHitsHB1","nRecHitsHB1",760,0,3800);
  TH1D *h_nRecHitsHB2 = new TH1D("nRecHitsHB2","nRecHitsHB2",760,0,3800);
  TH1D *h_nRecHitsHB3 = new TH1D("nRecHitsHB3","nRecHitsHB3",760,0,3800);
  TH1D *h_nRecHitsHB4 = new TH1D("nRecHitsHB4","nRecHitsHB4",760,0,3800);

  //TH1D *h_nRecHitsHE1 = new TH1D("nRecHitsHE1","nRecHitsHE1",760,0,3800);
  //TH1D *h_nRecHitsHE2 = new TH1D("nRecHitsHE2","nRecHitsHE2",760,0,3800);
  //TH1D *h_nRecHitsHE3 = new TH1D("nRecHitsHE3","nRecHitsHE3",760,0,3800);

  TH1D *h_nRecHitsHF1 = new TH1D("nRecHitsHF1","nRecHitsHF1",760,0,3800);
  TH1D *h_nRecHitsHF2 = new TH1D("nRecHitsHF2","nRecHitsHF2",760,0,3800);
  TH1D *h_nRecHitsHF3 = new TH1D("nRecHitsHF3","nRecHitsHF3",760,0,3800);
  TH1D *h_nRecHitsHF4 = new TH1D("nRecHitsHF4","nRecHitsHF4",760,0,3800);

  TH1D *h_nRecHitsHFLongDiff = new TH1D("nRecHitsHFLongDiff","nRecHitsHFLongDiff",101,-50.5,50.5);
  TH1D *h_nRecHitsHFShortDiff = new TH1D("nRecHitsHFShortDiff","nRecHitsHFShortDiff",101,-50.3,50.5);

  TH1D *h_nRecHitsHFLongDiffs = new TH1D("nRecHitsHFLongDiffs","nRecHitsHFLongDiffs",101,-50.5,50.5);
  TH1D *h_nRecHitsHFShortDiffs = new TH1D("nRecHitsHFShortDiffs","nRecHitsHFShortDiffs",101,-50.3,50.5);

  TH1D *h_nRecHitss = new TH1D("nRecHitss","nRecHitss",760,0,3800);

  TH1D *h_nRecHitsHBs = new TH1D("nRecHitsHBs","nRecHitsHBs",760,0,3800);
  //TH1D *h_nRecHitsHEs = new TH1D("nRecHitsHEs","nRecHitsHEs",760,0,3800);
  TH1D *h_nRecHitsHFs = new TH1D("nRecHitsHFs","nRecHitsHFs",760,0,3800);
  //TH1D *h_nRecHitsHOs = new TH1D("nRecHitsHOs","nRecHitsHOs",760,0,3800);

  TH1D *h_nRecHitsHB1s = new TH1D("nRecHitsHB1s","nRecHitsHB1s",760,0,3800);
  TH1D *h_nRecHitsHB2s = new TH1D("nRecHitsHB2s","nRecHitsHB2s",760,0,3800);
  TH1D *h_nRecHitsHB3s = new TH1D("nRecHitsHB3s","nRecHitsHB3s",760,0,3800);
  TH1D *h_nRecHitsHB4s = new TH1D("nRecHitsHB4s","nRecHitsHB4s",760,0,3800);

  //TH1D *h_nRecHitsHE1s = new TH1D("nRecHitsHE1s","nRecHitsHE1s",760,0,3800);
  //TH1D *h_nRecHitsHE2s = new TH1D("nRecHitsHE2s","nRecHitsHE2s",760,0,3800);
  //TH1D *h_nRecHitsHE3s = new TH1D("nRecHitsHE3s","nRecHitsHE3s",760,0,3800);

  TH1D *h_nRecHitsHF1s = new TH1D("nRecHitsHF1s","nRecHitsHF1s",760,0,3800);
  TH1D *h_nRecHitsHF2s = new TH1D("nRecHitsHF2s","nRecHitsHF2s",760,0,3800);
  TH1D *h_nRecHitsHF3s = new TH1D("nRecHitsHF3s","nRecHitsHF3s",760,0,3800);
  TH1D *h_nRecHitsHF4s = new TH1D("nRecHitsHF4s","nRecHitsHF4s",760,0,3800);

  TH1D *energyHB = new TH1D("energyHB","energyHB",60,-20,100);
  //TH1D *energyHE = new TH1D("energyHE","energyHE",110,-20,200);
  TH1D *energyHF = new TH1D("energyHF","energyHF",130,-100,1200);
  //TH1D *energyHO = new TH1D("energyHO","energyHO",60,-20,100);

  //TH1D *energyRAWHB = new TH1D("energyRAWHB","energyRAWHB",60,-20,100);
  //TH1D *energyRAWHE = new TH1D("energyRAWHE","energyRAWHE",110,-20,200);

  TH1D *energyCutHB = new TH1D("energyCutHB","energyCutHB",60,-20,100);
  //TH1D *energyCutHE = new TH1D("energyCutHE","energyCutHE",110,-20,200);
  TH1D *energyCutHF = new TH1D("energyCutHF","energyCutHF",130,-100,1200);
  //TH1D *energyCutHO = new TH1D("energyCutHO","energyCutHO",60,-20,100);

  TH1D *energyHB1 = new TH1D("energyHB1","energyHB1",60,-20,100);
  TH1D *energyHB2 = new TH1D("energyHB2","energyHB2",60,-20,100);
  TH1D *energyHB3 = new TH1D("energyHB3","energyHB3",60,-20,100);
  TH1D *energyHB4 = new TH1D("energyHB4","energyHB4",60,-20,100);

  //TH1D *energyHE1 = new TH1D("energyHE1","energyHE1",110,-20,200);
  //TH1D *energyHE2 = new TH1D("energyHE2","energyHE2",110,-20,200);
  //TH1D *energyHE3 = new TH1D("energyHE3","energyHE3",110,-20,200);

  //TH1D *energyRAWHB1 = new TH1D("energyRAWHB1","energyRAWHB1",60,-20,100);
  //TH1D *energyRAWHB2 = new TH1D("energyRAWHB2","energyRAWHB2",60,-20,100);

  //TH1D *energyRAWHE1 = new TH1D("energyRAWHE1","energyRAWHE1",110,-20,200);
  //TH1D *energyRAWHE2 = new TH1D("energyRAWHE2","energyRAWHE2",110,-20,200);
  //TH1D *energyRAWHE3 = new TH1D("energyRAWHE3","energyRAWHE3",110,-20,200);

  TH1D *energyHF1 = new TH1D("energyHF1","energyHF1",130,-100,1200);
  TH1D *energyHF2 = new TH1D("energyHF2","energyHF2",130,-100,1200);
  TH1D *energyHF3 = new TH1D("energyHF3","energyHF3",130,-100,1200);
  TH1D *energyHF4 = new TH1D("energyHF4","energyHF4",130,-100,1200);

  TH1D *energyHBs = new TH1D("energyHBs","energyHBs",60,-20,100);
  TH1D *energyHEs = new TH1D("energyHEs","energyHEs",110,-20,200);
  TH1D *energyHFs = new TH1D("energyHFs","energyHFs",130,-100,1200);
  TH1D *energyHOs = new TH1D("energyHOs","energyHOs",60,-20,100);

  //TH1D *energyRAWHBs = new TH1D("energyRAWHBs","energyRAWHBs",60,-20,100);
  //TH1D *energyRAWHEs = new TH1D("energyRAWHEs","energyRAWHEs",110,-20,200);

  TH1D *energyCutHBs = new TH1D("energyCutHBs","energyCutHBs",60,-20,100);
  //TH1D *energyCutHEs = new TH1D("energyCutHEs","energyCutHEs",110,-20,200);
  TH1D *energyCutHFs = new TH1D("energyCutHFs","energyCutHFs",130,-100,1200);
  //TH1D *energyCutHOs = new TH1D("energyCutHOs","energyCutHOs",60,-20,100);

  TH1D *energyHB1s = new TH1D("energyHB1s","energyHB1s",60,-20,100);
  TH1D *energyHB2s = new TH1D("energyHB2s","energyHB2s",60,-20,100);
  TH1D *energyHB3s = new TH1D("energyHB3s","energyHB3s",60,-20,100);
  TH1D *energyHB4s = new TH1D("energyHB4s","energyHB4s",60,-20,100);

  //TH1D *energyHE1s = new TH1D("energyHE1s","energyHE1s",110,-20,200);
  //TH1D *energyHE2s = new TH1D("energyHE2s","energyHE2s",110,-20,200);
  //TH1D *energyHE3s = new TH1D("energyHE3s","energyHE3s",110,-20,200);

  //TH1D *energyRAWHB1s = new TH1D("energyRAWHB1s","energyRAWHB1s",60,-20,100);
  //TH1D *energyRAWHB2s = new TH1D("energyRAWHB2s","energyRAWHB2s",60,-20,100);

  //TH1D *energyRAWHE1s = new TH1D("energyRAWHE1s","energyRAWHE1s",110,-20,200);
  //TH1D *energyRAWHE2s = new TH1D("energyRAWHE2s","energyRAWHE2s",110,-20,200);
  //TH1D *energyRAWHE3s = new TH1D("energyRAWHE3s","energyRAWHE3s",110,-20,200);

  TH1D *energyHF1s = new TH1D("energyHF1s","energyHF1s",130,-100,1200);
  TH1D *energyHF2s = new TH1D("energyHF2s","energyHF2s",130,-100,1200);
  TH1D *energyHF3s = new TH1D("energyHF3s","energyHF3s",130,-100,1200);
  TH1D *energyHF4s = new TH1D("energyHF4s","energyHF4s",130,-100,1200);

  TH1D *iphiHB1 = new TH1D("iphiHB1","iphiHB1",74,-0.5,73.5);	
  TH1D *iphiHB2 = new TH1D("iphiHB2","iphiHB2",74,-0.5,73.5);	
  TH1D *iphiHB3 = new TH1D("iphiHB3","iphiHB3",74,-0.5,73.5);	
  TH1D *iphiHB4 = new TH1D("iphiHB4","iphiHB4",74,-0.5,73.5);	

  TH1D *iphiHB1s = new TH1D("iphiHB1s","iphiHB1s",74,-0.5,73.5);	
  TH1D *iphiHB2s = new TH1D("iphiHB2s","iphiHB2s",74,-0.5,73.5);	
  TH1D *iphiHB3s = new TH1D("iphiHB3s","iphiHB3s",74,-0.5,73.5);	
  TH1D *iphiHB4s = new TH1D("iphiHB4s","iphiHB4s",74,-0.5,73.5);	

  TH1D *iphiHF1 = new TH1D("iphiHF1","iphiHF1",74,-0.5,73.5);	
  TH1D *iphiHF2 = new TH1D("iphiHF2","iphiHF2",74,-0.5,73.5);	
  TH1D *iphiHF3 = new TH1D("iphiHF3","iphiHF3",74,-0.5,73.5);	
  TH1D *iphiHF4 = new TH1D("iphiHF4","iphiHF4",74,-0.5,73.5);	

  TH1D *iphiHF1s = new TH1D("iphiHF1s","iphiHF1s",74,-0.5,73.5);	
  TH1D *iphiHF2s = new TH1D("iphiHF2s","iphiHF2s",74,-0.5,73.5);	
  TH1D *iphiHF3s = new TH1D("iphiHF3s","iphiHF3s",74,-0.5,73.5);	
  TH1D *iphiHF4s = new TH1D("iphiHF4s","iphiHF4s",74,-0.5,73.5);	

  TH1D *ietaHB1 = new TH1D("ietaHB1","ietaHB1",35,-17.5,17.5);
  TH1D *ietaHB2 = new TH1D("ietaHB2","ietaHB2",35,-17.5,17.5);
  TH1D *ietaHB3 = new TH1D("ietaHB3","ietaHB3",35,-17.5,17.5);
  TH1D *ietaHB4 = new TH1D("ietaHB4","ietaHB4",35,-17.5,17.5);

  TH1D *ietaHB1s = new TH1D("ietaHB1s","ietaHB1s",35,-17.5,17.5);
  TH1D *ietaHB2s = new TH1D("ietaHB2s","ietaHB2s",35,-17.5,17.5);
  TH1D *ietaHB3s = new TH1D("ietaHB3s","ietaHB3s",35,-17.5,17.5);
  TH1D *ietaHB4s = new TH1D("ietaHB4s","ietaHB4s",35,-17.5,17.5);

  TH1D *ietaHF1 = new TH1D("ietaHF1","ietaHF1",85,-42.5,42.5);
  TH1D *ietaHF2 = new TH1D("ietaHF2","ietaHF2",85,-42.5,42.5);
  TH1D *ietaHF3 = new TH1D("ietaHF3","ietaHF3",85,-42.5,42.5);
  TH1D *ietaHF4 = new TH1D("ietaHF4","ietaHF4",85,-42.5,42.5);

  TH1D *ietaHF1s = new TH1D("ietaHF1s","ietaHF1s",85,-42.5,42.5);
  TH1D *ietaHF2s = new TH1D("ietaHF2s","ietaHF2s",85,-42.5,42.5);
  TH1D *ietaHF3s = new TH1D("ietaHF3s","ietaHF3s",85,-42.5,42.5);
  TH1D *ietaHF4s = new TH1D("ietaHF4s","ietaHF4s",85,-42.5,42.5);

  TH1D *recHitNum_ieta = new TH1D("recHitNum_ieta","recHitNum_ieta", 83, -41.5, 41.5);
  TH1D *recHitCutNum_ieta = new TH1D("recHitNumCut_ieta","recHitNumCut_ieta", 83, -41.5, 41.5);

  TH1D *recHitNum_ieta_s = new TH1D("recHitNum_ieta_s","recHitNum_ieta_s", 83, -41.5, 41.5);
  TH1D *recHitCutNum_ieta_s = new TH1D("recHitNumCut_ieta_s","recHitNumCut_ieta_s", 83, -41.5, 41.5);

  TH1D *recHitTimingHB1 = new TH1D("recHitTimingHB1","recHitTimingHB1",70,-48,92);
  TH1D *recHitTimingHB2 = new TH1D("recHitTimingHB2","recHitTimingHB2",70,-48,92);
  TH1D *recHitTimingHB3 = new TH1D("recHitTimingHB3","recHitTimingHB3",70,-48,92);
  TH1D *recHitTimingHB4 = new TH1D("recHitTimingHB4","recHitTimingHB4",70,-48,92);

  //TH1D *recHitTimingHE1 = new TH1D("recHitTimingHE1","recHitTimingHE1",70,-48,92);
  //TH1D *recHitTimingHE2 = new TH1D("recHitTimingHE2","recHitTimingHE2",70,-48,92);
  //TH1D *recHitTimingHE3 = new TH1D("recHitTimingHE3","recHitTimingHE3",70,-48,92);

  TH1D *recHitTimingHB1s = new TH1D("recHitTimingHB1s","recHitTimingHB1s",70,-48,92);
  TH1D *recHitTimingHB2s = new TH1D("recHitTimingHB2s","recHitTimingHB2s",70,-48,92);
  TH1D *recHitTimingHB3s = new TH1D("recHitTimingHB3s","recHitTimingHB3s",70,-48,92);
  TH1D *recHitTimingHB4s = new TH1D("recHitTimingHB4s","recHitTimingHB4s",70,-48,92);

  //TH1D *recHitTimingHE1s = new TH1D("recHitTimingHE1s","recHitTimingHE1s",70,-48,92);
  //TH1D *recHitTimingHE2s = new TH1D("recHitTimingHE2s","recHitTimingHE2s",70,-48,92);
  //TH1D *recHitTimingHE3s = new TH1D("recHitTimingHE3s","recHitTimingHE3s",70,-48,92);

  TH1D *recHitTimingHF1 = new TH1D("recHitTimingHF1","recHitTimingHF1",70,-48,92);
  TH1D *recHitTimingHF2 = new TH1D("recHitTimingHF2","recHitTimingHF2",70,-48,92);
  TH1D *recHitTimingHF3 = new TH1D("recHitTimingHF3","recHitTimingHF3",70,-48,92);
  TH1D *recHitTimingHF4 = new TH1D("recHitTimingHF4","recHitTimingHF4",70,-48,92);

  //TH1D *recHitTimingHO = new TH1D("recHitTimingHO","recHitTimingHO",70,-48,92);

  TH1D *recHitTimingHF1s = new TH1D("recHitTimingHF1s","recHitTimingHF1s",70,-48,92);
  TH1D *recHitTimingHF2s = new TH1D("recHitTimingHF2s","recHitTimingHF2s",70,-48,92);
  TH1D *recHitTimingHF3s = new TH1D("recHitTimingHF3s","recHitTimingHF3s",70,-48,92);
  TH1D *recHitTimingHF4s = new TH1D("recHitTimingHF4s","recHitTimingHF4s",70,-48,92);

  //TH1D *recHitTimingHOs = new TH1D("recHitTimingHOs","recHitTimingHOs",70,-48,92);

  //Occupancy maps
  TH2D *recHitOccupancyHB1 = new TH2D("recHitOccupancyHB1","recHitOccupancyHB1",83,-41.5,41.5,73,-0.5,72.5);
  TH2D *recHitOccupancyHB2 = new TH2D("recHitOccupancyHB2","recHitOccupancyHB2",83,-41.5,41.5,73,-0.5,72.5);
  TH2D *recHitOccupancyHB3 = new TH2D("recHitOccupancyHB3","recHitOccupancyHB3",83,-41.5,41.5,73,-0.5,72.5);
  TH2D *recHitOccupancyHB4 = new TH2D("recHitOccupancyHB4","recHitOccupancyHB4",83,-41.5,41.5,73,-0.5,72.5);

  //TH2D *recHitOccupancyHE1 = new TH2D("recHitOccupancyHE1","recHitOccupancyHE1",83,-41.5,41.5,73,-0.5,72.5);
  //TH2D *recHitOccupancyHE2 = new TH2D("recHitOccupancyHE2","recHitOccupancyHE2",83,-41.5,41.5,73,-0.5,72.5);
  //TH2D *recHitOccupancyHE3 = new TH2D("recHitOccupancyHE3","recHitOccupancyHE3",83,-41.5,41.5,73,-0.5,72.5);

  TH2D *recHitOccupancyHF1 = new TH2D("recHitOccupancyHF1","recHitOccupancyHF1",83,-41.5,41.5,73,-0.5,72.5);
  TH2D *recHitOccupancyHF2 = new TH2D("recHitOccupancyHF2","recHitOccupancyHF2",83,-41.5,41.5,73,-0.5,72.5);
  TH2D *recHitOccupancyHF3 = new TH2D("recHitOccupancyHF3","recHitOccupancyHF3",83,-41.5,41.5,73,-0.5,72.5);
  TH2D *recHitOccupancyHF4 = new TH2D("recHitOccupancyHF4","recHitOccupancyHF4",83,-41.5,41.5,73,-0.5,72.5);

  //TH2D *recHitOccupancyHO = new TH2D("recHitOccupancyHO","recHitOccupancyHO",83,-41.5,41.5,73,-0.5,72.5);

  //TH2D *recHitHBM0M2 = new TH2D("recHitHBM0M2","recHitHBM0M2",2000,0,2000,2000,0,2000);
  //TH2D *recHitHEM0M2 = new TH2D("recHitHEM0M2","recHitHEM0M2",2000,0,2000,2000,0,2000);

  //TH2D *recHitHBM2M0Dist = new TH2D("recHitHBM2M0Dist","recHitHBM2M0Dist",2000,0,2000,100,0,2);
  //TH2D *recHitHEM2M0Dist = new TH2D("recHitHEM2M0Dist","recHitHEM2M0Dist",2000,0,2000,100,0,2);

  //TH2D *recHitHBMRatio5plus = new TH2D("recHitHBMRatio5plus","recHitHBMRatio5plus",30,0,30,100,0,2);
  //TH2D *recHitHBMRatio5minus = new TH2D("recHitHBMRatio5minus","recHitHBMRatio5minus",30,0,30,100,0,2);
  //TH2D *recHitHBMRatio5Part = new TH2D("recHitHBMRatio5etaPart","recHitHBMRatio5etaPart",8,ieta_partitions,100,0,2);

  //TH2D *recHitHBMRatio10plus = new TH2D("recHitHBMRatio10plus","recHitHBMRatio10plus",30,0,30,100,0,2);
  //TH2D *recHitHBMRatio10minus = new TH2D("recHitHBMRatio10minus","recHitHBMRatio10minus",30,0,30,100,0,2);
  //TH2D *recHitHBMRatio10Part = new TH2D("recHitHBMRatio10etaPart","recHitHBMRatio10etaPart",8,ieta_partitions,100,0,2);

  //TH2D *recHitHBMRatio20plus = new TH2D("recHitHBMRatio20plus","recHitHBMRatio20plus",30,0,30,100,0,2);
  //TH2D *recHitHBMRatio20minus = new TH2D("recHitHBMRatio20minus","recHitHBMRatio20minus",30,0,30,100,0,2);
  //TH2D *recHitHBMRatio20Part = new TH2D("recHitHBMRatio20etaPart","recHitHBMRatio20etaPart",8,ieta_partitions,100,0,2);

  //TH2D *recHitHEMRatio5plus = new TH2D("recHitHEMRatio5plus","recHitHEMRatio5plus",30,0,30,100,0,2);
  //TH2D *recHitHEMRatio5minus = new TH2D("recHitHEMRatio5minus","recHitHEMRatio5minus",30,0,30,100,0,2);
  //TH2D *recHitHEMRatio5Part = new TH2D("recHitHEMRatio5etaPart","recHitHEMRatio5etaPart",8,ieta_partitions,100,0,2);

  //TH2D *recHitHEMRatio10plus = new TH2D("recHitHEMRatio10plus","recHitHEMRatio10plus",30,0,30,100,0,2);
  //TH2D *recHitHEMRatio10minus = new TH2D("recHitHEMRatio10minus","recHitHEMRatio10minus",30,0,30,100,0,2);
  //TH2D *recHitHEMRatio10Part = new TH2D("recHitHEMRatio10etaPart","recHitHEMRatio10etaPart",8,ieta_partitions,100,0,2);

  //TH2D *recHitHEMRatio20plus = new TH2D("recHitHEMRatio20plus","recHitHEMRatio20plus",30,0,30,100,0,2);
  //TH2D *recHitHEMRatio20minus = new TH2D("recHitHEMRatio20minus","recHitHEMRatio20minus",30,0,30,100,0,2);
  //TH2D *recHitHEMRatio20Part = new TH2D("recHitHEMRatio20etaPart","recHitHEMRatio20etaPart",8,ieta_partitions,100,0,2);

  //TH2D *recHitHBM0M2Ratio = new TH2D("recHitHBM0M2Ratio","recHitHBM0M2Ratio",2000,0,2000,100,0,2);
  //TH2D *recHitHEM0M2Ratio = new TH2D("recHitHEM0M2Ratio","recHitHEM0M2Ratio",2000,0,2000,100,0,2);
  
  //Timing map

  TProfile2D *recHitTimingMapHB1 = new TProfile2D("recHitTimingMapHB1","recHitTimingMapHB1",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *recHitTimingMapHB2 = new TProfile2D("recHitTimingMapHB2","recHitTimingMapHB2",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *recHitTimingMapHB3 = new TProfile2D("recHitTimingMapHB3","recHitTimingMapHB3",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *recHitTimingMapHB4 = new TProfile2D("recHitTimingMapHB4","recHitTimingMapHB4",83, -41.5,41.5,73,-0.5,72.5);

  //TProfile2D *recHitTimingMapHE1 = new TProfile2D("recHitTimingMapHE1","recHitTimingMapHE1",83, -41.5,41.5,73,-0.5,72.5);
  //TProfile2D *recHitTimingMapHE2 = new TProfile2D("recHitTimingMapHE2","recHitTimingMapHE2",83, -41.5,41.5,73,-0.5,72.5);
  //TProfile2D *recHitTimingMapHE3 = new TProfile2D("recHitTimingMapHE3","recHitTimingMapHE3",83, -41.5,41.5,73,-0.5,72.5);

  TProfile2D *recHitTimingMapHF1 = new TProfile2D("recHitTimingMapHF1","recHitTimingMapHF1",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *recHitTimingMapHF2 = new TProfile2D("recHitTimingMapHF2","recHitTimingMapHF2",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *recHitTimingMapHF3 = new TProfile2D("recHitTimingMapHF3","recHitTimingMapHF3",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *recHitTimingMapHF4 = new TProfile2D("recHitTimingMapHF4","recHitTimingMapHF4",83, -41.5,41.5,73,-0.5,72.5);

  //TProfile2D *recHitTimingMapHO = new TProfile2D("recHitTimingMapHO","recHitTimingMapHO",83, -41.5,41.5,73,-0.5,72.5);

  //RecHit Energy map

  TProfile2D *energyMapHB1 = new TProfile2D("energyMapHB1","energyMapHB1",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *energyMapHB2 = new TProfile2D("energyMapHB2","energyMapHB2",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *energyMapHB3 = new TProfile2D("energyMapHB3","energyMapHB3",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *energyMapHB4 = new TProfile2D("energyMapHB4","energyMapHB4",83, -41.5,41.5,73,-0.5,72.5);

  //TProfile2D *energyMapHE1 = new TProfile2D("energyMapHE1","energyMapHE1",83, -41.5,41.5,73,-0.5,72.5);
  //TProfile2D *energyMapHE2 = new TProfile2D("energyMapHE2","energyMapHE2",83, -41.5,41.5,73,-0.5,72.5);
  //TProfile2D *energyMapHE3 = new TProfile2D("energyMapHE3","energyMapHE3",83, -41.5,41.5,73,-0.5,72.5);

  TProfile2D *energyMapHF1 = new TProfile2D("energyMapHF1","energyMapHF1",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *energyMapHF2 = new TProfile2D("energyMapHF2","energyMapHF2",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *energyMapHF3 = new TProfile2D("energyMapHF3","energyMapHF3",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *energyMapHF4 = new TProfile2D("energyMapHF4","energyMapHF4",83, -41.5,41.5,73,-0.5,72.5);

  //TProfile2D *energyMapHO = new TProfile2D("energyMapHO","energyMapHO",83, -41.5,41.5,73,-0.5,72.5);

  //Fiber Ratio (plus)
  //TProfile *fiberRatio_ieta_long5_plus = new TProfile("fiberRatio_ieta_long5_plus","fiberRatio_ieta_long5_plus",51,-0.5,50.5);
  //TProfile *fiberRatio_ieta_long10_plus = new TProfile("fiberRatio_ieta_long10_plus","fiberRatio_ieta_long10_plus",51,-0.5,50.5);
  //TProfile *fiberRatio_ieta_sum5_plus = new TProfile("fiberRatio_ieta_sum5_plus","fiberRatio_ieta_sum5_plus",51,-0.5,50.5);
  //TProfile *fiberRatio_ieta_sum10_plus = new TProfile("fiberRatio_ieta_sum10_plus","fiberRatio_ieta_sum10_plus",51,-0.5,50.5);
  //TProfile *fiberRatio_ieta_long5sum10_plus = new TProfile("fiberRatio_ieta_long5sum10_plus","fiberRatio_ieta_long5sum10_plus",51,-0.5,50.5);
  //TProfile *fiberRatio_ieta_sum5Threshold_plus = new TProfile("fiberRatio_ieta_sum5Threshold_plus","fiberRatio_ieta_sum5Threshold_plus",51,-0.5,50.5);
  //TProfile *fiberRatio_ieta_sum10Threshold_plus = new TProfile("fiberRatio_ieta_sum10Threshold_plus","fiberRatio_ieta_sum10Threshold_plus",51,-0.5,50.5);

  //Fiber Ratio (minus)
  //TProfile *fiberRatio_ieta_long5_minus = new TProfile("fiberRatio_ieta_long5_minus","fiberRatio_ieta_long5_minus",51,-0.5,50.5);
  //TProfile *fiberRatio_ieta_long10_minus = new TProfile("fiberRatio_ieta_long10_minus","fiberRatio_ieta_long10_minus",51,-0.5,50.5);
  //TProfile *fiberRatio_ieta_sum5_minus = new TProfile("fiberRatio_ieta_sum5_minus","fiberRatio_ieta_sum5_minus",51,-0.5,50.5);
  //TProfile *fiberRatio_ieta_sum10_minus = new TProfile("fiberRatio_ieta_sum10_minus","fiberRatio_ieta_sum10_minus",51,-0.5,50.5);
  //TProfile *fiberRatio_ieta_long5sum10_minus = new TProfile("fiberRatio_ieta_long5sum10_minus","fiberRatio_ieta_long5sum10_minus",51,-0.5,50.5);
  //TProfile *fiberRatio_ieta_sum5Threshold_minus = new TProfile("fiberRatio_ieta_sum5Threshold_minus","fiberRatio_ieta_sum5Threshold_minus",51,-0.5,50.5);
  //TProfile *fiberRatio_ieta_sum10Threshold_minus = new TProfile("fiberRatio_ieta_sum10Threshold_minus","fiberRatio_ieta_sum10Threshold_minus",51,-0.5,50.5);

  //TProfile *fiberRatio_ieta_4GeVMean_plus = new TProfile("fiberRatio_ieta_4GeVMean_plus","fiberRatio_ieta_4GeVMean_plus",51,-0.5,50.5);
  //TProfile *fiberRatio_ieta_4GeVMean_minus = new TProfile("fiberRatio_ieta_4GeVMean_minus","fiberRatio_ieta_4GeVMean_minus",51,-0.5,50.5);
  //TProfile *fiberRatio_ieta_4GeVMean = new TProfile("fiberRatio_ieta_4GeVMean","fiberRatio_ieta_4GeVMean",51,-0.5,50.5);

  TProfile *energyFlow_plus = new TProfile("energyFlow_plus","energyFlow_plus",46,-0.5,45.5);
  TProfile *energyFlow_minus = new TProfile("energyFlow_minus","energyFlow_minus",46,-0.5,45.5);
  TProfile *energyFlow_eta_plus = new TProfile("energyFlow_eta_plus","energyFlow_eta_plus",46,-0.5,45.5);
  TProfile *energyFlow_eta_minus = new TProfile("energyFlow_eta_minus","energyFlow_eta_minus",46,-0.5,45.5);
  TProfile *energyFlowEt_eta_plus = new TProfile("energyFlowEt_eta_plus","energyFlowEt_eta_plus",46,-0.5,45.5);
  TProfile *energyFlowEt_eta_minus = new TProfile("energyFlowEt_eta_minus","energyFlowEt_eta_minus",46,-0.5,45.5);

  TProfile *energyFlowCut_eta_plus = new TProfile("energyFlowCut_eta_plus","energyFlowCut_eta_plus",46,-0.5,45.5);
  TProfile *energyFlowCut_eta_minus = new TProfile("energyFlowCut_eta_minus","energyFlowCut_eta_minus",46,-0.5,45.5);
  TProfile *energyFlowCutEt_eta_plus = new TProfile("energyFlowCutEt_eta_plus","energyFlowCutEt_eta_plus",46,-0.5,45.5);
  TProfile *energyFlowCutEt_eta_minus = new TProfile("energyFlowCutEt_eta_minus","energyFlowCutEt_eta_minus",46,-0.5,45.5);

  //TProfile *caloFlowHad_plus = new TProfile("caloFlowHad_plus","caloFlowHad_plus",46,-0.5,45.5);
  //TProfile *caloFlowHad_minus = new TProfile("caloFlowHad_minus","caloFlowHad_minus",46,-0.5,45.5);
  //TProfile *caloFlowHad_eta_plus = new TProfile("caloFlowHad_eta_plus","caloFlowHad_eta_plus",46,-0.5,45.5);
  //TProfile *caloFlowHad_eta_minus = new TProfile("caloFlowHad_eta_minus","caloFlowHad_eta_minus",46,-0.5,45.5);

  //TProfile *caloFlowEm_plus = new TProfile("caloFlowEm_plus","caloFlowEm_plus",46,-0.5,45.5);
  //TProfile *caloFlowEm_minus = new TProfile("caloFlowEm_minus","caloFlowEm_minus",46,-0.5,45.5);
  //TProfile *caloFlowEm_eta_plus = new TProfile("caloFlowEm_eta_plus","caloFlowEm_eta_plus",46,-0.5,45.5);
  //TProfile *caloFlowEm_eta_minus = new TProfile("caloFlowEm_eta_minus","caloFlowEm_eta_minus",46,-0.5,45.5);

  //TProfile *HE_RecHitNum_ieta_plus = new TProfile("HE_RecHitNum_ieta_plus","HE_RecHitNum_ieta_plus",13,15.5,28.5);
  //TProfile *HE_RecHitMeanEnergy_ieta_plus = new TProfile("HE_RecHitMeanEnergy_ieta_plus","HE_RecHitMeanEnergy_ieta_plus",13,15.5,28.5);

  //TProfile *HE_RecHitCutNum_ieta_plus = new TProfile("HE_RecHitCutNum_ieta_plus","HE_RecHitCutNum_ieta_plus",13,15.5,28.5);
  //TProfile *HE_RecHitCutMeanEnergy_ieta_plus = new TProfile("HE_RecHitCutMeanEnergy_ieta_plus","HE_RecHitCutMeanEnergy_ieta_plus",13,15.5,28.5);

  int minEventNum = 0;
  int maxEventNum = 100;
  int lumisInRange = 100;
  int bestBinsNumber = 100;
  int minLumiBlock = 0;
  int maxLumiBlock = 100;

  if(byLumi){  
     minEventNum = (int)tReader->GetTree()->GetMinimum("event");
     maxEventNum = (int)tReader->GetTree()->GetMaximum("event");

    cout << "Minimum event number: " << minEventNum << endl;
    cout << "Maximum event number: " << maxEventNum << endl;

    //As is this lumi code may only work correctly for datasets confined to a single run
     minLumiBlock = (int)tReader->GetTree()->GetMinimum("lumi");
    maxLumiBlock = (int)tReader->GetTree()->GetMaximum("lumi");
    lumisInRange = maxLumiBlock - minLumiBlock + 1;

    //Find good number of bins
    int minRemainder = lumisInRange;
    int maxBinsNumber = 100; //The maximum number of bins that we want to consider
    int minBinsNumber = 75; //The minimum number of bins that we want to consider
    bestBinsNumber = maxBinsNumber; // This is updated as we test

    for(int i = maxBinsNumber; i >= minBinsNumber; i--){
      if(lumisInRange % i < minRemainder){
	bestBinsNumber = i;
	minRemainder = lumisInRange % i;
	cout << "Current best bins number: " << bestBinsNumber << " Remainder: " << minRemainder << endl;
      }
    }
  }

  //TProfile *meanNVertx_event = new TProfile("meanNVertx_event","meanNVertx_event",100,minEventNum,maxEventNum);
  //TProfile *meanNVertx_lumi = new TProfile("meanNVertx_lumi","meanNVertx_lumi",(lumisInRange < bestBinsNumber ? lumisInRange: bestBinsNumber), minLumiBlock, maxLumiBlock);
  //TH1D *oneVertxEvents_lumi = new TH1D("oneVertxEvents_lumi","oneVertxEvents_lumi",(lumisInRange < bestBinsNumber ? lumisInRange: bestBinsNumber), minLumiBlock, maxLumiBlock);
  

  int nRecHitsHE_ieta[13] = {0};
  int nRecHitsCutHE_ieta[13] = {0};

  cout << "Filling histograms..." << endl;
  
  //We loop over all of the events and fill the histograms
  int numRecHits = 0;
  int ieta = 0;
  float energy = 0;
  
  while(tReader->Next()){
    numRecHits = 0;
    //Reject any event where nVertx != 1
    //nVertxAll->Fill(*numVertices);
    //if(byLumi){
    //  meanNVertx_event->Fill(*eventID,*numVertices);
    //  meanNVertx_lumi->Fill(*lumiID,*numVertices);
    //  if(*numVertices == 1) oneVertxEvents_lumi->Fill(*lumiID);
    //}
    //Uncomment the next line for normalization to nVertx == 1
    //if(*numVertices != 1) continue;
    nEvents++;
    
    if(debug){
      if(nEvents > 100) break;
      cout << endl << "Event: " << nEvents << endl;
    }

    if(nEvents % 5000 == 0) cout << "Added " << nEvents << " events." << endl;
    //Let's fill a histogram with the nVertices, it should just be one, but we can catch any problems this way
    //nVertx->Fill(*numVertices);

    //numRecHits
    //h_nRecHits->Fill(*numRecHits);
    //h_nRecHitss->Fill(*numRecHits);

    //RecHit Energy HB
    
    nRecHitsHB = 0;
    nRecHitsHB1 = 0;
    nRecHitsHB2 = 0;
    nRecHitsHB3 = 0;
    nRecHitsHB4 = 0;

    for(int ridx = 0; ridx < (*readRecHitsHB_En).size(); ridx++){
      float energy = (*readRecHitsHB_En).at(ridx);
      //float enRaw = (*readRecHitsHB_EnRAW).at(ridx);

      //float MRatio = energy/enRaw;

      int depth = (*readRecHitsHB_depth).at(ridx);
      float time = (*readRecHitsHB_time).at(ridx);
      
      int ieta = (*readRecHitsHB_ieta).at(ridx);
      int iphi = (*readRecHitsHB_iphi).at(ridx);

      energyHB->Fill(energy);
      energyHBs->Fill(energy);
      if(energy > 0.7){
         energyCutHB->Fill(energy);
	 energyCutHBs->Fill(energy);
      }
      nRecHitsHB = nRecHitsHB + 1;
      numRecHits++;


      if(depth == 1){
         energyHB1->Fill(energy);
         energyHB1s->Fill(energy);

         recHitTimingHB1->Fill(time);
         recHitTimingHB1s->Fill(time);

         recHitOccupancyHB1->Fill(ieta,iphi);
         recHitTimingMapHB1->Fill(ieta,iphi,time);
         energyMapHB1->Fill(ieta,iphi,energy);

         iphiHB1->Fill(iphi);
         iphiHB1s->Fill(iphi);
         ietaHB1->Fill(ieta);
         ietaHB1s->Fill(ieta);

         nRecHitsHB1 = nRecHitsHB1 + 1;
      }

      if(depth == 2){
         energyHB2->Fill(energy);
         energyHB2s->Fill(energy);

         recHitTimingHB2->Fill(time);
         recHitTimingHB2s->Fill(time);

         recHitOccupancyHB2->Fill(ieta,iphi);
         recHitTimingMapHB2->Fill(ieta,iphi,time);
         energyMapHB2->Fill(ieta,iphi,energy);

         iphiHB2->Fill(iphi);
         iphiHB2s->Fill(iphi);
         ietaHB2->Fill(ieta);
         ietaHB2s->Fill(ieta);

         nRecHitsHB2 = nRecHitsHB2 + 1;
      }

      if(depth == 3){
         energyHB3->Fill(energy);
         energyHB3s->Fill(energy);

         recHitTimingHB3->Fill(time);
         recHitTimingHB3s->Fill(time);

         recHitOccupancyHB3->Fill(ieta,iphi);
         recHitTimingMapHB3->Fill(ieta,iphi,time);
         energyMapHB3->Fill(ieta,iphi,energy);

         iphiHB3->Fill(iphi);
         iphiHB3s->Fill(iphi);
         ietaHB3->Fill(ieta);
         ietaHB3s->Fill(ieta);

         nRecHitsHB3 = nRecHitsHB3 + 1;
      }

      if(depth == 4){
         energyHB4->Fill(energy);
         energyHB4s->Fill(energy);

         recHitTimingHB4->Fill(time);
         recHitTimingHB4s->Fill(time);

         recHitOccupancyHB4->Fill(ieta,iphi);
         recHitTimingMapHB4->Fill(ieta,iphi,time);
         energyMapHB4->Fill(ieta,iphi,energy);

         iphiHB4->Fill(iphi);
         iphiHB4s->Fill(iphi);
         ietaHB4->Fill(ieta);
         ietaHB4s->Fill(ieta);

         nRecHitsHB4 = nRecHitsHB4 + 1;
      }

      recHitNum_ieta->Fill(ieta);
      recHitNum_ieta_s->Fill(ieta);

      if(energy > 0.7){
	recHitCutNum_ieta->Fill(ieta);
	recHitCutNum_ieta_s->Fill(ieta);
      }

      if(ieta > 0){
	recHitSum_ieta_plus[ieta] += energy;
	if(energy > 0.7){
	  recHitSumCut_ieta_plus[ieta] += energy;
	}
      }
      if(ieta < 0){
	recHitSum_ieta_minus[-ieta] += energy;
	if(energy > 0.7){
	  recHitSumCut_ieta_minus[-ieta] += energy;
	}
      }
    }

     h_nRecHitsHB->Fill(nRecHitsHB);
     h_nRecHitsHBs->Fill(nRecHitsHB);

     h_nRecHitsHB1->Fill(nRecHitsHB1);
     h_nRecHitsHB1s->Fill(nRecHitsHB1);
     h_nRecHitsHB2->Fill(nRecHitsHB2);
     h_nRecHitsHB2s->Fill(nRecHitsHB2);
     h_nRecHitsHB3->Fill(nRecHitsHB3);
     h_nRecHitsHB3s->Fill(nRecHitsHB3);
     h_nRecHitsHB4->Fill(nRecHitsHB4);
     h_nRecHitsHB4s->Fill(nRecHitsHB4);


    //RecHit Energy HF
    nRecHitsHF = 0;

    nRecHitsHF1 = 0;
    nRecHitsHF2 = 0;
    nRecHitsHF3 = 0;
    nRecHitsHF4 = 0;

    for(int ridx = 0; ridx < (*readRecHitsHF_En).size(); ridx++){
      float energy = (*readRecHitsHF_En).at(ridx);

      int depth = (*readRecHitsHF_depth).at(ridx);
      float time = (*readRecHitsHF_time).at(ridx);
      
      int ieta = (*readRecHitsHF_ieta).at(ridx);
      int iphi = (*readRecHitsHF_iphi).at(ridx);

      energyHF->Fill(energy);
      energyHFs->Fill(energy);

      if(energy > 0.5){
	energyCutHF->Fill(energy);
	energyCutHFs->Fill(energy);
      }

      numRecHits++;
      nRecHitsHF++;


      if(depth == 1){
         energyHF1->Fill(energy);
         energyHF1s->Fill(energy);

         recHitTimingHF1->Fill(time);
         recHitTimingHF1s->Fill(time);

         recHitOccupancyHF1->Fill(ieta,iphi);
         recHitTimingMapHF1->Fill(ieta,iphi,time);
         energyMapHF1->Fill(ieta,iphi,energy);

         iphiHF1->Fill(iphi);
         iphiHF1s->Fill(iphi);
         ietaHF1->Fill(ieta);
         ietaHF1s->Fill(ieta);

         nRecHitsHF1 = nRecHitsHF1 + 1;

      }         

      if(depth == 2){
         energyHF2->Fill(energy);
         energyHF2s->Fill(energy);

         recHitTimingHF2->Fill(time);
         recHitTimingHF2s->Fill(time);

         recHitOccupancyHF2->Fill(ieta,iphi);
         recHitTimingMapHF2->Fill(ieta,iphi,time);
         energyMapHF2->Fill(ieta,iphi,energy);

         iphiHF2->Fill(iphi);
         iphiHF2s->Fill(iphi);
         ietaHF2->Fill(ieta);
         ietaHF2s->Fill(ieta);

         nRecHitsHF2 = nRecHitsHF2 + 1;

      }         

      if(depth == 3){
         energyHF3->Fill(energy);
         energyHF3s->Fill(energy);

         recHitTimingHF3->Fill(time);
         recHitTimingHF3s->Fill(time);

         recHitOccupancyHF3->Fill(ieta,iphi);
         recHitTimingMapHF3->Fill(ieta,iphi,time);
         energyMapHF3->Fill(ieta,iphi,energy);

         iphiHF3->Fill(iphi);
         iphiHF3s->Fill(iphi);
         ietaHF3->Fill(ieta);
         ietaHF3s->Fill(ieta);

         nRecHitsHF3 = nRecHitsHF3 + 1;

      }         

      if(depth == 4){
         energyHF4->Fill(energy);
         energyHF4s->Fill(energy);

         recHitTimingHF4->Fill(time);
         recHitTimingHF4s->Fill(time);

         recHitOccupancyHF4->Fill(ieta,iphi);
         recHitTimingMapHF4->Fill(ieta,iphi,time);
         energyMapHF4->Fill(ieta,iphi,energy);

         iphiHF4->Fill(iphi);
         iphiHF4s->Fill(iphi);
         ietaHF4->Fill(ieta);
         ietaHF4s->Fill(ieta);

         nRecHitsHF4 = nRecHitsHF4 + 1;

      }         

      recHitNum_ieta->Fill(ieta);
      recHitNum_ieta_s->Fill(ieta);
      if(energy > 0.5){
	recHitCutNum_ieta->Fill(ieta);
	recHitCutNum_ieta_s->Fill(ieta);
      }
      if(ieta > 0){
	recHitSum_ieta_plus[ieta] += energy;
	if(energy > 0.5) recHitSumCut_ieta_plus[ieta] += energy;
      }
      if(ieta < 0){
	recHitSum_ieta_minus[-ieta] += energy;
	if(energy > 0.5) recHitSumCut_ieta_minus[-ieta] += energy;
      }

    }

    h_nRecHitsHF->Fill(nRecHitsHF);
    h_nRecHitsHFs->Fill(nRecHitsHF);

    h_nRecHitsHF1->Fill(nRecHitsHF1);
    h_nRecHitsHF1s->Fill(nRecHitsHF1);
    h_nRecHitsHF2->Fill(nRecHitsHF2);
    h_nRecHitsHF2s->Fill(nRecHitsHF2);
    h_nRecHitsHF3->Fill(nRecHitsHF3);
    h_nRecHitsHF3s->Fill(nRecHitsHF3);
    h_nRecHitsHF4->Fill(nRecHitsHF4);
    h_nRecHitsHF4s->Fill(nRecHitsHF4);

    h_nRecHitsHFLongDiff->Fill(nRecHitsHF1-nRecHitsHF3);
    h_nRecHitsHFShortDiff->Fill(nRecHitsHF2-nRecHitsHF4);

    h_nRecHitsHFLongDiffs->Fill(nRecHitsHF1-nRecHitsHF3);
    h_nRecHitsHFShortDiffs->Fill(nRecHitsHF2-nRecHitsHF4);

    h_nRecHits->Fill(numRecHits);
    h_nRecHitss->Fill(numRecHits);


    float theta;

    for(int i = 1; i < 86; i++){
      if(true){
	energyFlow_plus->Fill(i,recHitSum_ieta_plus[i]);
	
	if(i < 42){
	  theta = 2*atan(exp(-towerCenter[i]));
	  energyFlow_eta_plus->Fill(i,recHitSum_ieta_plus[i]/towerWidth[i]);
	  if(debug && (bin == i)) cout << "RecHit Sum (All): " << recHitSum_ieta_plus[i] << endl;
	  energyFlowEt_eta_plus->Fill(i,recHitSum_ieta_plus[i]*sin(theta)/towerWidth[i]);

	  energyFlowCut_eta_plus->Fill(i,recHitSumCut_ieta_plus[i]/towerWidth[i]);
	  energyFlowCutEt_eta_plus->Fill(i,recHitSumCut_ieta_plus[i]*sin(theta)/towerWidth[i]);
	  if (debug && (bin == i)) cout << "RecHit Fill: " << recHitSumCut_ieta_plus[i]*sin(theta)/towerWidth[i] << endl;
	}
      }
      if(true){
	energyFlow_minus->Fill(i,recHitSum_ieta_minus[i]);
	if(i < 42){
	  theta = 2*atan(exp(-towerCenter[i]));
	  energyFlow_eta_minus->Fill(i,recHitSum_ieta_minus[i]/towerWidth[i]);

	  energyFlowEt_eta_minus->Fill(i,recHitSum_ieta_minus[i]*sin(theta)/towerWidth[i]);

	  energyFlowCut_eta_minus->Fill(i,recHitSumCut_ieta_minus[i]/towerWidth[i]);
	  energyFlowCutEt_eta_minus->Fill(i,recHitSumCut_ieta_minus[i]*sin(theta)/towerWidth[i]);
	}
      }

      recHitSum_ieta_plus[i] = 0;
      recHitSum_ieta_minus[i] = 0;

      recHitSumCut_ieta_plus[i] = 0;
      recHitSumCut_ieta_minus[i] = 0;

    }

  } //Fill Histograms

  energyHBs->Scale(1/(float)nEvents);
  //energyHEs->Scale(1/(float)nEvents);
  energyHFs->Scale(1/(float)nEvents);
  //energyHOs->Scale(1/(float)nEvents);

  //energyRAWHBs->Scale(1/(float)nEvents);
  //energyRAWHEs->Scale(1/(float)nEvents);

  energyCutHBs->Scale(1/(float)nEvents);
  //energyCutHEs->Scale(1/(float)nEvents);
  energyCutHFs->Scale(1/(float)nEvents);
  //energyCutHOs->Scale(1/(float)nEvents);

  energyHB1s->Scale(1/(float)nEvents);
  energyHB2s->Scale(1/(float)nEvents);
  energyHB3s->Scale(1/(float)nEvents);
  energyHB4s->Scale(1/(float)nEvents);

  //energyHE1s->Scale(1/(float)nEvents);
  //energyHE2s->Scale(1/(float)nEvents);
  //energyHE3s->Scale(1/(float)nEvents);

  energyHF1s->Scale(1/(float)nEvents);
  energyHF2s->Scale(1/(float)nEvents);
  energyHF3s->Scale(1/(float)nEvents);
  energyHF4s->Scale(1/(float)nEvents);

  //energyRAWHB1s->Scale(1/(float)nEvents);
  //energyRAWHB2s->Scale(1/(float)nEvents);

  //energyRAWHE1s->Scale(1/(float)nEvents);
  //energyRAWHE2s->Scale(1/(float)nEvents);
  //energyRAWHE3s->Scale(1/(float)nEvents);

  recHitTimingHB1s->Scale(1/(float)nEvents);
  recHitTimingHB2s->Scale(1/(float)nEvents);
  recHitTimingHB3s->Scale(1/(float)nEvents);
  recHitTimingHB4s->Scale(1/(float)nEvents);

  //recHitTimingHE1s->Scale(1/(float)nEvents);
  //recHitTimingHE2s->Scale(1/(float)nEvents);
  //recHitTimingHE3s->Scale(1/(float)nEvents);

  h_nRecHitss->Scale(1/(float)nEvents);

  h_nRecHitsHBs->Scale(1/(float)nEvents);
  //h_nRecHitsHEs->Scale(1/(float)nEvents);
  h_nRecHitsHFs->Scale(1/(float)nEvents);
  //h_nRecHitsHOs->Scale(1/(float)nEvents);

  h_nRecHitsHB1s->Scale(1/(float)nEvents);
  h_nRecHitsHB2s->Scale(1/(float)nEvents);
  h_nRecHitsHB3s->Scale(1/(float)nEvents);
  h_nRecHitsHB4s->Scale(1/(float)nEvents);

  //h_nRecHitsHE1s->Scale(1/(float)nEvents);
  //h_nRecHitsHE2s->Scale(1/(float)nEvents);
  //h_nRecHitsHE3s->Scale(1/(float)nEvents);

  h_nRecHitsHF1s->Scale(1/(float)nEvents);
  h_nRecHitsHF2s->Scale(1/(float)nEvents);
  h_nRecHitsHF3s->Scale(1/(float)nEvents);
  h_nRecHitsHF4s->Scale(1/(float)nEvents);

  h_nRecHitsHFLongDiffs->Scale(1/(float)nEvents);
  h_nRecHitsHFShortDiffs->Scale(1/(float)nEvents);

  recHitNum_ieta_s->Scale(1/(float)nEvents);
  recHitCutNum_ieta_s->Scale(1/(float)nEvents);

  iphiHB1s->Scale(1/(float)nEvents);
  iphiHB2s->Scale(1/(float)nEvents);
  iphiHB3s->Scale(1/(float)nEvents);
  iphiHB4s->Scale(1/(float)nEvents);

  iphiHF1s->Scale(1/(float)nEvents);
  iphiHF2s->Scale(1/(float)nEvents);
  iphiHF3s->Scale(1/(float)nEvents);
  iphiHF4s->Scale(1/(float)nEvents);

  ietaHB1s->Scale(1/(float)nEvents);
  ietaHB2s->Scale(1/(float)nEvents);
  ietaHB3s->Scale(1/(float)nEvents);
  ietaHB4s->Scale(1/(float)nEvents);

  ietaHF1s->Scale(1/(float)nEvents);
  ietaHF2s->Scale(1/(float)nEvents);
  ietaHF3s->Scale(1/(float)nEvents);
  ietaHF4s->Scale(1/(float)nEvents);

  f2->Write();

}
