
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

//This file takes two trees.
//The first tree will be taken as the ref
//The second tree will be the tree that needs reweighting
//As with FillHisto, we can also take a list of root files that make up a single tree.
//The options are
//"single" both of the trees are single root files
//"ref" just the reference tree is spread over multiple root files
//"second" just the second tree is spread over multiple root files
//
//An aditional parameter is specified for the number of events to look at. This will be used for debugging, but in
//production, I think that this should be left as -1
void produceVertexWeights(const char *refFile, const char *secFile, const char *outFile, const char *flag = "single", int runlength = -1){

  TTreeReader *tReaderRef, *tReaderSec;

  bool refsingle = true;
  bool secsingle = true;

  if(!strcmp(flag,"ref")){
	refsingle = false;
	secsingle = true;
  }
  if(!strcmp(flag,"second")){
	refsingle = true;
	secsingle = false;
  }
  if(!strcmp(flag,"both")){
	refsingle = false;
	secsingle = false;
  }
  if(!strcmp(flag,"single")){
	refsingle = true;
	secsingle = true;
  }


  //Open the first tree
  if(refsingle){

    TFile *f1 = new TFile(refFile);
    if(f1->IsZombie()){
      cout << "Root file: " << refFile << " not found!" << endl;
      return;
    }
    cout << "Opened Input file: " << refFile << endl;
    tReaderRef = new TTreeReader("RecHitTree",f1);
    if(!tReaderRef){
	cout << "Failed to create TTreeReader object" << endl;
	return;
    } else {
	cout << "TTreeReader object created" << endl;
    }

  } else {

    ifstream inRef;
    inRef.open(refFile);
    if(!inRef.is_open()){
      cout << "Cannot open list file: " << refFile << endl;
      return;  
    }

    TChain *refChain = new TChain("RecHitTree");
    if(!refChain){
	cout << "Failed to create TChain object" << endl;
	return;
    }else{
	cout << "Created TChain object" << endl;
    }    

    string refLine;
    while(inRef.good()){
      if(!std::getline(inRef,refLine)) break; // We read a line from the file
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
    tReaderSec = new TTreeReader("RecHitTree",f2);
    if(!tReaderSec){
	cout << "Failed to create TTreeReader object" << endl;
	return;
    } else {
	cout << "TTreeReader object created" << endl;
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

  //Define the branchs to be read
  TTreeReaderValue<Int_t> numVerticesRef(*tReaderRef, "nVertices");
  TTreeReaderValue<Int_t> numVerticesSec(*tReaderSec, "nVertices");

  //Loop over Reference tree
  while(tReaderRef->Next()){
    if(nEvents == 0) cout << "Starting Loop" << endl;
    if(nEvents % 100 == 0) cout << "Processed " << nEvents << " events." << endl;
    if(nEvents >= runlength && runlength != -1) break;

    curVertx = *numVerticesRef;
    h_nVerticesRef->Fill(curVertx);
    nEvents++;
  }
  cout << "Finished looping over Reference Tree." << endl;


  //Loop over Second Tree
  nEvents = 0;
  while(tReaderSec->Next()){
    if(nEvents == 0) cout << "Starting Loop" << endl;
    if(nEvents % 100 == 0) cout << "Processed " << nEvents << " events." << endl;
    if(nEvents >= runlength && runlength != -1) break;

    curVertx = *numVerticesSec;
    h_nVerticesSec->Fill(curVertx);
    nEvents++;
  }
  cout << "Finished looping over Second Tree." << endl;

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
