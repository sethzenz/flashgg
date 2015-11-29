import FWCore.ParameterSet.Config as cms

flashggTagSorter = cms.EDProducer('FlashggTagSorter',
                                  DiPhotonTag = cms.InputTag('flashggDiPhotons'),
                                  # Top of list is highest priority
                                  # Optionally can add category ranges if priority depends on category number
                                  TagPriorityRanges = cms.VPSet(
                                                                cms.PSet(TagName = cms.InputTag('flashggVBFTag')),
                                                                cms.PSet(TagName = cms.InputTag('flashggUntagged'))
                                                                ),
                                  massCutUpper=cms.untracked.double(180.),
                                  massCutLower=cms.untracked.double(100)
                                  )

#                                  TagVectorTag = cms.untracked.VInputTag(
#                                                                        cms.untracked.InputTag('flashggVHLooseTag'),
#                                                                        cms.untracked.InputTag('flashggVHTightTag'),
#                                                                        cms.untracked.InputTag('flashggTTHLeptonicTag'),
#                                                                        cms.untracked.InputTag('flashggTTHHadronicTag'),
#                                                                        cms.untracked.InputTag('flashggVBFTag'),
#                                                                        cms.untracked.InputTag('flashggVHHadronicTag'),
#                                                                        cms.untracked.InputTag('flashggUntagged'),

