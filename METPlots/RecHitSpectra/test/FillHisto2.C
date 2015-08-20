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


void FillHisto2(const char *infile, const char *outfile, const char *flag = "single"){

  /*1 0.000 0.087 
2 0.087 0.174
3 0.174 0.261 
4 0.261 0.348 
5 0.348 0.435 
6 0.435 0.522
7 0.522 0.609 
8 0.609 0.696
9 0.696 0.783
10 0.783 0.870
11 0.879 0.957
12 0.957 1.044
13 1.044 1.131
14 1.131 1.218
15 1.218 1.305
16 1.305 1.392
17 1.392 1.479
18 1.479 1.566
19 1.566 1.653
20 1.653 1.740
21 1.740 1.830
22 1.830 1.930
23 1.930 2.043
24 2.043 2.172
25 2.172 2.322
26 2.322 2.500
27 2.500 2.650
28 2.650 3.000
29 2.853 2.964
30 2.964 3.139
31 3.139 3.314
32 3.314 3.489
33 3.489 3.664
34 3.664 3.839
35 3.839 4.013
36 4.013 4.191
37 4.191 4.363
38 4.363 4.538
39 4.538 4.716
40 4.716 4.889
41 4.889 5.191*/

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
  int nRecHitsHO = 0;

  int nRecHitsHB1 = 0;
  int nRecHitsHB2 = 0;

  int nRecHitsHE1 = 0;
  int nRecHitsHE2 = 0;
  int nRecHitsHE3 = 0;

  int nRecHitsHF1 = 0;
  int nRecHitsHF2 = 0;

  double recHitSum_ieta_plus[86];
  double recHitSum_ieta_minus[86];

  double recHitSumAllHF1_ieta_plus[86];
  double recHitSumAllHF1_ieta_minus[86];
  double recHitSumAllHF2_ieta_plus[86];
  double recHitSumAllHF2_ieta_minus[86];

  double recHitSumCut_ieta_plus[86];
  double recHitSumCut_ieta_minus[86];

  double caloHad_ieta_plus[86];
  double caloHad_ieta_minus[86];

  double caloEm_ieta_plus[86];
  double caloEm_ieta_minus[86];

  for(int i = 0; i < 86; i++){
    recHitSum_ieta_plus[i]=0;
    recHitSum_ieta_minus[i]=0;

    recHitSumAllHF1_ieta_plus[i]=0;
    recHitSumAllHF1_ieta_minus[i]=0;
    recHitSumAllHF2_ieta_plus[i]=0;
    recHitSumAllHF2_ieta_minus[i]=0;

    recHitSumCut_ieta_plus[i]=0;
    recHitSumCut_ieta_minus[i]=0;

    caloHad_ieta_plus[i]=0;
    caloHad_ieta_minus[i]=0;

    caloEm_ieta_plus[i]=0;
    caloEm_ieta_minus[i]=0;
  }

  //Define the branchs to be read
  //TTreeReaderValue<Int_t> numRecHits(*tReader, "nRecHits");
  TTreeReaderValue<Int_t> numVertices(*tReader, "nVertices");

  TTreeReaderValue<Int_t> eventID(*tReader, "event");
  TTreeReaderValue<Int_t> lumiID(*tReader, "lumi");
  TTreeReaderValue<Int_t> runID(*tReader, "run");  

  TTreeReaderValue<std::vector<float>> readRecHitsHB1_En(*tReader, "recHitHB1_En");
  TTreeReaderValue<std::vector<float>> readRecHitsHB1_EnRAW(*tReader, "recHitHB1_EnRAW");
  TTreeReaderValue<std::vector<int>> readRecHitsHB1_ieta(*tReader, "recHitHB1_ieta");
  TTreeReaderValue<std::vector<int>> readRecHitsHB1_iphi(*tReader, "recHitHB1_iphi");
  TTreeReaderValue<std::vector<float>> readRecHitsHB1_time(*tReader, "recHitHB1_time");

  TTreeReaderValue<std::vector<float>> readRecHitsHB2_En(*tReader, "recHitHB2_En");
  TTreeReaderValue<std::vector<float>> readRecHitsHB2_EnRAW(*tReader, "recHitHB2_EnRAW");
  TTreeReaderValue<std::vector<int>> readRecHitsHB2_ieta(*tReader, "recHitHB2_ieta");
  TTreeReaderValue<std::vector<int>> readRecHitsHB2_iphi(*tReader, "recHitHB2_iphi");
  TTreeReaderValue<std::vector<float>> readRecHitsHB2_time(*tReader, "recHitHB2_time");


  TTreeReaderValue<std::vector<float>> readRecHitsHE1_En(*tReader, "recHitHE1_En");
  TTreeReaderValue<std::vector<float>> readRecHitsHE1_EnRAW(*tReader, "recHitHE1_EnRAW");
  TTreeReaderValue<std::vector<int>> readRecHitsHE1_ieta(*tReader, "recHitHE1_ieta");
  TTreeReaderValue<std::vector<int>> readRecHitsHE1_iphi(*tReader, "recHitHE1_iphi");
  TTreeReaderValue<std::vector<float>> readRecHitsHE1_time(*tReader, "recHitHE1_time");

  TTreeReaderValue<std::vector<float>> readRecHitsHE2_En(*tReader, "recHitHE2_En");
  TTreeReaderValue<std::vector<float>> readRecHitsHE2_EnRAW(*tReader, "recHitHE2_EnRAW");
  TTreeReaderValue<std::vector<int>> readRecHitsHE2_ieta(*tReader, "recHitHE2_ieta");
  TTreeReaderValue<std::vector<int>> readRecHitsHE2_iphi(*tReader, "recHitHE2_iphi");
  TTreeReaderValue<std::vector<float>> readRecHitsHE2_time(*tReader, "recHitHE2_time");

  TTreeReaderValue<std::vector<float>> readRecHitsHE3_En(*tReader, "recHitHE3_En");
  TTreeReaderValue<std::vector<float>> readRecHitsHE3_EnRAW(*tReader, "recHitHE3_EnRAW");
  TTreeReaderValue<std::vector<int>> readRecHitsHE3_ieta(*tReader, "recHitHE3_ieta");
  TTreeReaderValue<std::vector<int>> readRecHitsHE3_iphi(*tReader, "recHitHE3_iphi");
  TTreeReaderValue<std::vector<float>> readRecHitsHE3_time(*tReader, "recHitHE3_time");


  TTreeReaderValue<std::vector<float>> readRecHitsHF1_En(*tReader, "recHitHF1_En");
  TTreeReaderValue<std::vector<int>> readRecHitsHF1_ieta(*tReader, "recHitHF1_ieta");
  TTreeReaderValue<std::vector<int>> readRecHitsHF1_iphi(*tReader, "recHitHF1_iphi");
  TTreeReaderValue<std::vector<float>> readRecHitsHF1_time(*tReader, "recHitHF1_time");

  TTreeReaderValue<std::vector<float>> readRecHitsHF2_En(*tReader, "recHitHF2_En");
  TTreeReaderValue<std::vector<int>> readRecHitsHF2_ieta(*tReader, "recHitHF2_ieta");
  TTreeReaderValue<std::vector<int>> readRecHitsHF2_iphi(*tReader, "recHitHF2_iphi");
  TTreeReaderValue<std::vector<float>> readRecHitsHF2_time(*tReader, "recHitHF2_time");


  TTreeReaderValue<std::vector<float>> readRecHitsHO_En(*tReader, "recHitHO_En");
  TTreeReaderValue<std::vector<int>> readRecHitsHO_ieta(*tReader, "recHitHO_ieta");
  TTreeReaderValue<std::vector<int>> readRecHitsHO_iphi(*tReader, "recHitHO_iphi");
  TTreeReaderValue<std::vector<float>> readRecHitsHO_time(*tReader, "recHitHO_time");


  TTreeReaderValue<std::vector<float>> readCaloTower_hadEt(*tReader, "caloTower_HadEt");
  TTreeReaderValue<std::vector<float>> readCaloTower_emEt(*tReader, "caloTower_EMEt");
  TTreeReaderValue<std::vector<int>> readCaloTower_ieta(*tReader, "caloTower_ieta");
  TTreeReaderValue<std::vector<int>> readCaloTower_iphi(*tReader, "caloTower_iphi");

  //TTreeReaderValue<std::vector<float>> recHitsHE(*tReader, "recHitEn_HE");
  //TTreeReaderValue<std::vector<float>> recHitsHF(*tReader, "recHitEn_HF");
  //TTreeReaderValue<std::vector<float>> recHitsHO(*tReader, "recHitEn_HO");

  //TTreeReaderValue<std::vector<float>> recHitsHB1(*tReader, "recHitEn_HB1");
  //TTreeReaderValue<std::vector<float>> recHitsHB2(*tReader, "recHitEn_HB2");

  //TTreeReaderValue<std::vector<float>> recHitsHE1(*tReader, "recHitEn_HE1");
  //TTreeReaderValue<std::vector<float>> recHitsHE2(*tReader, "recHitEn_HE2");
  //TTreeReaderValue<std::vector<float>> recHitsHE3(*tReader, "recHitEn_HE3");

  //TTreeReaderValue<std::vector<float>> recHitsHF1(*tReader, "recHitEn_HF1");
  //TTreeReaderValue<std::vector<float>> recHitsHF2(*tReader, "recHitEn_HF2");

  f2->cd();

  //Define the Histograms to be filled

  TH1F *nVertx = new TH1F("nVertx","nVertx", 51, -0.5, 50.5);
  TH1F *nVertxAll = new TH1F("nVertxAll","nVertxAll", 51, -0.5, 50.5);

  TH1D *h_nRecHits = new TH1D("nRecHits","nRecHits",760,0,3800);

  TH1D *h_nRecHitsHB = new TH1D("nRecHitsHB","nRecHitsHB",760,0,3800);
  TH1D *h_nRecHitsHE = new TH1D("nRecHitsHE","nRecHitsHE",760,0,3800);
  TH1D *h_nRecHitsHF = new TH1D("nRecHitsHF","nRecHitsHF",760,0,3800);
  TH1D *h_nRecHitsHO = new TH1D("nRecHitsHO","nRecHitsHO",760,0,3800);

  TH1D *h_nRecHitsHB1 = new TH1D("nRecHitsHB1","nRecHitsHB1",760,0,3800);
  TH1D *h_nRecHitsHB2 = new TH1D("nRecHitsHB2","nRecHitsHB2",760,0,3800);

  TH1D *h_nRecHitsHE1 = new TH1D("nRecHitsHE1","nRecHitsHE1",760,0,3800);
  TH1D *h_nRecHitsHE2 = new TH1D("nRecHitsHE2","nRecHitsHE2",760,0,3800);
  TH1D *h_nRecHitsHE3 = new TH1D("nRecHitsHE3","nRecHitsHE3",760,0,3800);

  TH1D *h_nRecHitsHF1 = new TH1D("nRecHitsHF1","nRecHitsHF1",760,0,3800);
  TH1D *h_nRecHitsHF2 = new TH1D("nRecHitsHF2","nRecHitsHF2",760,0,3800);

  TH1D *h_nRecHitss = new TH1D("nRecHitss","nRecHitss",760,0,3800);

  TH1D *h_nRecHitsHBs = new TH1D("nRecHitsHBs","nRecHitsHBs",760,0,3800);
  TH1D *h_nRecHitsHEs = new TH1D("nRecHitsHEs","nRecHitsHEs",760,0,3800);
  TH1D *h_nRecHitsHFs = new TH1D("nRecHitsHFs","nRecHitsHFs",760,0,3800);
  TH1D *h_nRecHitsHOs = new TH1D("nRecHitsHOs","nRecHitsHOs",760,0,3800);

  TH1D *h_nRecHitsHB1s = new TH1D("nRecHitsHB1s","nRecHitsHB1s",760,0,3800);
  TH1D *h_nRecHitsHB2s = new TH1D("nRecHitsHB2s","nRecHitsHB2s",760,0,3800);

  TH1D *h_nRecHitsHE1s = new TH1D("nRecHitsHE1s","nRecHitsHE1s",760,0,3800);
  TH1D *h_nRecHitsHE2s = new TH1D("nRecHitsHE2s","nRecHitsHE2s",760,0,3800);
  TH1D *h_nRecHitsHE3s = new TH1D("nRecHitsHE3s","nRecHitsHE3s",760,0,3800);

  TH1D *h_nRecHitsHF1s = new TH1D("nRecHitsHF1s","nRecHitsHF1s",760,0,3800);
  TH1D *h_nRecHitsHF2s = new TH1D("nRecHitsHF2s","nRecHitsHF2s",760,0,3800);

  TH1D *recHitEnergyHB = new TH1D("recHitEnergyHB","recHitEnergyHB",60,-20,100);
  TH1D *recHitEnergyHE = new TH1D("recHitEnergyHE","recHitEnergyHE",110,-20,200);
  TH1D *recHitEnergyHF = new TH1D("recHitEnergyHF","recHitEnergyHF",130,-100,1200);
  TH1D *recHitEnergyHO = new TH1D("recHitEnergyHO","recHitEnergyHO",60,-20,100);

  TH1D *recHitEnergyRAWHB = new TH1D("recHitEnergyRAWHB","recHitEnergyRAWHB",60,-20,100);
  TH1D *recHitEnergyRAWHE = new TH1D("recHitEnergyRAWHE","recHitEnergyRAWHE",110,-20,200);

  TH1D *recHitEnergyCutHB = new TH1D("recHitEnergyCutHB","recHitEnergyCutHB",60,-20,100);
  TH1D *recHitEnergyCutHE = new TH1D("recHitEnergyCutHE","recHitEnergyCutHE",110,-20,200);
  TH1D *recHitEnergyCutHF = new TH1D("recHitEnergyCutHF","recHitEnergyCutHF",130,-100,1200);
  TH1D *recHitEnergyCutHO = new TH1D("recHitEnergyCutHO","recHitEnergyCutHO",60,-20,100);

  TH1D *recHitEnergyHB1 = new TH1D("recHitEnergyHB1","recHitEnergyHB1",60,-20,100);
  TH1D *recHitEnergyHB2 = new TH1D("recHitEnergyHB2","recHitEnergyHB2",60,-20,100);

  TH1D *recHitEnergyHE1 = new TH1D("recHitEnergyHE1","recHitEnergyHE1",110,-20,200);
  TH1D *recHitEnergyHE2 = new TH1D("recHitEnergyHE2","recHitEnergyHE2",110,-20,200);
  TH1D *recHitEnergyHE3 = new TH1D("recHitEnergyHE3","recHitEnergyHE3",110,-20,200);

  TH1D *recHitEnergyRAWHB1 = new TH1D("recHitEnergyRAWHB1","recHitEnergyRAWHB1",60,-20,100);
  TH1D *recHitEnergyRAWHB2 = new TH1D("recHitEnergyRAWHB2","recHitEnergyRAWHB2",60,-20,100);

  TH1D *recHitEnergyRAWHE1 = new TH1D("recHitEnergyRAWHE1","recHitEnergyRAWHE1",110,-20,200);
  TH1D *recHitEnergyRAWHE2 = new TH1D("recHitEnergyRAWHE2","recHitEnergyRAWHE2",110,-20,200);
  TH1D *recHitEnergyRAWHE3 = new TH1D("recHitEnergyRAWHE3","recHitEnergyRAWHE3",110,-20,200);

  TH1D *recHitEnergyHF1 = new TH1D("recHitEnergyHF1","recHitEnergyHF1",130,-100,1200);
  TH1D *recHitEnergyHF2 = new TH1D("recHitEnergyHF2","recHitEnergyHF2",130,-100,1200);

  TH1D *recHitEnergyHBs = new TH1D("recHitEnergyHBs","recHitEnergyHBs",60,-20,100);
  TH1D *recHitEnergyHEs = new TH1D("recHitEnergyHEs","recHitEnergyHEs",110,-20,200);
  TH1D *recHitEnergyHFs = new TH1D("recHitEnergyHFs","recHitEnergyHFs",130,-100,1200);
  TH1D *recHitEnergyHOs = new TH1D("recHitEnergyHOs","recHitEnergyHOs",60,-20,100);

  TH1D *recHitEnergyRAWHBs = new TH1D("recHitEnergyRAWHBs","recHitEnergyRAWHBs",60,-20,100);
  TH1D *recHitEnergyRAWHEs = new TH1D("recHitEnergyRAWHEs","recHitEnergyRAWHEs",110,-20,200);

  TH1D *recHitEnergyCutHBs = new TH1D("recHitEnergyCutHBs","recHitEnergyCutHBs",60,-20,100);
  TH1D *recHitEnergyCutHEs = new TH1D("recHitEnergyCutHEs","recHitEnergyCutHEs",110,-20,200);
  TH1D *recHitEnergyCutHFs = new TH1D("recHitEnergyCutHFs","recHitEnergyCutHFs",130,-100,1200);
  TH1D *recHitEnergyCutHOs = new TH1D("recHitEnergyCutHOs","recHitEnergyCutHOs",60,-20,100);

  TH1D *recHitEnergyHB1s = new TH1D("recHitEnergyHB1s","recHitEnergyHB1s",60,-20,100);
  TH1D *recHitEnergyHB2s = new TH1D("recHitEnergyHB2s","recHitEnergyHB2s",60,-20,100);

  TH1D *recHitEnergyHE1s = new TH1D("recHitEnergyHE1s","recHitEnergyHE1s",110,-20,200);
  TH1D *recHitEnergyHE2s = new TH1D("recHitEnergyHE2s","recHitEnergyHE2s",110,-20,200);
  TH1D *recHitEnergyHE3s = new TH1D("recHitEnergyHE3s","recHitEnergyHE3s",110,-20,200);

  TH1D *recHitEnergyRAWHB1s = new TH1D("recHitEnergyRAWHB1s","recHitEnergyRAWHB1s",60,-20,100);
  TH1D *recHitEnergyRAWHB2s = new TH1D("recHitEnergyRAWHB2s","recHitEnergyRAWHB2s",60,-20,100);

  TH1D *recHitEnergyRAWHE1s = new TH1D("recHitEnergyRAWHE1s","recHitEnergyRAWHE1s",110,-20,200);
  TH1D *recHitEnergyRAWHE2s = new TH1D("recHitEnergyRAWHE2s","recHitEnergyRAWHE2s",110,-20,200);
  TH1D *recHitEnergyRAWHE3s = new TH1D("recHitEnergyRAWHE3s","recHitEnergyRAWHE3s",110,-20,200);

  TH1D *recHitEnergyHF1s = new TH1D("recHitEnergyHF1s","recHitEnergyHF1s",130,-100,1200);
  TH1D *recHitEnergyHF2s = new TH1D("recHitEnergyHF2s","recHitEnergyHF2s",130,-100,1200);

  TH1D *recHitNum_ieta = new TH1D("recHitNum_ieta","recHitNum_ieta", 83, -41.5, 41.5);
  TH1D *recHitCutNum_ieta = new TH1D("recHitNumCut_ieta","recHitNumCut_ieta", 83, -41.5, 41.5);

  TH1D *recHitNum_ieta_s = new TH1D("recHitNum_ieta_s","recHitNum_ieta_s", 83, -41.5, 41.5);
  TH1D *recHitCutNum_ieta_s = new TH1D("recHitNumCut_ieta_s","recHitNumCut_ieta_s", 83, -41.5, 41.5);

  TH1D *recHitTimingHB1 = new TH1D("recHitTimingHB1","recHitTimingHB1",70,-48,92);
  TH1D *recHitTimingHB2 = new TH1D("recHitTimingHB2","recHitTimingHB2",70,-48,92);

  TH1D *recHitTimingHE1 = new TH1D("recHitTimingHE1","recHitTimingHE1",70,-48,92);
  TH1D *recHitTimingHE2 = new TH1D("recHitTimingHE2","recHitTimingHE2",70,-48,92);
  TH1D *recHitTimingHE3 = new TH1D("recHitTimingHE3","recHitTimingHE3",70,-48,92);

  TH1D *recHitTimingHB1s = new TH1D("recHitTimingHB1s","recHitTimingHB1s",70,-48,92);
  TH1D *recHitTimingHB2s = new TH1D("recHitTimingHB2s","recHitTimingHB2s",70,-48,92);

  TH1D *recHitTimingHE1s = new TH1D("recHitTimingHE1s","recHitTimingHE1s",70,-48,92);
  TH1D *recHitTimingHE2s = new TH1D("recHitTimingHE2s","recHitTimingHE2s",70,-48,92);
  TH1D *recHitTimingHE3s = new TH1D("recHitTimingHE3s","recHitTimingHE3s",70,-48,92);

  TH1D *recHitTimingHF1 = new TH1D("recHitTimingHF1","recHitTimingHF1",70,-48,92);
  TH1D *recHitTimingHF2 = new TH1D("recHitTimingHF2","recHitTimingHF2",70,-48,92);

  TH1D *recHitTimingHO = new TH1D("recHitTimingHO","recHitTimingHO",70,-48,92);

  TH1D *recHitTimingHF1s = new TH1D("recHitTimingHF1s","recHitTimingHF1s",70,-48,92);
  TH1D *recHitTimingHF2s = new TH1D("recHitTimingHF2s","recHitTimingHF2s",70,-48,92);

  TH1D *recHitTimingHOs = new TH1D("recHitTimingHOs","recHitTimingHOs",70,-48,92);

  //Occupancy maps
  TH2D *recHitOccupancyHB1 = new TH2D("recHitOccupancyHB1","recHitOccupancyHB1",83,-41.5,41.5,73,-0.5,72.5);
  TH2D *recHitOccupancyHB2 = new TH2D("recHitOccupancyHB2","recHitOccupancyHB2",83,-41.5,41.5,73,-0.5,72.5);

  TH2D *recHitOccupancyHE1 = new TH2D("recHitOccupancyHE1","recHitOccupancyHE1",83,-41.5,41.5,73,-0.5,72.5);
  TH2D *recHitOccupancyHE2 = new TH2D("recHitOccupancyHE2","recHitOccupancyHE2",83,-41.5,41.5,73,-0.5,72.5);
  TH2D *recHitOccupancyHE3 = new TH2D("recHitOccupancyHE3","recHitOccupancyHE3",83,-41.5,41.5,73,-0.5,72.5);

  TH2D *recHitOccupancyHF1 = new TH2D("recHitOccupancyHF1","recHitOccupancyHF1",83,-41.5,41.5,73,-0.5,72.5);
  TH2D *recHitOccupancyHF2 = new TH2D("recHitOccupancyHF2","recHitOccupancyHF2",83,-41.5,41.5,73,-0.5,72.5);

  TH2D *recHitOccupancyHO = new TH2D("recHitOccupancyHO","recHitOccupancyHO",83,-41.5,41.5,73,-0.5,72.5);

  TH2D *recHitHBM0M2 = new TH2D("recHitHBM0M2","recHitHBM0M2",2000,0,2000,2000,0,2000);
  TH2D *recHitHEM0M2 = new TH2D("recHitHEM0M2","recHitHEM0M2",2000,0,2000,2000,0,2000);

  TH2D *recHitHBM2M0Dist = new TH2D("recHitHBM2M0Dist","recHitHBM2M0Dist",2000,0,2000,40,0,2);
  TH2D *recHitHEM2M0Dist = new TH2D("recHitHEM2M0Dist","recHitHEM2M0Dist",2000,0,2000,40,0,2);

  TH2D *recHitHBMRatio5plus = new TH2D("recHitHBMRatio5plus","recHitHBMRatio5plus",30,0,30,40,0,2);
  TH2D *recHitHBMRatio5minus = new TH2D("recHitHBMRatio5minus","recHitHBMRatio5minus",30,0,30,40,0,2);

  TH2D *recHitHBMRatio10plus = new TH2D("recHitHBMRatio10plus","recHitHBMRatio10plus",30,0,30,40,0,2);
  TH2D *recHitHBMRatio10minus = new TH2D("recHitHBMRatio10minus","recHitHBMRatio10minus",30,0,30,40,0,2);

  TH2D *recHitHBMRatio20plus = new TH2D("recHitHBMRatio20plus","recHitHBMRatio20plus",30,0,30,40,0,2);
  TH2D *recHitHBMRatio20minus = new TH2D("recHitHBMRatio20minus","recHitHBMRatio20minus",30,0,30,40,0,2);

  TH2D *recHitHEMRatio5plus = new TH2D("recHitHEMRatio5plus","recHitHEMRatio5plus",30,0,30,40,0,2);
  TH2D *recHitHEMRatio5minus = new TH2D("recHitHEMRatio5minus","recHitHEMRatio5minus",30,0,30,40,0,2);

  TH2D *recHitHEMRatio10plus = new TH2D("recHitHEMRatio10plus","recHitHEMRatio10plus",30,0,30,40,0,2);
  TH2D *recHitHEMRatio10minus = new TH2D("recHitHEMRatio10minus","recHitHEMRatio10minus",30,0,30,40,0,2);

  TH2D *recHitHEMRatio20plus = new TH2D("recHitHEMRatio20plus","recHitHEMRatio20plus",30,0,30,40,0,2);
  TH2D *recHitHEMRatio20minus = new TH2D("recHitHEMRatio20minus","recHitHEMRatio20minus",30,0,30,40,0,2);

  TH2D *recHitHBM0M2Ratio = new TH2D("recHitHBM0M2Ratio","recHitHBM0M2Ratio",2000,0,2000,40,0,2);
  TH2D *recHitHEM0M2Ratio = new TH2D("recHitHEM0M2Ratio","recHitHEM0M2Ratio",2000,0,2000,40,0,2);
  
  //Timing map

  TProfile2D *recHitTimingMapHB1 = new TProfile2D("recHitTimingMapHB1","recHitTimingMapHB1",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *recHitTimingMapHB2 = new TProfile2D("recHitTimingMapHB2","recHitTimingMapHB2",83, -41.5,41.5,73,-0.5,72.5);

  TProfile2D *recHitTimingMapHE1 = new TProfile2D("recHitTimingMapHE1","recHitTimingMapHE1",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *recHitTimingMapHE2 = new TProfile2D("recHitTimingMapHE2","recHitTimingMapHE2",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *recHitTimingMapHE3 = new TProfile2D("recHitTimingMapHE3","recHitTimingMapHE3",83, -41.5,41.5,73,-0.5,72.5);

  TProfile2D *recHitTimingMapHF1 = new TProfile2D("recHitTimingMapHF1","recHitTimingMapHF1",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *recHitTimingMapHF2 = new TProfile2D("recHitTimingMapHF2","recHitTimingMapHF2",83, -41.5,41.5,73,-0.5,72.5);

  TProfile2D *recHitTimingMapHO = new TProfile2D("recHitTimingMapHO","recHitTimingMapHO",83, -41.5,41.5,73,-0.5,72.5);

  //RecHit Energy map

  TProfile2D *recHitEnergyMapHB1 = new TProfile2D("recHitEnergyMapHB1","recHitEnergyMapHB1",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *recHitEnergyMapHB2 = new TProfile2D("recHitEnergyMapHB2","recHitEnergyMapHB2",83, -41.5,41.5,73,-0.5,72.5);

  TProfile2D *recHitEnergyMapHE1 = new TProfile2D("recHitEnergyMapHE1","recHitEnergyMapHE1",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *recHitEnergyMapHE2 = new TProfile2D("recHitEnergyMapHE2","recHitEnergyMapHE2",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *recHitEnergyMapHE3 = new TProfile2D("recHitEnergyMapHE3","recHitEnergyMapHE3",83, -41.5,41.5,73,-0.5,72.5);

  TProfile2D *recHitEnergyMapHF1 = new TProfile2D("recHitEnergyMapHF1","recHitEnergyMapHF1",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *recHitEnergyMapHF2 = new TProfile2D("recHitEnergyMapHF2","recHitEnergyMapHF2",83, -41.5,41.5,73,-0.5,72.5);

  TProfile2D *recHitEnergyMapHO = new TProfile2D("recHitEnergyMapHO","recHitEnergyMapHO",83, -41.5,41.5,73,-0.5,72.5);

  //Fiber Ratio (plus)
  TProfile *fiberRatio_ieta_long5_plus = new TProfile("fiberRatio_ieta_long5_plus","fiberRatio_ieta_long5_plus",51,-0.5,50.5);
  TProfile *fiberRatio_ieta_long10_plus = new TProfile("fiberRatio_ieta_long10_plus","fiberRatio_ieta_long10_plus",51,-0.5,50.5);
  TProfile *fiberRatio_ieta_sum5_plus = new TProfile("fiberRatio_ieta_sum5_plus","fiberRatio_ieta_sum5_plus",51,-0.5,50.5);
  TProfile *fiberRatio_ieta_sum10_plus = new TProfile("fiberRatio_ieta_sum10_plus","fiberRatio_ieta_sum10_plus",51,-0.5,50.5);
  TProfile *fiberRatio_ieta_long5sum10_plus = new TProfile("fiberRatio_ieta_long5sum10_plus","fiberRatio_ieta_long5sum10_plus",51,-0.5,50.5);
  TProfile *fiberRatio_ieta_sum5Threshold_plus = new TProfile("fiberRatio_ieta_sum5Threshold_plus","fiberRatio_ieta_sum5Threshold_plus",51,-0.5,50.5);
  TProfile *fiberRatio_ieta_sum10Threshold_plus = new TProfile("fiberRatio_ieta_sum10Threshold_plus","fiberRatio_ieta_sum10Threshold_plus",51,-0.5,50.5);

  //Fiber Ratio (minus)
  TProfile *fiberRatio_ieta_long5_minus = new TProfile("fiberRatio_ieta_long5_minus","fiberRatio_ieta_long5_minus",51,-0.5,50.5);
  TProfile *fiberRatio_ieta_long10_minus = new TProfile("fiberRatio_ieta_long10_minus","fiberRatio_ieta_long10_minus",51,-0.5,50.5);
  TProfile *fiberRatio_ieta_sum5_minus = new TProfile("fiberRatio_ieta_sum5_minus","fiberRatio_ieta_sum5_minus",51,-0.5,50.5);
  TProfile *fiberRatio_ieta_sum10_minus = new TProfile("fiberRatio_ieta_sum10_minus","fiberRatio_ieta_sum10_minus",51,-0.5,50.5);
  TProfile *fiberRatio_ieta_long5sum10_minus = new TProfile("fiberRatio_ieta_long5sum10_minus","fiberRatio_ieta_long5sum10_minus",51,-0.5,50.5);
  TProfile *fiberRatio_ieta_sum5Threshold_minus = new TProfile("fiberRatio_ieta_sum5Threshold_minus","fiberRatio_ieta_sum5Threshold_minus",51,-0.5,50.5);
  TProfile *fiberRatio_ieta_sum10Threshold_minus = new TProfile("fiberRatio_ieta_sum10Threshold_minus","fiberRatio_ieta_sum10Threshold_minus",51,-0.5,50.5);

  TProfile *fiberRatio_ieta_4GeVMean_plus = new TProfile("fiberRatio_ieta_4GeVMean_plus","fiberRatio_ieta_4GeVMean_plus",51,-0.5,50.5);
  TProfile *fiberRatio_ieta_4GeVMean_minus = new TProfile("fiberRatio_ieta_4GeVMean_minus","fiberRatio_ieta_4GeVMean_minus",51,-0.5,50.5);
  TProfile *fiberRatio_ieta_4GeVMean = new TProfile("fiberRatio_ieta_4GeVMean","fiberRatio_ieta_4GeVMean",51,-0.5,50.5);

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

  TProfile *caloFlowHad_plus = new TProfile("caloFlowHad_plus","caloFlowHad_plus",46,-0.5,45.5);
  TProfile *caloFlowHad_minus = new TProfile("caloFlowHad_minus","caloFlowHad_minus",46,-0.5,45.5);
  TProfile *caloFlowHad_eta_plus = new TProfile("caloFlowHad_eta_plus","caloFlowHad_eta_plus",46,-0.5,45.5);
  TProfile *caloFlowHad_eta_minus = new TProfile("caloFlowHad_eta_minus","caloFlowHad_eta_minus",46,-0.5,45.5);

  TProfile *caloFlowEm_plus = new TProfile("caloFlowEm_plus","caloFlowEm_plus",46,-0.5,45.5);
  TProfile *caloFlowEm_minus = new TProfile("caloFlowEm_minus","caloFlowEm_minus",46,-0.5,45.5);
  TProfile *caloFlowEm_eta_plus = new TProfile("caloFlowEm_eta_plus","caloFlowEm_eta_plus",46,-0.5,45.5);
  TProfile *caloFlowEm_eta_minus = new TProfile("caloFlowEm_eta_minus","caloFlowEm_eta_minus",46,-0.5,45.5);

  TProfile *HE_RecHitNum_ieta_plus = new TProfile("HE_RecHitNum_ieta_plus","HE_RecHitNum_ieta_plus",13,15.5,28.5);
  TProfile *HE_RecHitMeanEnergy_ieta_plus = new TProfile("HE_RecHitMeanEnergy_ieta_plus","HE_RecHitMeanEnergy_ieta_plus",13,15.5,28.5);

  TProfile *HE_RecHitCutNum_ieta_plus = new TProfile("HE_RecHitCutNum_ieta_plus","HE_RecHitCutNum_ieta_plus",13,15.5,28.5);
  TProfile *HE_RecHitCutMeanEnergy_ieta_plus = new TProfile("HE_RecHitCutMeanEnergy_ieta_plus","HE_RecHitCutMeanEnergy_ieta_plus",13,15.5,28.5);

  
  int minEventNum = (int)tReader->GetTree()->GetMinimum("event");
  int maxEventNum = (int)tReader->GetTree()->GetMaximum("event");

  cout << "Minimum event number: " << minEventNum << endl;
  cout << "Maximum event number: " << maxEventNum << endl;

  //As is this lumi code may only work correctly for datasets confined to a single run
  int minLumiBlock = (int)tReader->GetTree()->GetMinimum("lumi");
  int maxLumiBlock = (int)tReader->GetTree()->GetMaximum("lumi");
  int lumisInRange = maxLumiBlock - minLumiBlock + 1;

  //Find good number of bins
  int minRemainder = lumisInRange;
  int maxBinsNumber = 100; //The maximum number of bins that we want to consider
  int minBinsNumber = 75; //The minimum number of bins that we want to consider
  int bestBinsNumber = maxBinsNumber; // This is updated as we test

  for(int i = maxBinsNumber; i >= minBinsNumber; i--){
    if(lumisInRange % i < minRemainder){
      bestBinsNumber = i;
      minRemainder = lumisInRange % i;
      cout << "Current best bins number: " << bestBinsNumber << " Remainder: " << minRemainder << endl;
    }
  }


  TProfile *meanNVertx_event = new TProfile("meanNVertx_event","meanNVertx_event",100,minEventNum-0.5,maxEventNum+0.5);
  TProfile *meanNVertx_lumi = new TProfile("meanNVertx_lumi","meanNVertx_lumi",(lumisInRange < bestBinsNumber ? lumisInRange: bestBinsNumber), minLumiBlock-0.5, maxLumiBlock+0.5);


  int nRecHitsHE_ieta[13] = {0};
  int nRecHitsCutHE_ieta[13] = {0};

  cout << "Filling histograms..." << endl;
  
  //We loop over all of the events and fill the histograms
  int numRecHits = 0;
  int recHitIeta = 0;
  float recHitEnergy = 0;
  
  while(tReader->Next()){
    numRecHits = 0;
    //Reject any event where nVertx != 1
    nVertxAll->Fill(*numVertices);
    meanNVertx_event->Fill(*eventID,*numVertices);
    meanNVertx_lumi->Fill(*lumiID,*numVertices);
    if(*numVertices != 1) continue;
    nEvents++;
    
    if(debug){
      if(nEvents > 100) break;
      cout << endl << "Event: " << nEvents << endl;
    }

    if(nEvents % 5000 == 0) cout << "Added " << nEvents << " events." << endl;
    //Let's fill a histogram with the nVertices, it should just be one, but we can catch any problems this way
    nVertx->Fill(*numVertices);

    //numRecHits
    //h_nRecHits->Fill(*numRecHits);
    //h_nRecHitss->Fill(*numRecHits);

    //RecHit Energy HB
    /*
    nRecHitsHB = 0;
     for(int ridx = 0; ridx < (*recHitsHB).size(); ridx++){
      recHitEnergyHB->Fill((*recHitsHB).at(ridx));
      recHitEnergyHBs->Fill((*recHitsHB).at(ridx));
      nRecHitsHB = nRecHitsHB + 1;
      numRecHits++;
    }
    h_nRecHitsHB->Fill(nRecHitsHB);
    h_nRecHitsHBs->Fill(nRecHitsHB);
    */
    
    nRecHitsHB = 0;
    //RecHit Energy HB - depth 2
    nRecHitsHB2 = 0;
     for(int ridx = 0; ridx < (*readRecHitsHB2_En).size(); ridx++){
      float energy = (*readRecHitsHB2_En).at(ridx);
      float enRaw = (*readRecHitsHB2_EnRAW).at(ridx);

      float MRatio = energy/enRaw;

      recHitEnergyHB2->Fill((*readRecHitsHB2_En).at(ridx));
      recHitEnergyHB2s->Fill((*readRecHitsHB2_En).at(ridx));
      recHitEnergyHB->Fill((*readRecHitsHB2_En).at(ridx));
      recHitEnergyHBs->Fill((*readRecHitsHB2_En).at(ridx));

      recHitEnergyRAWHB2->Fill((*readRecHitsHB2_EnRAW).at(ridx));
      recHitEnergyRAWHB2s->Fill((*readRecHitsHB2_EnRAW).at(ridx));
      recHitEnergyRAWHB->Fill((*readRecHitsHB2_EnRAW).at(ridx));
      recHitEnergyRAWHBs->Fill((*readRecHitsHB2_EnRAW).at(ridx));
 
      if((*readRecHitsHB2_EnRAW).at(ridx) > 0.1) recHitHBM0M2->Fill((*readRecHitsHB2_EnRAW).at(ridx),(*readRecHitsHB2_En).at(ridx));
      if((*readRecHitsHB2_EnRAW).at(ridx) > 0.1) recHitHBM0M2Ratio->Fill((*readRecHitsHB2_EnRAW).at(ridx),(*readRecHitsHB2_En).at(ridx)/(*readRecHitsHB2_EnRAW).at(ridx));
      if((*readRecHitsHB2_EnRAW).at(ridx) > 0.1) recHitHBM2M0Dist->Fill((*readRecHitsHB2_EnRAW).at(ridx),(*readRecHitsHB2_En).at(ridx)/(*readRecHitsHB2_EnRAW).at(ridx));

      recHitTimingHB2->Fill((*readRecHitsHB2_time).at(ridx));
      recHitTimingHB2s->Fill((*readRecHitsHB2_time).at(ridx));

      recHitOccupancyHB2->Fill((*readRecHitsHB2_ieta).at(ridx),(*readRecHitsHB2_iphi).at(ridx));
      recHitTimingMapHB2->Fill((*readRecHitsHB2_ieta).at(ridx),(*readRecHitsHB2_iphi).at(ridx),(*readRecHitsHB2_time).at(ridx));
      recHitEnergyMapHB2->Fill((*readRecHitsHB2_ieta).at(ridx),(*readRecHitsHB2_iphi).at(ridx),(*readRecHitsHB2_En).at(ridx));

      if((*readRecHitsHB2_En).at(ridx) > 0.7){
	recHitEnergyCutHB->Fill((*readRecHitsHB2_En).at(ridx));
	recHitEnergyCutHBs->Fill((*readRecHitsHB2_En).at(ridx));
      }
      nRecHitsHB2 = nRecHitsHB2 + 1;
      nRecHitsHB = nRecHitsHB + 1;
      numRecHits++;
      recHitIeta = (*readRecHitsHB2_ieta).at(ridx);
      recHitEnergy = (*readRecHitsHB2_En).at(ridx);

      if(enRaw > 5){
	if(recHitIeta > 0) recHitHBMRatio5plus->Fill(recHitIeta,MRatio);
	if(recHitIeta < 0) recHitHBMRatio5minus->Fill(-recHitIeta,MRatio);
      }

      if(enRaw > 10){
	if(recHitIeta > 0) recHitHBMRatio10plus->Fill(recHitIeta,MRatio);
	if(recHitIeta < 0) recHitHBMRatio10minus->Fill(-recHitIeta,MRatio);
      }

      if(enRaw > 20){
	if(recHitIeta > 0) recHitHBMRatio20plus->Fill(recHitIeta,MRatio);
	if(recHitIeta < 0) recHitHBMRatio20minus->Fill(-recHitIeta,MRatio);
      }

      recHitNum_ieta->Fill(recHitIeta);
      recHitNum_ieta_s->Fill(recHitIeta);
      if(recHitEnergy > 0.7){
	recHitCutNum_ieta->Fill(recHitIeta);
	recHitCutNum_ieta_s->Fill(recHitIeta);
      }
      if(recHitIeta > 0){
	if(debug && (recHitIeta == bin)) cout << "RecHit Sum Before: " << recHitSum_ieta_plus[recHitIeta] << endl;	
	recHitSum_ieta_plus[recHitIeta] += recHitEnergy;
	if(debug && (recHitIeta == bin)) cout << "HB depth 2 All: " << recHitEnergy << endl
					      << "RecHit Sum After " << recHitSum_ieta_plus[recHitIeta] << endl;
	if(recHitEnergy > 0.7){
	  recHitSumCut_ieta_plus[recHitIeta] += recHitEnergy;
	  if(debug && (recHitIeta == bin)) cout << "HB depth 2: " << recHitEnergy*sin(2*atan(exp(-towerCenter[recHitIeta]))) << endl;
	}
      }
      if(recHitIeta < 0){
	recHitSum_ieta_minus[-recHitIeta] += recHitEnergy;
	if(recHitEnergy > 0.7){
	  recHitSumCut_ieta_minus[-recHitIeta] += recHitEnergy;
	  if(debug && (recHitIeta == bin)) cout << "HB depth 2: " << recHitEnergy*sin(2*atan(exp(-towerCenter[-recHitIeta]))) << endl;
	}
      }
    }
     h_nRecHitsHB2->Fill(nRecHitsHB2);
     h_nRecHitsHB2s->Fill(nRecHitsHB2);

    //RecHit Energy HB - depth1
    nRecHitsHB1 = 0;
     for(int ridx = 0; ridx < (*readRecHitsHB1_En).size(); ridx++){
      float energy = (*readRecHitsHB1_En).at(ridx);
      float enRaw = (*readRecHitsHB1_EnRAW).at(ridx);

      float MRatio = energy/enRaw;

      recHitEnergyHB1->Fill((*readRecHitsHB1_En).at(ridx));
      recHitEnergyHB1s->Fill((*readRecHitsHB1_En).at(ridx));
      recHitEnergyHB->Fill((*readRecHitsHB1_En).at(ridx));
      recHitEnergyHBs->Fill((*readRecHitsHB1_En).at(ridx));

      recHitEnergyRAWHB1->Fill((*readRecHitsHB1_EnRAW).at(ridx));
      recHitEnergyRAWHB1s->Fill((*readRecHitsHB1_EnRAW).at(ridx));
      recHitEnergyRAWHB->Fill((*readRecHitsHB1_EnRAW).at(ridx));
      recHitEnergyRAWHBs->Fill((*readRecHitsHB1_EnRAW).at(ridx));

      if((*readRecHitsHB1_EnRAW).at(ridx) > 0.1) recHitHBM0M2->Fill((*readRecHitsHB1_EnRAW).at(ridx),(*readRecHitsHB1_En).at(ridx));
      if((*readRecHitsHB1_EnRAW).at(ridx) > 0.1) recHitHBM0M2Ratio->Fill((*readRecHitsHB1_EnRAW).at(ridx),(*readRecHitsHB1_En).at(ridx)/(*readRecHitsHB1_EnRAW).at(ridx));
      if((*readRecHitsHB1_EnRAW).at(ridx) > 0.1) recHitHBM2M0Dist->Fill((*readRecHitsHB1_EnRAW).at(ridx),(*readRecHitsHB1_En).at(ridx)/(*readRecHitsHB1_EnRAW).at(ridx));

      recHitTimingHB1->Fill((*readRecHitsHB1_time).at(ridx));
      recHitTimingHB1s->Fill((*readRecHitsHB1_time).at(ridx));

      recHitOccupancyHB1->Fill((*readRecHitsHB1_ieta).at(ridx),(*readRecHitsHB1_iphi).at(ridx));
      recHitTimingMapHB1->Fill((*readRecHitsHB1_ieta).at(ridx),(*readRecHitsHB1_iphi).at(ridx),(*readRecHitsHB1_time).at(ridx));
      recHitEnergyMapHB1->Fill((*readRecHitsHB1_ieta).at(ridx),(*readRecHitsHB1_iphi).at(ridx),(*readRecHitsHB1_En).at(ridx));

      if((*readRecHitsHB1_En).at(ridx) > 0.7){
	recHitEnergyCutHB->Fill((*readRecHitsHB1_En).at(ridx));
	recHitEnergyCutHBs->Fill((*readRecHitsHB1_En).at(ridx));
      }
      nRecHitsHB1 = nRecHitsHB1 + 1;
      nRecHitsHB++;
      numRecHits++;
      recHitIeta = (*readRecHitsHB1_ieta).at(ridx);
      recHitEnergy = (*readRecHitsHB1_En).at(ridx);

      if(enRaw > 5){
        if(recHitIeta > 0) recHitHBMRatio5plus->Fill(recHitIeta,MRatio);
        if(recHitIeta < 0) recHitHBMRatio5minus->Fill(-recHitIeta,MRatio);
      }

      if(enRaw > 10){
        if(recHitIeta > 0) recHitHBMRatio10plus->Fill(recHitIeta,MRatio);
        if(recHitIeta < 0) recHitHBMRatio10minus->Fill(-recHitIeta,MRatio);
      }

      if(enRaw > 20){
        if(recHitIeta > 0) recHitHBMRatio20plus->Fill(recHitIeta,MRatio);
        if(recHitIeta < 0) recHitHBMRatio20minus->Fill(-recHitIeta,MRatio);
      }


      recHitNum_ieta->Fill(recHitIeta);
      recHitNum_ieta_s->Fill(recHitIeta);
      if(recHitEnergy > 0.7){
	recHitCutNum_ieta->Fill(recHitIeta);
	recHitCutNum_ieta_s->Fill(recHitIeta);
      }
      if(recHitIeta > 0){
	if(debug && (recHitIeta == bin)) cout << "RecHit Sum Before: " << recHitSum_ieta_plus[recHitIeta] << endl;	
	recHitSum_ieta_plus[recHitIeta] += recHitEnergy;
	if(debug && (recHitIeta == bin)) cout << "HB depth 1 All: " << recHitEnergy << endl
					      << "RecHit Sum After " << recHitSum_ieta_plus[recHitIeta] << endl;
	if(recHitEnergy > 0.7){
	  recHitSumCut_ieta_plus[recHitIeta] += recHitEnergy;
	  if(debug && (recHitIeta == bin)) cout << "HB depth 1: " << recHitEnergy*sin(2*atan(exp(-towerCenter[recHitIeta]))) << endl;
	}
      }
      if(recHitIeta < 0){
	recHitSum_ieta_minus[-recHitIeta] += recHitEnergy;
	if(recHitEnergy > 0.7){
	  recHitSumCut_ieta_minus[-recHitIeta] += recHitEnergy;
	  if(debug && (recHitIeta == bin)) cout << "HB depth 1: " << recHitEnergy*sin(2*atan(exp(-towerCenter[-recHitIeta]))) << endl;
	}
      }
    }
     h_nRecHitsHB1->Fill(nRecHitsHB1);
     h_nRecHitsHB1s->Fill(nRecHitsHB1);

     h_nRecHitsHB->Fill(nRecHitsHB);
     h_nRecHitsHBs->Fill(nRecHitsHB);

    //RecHit Energy HE
    nRecHitsHE = 0;
    //nRecHitsCutHE = 0;
    /*
    for(int ridx = 0; ridx < (*recHitsHE).size(); ridx++){
      recHitEnergyHE->Fill((*recHitsHE).at(ridx));
      recHitEnergyHEs->Fill((*recHitsHE).at(ridx));
      nRecHitsHE = nRecHitsHE + 1;
    }
    h_nRecHitsHE->Fill(nRecHitsHE);
    h_nRecHitsHEs->Fill(nRecHitsHE);
    */

    //RecHit Energy HE - depth 1
    nRecHitsHE1 = 0;
     for(int ridx = 0; ridx < (*readRecHitsHE1_En).size(); ridx++){
      float energy = (*readRecHitsHE1_En).at(ridx);
      float enRaw = (*readRecHitsHE1_EnRAW).at(ridx);

      float MRatio = energy/enRaw;

      recHitEnergyHE1->Fill((*readRecHitsHE1_En).at(ridx));
      recHitEnergyHE1s->Fill((*readRecHitsHE1_En).at(ridx));
      recHitEnergyHE->Fill((*readRecHitsHE1_En).at(ridx));
      recHitEnergyHEs->Fill((*readRecHitsHE1_En).at(ridx));
      recHitTimingHE1->Fill((*readRecHitsHE1_time).at(ridx));

      recHitEnergyRAWHE1->Fill((*readRecHitsHE1_EnRAW).at(ridx));
      recHitEnergyRAWHE1s->Fill((*readRecHitsHE1_EnRAW).at(ridx));
      recHitEnergyRAWHE->Fill((*readRecHitsHE1_EnRAW).at(ridx));
      recHitEnergyRAWHEs->Fill((*readRecHitsHE1_EnRAW).at(ridx));

      if((*readRecHitsHE1_EnRAW).at(ridx) > 0.1) recHitHEM0M2->Fill((*readRecHitsHE1_EnRAW).at(ridx),(*readRecHitsHE1_En).at(ridx));
      if((*readRecHitsHE1_EnRAW).at(ridx) > 0.1) recHitHEM0M2Ratio->Fill((*readRecHitsHE1_EnRAW).at(ridx),(*readRecHitsHE1_En).at(ridx)/(*readRecHitsHE1_EnRAW).at(ridx));
      if((*readRecHitsHE1_EnRAW).at(ridx) > 0.1) recHitHEM2M0Dist->Fill((*readRecHitsHE1_EnRAW).at(ridx),(*readRecHitsHE1_En).at(ridx)/(*readRecHitsHE1_EnRAW).at(ridx));

      recHitTimingHE1s->Fill((*readRecHitsHE1_time).at(ridx));

      recHitOccupancyHE1->Fill((*readRecHitsHE1_ieta).at(ridx),(*readRecHitsHE1_iphi).at(ridx));
      recHitTimingMapHE1->Fill((*readRecHitsHE1_ieta).at(ridx),(*readRecHitsHE1_iphi).at(ridx),(*readRecHitsHE1_time).at(ridx));
      recHitEnergyMapHE1->Fill((*readRecHitsHE1_ieta).at(ridx),(*readRecHitsHE1_iphi).at(ridx),(*readRecHitsHE1_En).at(ridx));
      if((*readRecHitsHE1_En).at(ridx) > 0.8){
	recHitEnergyCutHE->Fill((*readRecHitsHE1_En).at(ridx));
	recHitEnergyCutHEs->Fill((*readRecHitsHE1_En).at(ridx));
      }
      nRecHitsHE1 = nRecHitsHE1 + 1;
      nRecHitsHE++;
      numRecHits++;
      recHitIeta = (*readRecHitsHE1_ieta).at(ridx);
      recHitEnergy = (*readRecHitsHE1_En).at(ridx);

      if(recHitIeta == 29) recHitIeta = 28;
      if(recHitIeta == -29) recHitIeta = -28;

      if(enRaw > 5){
        if(recHitIeta > 0) recHitHEMRatio5plus->Fill(recHitIeta,MRatio);
        if(recHitIeta < 0) recHitHEMRatio5minus->Fill(-recHitIeta,MRatio);
      }

      if(enRaw > 10){
        if(recHitIeta > 0) recHitHEMRatio10plus->Fill(recHitIeta,MRatio);
        if(recHitIeta < 0) recHitHEMRatio10minus->Fill(-recHitIeta,MRatio);
      }

      if(enRaw > 20){
        if(recHitIeta > 0) recHitHEMRatio20plus->Fill(recHitIeta,MRatio);
        if(recHitIeta < 0) recHitHEMRatio20minus->Fill(-recHitIeta,MRatio);
      }


      recHitNum_ieta->Fill(recHitIeta);
      recHitNum_ieta_s->Fill(recHitIeta);
      if(recHitEnergy > 0.8){
	//nRecHitsCutHE++;
	recHitCutNum_ieta->Fill(recHitIeta);
	recHitCutNum_ieta_s->Fill(recHitIeta);
      }
      if(recHitIeta > 0){
	nRecHitsHE_ieta[recHitIeta-16]++;
	if(debug && (recHitIeta == bin)) cout << "RecHit Sum Before: " << recHitSum_ieta_plus[recHitIeta] << endl;	
	recHitSum_ieta_plus[recHitIeta] += recHitEnergy;
	if(debug && (recHitIeta == bin)) cout << "HE depth 1 All: " << recHitEnergy << endl
					      << "RecHit Sum After " << recHitSum_ieta_plus[recHitIeta] << endl;
	if(recHitEnergy > 0.8){
	  nRecHitsCutHE_ieta[recHitIeta-16]++;
	  recHitSumCut_ieta_plus[recHitIeta] += recHitEnergy;
	  if(debug && (recHitIeta == bin)) cout << "HE depth 1: " << recHitEnergy*sin(2*atan(exp(-towerCenter[recHitIeta]))) << endl;
	}
      }
      if(recHitIeta < 0){
	recHitSum_ieta_minus[-recHitIeta] += recHitEnergy;
	if(recHitEnergy > 0.8){
	  recHitSumCut_ieta_minus[-recHitIeta] += recHitEnergy;
	  if(debug && (recHitIeta == bin)) cout << "HE depth 1: " << recHitEnergy*sin(2*atan(exp(-towerCenter[-recHitIeta]))) << endl;
	}
      }
    }
    h_nRecHitsHE1->Fill(nRecHitsHE1);
    h_nRecHitsHE1s->Fill(nRecHitsHE1);

    //RecHit Energy HE - depth 2
    nRecHitsHE2 = 0;
     for(int ridx = 0; ridx < (*readRecHitsHE2_En).size(); ridx++){
      float energy = (*readRecHitsHE2_En).at(ridx);
      float enRaw = (*readRecHitsHE2_EnRAW).at(ridx);

      float MRatio = energy/enRaw;


      recHitEnergyHE2->Fill((*readRecHitsHE2_En).at(ridx));
      recHitEnergyHE2s->Fill((*readRecHitsHE2_En).at(ridx));
      recHitEnergyHE->Fill((*readRecHitsHE2_En).at(ridx));
      recHitEnergyHEs->Fill((*readRecHitsHE2_En).at(ridx));

      recHitEnergyRAWHE2->Fill((*readRecHitsHE2_EnRAW).at(ridx));
      recHitEnergyRAWHE2s->Fill((*readRecHitsHE2_EnRAW).at(ridx));
      recHitEnergyRAWHE->Fill((*readRecHitsHE2_EnRAW).at(ridx));
      recHitEnergyRAWHEs->Fill((*readRecHitsHE2_EnRAW).at(ridx));

      if((*readRecHitsHE2_EnRAW).at(ridx) > 0.1) recHitHEM0M2->Fill((*readRecHitsHE2_EnRAW).at(ridx),(*readRecHitsHE2_En).at(ridx));
      if((*readRecHitsHE2_EnRAW).at(ridx) > 0.1) recHitHEM0M2Ratio->Fill((*readRecHitsHE2_EnRAW).at(ridx),(*readRecHitsHE2_En).at(ridx)/(*readRecHitsHE2_EnRAW).at(ridx));
      if((*readRecHitsHE2_EnRAW).at(ridx) > 0.1) recHitHEM2M0Dist->Fill((*readRecHitsHE2_EnRAW).at(ridx),(*readRecHitsHE2_En).at(ridx)/(*readRecHitsHE2_EnRAW).at(ridx));

      recHitTimingHE2->Fill((*readRecHitsHE2_time).at(ridx));
      recHitTimingHE2s->Fill((*readRecHitsHE2_time).at(ridx));

      recHitOccupancyHE2->Fill((*readRecHitsHE2_ieta).at(ridx),(*readRecHitsHE2_iphi).at(ridx));
      recHitTimingMapHE2->Fill((*readRecHitsHE2_ieta).at(ridx),(*readRecHitsHE2_iphi).at(ridx),(*readRecHitsHE2_time).at(ridx));
      recHitEnergyMapHE2->Fill((*readRecHitsHE2_ieta).at(ridx),(*readRecHitsHE2_iphi).at(ridx),(*readRecHitsHE2_En).at(ridx));
      if((*readRecHitsHE2_En).at(ridx) > 0.8){
	recHitEnergyCutHE->Fill((*readRecHitsHE2_En).at(ridx));
	recHitEnergyCutHEs->Fill((*readRecHitsHE2_En).at(ridx));
      }
      nRecHitsHE2 = nRecHitsHE2 + 1;
      nRecHitsHE++;
      numRecHits++;
      recHitIeta = (*readRecHitsHE2_ieta).at(ridx);
      recHitEnergy = (*readRecHitsHE2_En).at(ridx);
      if(recHitIeta == 29) recHitIeta = 28;
      if(recHitIeta == -29) recHitIeta = -28;

      if(enRaw > 5){
        if(recHitIeta > 0) recHitHEMRatio5plus->Fill(recHitIeta,MRatio);
        if(recHitIeta < 0) recHitHEMRatio5minus->Fill(-recHitIeta,MRatio);
      }

      if(enRaw > 10){
        if(recHitIeta > 0) recHitHEMRatio10plus->Fill(recHitIeta,MRatio);
        if(recHitIeta < 0) recHitHEMRatio10minus->Fill(-recHitIeta,MRatio);
      }

      if(enRaw > 20){
        if(recHitIeta > 0) recHitHEMRatio20plus->Fill(recHitIeta,MRatio);
        if(recHitIeta < 0) recHitHEMRatio20minus->Fill(-recHitIeta,MRatio);
      }


      recHitNum_ieta->Fill(recHitIeta);
      recHitNum_ieta_s->Fill(recHitIeta);
      if(recHitEnergy > 0.8){
	//nRecHitsCutHE++;
	recHitCutNum_ieta->Fill(recHitIeta);
	recHitCutNum_ieta_s->Fill(recHitIeta);
      }
      if(recHitIeta > 0){
	nRecHitsHE_ieta[recHitIeta-16]++;
	if(debug && (recHitIeta == bin)) cout << "RecHit Sum Before: " << recHitSum_ieta_plus[recHitIeta] << endl;	
	recHitSum_ieta_plus[recHitIeta] += recHitEnergy;
	if(debug && (recHitIeta == bin)) cout << "HE depth 2 All: " << recHitEnergy << endl
					      << "RecHit Sum After " << recHitSum_ieta_plus[recHitIeta] << endl;
	if(recHitEnergy > 0.8){
	  nRecHitsCutHE_ieta[recHitIeta-16]++;
	  recHitSumCut_ieta_plus[recHitIeta] += recHitEnergy;
	  if(debug && (recHitIeta == bin)) cout << "HE depth 2: " << recHitEnergy*sin(2*atan(exp(-towerCenter[recHitIeta]))) << endl;
	}
      }
      if(recHitIeta < 0){
	recHitSum_ieta_minus[-recHitIeta] += recHitEnergy;
	if(recHitEnergy > 0.8){
	  recHitSumCut_ieta_minus[-recHitIeta] += recHitEnergy;
	  if(debug && (recHitIeta == bin)) cout << "HE depth 2: " << recHitEnergy*sin(2*atan(exp(-towerCenter[-recHitIeta]))) << endl;
	}
      }
    }
    h_nRecHitsHE2->Fill(nRecHitsHE2);
    h_nRecHitsHE2s->Fill(nRecHitsHE2);

    //RecHit Energy HE - depth 3
    nRecHitsHE3 = 0;
     for(int ridx = 0; ridx < (*readRecHitsHE3_En).size(); ridx++){
      float energy = (*readRecHitsHE3_En).at(ridx);
      float enRaw = (*readRecHitsHE3_EnRAW).at(ridx);

      float MRatio = energy/enRaw;

      recHitEnergyHE3->Fill((*readRecHitsHE3_En).at(ridx));
      recHitEnergyHE3s->Fill((*readRecHitsHE3_En).at(ridx));
      recHitEnergyHE->Fill((*readRecHitsHE3_En).at(ridx));
      recHitEnergyHEs->Fill((*readRecHitsHE3_En).at(ridx));

      recHitEnergyRAWHE3->Fill((*readRecHitsHE3_EnRAW).at(ridx));
      recHitEnergyRAWHE3s->Fill((*readRecHitsHE3_EnRAW).at(ridx));
      recHitEnergyRAWHE->Fill((*readRecHitsHE3_EnRAW).at(ridx));
      recHitEnergyRAWHEs->Fill((*readRecHitsHE3_EnRAW).at(ridx));

      if((*readRecHitsHE3_EnRAW).at(ridx) > 0.1) recHitHEM0M2->Fill((*readRecHitsHE3_EnRAW).at(ridx),(*readRecHitsHE3_En).at(ridx));
      if((*readRecHitsHE3_EnRAW).at(ridx) > 0.1) recHitHEM0M2Ratio->Fill((*readRecHitsHE3_EnRAW).at(ridx),(*readRecHitsHE3_En).at(ridx)/(*readRecHitsHE3_EnRAW).at(ridx));
      if((*readRecHitsHE3_EnRAW).at(ridx) > 0.1) recHitHEM2M0Dist->Fill((*readRecHitsHE3_EnRAW).at(ridx),(*readRecHitsHE3_En).at(ridx)/(*readRecHitsHE3_EnRAW).at(ridx));

      recHitTimingHE3->Fill((*readRecHitsHE3_time).at(ridx));
      recHitTimingHE3s->Fill((*readRecHitsHE3_time).at(ridx));

      recHitOccupancyHE3->Fill((*readRecHitsHE3_ieta).at(ridx),(*readRecHitsHE3_iphi).at(ridx));
      recHitTimingMapHE3->Fill((*readRecHitsHE3_ieta).at(ridx),(*readRecHitsHE3_iphi).at(ridx),(*readRecHitsHE3_time).at(ridx));
      recHitEnergyMapHE3->Fill((*readRecHitsHE3_ieta).at(ridx),(*readRecHitsHE3_iphi).at(ridx),(*readRecHitsHE3_En).at(ridx));
      if((*readRecHitsHE3_En).at(ridx) > 0.8){
	recHitEnergyCutHE->Fill((*readRecHitsHE3_En).at(ridx));
	recHitEnergyCutHEs->Fill((*readRecHitsHE3_En).at(ridx));
      }
      nRecHitsHE3 = nRecHitsHE3 + 1;
      nRecHitsHE++;
      numRecHits++;
      recHitIeta = (*readRecHitsHE3_ieta).at(ridx);
      recHitEnergy = (*readRecHitsHE3_En).at(ridx);
      if(recHitIeta == 29) recHitIeta = 28;
      if(recHitIeta == -29) recHitIeta = -28;

      if(enRaw > 5){
        if(recHitIeta > 0) recHitHEMRatio5plus->Fill(recHitIeta,MRatio);
        if(recHitIeta < 0) recHitHEMRatio5minus->Fill(-recHitIeta,MRatio);
      }

      if(enRaw > 10){
        if(recHitIeta > 0) recHitHEMRatio10plus->Fill(recHitIeta,MRatio);
        if(recHitIeta < 0) recHitHEMRatio10minus->Fill(-recHitIeta,MRatio);
      }

      if(enRaw > 20){
        if(recHitIeta > 0) recHitHEMRatio20plus->Fill(recHitIeta,MRatio);
        if(recHitIeta < 0) recHitHEMRatio20minus->Fill(-recHitIeta,MRatio);
      }


      recHitNum_ieta->Fill(recHitIeta);
      recHitNum_ieta_s->Fill(recHitIeta);
      if(recHitEnergy > 0.8){
	//nRecHitsCutHE++;
	recHitCutNum_ieta->Fill(recHitIeta);
	recHitCutNum_ieta_s->Fill(recHitIeta);
      }
      if(recHitIeta > 0){
	nRecHitsHE_ieta[recHitIeta-16]++;
	if(debug && (recHitIeta == bin)) cout << "RecHit Sum Before: " << recHitSum_ieta_plus[recHitIeta] << endl;	
	recHitSum_ieta_plus[recHitIeta] += recHitEnergy;
	if(debug && (recHitIeta == bin)) cout << "HE depth 3 All: " << recHitEnergy << endl
					      << "RecHit Sum After " << recHitSum_ieta_plus[recHitIeta] << endl;
	if(recHitEnergy > 0.8){
	  nRecHitsCutHE_ieta[recHitIeta-16]++;
	  recHitSumCut_ieta_plus[recHitIeta] += recHitEnergy;
	  if(debug && (recHitIeta == bin)) cout << "HE depth 3: " << recHitEnergy*sin(2*atan(exp(-towerCenter[recHitIeta]))) << endl;
	}
      }
      if(recHitIeta < 0){
	recHitSum_ieta_minus[-recHitIeta] += recHitEnergy;
	if(recHitEnergy > 0.8){
	  recHitSumCut_ieta_minus[-recHitIeta] += recHitEnergy;
	  if(debug && (recHitIeta == bin)) cout << "HE depth 3: " << recHitEnergy*sin(2*atan(exp(-towerCenter[-recHitIeta]))) << endl;
	}
      }
    }
    h_nRecHitsHE3->Fill(nRecHitsHE3);
    h_nRecHitsHE3s->Fill(nRecHitsHE3);

    h_nRecHitsHE->Fill(nRecHitsHE);
    h_nRecHitsHEs->Fill(nRecHitsHE);

    for(int i = 0; i < 13; i++) HE_RecHitNum_ieta_plus->Fill(i+16,nRecHitsHE_ieta[i]);
    for(int i = 0; i < 13; i++) HE_RecHitCutNum_ieta_plus->Fill(i+16,nRecHitsCutHE_ieta[i]);

    //RecHit Energy HF
    nRecHitsHF = 0;
    /*
     for(int ridx = 0; ridx < (*recHitsHF).size(); ridx++){
      recHitEnergyHF->Fill((*recHitsHF).at(ridx));
      recHitEnergyHFs->Fill((*recHitsHF).at(ridx));
      nRecHitsHF = nRecHitsHF + 1;
    }
    h_nRecHitsHF->Fill(nRecHitsHF);
    h_nRecHitsHFs->Fill(nRecHitsHF);
    */

    //RecHit Energy HF - depth 1
    nRecHitsHF1 = 0;
     for(int ridx = 0; ridx < (*readRecHitsHF1_En).size(); ridx++){
      recHitEnergyHF1->Fill((*readRecHitsHF1_En).at(ridx));
      recHitEnergyHF1s->Fill((*readRecHitsHF1_En).at(ridx));
      recHitEnergyHF->Fill((*readRecHitsHF1_En).at(ridx));
      recHitEnergyHFs->Fill((*readRecHitsHF1_En).at(ridx));

      recHitTimingHF1->Fill((*readRecHitsHF1_time).at(ridx));
      recHitTimingHF1s->Fill((*readRecHitsHF1_time).at(ridx));

      recHitOccupancyHF1->Fill((*readRecHitsHF1_ieta).at(ridx),(*readRecHitsHF1_iphi).at(ridx));
      recHitTimingMapHF1->Fill((*readRecHitsHF1_ieta).at(ridx),(*readRecHitsHF1_iphi).at(ridx),(*readRecHitsHF1_time).at(ridx));
      recHitEnergyMapHF1->Fill((*readRecHitsHF1_ieta).at(ridx),(*readRecHitsHF1_iphi).at(ridx),(*readRecHitsHF1_En).at(ridx));
      if((*readRecHitsHF1_En).at(ridx) > 0.5){
	recHitEnergyCutHF->Fill((*readRecHitsHF1_En).at(ridx));
	recHitEnergyCutHFs->Fill((*readRecHitsHF1_En).at(ridx));
      }
      nRecHitsHF1 = nRecHitsHF1 + 1;
      numRecHits++;
      nRecHitsHF++;

      recHitIeta = (*readRecHitsHF1_ieta).at(ridx);
      recHitEnergy = (*readRecHitsHF1_En).at(ridx);

      //Lev's Method
      if(recHitEnergy > 4 && recHitIeta > 0){
	recHitSumAllHF1_ieta_plus[recHitIeta] += recHitEnergy;
      }
      if(recHitEnergy > 4 && recHitIeta < 0){
	recHitSumAllHF1_ieta_minus[-recHitIeta] += recHitEnergy; 
      }

      recHitNum_ieta->Fill(recHitIeta);
      recHitNum_ieta_s->Fill(recHitIeta);
      if(recHitEnergy > 0.5){
	recHitCutNum_ieta->Fill(recHitIeta);
	recHitCutNum_ieta_s->Fill(recHitIeta);
      }
      if(recHitIeta > 0){
	if(debug && (recHitIeta == bin)) cout << "RecHit Sum Before: " << recHitSum_ieta_plus[recHitIeta] << endl;	
	recHitSum_ieta_plus[recHitIeta] += recHitEnergy;
	if(debug && (recHitIeta == bin)) cout << "HF depth 1 All: " << recHitEnergy << endl
					      << "RecHit Sum After " << recHitSum_ieta_plus[recHitIeta] << endl;
	if(recHitEnergy > 0.5) recHitSumCut_ieta_plus[recHitIeta] += recHitEnergy;
      }
      if(recHitIeta < 0){
	recHitSum_ieta_minus[-recHitIeta] += recHitEnergy;
	if(recHitEnergy > 0.5) recHitSumCut_ieta_minus[-recHitIeta] += recHitEnergy;
      }

      //*******Fiber Ratio
      
      float longFiberEnergy  =  (*readRecHitsHF1_En).at(ridx);
      int   longFiberIeta    =  (*readRecHitsHF1_ieta).at(ridx);
      int   longFiberIphi    =  (*readRecHitsHF1_iphi).at(ridx);

      float shortFiberEnergy =  0;
      int   shortFiberIeta   =  0;
      int   shortFiberIphi   =  0;

      bool  matched          =  false; //Is there a match between a HF1 RecHit and a HF2 RecHit
                                       //We don't look for the case where there is a RecHit in HF2, but not in HF1,
                                       //Since we divide by the long fiber, it doesn't make sense to divide by 0

      bool  sum10            =  false; //Is long+short > 10
      bool  sum5             =  false; //Is long+short > 5

      bool  long5            =  false; //Is long > 5
      bool  long10           =  false; //Is long > 10

      if(longFiberEnergy > 5)  long5  = true;
      if(longFiberEnergy > 10) long10 = true;
            
      for(int sidx = 0; sidx < (*readRecHitsHF2_En).size(); sidx++){

	shortFiberEnergy  =  (*readRecHitsHF2_En).at(sidx);
	shortFiberIeta    =  (*readRecHitsHF2_ieta).at(sidx);
	shortFiberIphi    =  (*readRecHitsHF2_iphi).at(sidx);

	if((longFiberIeta == shortFiberIeta) && (longFiberIphi == shortFiberIphi)){
	  matched = true;
	  break;
	}
      }

      if(!matched) shortFiberEnergy = 0;

      float ratio          = shortFiberEnergy/longFiberEnergy;
      float ratioThreshold = (longFiberEnergy < 0.5 ? 0 : (shortFiberEnergy < 0.85 ? 0 : shortFiberEnergy)/longFiberEnergy);

      if(longFiberEnergy + shortFiberEnergy > 5)  sum5  = true;
      if(longFiberEnergy + shortFiberEnergy > 10) sum10 = true;

      int idx = (longFiberIeta > 0 ? longFiberIeta : -longFiberIeta);
      
      if(longFiberIeta > 0){
	if(long5)  fiberRatio_ieta_long5_plus->Fill(idx, ratio);
	if(long10) fiberRatio_ieta_long10_plus->Fill(idx,ratio);

	if(sum5)  fiberRatio_ieta_sum5_plus->Fill(idx,ratio);
	if(sum10) fiberRatio_ieta_sum10_plus->Fill(idx,ratio);

	if(long5&&sum10) fiberRatio_ieta_long5sum10_plus->Fill(idx,ratio);

	if(longFiberEnergy > .5){
	  if(sum5)  fiberRatio_ieta_sum5Threshold_plus->Fill(idx,ratioThreshold);
	  if(sum10) fiberRatio_ieta_sum10Threshold_plus->Fill(idx,ratioThreshold);
	}
      }
      if(longFiberIeta < 0){
	if(long5)  fiberRatio_ieta_long5_minus->Fill(idx, ratio);
	if(long10) fiberRatio_ieta_long10_minus->Fill(idx,ratio);

	if(sum5)  fiberRatio_ieta_sum5_minus->Fill(idx,ratio);
	if(sum10) fiberRatio_ieta_sum10_minus->Fill(idx,ratio);

	if(long5&&sum10) fiberRatio_ieta_long5sum10_minus->Fill(idx,ratio);

	if(longFiberEnergy > .5){
	  if(sum5)  fiberRatio_ieta_sum5Threshold_minus->Fill(idx,ratioThreshold);
	  if(sum10) fiberRatio_ieta_sum10Threshold_minus->Fill(idx,ratioThreshold);
	}
      }
      
      //Loop over the Short Fiber events
      
      

      //*******Fiber Ratio

      //Now we do the fiber ratio
      /* Rewrite for complete sum
      if((*readRecHitsHF1_En).at(ridx) > 5){
	bool found = false;
	//Search for a matching RecHit in the Short Fiber
	for(int sidx = 0; sidx < (*readRecHitsHF2_En).size(); sidx++){
	  if(((*readRecHitsHF1_ieta).at(ridx)==(*readRecHitsHF2_ieta).at(sidx)) &&
	     ((*readRecHitsHF1_iphi).at(ridx)==(*readRecHitsHF2_iphi).at(sidx))){
	    fiberRatio_eta->Fill((*readRecHitsHF1_ieta).at(ridx),
				 (*readRecHitsHF2_En).at(sidx)/(*readRecHitsHF1_En).at(ridx));
	    if((*readRecHitsHF1_ieta).at(ridx) > 0){
	      fiberRatio_ieta_plus->Fill((*readRecHitsHF1_ieta).at(ridx),(*readRecHitsHF2_En).at(sidx)/(*readRecHitsHF1_En).at(ridx));
	      fiberRatioEx_ieta_plus->Fill((*readRecHitsHF1_ieta).at(ridx),(*readRecHitsHF2_En).at(sidx)/(*readRecHitsHF1_En).at(ridx));
	    }
	    if((*readRecHitsHF1_ieta).at(ridx) < 0){
	      fiberRatio_ieta_minus->Fill(-(*readRecHitsHF1_ieta).at(ridx), (*readRecHitsHF2_En).at(sidx)/(*readRecHitsHF1_En).at(ridx));
	      fiberRatioEx_ieta_minus->Fill(-(*readRecHitsHF1_ieta).at(ridx), (*readRecHitsHF2_En).at(sidx)/(*readRecHitsHF1_En).at(ridx));
	    }
	    if((*readRecHitsHF2_En).at(sidx) + (*readRecHitsHF1_En).at(ridx) > 10){
	      if((*readRecHitsHF1_ieta).at(ridx) > 0) fiberRatioEnReq_ieta_plus->Fill((*readRecHitsHF1_ieta).at(ridx),
									       (*readRecHitsHF2_En).at(sidx)/(*readRecHitsHF1_En).at(ridx));
	      if((*readRecHitsHF1_ieta).at(ridx) < 0) fiberRatioEnReq_ieta_minus->Fill(-(*readRecHitsHF1_ieta).at(ridx),
									       (*readRecHitsHF2_En).at(sidx)/(*readRecHitsHF1_En).at(ridx));
	    }
	    found = true;
	    break;
	  }

	}
	if(!found && (*readRecHitsHF1_ieta).at(ridx) > 0) fiberRatioEx_ieta_plus->Fill((*readRecHitsHF1_ieta).at(ridx),0);
	if(!found && (*readRecHitsHF1_ieta).at(ridx) < 0) fiberRatioEx_ieta_minus->Fill(-(*readRecHitsHF1_ieta).at(ridx),0);
	if(!found && ((*readRecHitsHF1_En).at(ridx) > 10) && ((*readRecHitsHF1_ieta).at(ridx) > 0)) fiberRatioEnReq_ieta_plus->Fill((*readRecHitsHF1_ieta).at(ridx),0);
	if(!found && ((*readRecHitsHF1_En).at(ridx) > 10) &&((*readRecHitsHF1_ieta).at(ridx) < 0)) fiberRatioEnReq_ieta_minus->Fill(-(*readRecHitsHF1_ieta).at(ridx),0);
	
      }
      */
     }

    h_nRecHitsHF1->Fill(nRecHitsHF1);
    h_nRecHitsHF1s->Fill(nRecHitsHF1);

    //RecHit Energy HF - Depth 2
    nRecHitsHF2 = 0;
     for(int ridx = 0; ridx < (*readRecHitsHF2_En).size(); ridx++){
      recHitEnergyHF2->Fill((*readRecHitsHF2_En).at(ridx));
      recHitEnergyHF2s->Fill((*readRecHitsHF2_En).at(ridx));
      recHitEnergyHF->Fill((*readRecHitsHF2_En).at(ridx));
      recHitEnergyHFs->Fill((*readRecHitsHF2_En).at(ridx));

      recHitTimingHF2->Fill((*readRecHitsHF2_time).at(ridx));
      recHitTimingHF2->Fill((*readRecHitsHF2_time).at(ridx));

      recHitOccupancyHF2->Fill((*readRecHitsHF2_ieta).at(ridx),(*readRecHitsHF2_iphi).at(ridx));
      recHitTimingMapHF2->Fill((*readRecHitsHF2_ieta).at(ridx),(*readRecHitsHF2_iphi).at(ridx),(*readRecHitsHF2_time).at(ridx));
      recHitEnergyMapHF2->Fill((*readRecHitsHF2_ieta).at(ridx),(*readRecHitsHF2_iphi).at(ridx),(*readRecHitsHF2_En).at(ridx));
      if((*readRecHitsHF2_En).at(ridx) > 0.85){
	recHitEnergyCutHF->Fill((*readRecHitsHF2_En).at(ridx));
	recHitEnergyCutHFs->Fill((*readRecHitsHF2_En).at(ridx));
      }
      nRecHitsHF2 = nRecHitsHF2 + 1;
      numRecHits++;
      nRecHitsHF++;
      recHitIeta = (*readRecHitsHF2_ieta).at(ridx);
      recHitEnergy = (*readRecHitsHF2_En).at(ridx);

      //Lev's Method
      if(recHitEnergy > 4 && recHitIeta > 0){
	recHitSumAllHF2_ieta_plus[recHitIeta] += recHitEnergy;
      }

      if(recHitEnergy > 4 && recHitIeta < 0){
	recHitSumAllHF2_ieta_minus[-recHitIeta] += recHitEnergy;
      }

      recHitNum_ieta->Fill(recHitIeta);
      recHitNum_ieta_s->Fill(recHitIeta);

      if(recHitEnergy > 0.85){
	recHitCutNum_ieta->Fill(recHitIeta);
	recHitCutNum_ieta_s->Fill(recHitIeta);
      }
      if(recHitIeta > 0){
	if(debug && (recHitIeta == bin)) cout << "RecHit Sum Before: " << recHitSum_ieta_plus[recHitIeta] << endl;	
	recHitSum_ieta_plus[recHitIeta] += recHitEnergy;
	if(debug && (recHitIeta == bin)) cout << "HF depth 2 All: " << recHitEnergy << endl
					      << "RecHit Sum After " << recHitSum_ieta_plus[recHitIeta] << endl;
	if(recHitEnergy > 0.85) recHitSumCut_ieta_plus[recHitIeta] += recHitEnergy;
      }
      if(recHitIeta < 0){
	recHitSum_ieta_minus[-recHitIeta] += recHitEnergy;
	if(recHitEnergy > 0.85) recHitSumCut_ieta_minus[-recHitIeta] += recHitEnergy;
      }
      
    }
    h_nRecHitsHF2->Fill(nRecHitsHF2);
    h_nRecHitsHF2s->Fill(nRecHitsHF2);

    h_nRecHitsHF->Fill(nRecHitsHF);
    h_nRecHitsHFs->Fill(nRecHitsHF);

    //RecHit Energy HO
    nRecHitsHO = 0;
     for(int ridx = 0; ridx < (*readRecHitsHO_En).size(); ridx++){
      recHitEnergyHO->Fill((*readRecHitsHO_En).at(ridx));
      recHitEnergyHOs->Fill((*readRecHitsHO_En).at(ridx));

      recHitTimingHO->Fill((*readRecHitsHO_time).at(ridx));
      recHitTimingHOs->Fill((*readRecHitsHO_time).at(ridx));

      recHitOccupancyHO->Fill((*readRecHitsHO_ieta).at(ridx),(*readRecHitsHO_iphi).at(ridx));
      recHitTimingMapHO->Fill((*readRecHitsHO_ieta).at(ridx),(*readRecHitsHO_iphi).at(ridx),(*readRecHitsHO_time).at(ridx));
      recHitEnergyMapHO->Fill((*readRecHitsHO_ieta).at(ridx),(*readRecHitsHO_iphi).at(ridx),(*readRecHitsHO_En).at(ridx));
      nRecHitsHO = nRecHitsHO + 1;
      numRecHits++;
    }
    h_nRecHitsHO->Fill(nRecHitsHO);
    h_nRecHitsHOs->Fill(nRecHitsHO);

    h_nRecHits->Fill(numRecHits);
    h_nRecHitss->Fill(numRecHits);

    for(int ridx = 0; ridx < (*readCaloTower_hadEt).size();ridx++){
      int calo_ieta = (*readCaloTower_ieta).at(ridx);
      if(calo_ieta > 0){
	caloEm_ieta_plus[calo_ieta] += (*readCaloTower_emEt).at(ridx);
	if((*readCaloTower_hadEt).at(ridx) > 0) caloHad_ieta_plus[calo_ieta] += (*readCaloTower_hadEt).at(ridx);
	if(debug && (calo_ieta==bin)) cout << "CaloTower Hadronic: " << (*readCaloTower_hadEt).at(ridx) << endl;
      }
      if (calo_ieta <0){
	caloEm_ieta_minus[-calo_ieta] += (*readCaloTower_emEt).at(ridx);
	if((*readCaloTower_hadEt).at(ridx) > 0) caloHad_ieta_minus[-calo_ieta] += (*readCaloTower_hadEt).at(ridx);
	if(debug && (calo_ieta==bin)) cout << "CaloTower Hadronic: " << (*readCaloTower_hadEt).at(ridx) << endl;
      }
    }

    float theta;

    for(int i = 1; i < 86; i++){
      if(true){
	energyFlow_plus->Fill(i,recHitSum_ieta_plus[i]);
	if((i >= 16)&&(i <= 28)) HE_RecHitMeanEnergy_ieta_plus->Fill(i,(nRecHitsHE_ieta[i-16] > 0 ? recHitSum_ieta_plus[i]/nRecHitsHE_ieta[i-16] : 0));
	if((i >= 16)&&(i <= 28)) HE_RecHitCutMeanEnergy_ieta_plus->Fill(i,(nRecHitsCutHE_ieta[i-16] > 0 ? recHitSumCut_ieta_plus[i]/nRecHitsCutHE_ieta[i-16] : 0));
	
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

      if(true){
	caloFlowEm_plus->Fill(i,caloEm_ieta_plus[i]);
	if(i < 42) caloFlowEm_eta_plus->Fill(i,caloEm_ieta_plus[i]/towerWidth[i]);
      }
      if(true){
	caloFlowEm_minus->Fill(i,caloEm_ieta_minus[i]);
	if(i < 42) caloFlowEm_eta_minus->Fill(i,caloEm_ieta_minus[i]/towerWidth[i]);
      }

      if(true){
	caloFlowHad_plus->Fill(i,caloHad_ieta_plus[i]);
	if(i < 42){
	  caloFlowHad_eta_plus->Fill(i,caloHad_ieta_plus[i]/towerWidth[i]);
	  if(debug && (i == bin)) cout << "Calo Fill: " << caloHad_ieta_plus[i]/towerWidth[i] << endl;
	}
      }
      if(true){
	caloFlowHad_minus->Fill(i,caloHad_ieta_minus[i]);
	if(i < 42) caloFlowHad_eta_minus->Fill(i,caloHad_ieta_minus[i]/towerWidth[i]);
      }

      recHitSum_ieta_plus[i] = 0;
      recHitSum_ieta_minus[i] = 0;

      recHitSumCut_ieta_plus[i] = 0;
      recHitSumCut_ieta_minus[i] = 0;

      caloEm_ieta_plus[i] = 0;
      caloEm_ieta_minus[i] = 0;
      
      caloHad_ieta_plus[i] = 0;
      caloHad_ieta_minus[i] = 0;

      if((i >=16) && (i <= 28)) nRecHitsHE_ieta[i-16] = 0;
      if((i >=16) && (i <= 28)) nRecHitsCutHE_ieta[i-16] = 0;
    }

  } //Fill Histograms

  //Now I want to provide some distributions scaled by the number of events, this should allow direct comparisons

  //Lev's Method
  double long_mean = 0;
  double short_mean = 0;
  double slratio = 0;
  for(int i = 0; i < 51; i++){
    
    // eta+
	long_mean = recHitSumAllHF1_ieta_plus[i] / nEvents;
	short_mean = recHitSumAllHF2_ieta_plus[i] / nEvents;

	slratio = (long_mean <= 0 ? 0 : short_mean/long_mean);

	printf("eta+: ieta,<E_S>,<E_L>,<E_S>/<E_L> %4d %8.2f %8.2f %8.2f\n",i,short_mean,long_mean,slratio);
	fiberRatio_ieta_4GeVMean_plus->Fill(i,slratio);

    // eta-
	long_mean = recHitSumAllHF1_ieta_minus[i] / nEvents;
	short_mean = recHitSumAllHF2_ieta_minus[i] / nEvents;

	slratio = (long_mean <= 0 ? 0 : short_mean / long_mean);

	printf("eta-: ieta,<E_S>,<E_L>,<E_S>/<E_L> %4d %8.2f %8.2f %8.2f\n",i,short_mean,long_mean,slratio);
	fiberRatio_ieta_4GeVMean_minus->Fill(i,slratio);

    // both
	long_mean = (recHitSumAllHF1_ieta_plus[i]+recHitSumAllHF1_ieta_minus[i]) / nEvents;
	short_mean = (recHitSumAllHF2_ieta_plus[i]+recHitSumAllHF2_ieta_minus[i]) / nEvents;

	slratio = (long_mean <= 0 ? 0 : short_mean / long_mean);

	printf("both: ieta,<E_S>,<E_L>,<E_S>/<E_L> %4d %8.2f %8.2f %8.2f\n",i,short_mean,long_mean,slratio);
	fiberRatio_ieta_4GeVMean->Fill(i,slratio);

  }


  recHitEnergyHBs->Scale(1/(float)nEvents);
  recHitEnergyHEs->Scale(1/(float)nEvents);
  recHitEnergyHFs->Scale(1/(float)nEvents);
  recHitEnergyHOs->Scale(1/(float)nEvents);

  recHitEnergyRAWHBs->Scale(1/(float)nEvents);
  recHitEnergyRAWHEs->Scale(1/(float)nEvents);

  recHitEnergyCutHBs->Scale(1/(float)nEvents);
  recHitEnergyCutHEs->Scale(1/(float)nEvents);
  recHitEnergyCutHFs->Scale(1/(float)nEvents);
  recHitEnergyCutHOs->Scale(1/(float)nEvents);

  recHitEnergyHB1s->Scale(1/(float)nEvents);
  recHitEnergyHB2s->Scale(1/(float)nEvents);

  recHitEnergyHE1s->Scale(1/(float)nEvents);
  recHitEnergyHE2s->Scale(1/(float)nEvents);
  recHitEnergyHE3s->Scale(1/(float)nEvents);

  recHitEnergyHF1s->Scale(1/(float)nEvents);
  recHitEnergyHF2s->Scale(1/(float)nEvents);

  recHitEnergyRAWHB1s->Scale(1/(float)nEvents);
  recHitEnergyRAWHB2s->Scale(1/(float)nEvents);

  recHitEnergyRAWHE1s->Scale(1/(float)nEvents);
  recHitEnergyRAWHE2s->Scale(1/(float)nEvents);
  recHitEnergyRAWHE3s->Scale(1/(float)nEvents);

  recHitTimingHB1s->Scale(1/(float)nEvents);
  recHitTimingHB2s->Scale(1/(float)nEvents);

  recHitTimingHE1s->Scale(1/(float)nEvents);
  recHitTimingHE2s->Scale(1/(float)nEvents);
  recHitTimingHE3s->Scale(1/(float)nEvents);

  h_nRecHitss->Scale(1/(float)nEvents);

  h_nRecHitsHBs->Scale(1/(float)nEvents);
  h_nRecHitsHEs->Scale(1/(float)nEvents);
  h_nRecHitsHFs->Scale(1/(float)nEvents);
  h_nRecHitsHOs->Scale(1/(float)nEvents);

  h_nRecHitsHB1s->Scale(1/(float)nEvents);
  h_nRecHitsHB2s->Scale(1/(float)nEvents);

  h_nRecHitsHE1s->Scale(1/(float)nEvents);
  h_nRecHitsHE2s->Scale(1/(float)nEvents);
  h_nRecHitsHE3s->Scale(1/(float)nEvents);

  h_nRecHitsHF1s->Scale(1/(float)nEvents);
  h_nRecHitsHF2s->Scale(1/(float)nEvents);

  recHitNum_ieta_s->Scale(1/(float)nEvents);
  recHitCutNum_ieta_s->Scale(1/(float)nEvents);

  f2->Write();

}
