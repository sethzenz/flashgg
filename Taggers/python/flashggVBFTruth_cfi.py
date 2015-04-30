import FWCore.ParameterSet.Config as cms

flashggVBFTruth = cms.EDProducer('FlashggVBFTruthProducer',
                                 VbfTag = cms.untracked.InputTag('flashggVBFTag'),
                                 GenParticleTag   = cms.untracked.InputTag('prunedGenParticles'),
                                 GenJetTag = cms.untracked.InputTag('ak4GenJets'),
                                 Debug = cms.untracked.bool(True)
                                 )
