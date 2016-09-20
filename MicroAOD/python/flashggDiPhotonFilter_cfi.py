import FWCore.ParameterSet.Config as cms

diPhotonFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("flashggSelectedDiPhotons"),
    minNumber = cms.uint32(1)
)
