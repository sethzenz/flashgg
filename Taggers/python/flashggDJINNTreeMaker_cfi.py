import FWCore.ParameterSet.Config as cms
from flashgg.MicroAOD.flashggJets_cfi import flashggBTag, maxJetCollections

UnpackedJetCollectionVInputTag = cms.VInputTag()
for i in range(0,maxJetCollections):
    UnpackedJetCollectionVInputTag.append(cms.InputTag('flashggUnpackedJets',str(i)))

flashggDJINNTreeMaker = cms.EDAnalyzer('FlashggDJINNTreeMaker',
                                    MVAResultTag = cms.InputTag('flashggDiPhotonMVA'),
                                    DiPhotonTag = cms.InputTag('flashggPreselectedDiPhotons'),
                                    VBFMVAResultTag=cms.InputTag('flashggVBFMVA'),
                                    inputTagJets = UnpackedJetCollectionVInputTag, 
                                    TagSorter = cms.InputTag('flashggTagSorter')
                                  )
