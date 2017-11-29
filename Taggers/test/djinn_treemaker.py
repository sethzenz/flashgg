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

test_path = 'root://gfe02.grid.hep.ph.ic.ac.uk:1095//store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISummer16-2_4_5-25ns_Moriond17/2_4_5/GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8/RunIISummer16-2_4_5-25ns_Moriond17-2_4_5-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/171115_091126/0000/myMicroAODOutputFile_103.root'
snip = test_path.split('/2_4_5/')[-1].split('/RunIISummer16')[0]


process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring("file:ForJack_VBF_1k_10Nov.root"))
#process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring("file:myMicroAODOutputFile_103.root"))
#process.source = cms.Source ("PoolSource", fileNames = cms.untracked.vstring("root://gfe02.grid.hep.ph.ic.ac.uk:1095//store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISummer16-2_4_5-25ns_Moriond17/2_4_5/GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8/RunIISummer16-2_4_5-25ns_Moriond17-2_4_5-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/171115_091126/0000/myMicroAODOutputFile_103.root"))

from flashgg.MetaData.JobConfig import JobConfig
customize = JobConfig(crossSections=["$CMSSW_BASE/src/flashgg/MetaData/data/cross_sections.json"])
customize.setDefault("maxEvents",-1)
customize.setDefault("targetLumi",2.6e+4)
customize.parse()


process.load("flashgg/Taggers/flashggTagSequence_cfi")
process.load("flashgg/Taggers/flashggDJINNTreeMaker_cfi")

process.flashggDJINNTreeMaker.lumiWeight = cms.double(0.5)
process.flashggDJINNTreeMaker.xs = cms.double(0.5)

#from flashgg.Taggers.flashggTagOutputCommands_cff import tagDefaultOutputCommand
#process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myTagOutputFile.root'),
#                               outputCommands = tagDefaultOutputCommand			       
#                               )
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("JetVariableOutput.root"),
                                   closeFileFast = cms.untracked.bool(True))

process.p = cms.Path(process.flashggTagSequence*process.flashggDJINNTreeMaker)

#process.e = cms.EndPath(process.out)
