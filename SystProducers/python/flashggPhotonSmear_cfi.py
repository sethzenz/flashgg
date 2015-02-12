import FWCore.ParameterSet.Config as cms

flashggSmearPhoton = cms.EDProducer('FlashggPhotonSystematicProducer',
		PhotonTag = cms.untracked.InputTag('flashggPhotons'),
		SystMethodNames = cms.VPSet(cms.PSet( name = cms.untracked.string("FlashggSimplePhotonSmear")),
			                    cms.PSet( name = cms.untracked.string("FlashggShiftPhotonId"))	
			)
		)
