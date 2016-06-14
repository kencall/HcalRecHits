// -*- C++ -*-
//
// Package:    test/SimHitTree
// Class:      SimHitTree
// 
/**\class SimHitTree SimHitTree.cc test/SimHitTree/plugins/SimHitTree.cc

 Description: Produces Trees from SimHits

 Implementation:
     Use TFileService to combine Trees in a single root file.
     The sudetector id is passed as parameter. Thus for each SimHits tree per subdetector this module is run.
*/
//
// Original Author:  Kenneth Call
//         Created:  Wed, 23 Mar 2016 17:40:31 GMT
//
//


// system include files
//#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include <iostream>

// include files to support the generation of TTrees. 
#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>

//include files to support access to the QIE10 digis
#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/HcalDetId/interface/HcalDetId.h"

#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"

//TFileService
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"


//SimHits stuff

#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/HcalCommonData/interface/HcalDDDRecConstants.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
 
#include "DataFormats/HcalRecHit/interface/HcalSourcePositionData.h"
 
#include "DataFormats/HcalDetId/interface/HcalElectronicsId.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
 
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"
 
//relabeling of test configs
#include "SimCalorimetry/HcalSimProducers/interface/HcalHitRelabeller.h"

#include <vector>
#include <utility>
#include <ostream>
#include <string>
#include <algorithm>
#include <cmath>
#include "Geometry/Records/interface/CaloGeometryRecord.h"

//
// class declaration
//

class SimHitTree : public edm::stream::EDFilter<> {
   public:
      explicit SimHitTree(const edm::ParameterSet&);
      ~SimHitTree();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginStream(edm::StreamID) override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endStream() override;

      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
	TTree *tt1;

	//Branchs
	std::vector<float> SimHitsEn;
	std::vector<int> SimHitsIphi;
	std::vector<int> SimHitsIeta;
	std::vector<int> SimHitsDepth;
	
	//std::vector<float> SimHitsHEen;
	//std::vector<int> SimHitsHEiphi;
	//std::vector<int> SimHitsHEieta;
	//std::vector<int> SimHitsHEdepth;
	
	//SimHit Stuff
	edm::EDGetTokenT<edm::PCaloHitContainer> tok_hcal_;
	edm::ESHandle<CaloGeometry> geometry ;
	edm::ESHandle<HcalDDDRecConstants> pHRNDC;

	const CaloGeometry * theGeometry;
	const HcalDDDRecConstants * theRecNumber;

	int sub_;
	bool testNumbering_;

	HcalHitRelabeller *theRelabeller;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
SimHitTree::SimHitTree(const edm::ParameterSet& iConfig)
{
	tok_hcal_ = consumes<edm::PCaloHitContainer>(iConfig.getUntrackedParameter<edm::InputTag>("SimHitCollectionLabel"));
	
	sub_ = iConfig.getUntrackedParameter<int>("SubDetector");

	testNumbering_ = iConfig.getParameter<bool>("TestNumbering");
	if (testNumbering_){
		theRelabeller=new HcalHitRelabeller(iConfig);
		std::cout << "Test numbering enabled" << std::endl;
	}

	edm::Service<TFileService> fs;
	tt1 = fs->make<TTree>("SimHitTree","SimHitTree");

	tt1->Branch("SimHitsEn","std::vector<float>",&SimHitsEn);
	tt1->Branch("SimHitsIeta","std::vector<int>",&SimHitsIeta);
	tt1->Branch("SimHitsIphi","std::vector<int>",&SimHitsIphi);
	tt1->Branch("SimHitsDepth","std::vector<int>",&SimHitsDepth);
}

SimHitTree::~SimHitTree()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
SimHitTree::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

	//SimHit Stuff

	SimHitsEn.clear();
	SimHitsIeta.clear();
	SimHitsIphi.clear();
	SimHitsDepth.clear();

	//////////////////////////////////
	///SimHits
	//////////////////////////////////
	edm::Handle<std::vector<PCaloHit>> hcalHits;
	iEvent.getByToken(tok_hcal_,hcalHits);
	std::vector<PCaloHit>  SimHitResult = *hcalHits.product () ;

	if(testNumbering_){
		iSetup.get<CaloGeometryRecord>().get(geometry);
		iSetup.get<HcalRecNumberingRecord>().get(pHRNDC);
		theGeometry = &*geometry;
		theRecNumber= &*pHRNDC;
		theRelabeller->setGeometry(theGeometry,theRecNumber);
		//std::cout << "Provided Geometry and Rec Numbering" << std::endl;

		theRelabeller->process(SimHitResult);
	}

//	std::cout << "sub_ = " << sub_ << std::endl;

	for (std::vector<PCaloHit>::const_iterator SimHits = SimHitResult.begin () ; SimHits != SimHitResult.end(); ++SimHits){
		HcalDetId cell(SimHits->id());
		double en   = SimHits->energy();    
 
		int sub     = cell.subdet();
		int depth   = cell.depth();
		int ieta = cell.ieta();
		int iphi = cell.iphi();

		if(sub == sub_ ){
			SimHitsEn.push_back(en);
			SimHitsIphi.push_back(iphi);
			SimHitsIeta.push_back(ieta);
			SimHitsDepth.push_back(depth);
			
		}

	}

	tt1->Fill(); //Fill the tree

	return 1;
} //SimHitTree::filter

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void
SimHitTree::beginStream(edm::StreamID)
{
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
SimHitTree::endStream() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
SimHitTree::beginRun(edm::Run const&, edm::EventSetup const& es)
{ 

}	
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
SimHitTree::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
SimHitTree::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
SimHitTree::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
SimHitTree::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(SimHitTree);
