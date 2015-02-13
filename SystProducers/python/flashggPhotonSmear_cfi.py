import FWCore.ParameterSet.Config as cms

flashggSmearPhoton = cms.EDProducer('FlashggPhotonSystematicProducer',
		PhotonTag = cms.untracked.InputTag('flashggPhotons'),
		SystMethodNames = cms.VPSet(cms.PSet( MethodName = cms.string("FlashggSimplePhotonSmear")),
			                    cms.PSet( MethodName = cms.string("FlashggShiftPhotonId"))	
			)
		)
