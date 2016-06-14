import FWCore.ParameterSet.Config as cms

hcalRecHitTree = cms.EDAnalyzer("RecHitTree",
    rootOutputFile            = cms.string('HcalRecHitSpectra.root'),

    HBHERecHitCollectionLabel = cms.untracked.InputTag("hbheUpgradeReco"),
    HFRecHitCollectionLabel   = cms.untracked.InputTag("hfUpgradeReco"),

)
