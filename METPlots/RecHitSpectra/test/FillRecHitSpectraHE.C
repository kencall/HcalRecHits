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


void FillRecHitSpectraHE(const char *infile, const char *outfile, const char *flag = "single"){

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

  int nEvents = 0;

  //Define the branchs to be read

  TTreeReaderValue<Int_t> eventID(*tReader, "event");
  TTreeReaderValue<Int_t> lumiID(*tReader, "lumi");
  TTreeReaderValue<Int_t> runID(*tReader, "run");  

  TTreeReaderValue<std::vector<float>> readRecHitsHE_en(*tReader, "recHitHE_En");
//  TTreeReaderValue<std::vector<float>> readRecHitsHE_enRaw(*tReader, "recHitHE_EnRAW");
  TTreeReaderValue<std::vector<int>> readRecHitsHE_ieta(*tReader, "recHitHE_ieta");
  TTreeReaderValue<std::vector<int>> readRecHitsHE_iphi(*tReader, "recHitHE_iphi");
  TTreeReaderValue<std::vector<int>> readRecHitsHE_depth(*tReader, "recHitHE_depth");
  TTreeReaderValue<std::vector<float>> readRecHitsHE_time(*tReader, "recHitHE_time");

  f2->cd();

  //Define the Histograms to be filled

  TH1D *h_RecHitSpectraHE[3][14][2]; //There is a seperate histogram for each ieta and depth
  TH1D *h_RecHitSpectraHESym[3][14]; //The positive and negative ietas are combined

  for(int depth = 1; depth<=3; depth++){
	TH1D *htmp;
	char tmp[40];

	for(int ieta = 16; ieta <= 29; ieta++){

		sprintf(tmp,"h_RecHitSpectraHE_ieta%d_depth%d",ieta,depth);
		h_RecHitSpectraHE[depth-1][ieta-16][0] = new TH1D(tmp,tmp,130,-100,1200);
		
		sprintf(tmp,"h_RecHitSpectraHE_ieta%d_depth%d",-ieta,depth);
		h_RecHitSpectraHE[depth-1][ieta-16][1] = new TH1D(tmp,tmp,130,-100,1200);

		sprintf(tmp,"h_RecHitSpectraHESym_ieta%d_depth%d",ieta,depth);
		h_RecHitSpectraHESym[depth-1][ieta-16] = new TH1D(tmp,tmp,130,-100,1200);
		
	}
  }

  cout << "Filling histograms..." << endl;
  
  //We loop over all of the events and fill the histograms
  int numRecHits = 0;
  int ieta = 0;
  float energy = 0;
  
  while(tReader->Next()){
    numRecHits = 0;
    nEvents++;
    
    if(nEvents % 5000 == 0) cout << "Added " << nEvents << " events." << endl;

    for(int ridx = 0; ridx < (*readRecHitsHE_en).size(); ridx++){

      float energy = (*readRecHitsHE_en).at(ridx);
      int depth = (*readRecHitsHE_depth).at(ridx);
      float time = (*readRecHitsHE_time).at(ridx);
      int ieta = (*readRecHitsHE_ieta).at(ridx);
      int iphi = (*readRecHitsHE_iphi).at(ridx);

      if(depth < 1 || depth >3){
	cout << "Invalid HE depth, " << depth << endl;
	continue;
      }

      int ietaabs = (ieta < 0 ? -ieta : ieta);

      if(ietaabs < 16 || ietaabs > 29){
        cout << "Invalid HE ieta, " << ieta << endl;
        continue;
      }      

      //cout << "Depth: " << depth << ", ieta: " << ieta << ", |ieta|: " << ietaabs << endl;
      
      h_RecHitSpectraHE[depth-1][ietaabs-16][(ieta < 0 ? 1 : 0)]->Fill(energy);
      h_RecHitSpectraHESym[depth-1][ietaabs-16]->Fill(energy);
    }//Run over RecHits
  } //Run over events

  f2->Write();

}
