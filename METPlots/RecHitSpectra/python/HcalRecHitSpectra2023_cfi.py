import FWCore.ParameterSet.Config as cms

hcalRecHitSpectra2023 = cms.EDAnalyzer("RecHitSpectra2023",
    rootOutputFile            = cms.string('HcalRecHitSpectra.root'),

    HBHERecHitCollectionLabel = cms.untracked.InputTag("hbheUpgradeReco"),
    HFRecHitCollectionLabel   = cms.untracked.InputTag("hfUpgradeReco"),

)
