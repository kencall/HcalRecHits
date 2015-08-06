import FWCore.ParameterSet.Config as cms

hcalRecHitSpectra = cms.EDAnalyzer("RecHitSpectra",
    rootOutputFile            = cms.string('HcalRecHitSpectra.root'),

    HBHERecHitCollectionLabel = cms.untracked.InputTag("hbhereco"),
    HFRecHitCollectionLabel   = cms.untracked.InputTag("hfreco"),
    HORecHitCollectionLabel   = cms.untracked.InputTag("horeco"),

    VertexCollectionLabel     = cms.untracked.InputTag("offlinePrimaryVertices"),

    CaloTowerCollectionLabel  = cms.untracked.InputTag("towerMaker")
)
