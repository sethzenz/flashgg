import FWCore.ParameterSet.Config as cms

flashggSelectedDiPhotons = cms.EDFilter("FLASHggDiPhotonSelector",
    src = cms.InputTag("flashggDiPhotons"),
    cut = cms.string("mass>50. && leadingPhoton.pt>20. && subLeadingPhoton.pt>15. && abs(leadingPhoton.eta)<2.5 && abs(subLeadingPhoton.eta)<2.5")
)
