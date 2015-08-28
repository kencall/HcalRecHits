
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
#include "TArrayF.h"
#include "TArrayL.h"
#include <cmath>

//This file takes a list of root files and a list histograms and adds the listed histograms from the root files
//together.
//It will also produce a nWeightedEvent normalized version of the histogram.

void produceVertexWeights(const char *rootList, const char *histFile, const char *outFile, const char *histEntries = "none"){

  //First let's make sure that the files given exist.

  ifstream inRootList;
  inRootList.open(rootList);
  if(!inRootList.is_open()){
      cout << "Cannot open list file: " << rootList << endl;
      return;  
  }

  ifstream inHistList;
  inHistList.open(histList);
  if(!inHistList.is_open()){
      cout << "Cannot open list file: " << histList << endl;
      return;
  }

  TFile *rootFile;
  std::vector<TFile*> rFileV;


  //In Progress***

  string rootLine;
  while(inRootList.good()){
      if(!std::getline(inRootList,rootLine)) break; // We read a line from the file
      if(!refChain->Add(refLine.c_str())){
	cout << "Problem loading tree from " << refLine << endl;
      }else{
	cout << "Adding file: " << refLine << "..." << endl;
      }
      
    }
    
    inRef.close();
    
    tReaderRef = new TTreeReader((TTree *)refChain);
    if(!tReaderRef){
	cout << "Could not create TTreeReader from TChain" << endl;
	return;
    } else {
	cout << "Successfully created TTreeReader from TChain" << endl;
    }
    cout << "Finished loading files." << endl;
  }


  //Open the second tree
  if(secsingle){

    TFile *f2 = new TFile(secFile);
    if(f2->IsZombie()){
      cout << "Root file: " << secFile << " not found!" << endl;
      return;
    }
    cout << "Opened Input file: " << secFile << endl;

    if(!secDFlag){
    	tReaderSec = new TTreeReader("RecHitTree",f2);
    	if(!tReaderSec){
		cout << "Failed to create TTreeReader object" << endl;
		return;
    	} else {
		cout << "TTreeReader object created" << endl;
    	}
    }
    if(secDFlag){
	h_Sec = (TH1D*)f2->Get(distSec);
	if(!h_Sec){
		cout << "Failed to load histogram, " << distSec << ", from file, " << secFile << endl;
		return;
	} else {
		cout << "Loaded histogram, " << distSec << ", from file, " << secFile << endl;
	}
    }

  } else {

    ifstream inSec;
    inSec.open(secFile);
    if(!inSec.is_open()){
      cout << "Cannot open list file: " << secFile << endl;
      return;  
    }

    TChain *secChain = new TChain("RecHitTree");
    if(!secChain){
	cout << "Failed to create TChain object" << endl;
	return;
    }else{
	cout << "Created TChain object" << endl;
    }    

    string secLine;
    while(inSec.good()){
      if(!std::getline(inSec,secLine)) break; // We read a line from the file
      if(!secChain->Add(secLine.c_str())){
	cout << "Problem loading tree from " << secLine << endl;
      }else{
	cout << "Adding file: " << secLine << "..." << endl;
      }
      
    }
    
    inSec.close();
    
    tReaderSec = new TTreeReader((TTree *)secChain);
    if(!tReaderSec){
	cout << "Could not create TTreeReader from TChain" << endl;
	return;
    } else {
	cout << "Successfully created TTreeReader from TChain" << endl;
    }
    cout << "Finished loading files." << endl;
  }

  TFile *fout = new TFile(outFile, "NEW");
  if(fout->IsZombie()){
    cout << "Root file: " << outFile << " cannot be created!" << endl;
    return;
  } else {
    cout << outFile << " created for output." << endl;
  }
      
   //Define the values to be calculated
   //We are going to save 3 histograms as the output:
   //1-The vertex distribution of the ref
   //2-The vertex distribution of the second
   //3-The vertex weights

  fout->cd();

  int nVertxMax = 60;
  TH1D * h_nVerticesRef = new TH1D("h_nVerticesRef","h_nVerticesRef",nVertxMax,-0.5,nVertxMax-0.5);
  TH1D * h_nVerticesSec = new TH1D("h_nVerticesSec","h_nVerticesSec",nVertxMax,-0.5,nVertxMax-0.5);
  TH1D * h_nVerticesWeights = new TH1D("h_nVerticesWeights","h_nVerticesWeights",nVertxMax,-0.5,nVertxMax-0.5);

  int nEvents = 0;
  int curVertx;

  int binIdx;

  //Define the branchs to be read

  TTreeReaderValue<Int_t> numVerticesSec(*tReaderSec, "nVertices");

  //Handle the Reference set -- Loop over Reference tree or convert provided nVertx distribution
  if(!refDFlag){
 	TTreeReaderValue<Int_t> numVerticesRef(*tReaderRef, "nVertices");
  	while(tReaderRef->Next()){
    		if(nEvents == 0) cout << "Starting Loop" << endl;
    		if(nEvents % 100 == 0) cout << "Processed " << nEvents << " events." << endl;
    		if(nEvents >= runlength && runlength != -1) break;

    		curVertx = *numVerticesRef;
    		h_nVerticesRef->Fill(curVertx);
    		nEvents++;
  	}
  	cout << "Finished looping over Reference Tree." << endl;
  }
  if(refDFlag){
	for(int i = 0; i <= nVertxMax; i++){
		binIdx = h_Ref->GetXaxis()->FindBin(i);
		h_nVerticesRef->SetBinContent(i+1, h_Ref->GetBinContent(binIdx));
	}
	cout << "Finished converting provided Reference distribution" << endl;
  }


  //Loop over Second Tree
  nEvents = 0;
  if(!secDFlag){
	TTreeReaderValue<Int_t> numVerticesSec(*tReaderSec, "nVertices");
  	while(tReaderSec->Next()){
    		if(nEvents == 0) cout << "Starting Loop" << endl;
    		if(nEvents % 100 == 0) cout << "Processed " << nEvents << " events." << endl;
    		if(nEvents >= runlength && runlength != -1) break;
	
    		curVertx = *numVerticesSec;
    		h_nVerticesSec->Fill(curVertx);
    		nEvents++;
  	}
  	cout << "Finished looping over Second Tree." << endl;
  }

  if(secDFlag){
	for(int i = 0; i <= nVertxMax; i++){
		binIdx = h_Sec->GetXaxis()->FindBin(i);
		h_nVerticesSec->SetBinContent(i+1, h_Sec->GetBinContent(binIdx));
	}
	cout << "Finished converting provided Second distribution" << endl;
  }

  //Now we will normalize the distributions.
  double refIntegral = h_nVerticesRef->Integral();
  double secIntegral = h_nVerticesSec->Integral();

  if(refIntegral != 0) h_nVerticesRef->Scale(1/refIntegral);
  if(secIntegral != 0) h_nVerticesSec->Scale(1/secIntegral);

  //Now we calculate the weights.
  double numerator, denominator, weight;
  for(int i = 0; i<nVertxMax+1; i++){
	numerator = h_nVerticesRef->GetBinContent(i);
	denominator = h_nVerticesSec->GetBinContent(i);
	weight = (denominator == 0 ? 0 : numerator / denominator);
	h_nVerticesWeights->SetBinContent(i,weight);

	cout << "nVertices: " << h_nVerticesWeights->GetBinCenter(i) << " Weight: " << h_nVerticesWeights->GetBinContent(i) << endl;
  }

  fout->Write();
  fout->Close();

}
