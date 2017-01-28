#include "TROOT.h"
#include "TFile.h"
#include <iostream>
#include "TTree.h"
#include "TTreeReader.h"

//This is a very straight forward macro
//It takes a single root file with a rechit tree, and it will create
//a second root file with a rechit tree, but the HE rechits have been collapsed
//to single depth. The rechit energies of the different depths are added together
void collapseDepths(const char *infile, const char *outfile){

	TFile *fin = new TFile(infile);
	if(fin->IsZombie()){
		cout << infile << " cannot be opened!" << endl;
		return;
	}

	TTreeReader *tReader = new TTreeReader("RecHitTree",fin);

	TTreeReaderValue<Int_t> eventID(*tReader, "event");
	TTreeReaderValue<Int_t> lumiID(*tReader, "lumi");
	TTreeReaderValue<Int_t> runID(*tReader, "run");

	TTreeReaderValue<std::vector<float>> readRecHitsHE_en(*tReader, "recHitHE_En");
//	TTreeReaderValue<std::vector<float>> readRecHitsHE_enRaw(*tReader, "recHitHE_EnRAW");
	TTreeReaderValue<std::vector<int>> readRecHitsHE_ieta(*tReader, "recHitHE_ieta");
	TTreeReaderValue<std::vector<int>> readRecHitsHE_iphi(*tReader, "recHitHE_iphi");
	TTreeReaderValue<std::vector<int>> readRecHitsHE_depth(*tReader, "recHitHE_depth");
	TTreeReaderValue<std::vector<float>> readRecHitsHE_time(*tReader, "recHitHE_time");

	TFile *fout = new TFile(outfile, "NEW");
	if(fout->IsZombie()){
		cout << outfile << " cannot be created!" << endl;
		return;
	}

	TTree *legacyTree = new TTree("RecHitTree","RecHitTree");

	int run;
	int lumi;
	int event;

	std::vector<float> legacyRecHitHE_en;
//	std::vector<float> legacyRecHitHE_enRaw;
	std::vector<float> legacyRecHitHE_time;
	std::vector<int>   legacyRecHitHE_ieta;
	std::vector<int>   legacyRecHitHE_iphi;
	std::vector<int>   legacyRecHitHE_depth;
	std::vector<int>   legacyRecHitHE_compressedDepths; //This will track how many rechits were summed to form this one

	legacyTree->Branch("run", &run, "run/I");
	legacyTree->Branch("lumi", &lumi, "lumi/I");
	legacyTree->Branch("event", &event, "event/I");

	legacyTree->Branch("recHitHE_En","std::vector<float>", &legacyRecHitHE_en, 32000, 0);
//	legacyTree->Branch("recHitHE_EnRAW","std::vector<float>", &legacyRecHitHE_enRaw, 32000, 0);
	legacyTree->Branch("recHitHE_ieta","std::vector<int>", &legacyRecHitHE_ieta, 32000, 0);
	legacyTree->Branch("recHitHE_iphi","std::vector<int>", &legacyRecHitHE_iphi, 32000, 0);
	legacyTree->Branch("recHitHE_depth","std::vector<int>", &legacyRecHitHE_depth, 32000, 0);
	legacyTree->Branch("recHitHE_time","std::vector<float>", &legacyRecHitHE_time, 32000, 0);
	legacyTree->Branch("recHitHE_2017hits","std::vector<int>", &legacyRecHitHE_compressedDepths, 32000, 0);

	//Loop over the original tree

	int nEvents = 0;
	while(tReader->Next()){
		legacyRecHitHE_en.clear();
//        	legacyRecHitHE_enRaw.clear();
        	legacyRecHitHE_time.clear();
        	legacyRecHitHE_ieta.clear();
        	legacyRecHitHE_iphi.clear();
        	legacyRecHitHE_depth.clear();
        	legacyRecHitHE_compressedDepths.clear();

		nEvents++;
		if(nEvents % 1000 == 1) cout << "Processing " << nEvents << " events." << endl;
		
		for(int ridx = 0; ridx < (*readRecHitsHE_en).size(); ridx++){
			float energy = (*readRecHitsHE_en).at(ridx);
//			float enRaw = (*readRecHitsHE_enRaw).at(ridx);
			int depth = (*readRecHitsHE_depth).at(ridx);
			float time = (*readRecHitsHE_time).at(ridx);
			int ieta = (*readRecHitsHE_ieta).at(ridx);
			int iphi = (*readRecHitsHE_iphi).at(ridx);

			int legacyDepth = 0; //Collapse everything to depth 0

			//Now we scan the legacy vector to see if there is already a matching rechit
			bool matched = false;
			int lidx = 0;
			for(lidx = 0; !matched && lidx < legacyRecHitHE_en.size(); lidx++){
				if(legacyRecHitHE_depth.at(lidx) == legacyDepth &&
				   legacyRecHitHE_ieta.at(lidx) == ieta &&
				   legacyRecHitHE_iphi.at(lidx) == iphi){

					matched = true;
					break;
				}
			}

			if(matched){
				legacyRecHitHE_en.at(lidx) += energy;
//				legacyRecHitHE_enRaw.at(lidx) += enRaw;
				legacyRecHitHE_compressedDepths.at(lidx)++;
				legacyRecHitHE_time.at(lidx) = (legacyRecHitHE_time.at(lidx) + time) / legacyRecHitHE_compressedDepths.at(lidx)++;
			} else {
				legacyRecHitHE_en.push_back(energy);
//				legacyRecHitHE_enRaw.push_back(enRaw);
				legacyRecHitHE_depth.push_back(legacyDepth);
				legacyRecHitHE_time.push_back(time);
				legacyRecHitHE_ieta.push_back(ieta);
				legacyRecHitHE_iphi.push_back(iphi);
				legacyRecHitHE_compressedDepths.push_back(1);
			}

		}//Loop over rechits in the event

		//Fill the new tree
		legacyTree->Fill();

	}//Loop over events


	//Finish with output
	fout->cd();
	legacyTree->Write();
	fout->Write();
	fout->Close();
}
