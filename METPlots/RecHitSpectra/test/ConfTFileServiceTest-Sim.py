import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

from Configuration.StandardSequences.Eras import eras

process = cms.Process("Trees",eras.Phase2)


# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryExtended2023simReco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )                                              

#mylist = FileUtils.loadListFromFile ('dataset.txt')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.MessageLogger.cerr.FwkReport.reportEvery = 500

#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'GR_R_74_V13A::All'
#process.GlobalTag.globaltag = 'GR_P_V56::All'
#process.GlobalTag.globaltag = 'MCRUN2_71_V1::All'
#process.GlobalTag.globaltag = 'MCRUN2_74_V8::All'
#process.GlobalTag.globaltag = 'MCRUN2_74_V8B::All'

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_v14', '')

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
	'/store/relval/CMSSW_8_1_0_pre6/RelValTTbar_14TeV/GEN-SIM-RECO/80X_mcRun2_asymptotic_v14_2023LReco-v1/00000/3C1D52ED-9629-E611-8E77-0025905B8600.root',
	'/store/relval/CMSSW_8_1_0_pre6/RelValTTbar_14TeV/GEN-SIM-RECO/80X_mcRun2_asymptotic_v14_2023LReco-v1/00000/4E5C0F20-3329-E611-B827-0CC47A4D7690.root',
	'/store/relval/CMSSW_8_1_0_pre6/RelValTTbar_14TeV/GEN-SIM-RECO/80X_mcRun2_asymptotic_v14_2023LReco-v1/00000/60B03426-6B28-E611-A89A-0CC47A4D7686.root',
	'/store/relval/CMSSW_8_1_0_pre6/RelValTTbar_14TeV/GEN-SIM-RECO/80X_mcRun2_asymptotic_v14_2023LReco-v1/00000/98D2ECF0-9629-E611-A7DF-0025905B857E.root',
	'/store/relval/CMSSW_8_1_0_pre6/RelValTTbar_14TeV/GEN-SIM-RECO/80X_mcRun2_asymptotic_v14_2023LReco-v1/00000/B0727767-7028-E611-ABA9-0025905B8572.root',
	'/store/relval/CMSSW_8_1_0_pre6/RelValTTbar_14TeV/GEN-SIM-RECO/80X_mcRun2_asymptotic_v14_2023LReco-v1/00000/B82574C8-7F28-E611-BA7B-0CC47A4D76B6.root',
	'/store/relval/CMSSW_8_1_0_pre6/RelValTTbar_14TeV/GEN-SIM-RECO/80X_mcRun2_asymptotic_v14_2023LReco-v1/00000/D4498450-5C28-E611-8C01-0CC47A4C8E8A.root',
	'/store/relval/CMSSW_8_1_0_pre6/RelValTTbar_14TeV/GEN-SIM-RECO/80X_mcRun2_asymptotic_v14_2023LReco-v1/00000/FC8EE7BB-7028-E611-B6DC-0CC47A4D7698.root',
	'/store/relval/CMSSW_8_1_0_pre6/RelValTTbar_14TeV/GEN-SIM-RECO/80X_mcRun2_asymptotic_v14_2023LReco-v1/00000/FE86EE41-7628-E611-8D0E-0CC47A4D75EC.root'
    )
)

########## Good run list ##########
#import PhysicsTools.PythonAnalysis.LumiList as LumiList
#process.source.lumisToProcess = LumiList.LumiList(filename = 'Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON_v2.txt').getVLuminosityBlockRange()
##########


process.TFileService = cms.Service("TFileService", fileName = cms.string("SimHits-TTBar6.root") )

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

process.load('METPlots/RecHitSpectra/SimHitTree_cfi')
process.hcalSimHitTree.TestNumbering = cms.untracked.bool(True)

process.plots = cms.Path(process.hcalSimHitTree)

