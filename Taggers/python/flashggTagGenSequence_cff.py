import FWCore.ParameterSet.Config as cms
from flashgg.Taggers.flashggVBFTruth_cfi import flashggVBFTruth

flashggTagGenSequence = cms.Sequence(flashggVBFTruth)

