import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

from Configuration.StandardSequences.Eras import eras

process = cms.Process("RecHitsSpectraGen",eras.Phase2)


# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryExtended2023simReco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )                                              

#mylist = FileUtils.loadListFromFile ('dataset.txt')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.MessageLogger.cerr.FwkReport.reportEvery = 500

#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'GR_R_74_V13A::All'
#process.GlobalTag.globaltag = 'GR_P_V56::All'
#process.GlobalTag.globaltag = 'MCRUN2_71_V1::All'
#process.GlobalTag.globaltag = 'MCRUN2_74_V8::All'
#process.GlobalTag.globaltag = 'MCRUN2_74_V8B::All'

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring('/store/relval/CMSSW_8_1_0_pre6/RelValSinglePiE50HCAL/GEN-SIM-RECO/80X_mcRun2_asymptotic_v14_2023LReco-v1/00000/243E21DB-4A28-E611-9F9A-0025905A611C.root')
)

########## Good run list ##########
#import PhysicsTools.PythonAnalysis.LumiList as LumiList
#process.source.lumisToProcess = LumiList.LumiList(filename = 'Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON_v2.txt').getVLuminosityBlockRange()
##########


#process.load('RecoLocalCalo/HcalRecAlgos/hcalRecAlgoESProd_cfi')

#process.load('CommonTools/RecoAlgos/HBHENoiseFilterResultProducer_cfi')
#process.HBHENoiseFilterResultProducer.minZeros = cms.int32(9999)

#process.ApplyBaselineHBHENoiseFilter = cms.EDFilter(
#	'BooleanFlagFilter',
#	inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResult'),
#	reverseDecision = cms.bool(False)
#)

#process.goodVertices = cms.EDFilter(
#	'VertexSelector',
#	filter = cms.bool(True),
#	src = cms.InputTag("offlinePrimaryVertices"),
#	cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.rho < 2")
#)

process.load('METPlots/RecHitSpectra/HcalRecHitSpectra2023_cfi')
process.hcalRecHitSpectra2023.rootOutputFile            = cms.string('HcalRecHitSpectra2023.root')
process.hcalRecHitSpectra2023.HBHERecHitCollectionLabel = cms.untracked.InputTag("hbheUpgradeReco")
process.hcalRecHitSpectra2023.HFRecHitCollectionLabel   = cms.untracked.InputTag("hfUpgradeReco")

process.plots = cms.Path(process.hcalRecHitSpectra2023)

