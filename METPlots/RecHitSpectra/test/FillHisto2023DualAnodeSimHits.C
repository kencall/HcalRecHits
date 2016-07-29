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


void FillHisto2023DualAnodeSimHits(const char *infile, const char *outfile, const char *flag = "single", const char *oflag = "none"){

  TTreeReader *tReader;

  if(!strcmp(flag,"single")){

    TFile *f1 = new TFile(infile);
    if(f1->IsZombie()){
      cout << "Root file: " << infile << " not found!" << endl;
      return;
    }
    tReader = new TTreeReader("hcalSimHitTree/SimHitTree",f1);

  } else if(!strcmp(flag,"list")){

    ifstream in;
    in.open(infile);
    if(!in.is_open()){
      cout << "Cannot open list file: " << infile << endl;
      return;  
    }

    TChain *chain = new TChain("hcalSimHitTree/SimHitTree");
    
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

  int nEvents = 0;

  int nSimHitsHF = 0;

  int nSimHitsHF1 = 0;
  int nSimHitsHF2 = 0;
  int nSimHitsHF3 = 0;
  int nSimHitsHF4 = 0;

  double SimHitsum_ieta_plus[86];
  double SimHitsum_ieta_minus[86];

  double SimHitsumAllHF1_ieta_plus[86];
  double SimHitsumAllHF1_ieta_minus[86];
  double SimHitsumAllHF2_ieta_plus[86];
  double SimHitsumAllHF2_ieta_minus[86];
  double SimHitsumAllHF3_ieta_plus[86];
  double SimHitsumAllHF3_ieta_minus[86];
  double SimHitsumAllHF4_ieta_plus[86];
  double SimHitsumAllHF4_ieta_minus[86];

  double SimHitsumCut_ieta_plus[86];
  double SimHitsumCut_ieta_minus[86];

  for(int i = 0; i < 86; i++){
    SimHitsum_ieta_plus[i]=0;
    SimHitsum_ieta_minus[i]=0;

    SimHitsumAllHF1_ieta_plus[i]=0;
    SimHitsumAllHF1_ieta_minus[i]=0;
    SimHitsumAllHF2_ieta_plus[i]=0;
    SimHitsumAllHF2_ieta_minus[i]=0;
    SimHitsumAllHF3_ieta_plus[i]=0;
    SimHitsumAllHF3_ieta_minus[i]=0;
    SimHitsumAllHF4_ieta_plus[i]=0;
    SimHitsumAllHF4_ieta_minus[i]=0;

    SimHitsumCut_ieta_plus[i]=0;
    SimHitsumCut_ieta_minus[i]=0;

  }

  //Define the branchs to be read

  //TTreeReaderValue<Int_t> eventID(*tReader, "event");
  //TTreeReaderValue<Int_t> lumiID(*tReader, "lumi");
  //TTreeReaderValue<Int_t> runID(*tReader, "run");  

  TTreeReaderValue<std::vector<float>> readSimHitsEn(*tReader, "SimHitsEn");
  TTreeReaderValue<std::vector<int>> readSimHitsIeta(*tReader, "SimHitsIeta");
  TTreeReaderValue<std::vector<int>> readSimHitsIphi(*tReader, "SimHitsIphi");
  TTreeReaderValue<std::vector<int>> readSimHitsDepth(*tReader, "SimHitsDepth");

  f2->cd();

  //Useful definitions
  Double_t ieta_partitions[9] = {-21,-16,-12,-6,0,6,12,16,21};

  //Define the Histograms to be filled

  TH1D *h_nSimHits = new TH1D("nSimHits","nSimHits",760,0,3800);

  TH1D *h_nSimHitsHF = new TH1D("nSimHitsHF","nSimHitsHF",760,0,3800);

  TH1D *h_nSimHitsHF1 = new TH1D("nSimHitsHF1","nSimHitsHF1",760,0,3800);
  TH1D *h_nSimHitsHF2 = new TH1D("nSimHitsHF2","nSimHitsHF2",760,0,3800);
  TH1D *h_nSimHitsHF3 = new TH1D("nSimHitsHF3","nSimHitsHF3",760,0,3800);
  TH1D *h_nSimHitsHF4 = new TH1D("nSimHitsHF4","nSimHitsHF4",760,0,3800);

  TH1D *h_nSimHitsHFLongDiff = new TH1D("nSimHitsHFLongDiff","nSimHitsHFLongDiff",101,-50.5,50.5);
  TH1D *h_nSimHitsHFShortDiff = new TH1D("nSimHitsHFShortDiff","nSimHitsHFShortDiff",101,-50.3,50.5);

  TH1D *h_nSimHitsHFLongDiffs = new TH1D("nSimHitsHFLongDiffs","nSimHitsHFLongDiffs",101,-50.5,50.5);
  TH1D *h_nSimHitsHFShortDiffs = new TH1D("nSimHitsHFShortDiffs","nSimHitsHFShortDiffs",101,-50.3,50.5);

  TH1D *h_nSimHitss = new TH1D("nSimHitss","nSimHitss",760,0,3800);

  TH1D *h_nSimHitsHFs = new TH1D("nSimHitsHFs","nSimHitsHFs",760,0,3800);

  TH1D *h_nSimHitsHF1s = new TH1D("nSimHitsHF1s","nSimHitsHF1s",760,0,3800);
  TH1D *h_nSimHitsHF2s = new TH1D("nSimHitsHF2s","nSimHitsHF2s",760,0,3800);
  TH1D *h_nSimHitsHF3s = new TH1D("nSimHitsHF3s","nSimHitsHF3s",760,0,3800);
  TH1D *h_nSimHitsHF4s = new TH1D("nSimHitsHF4s","nSimHitsHF4s",760,0,3800);

  TH1D *energyHF = new TH1D("energyHF","energyHF",130,-100,1200);

  TH1D *energyCutHF = new TH1D("energyCutHF","energyCutHF",130,-100,1200);

  TH1D *energyHF1 = new TH1D("energyHF1","energyHF1",130,-100,1200);
  TH1D *energyHF2 = new TH1D("energyHF2","energyHF2",130,-100,1200);
  TH1D *energyHF3 = new TH1D("energyHF3","energyHF3",130,-100,1200);
  TH1D *energyHF4 = new TH1D("energyHF4","energyHF4",130,-100,1200);

  TH1D *energyHFs = new TH1D("energyHFs","energyHFs",130,-100,1200);

  TH1D *energyCutHFs = new TH1D("energyCutHFs","energyCutHFs",130,-100,1200);

  TH1D *energyHF1s = new TH1D("energyHF1s","energyHF1s",130,-100,1200);
  TH1D *energyHF2s = new TH1D("energyHF2s","energyHF2s",130,-100,1200);
  TH1D *energyHF3s = new TH1D("energyHF3s","energyHF3s",130,-100,1200);
  TH1D *energyHF4s = new TH1D("energyHF4s","energyHF4s",130,-100,1200);

  TH1D *iphiHF1 = new TH1D("iphiHF1","iphiHF1",74,-0.5,73.5);	
  TH1D *iphiHF2 = new TH1D("iphiHF2","iphiHF2",74,-0.5,73.5);	
  TH1D *iphiHF3 = new TH1D("iphiHF3","iphiHF3",74,-0.5,73.5);	
  TH1D *iphiHF4 = new TH1D("iphiHF4","iphiHF4",74,-0.5,73.5);	

  TH1D *iphiHF1s = new TH1D("iphiHF1s","iphiHF1s",74,-0.5,73.5);	
  TH1D *iphiHF2s = new TH1D("iphiHF2s","iphiHF2s",74,-0.5,73.5);	
  TH1D *iphiHF3s = new TH1D("iphiHF3s","iphiHF3s",74,-0.5,73.5);	
  TH1D *iphiHF4s = new TH1D("iphiHF4s","iphiHF4s",74,-0.5,73.5);	

  TH1D *ietaHF1 = new TH1D("ietaHF1","ietaHF1",85,-42.5,42.5);
  TH1D *ietaHF2 = new TH1D("ietaHF2","ietaHF2",85,-42.5,42.5);
  TH1D *ietaHF3 = new TH1D("ietaHF3","ietaHF3",85,-42.5,42.5);
  TH1D *ietaHF4 = new TH1D("ietaHF4","ietaHF4",85,-42.5,42.5);

  TH1D *ietaHF1s = new TH1D("ietaHF1s","ietaHF1s",85,-42.5,42.5);
  TH1D *ietaHF2s = new TH1D("ietaHF2s","ietaHF2s",85,-42.5,42.5);
  TH1D *ietaHF3s = new TH1D("ietaHF3s","ietaHF3s",85,-42.5,42.5);
  TH1D *ietaHF4s = new TH1D("ietaHF4s","ietaHF4s",85,-42.5,42.5);

  TH1D *SimHitsNum_ieta = new TH1D("SimHitsNum_ieta","SimHitsNum_ieta", 83, -41.5, 41.5);
  TH1D *SimHitsCutNum_ieta = new TH1D("SimHitsNumCut_ieta","SimHitsNumCut_ieta", 83, -41.5, 41.5);

  TH1D *SimHitsNum_ieta_s = new TH1D("SimHitsNum_ieta_s","SimHitsNum_ieta_s", 83, -41.5, 41.5);
  TH1D *SimHitsCutNum_ieta_s = new TH1D("SimHitsNumCut_ieta_s","SimHitsNumCut_ieta_s", 83, -41.5, 41.5);

  //Occupancy maps
  TH2D *SimHitsOccupancyHF1 = new TH2D("SimHitsOccupancyHF1","SimHitsOccupancyHF1",83,-41.5,41.5,73,-0.5,72.5);
  TH2D *SimHitsOccupancyHF2 = new TH2D("SimHitsOccupancyHF2","SimHitsOccupancyHF2",83,-41.5,41.5,73,-0.5,72.5);
  TH2D *SimHitsOccupancyHF3 = new TH2D("SimHitsOccupancyHF3","SimHitsOccupancyHF3",83,-41.5,41.5,73,-0.5,72.5);
  TH2D *SimHitsOccupancyHF4 = new TH2D("SimHitsOccupancyHF4","SimHitsOccupancyHF4",83,-41.5,41.5,73,-0.5,72.5);

  TProfile2D *energyMapHF1 = new TProfile2D("energyMapHF1","energyMapHF1",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *energyMapHF2 = new TProfile2D("energyMapHF2","energyMapHF2",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *energyMapHF3 = new TProfile2D("energyMapHF3","energyMapHF3",83, -41.5,41.5,73,-0.5,72.5);
  TProfile2D *energyMapHF4 = new TProfile2D("energyMapHF4","energyMapHF4",83, -41.5,41.5,73,-0.5,72.5);

  int nSimHitsHE_ieta[13] = {0};
  int nSimHitsCutHE_ieta[13] = {0};

  cout << "Filling histograms..." << endl;
  
  //We loop over all of the events and fill the histograms
  int numSimHits = 0;
  int ieta = 0;
  float energy = 0;
  
  while(tReader->Next()){
    numSimHits = 0;

    nEvents++;
    
    if(debug){
      if(nEvents > 100) break;
      cout << endl << "Event: " << nEvents << endl;
    }

    if(nEvents % 5000 == 0) cout << "Added " << nEvents << " events." << endl;

    nSimHitsHF = 0;

    nSimHitsHF1 = 0;
    nSimHitsHF2 = 0;
    nSimHitsHF3 = 0;
    nSimHitsHF4 = 0;

    for(int ridx = 0; ridx < (*readSimHitsEn).size(); ridx++){
      float energy = (*readSimHitsEn).at(ridx);

      int depth = (*readSimHitsDepth).at(ridx);
      
      int ieta = (*readSimHitsIeta).at(ridx);
      int iphi = (*readSimHitsIphi).at(ridx);

      energyHF->Fill(energy);
      energyHFs->Fill(energy);

      if(energy > 0.5){
	energyCutHF->Fill(energy);
	energyCutHFs->Fill(energy);
      }

      numSimHits++;
      nSimHitsHF++;


      if(depth == 1){
         energyHF1->Fill(energy);
         energyHF1s->Fill(energy);

         SimHitsOccupancyHF1->Fill(ieta,iphi);
         energyMapHF1->Fill(ieta,iphi,energy);

         iphiHF1->Fill(iphi);
         iphiHF1s->Fill(iphi);
         ietaHF1->Fill(ieta);
         ietaHF1s->Fill(ieta);

         nSimHitsHF1 = nSimHitsHF1 + 1;

      }         

      if(depth == 2){
         energyHF2->Fill(energy);
         energyHF2s->Fill(energy);

         SimHitsOccupancyHF2->Fill(ieta,iphi);
         energyMapHF2->Fill(ieta,iphi,energy);

         iphiHF2->Fill(iphi);
         iphiHF2s->Fill(iphi);
         ietaHF2->Fill(ieta);
         ietaHF2s->Fill(ieta);

         nSimHitsHF2 = nSimHitsHF2 + 1;

      }         

      if(depth == 3){
         energyHF3->Fill(energy);
         energyHF3s->Fill(energy);

         SimHitsOccupancyHF3->Fill(ieta,iphi);
         energyMapHF3->Fill(ieta,iphi,energy);

         iphiHF3->Fill(iphi);
         iphiHF3s->Fill(iphi);
         ietaHF3->Fill(ieta);
         ietaHF3s->Fill(ieta);

         nSimHitsHF3 = nSimHitsHF3 + 1;

      }         

      if(depth == 4){
         energyHF4->Fill(energy);
         energyHF4s->Fill(energy);

         SimHitsOccupancyHF4->Fill(ieta,iphi);
         energyMapHF4->Fill(ieta,iphi,energy);

         iphiHF4->Fill(iphi);
         iphiHF4s->Fill(iphi);
         ietaHF4->Fill(ieta);
         ietaHF4s->Fill(ieta);

         nSimHitsHF4 = nSimHitsHF4 + 1;

      }         

      SimHitsNum_ieta->Fill(ieta);
      SimHitsNum_ieta_s->Fill(ieta);
      if(energy > 0.5){
	SimHitsCutNum_ieta->Fill(ieta);
	SimHitsCutNum_ieta_s->Fill(ieta);
      }
      if(ieta > 0){
	SimHitsum_ieta_plus[ieta] += energy;
	if(energy > 0.5) SimHitsumCut_ieta_plus[ieta] += energy;
      }
      if(ieta < 0){
	SimHitsum_ieta_minus[-ieta] += energy;
	if(energy > 0.5) SimHitsumCut_ieta_minus[-ieta] += energy;
      }

    }

    h_nSimHitsHF->Fill(nSimHitsHF);
    h_nSimHitsHFs->Fill(nSimHitsHF);

    h_nSimHitsHF1->Fill(nSimHitsHF1);
    h_nSimHitsHF1s->Fill(nSimHitsHF1);
    h_nSimHitsHF2->Fill(nSimHitsHF2);
    h_nSimHitsHF2s->Fill(nSimHitsHF2);
    h_nSimHitsHF3->Fill(nSimHitsHF3);
    h_nSimHitsHF3s->Fill(nSimHitsHF3);
    h_nSimHitsHF4->Fill(nSimHitsHF4);
    h_nSimHitsHF4s->Fill(nSimHitsHF4);

    h_nSimHitsHFLongDiff->Fill(nSimHitsHF1-nSimHitsHF3);
    h_nSimHitsHFShortDiff->Fill(nSimHitsHF2-nSimHitsHF4);

    h_nSimHitsHFLongDiffs->Fill(nSimHitsHF1-nSimHitsHF3);
    h_nSimHitsHFShortDiffs->Fill(nSimHitsHF2-nSimHitsHF4);

    h_nSimHits->Fill(numSimHits);
    h_nSimHitss->Fill(numSimHits);


  } //Fill Histograms

  energyHFs->Scale(1/(float)nEvents);

  energyCutHFs->Scale(1/(float)nEvents);

  energyHF1s->Scale(1/(float)nEvents);
  energyHF2s->Scale(1/(float)nEvents);
  energyHF3s->Scale(1/(float)nEvents);
  energyHF4s->Scale(1/(float)nEvents);

  h_nSimHitss->Scale(1/(float)nEvents);

  h_nSimHitsHFs->Scale(1/(float)nEvents);

  h_nSimHitsHF1s->Scale(1/(float)nEvents);
  h_nSimHitsHF2s->Scale(1/(float)nEvents);
  h_nSimHitsHF3s->Scale(1/(float)nEvents);
  h_nSimHitsHF4s->Scale(1/(float)nEvents);

  h_nSimHitsHFLongDiffs->Scale(1/(float)nEvents);
  h_nSimHitsHFShortDiffs->Scale(1/(float)nEvents);

  SimHitsNum_ieta_s->Scale(1/(float)nEvents);
  SimHitsCutNum_ieta_s->Scale(1/(float)nEvents);

  iphiHF1s->Scale(1/(float)nEvents);
  iphiHF2s->Scale(1/(float)nEvents);
  iphiHF3s->Scale(1/(float)nEvents);
  iphiHF4s->Scale(1/(float)nEvents);

  ietaHF1s->Scale(1/(float)nEvents);
  ietaHF2s->Scale(1/(float)nEvents);
  ietaHF3s->Scale(1/(float)nEvents);
  ietaHF4s->Scale(1/(float)nEvents);

  f2->Write();

}
