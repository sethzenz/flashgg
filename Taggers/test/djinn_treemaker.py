import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggTag")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring("file:ForJack_VBF_1k_10Nov.root"))

process.load("flashgg/Taggers/flashggTagSequence_cfi")
#flashggDJINNTreeMaker = cms.EDAnalyzer('FlashggDJINNTreeMaker',
#                                  TagSorter = cms.InputTag('flashggTagSorter'),
#                                  )
#process.load(flashggDJINNTreeMaker)
process.load("flashgg/Taggers/flashggTagTester_cfi")
process.load("flashgg/Taggers/flashggDJINNTreeMaker_cfi")


from flashgg.Taggers.flashggTagOutputCommands_cff import tagDefaultOutputCommand

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myTagOutputFile.root'),
                               outputCommands = tagDefaultOutputCommand			       
                               )

process.p = cms.Path(process.flashggTagSequence*process.flashggDJINNTreeMaker)

process.e = cms.EndPath(process.out)
