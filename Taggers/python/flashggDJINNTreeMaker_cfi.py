import FWCore.ParameterSet.Config as cms

flashggDJINNTreeMaker = cms.EDAnalyzer('FlashggDJINNTreeMaker',
                                  TagSorter = cms.InputTag('flashggTagSorter'),
                                  )
