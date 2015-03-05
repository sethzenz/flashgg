# ================================================
#   MicroAOD and Jet Tree Producer
#   Y. Haddad 01/2015
# ================================================

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggJetValidation")
process.load("FWCore.MessageService.MessageLogger_cfi")

# +++++ the number of processed events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( -1 ) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 100 )

# +++++ the input source file
process.source = cms.Source("PoolSource",                #fileNames=cms.untracked.vstring("file:/afs/cern.ch/work/y/yhaddad/JobSending/HggPUJIDValPhys14/crab_Phys14MicroAODV1_VBF_HToGG_M-125_13TeV-powheg-pythia6_00/results/JetValidationMicroAOD_1.root")) 
#                        fileNames=cms.untracked.vstring("/store/group/phys_higgs/cmshgg/sethzenz/flashgg/HggPhys14/Phys14MicroAODJetV6/VBF_HToGG_M-125_13TeV-powheg-pythia6/HggPhys14-Phys14MicroAODJetV6-v0-Phys14DR-PU20bx25_PHYS14_25_V1-v1/150228_112259/0000/myMicroAODOutputFile_1.root",
#"/store/group/phys_higgs/cmshgg/sethzenz/flashgg/HggPhys14/Phys14MicroAODJetV6/VBF_HToGG_M-125_13TeV-powheg-pythia6/HggPhys14-Phys14MicroAODJetV6-v0-Phys14DR-PU20bx25_PHYS14_25_V1-v1/150228_112259/0000/myMicroAODOutputFile_2.root",
#"/store/group/phys_higgs/cmshgg/sethzenz/flashgg/HggPhys14/Phys14MicroAODJetV6/VBF_HToGG_M-125_13TeV-powheg-pythia6/HggPhys14-Phys14MicroAODJetV6-v0-Phys14DR-PU20bx25_PHYS14_25_V1-v1/150228_112259/0000/myMicroAODOutputFile_3.root",
#"/store/group/phys_higgs/cmshgg/sethzenz/flashgg/HggPhys14/Phys14MicroAODJetV6/VBF_HToGG_M-125_13TeV-powheg-pythia6/HggPhys14-Phys14MicroAODJetV6-v0-Phys14DR-PU20bx25_PHYS14_25_V1-v1/150228_112259/0000/myMicroAODOutputFile_4.root"))
                            fileNames=cms.untracked.vstring("/store/group/phys_higgs/cmshgg/sethzenz/flashgg/HggPhys14/Phys14MicroAODJetV7/VBF_HToGG_M-125_13TeV-powheg-pythia6/HggPhys14-Phys14MicroAODJetV7-v0-Phys14DR-PU20bx25_PHYS14_25_V1-v1/150303_124646/0000/myMicroAODOutputFile_1.root",
                                                            "/store/group/phys_higgs/cmshgg/sethzenz/flashgg/HggPhys14/Phys14MicroAODJetV7/VBF_HToGG_M-125_13TeV-powheg-pythia6/HggPhys14-Phys14MicroAODJetV7-v0-Phys14DR-PU20bx25_PHYS14_25_V1-v1/150303_124646/0000/myMicroAODOutputFile_2.root",
                                                            "/store/group/phys_higgs/cmshgg/sethzenz/flashgg/HggPhys14/Phys14MicroAODJetV7/VBF_HToGG_M-125_13TeV-powheg-pythia6/HggPhys14-Phys14MicroAODJetV7-v0-Phys14DR-PU20bx25_PHYS14_25_V1-v1/150303_124646/0000/myMicroAODOutputFile_3.root",
                                                            "/store/group/phys_higgs/cmshgg/sethzenz/flashgg/HggPhys14/Phys14MicroAODJetV7/VBF_HToGG_M-125_13TeV-powheg-pythia6/HggPhys14-Phys14MicroAODJetV7-v0-Phys14DR-PU20bx25_PHYS14_25_V1-v1/150303_124646/0000/myMicroAODOutputFile_4.root"))

process.MessageLogger.cerr.threshold = 'ERROR'

# +++++ the output files
process.TFileService = cms.Service("TFileService",fileName = 
                                   #cms.string("/afs/cern.ch/work/y/yhaddad/VBF_HToGG_M-125_13TeV_JetValidationTree_v02.root"))
                                   cms.string("Seth_V7_VBF.root"))

# +++++ the processes
process.flashggJetValidationTreeMaker         = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                               GenParticleTag = cms.untracked.InputTag('prunedGenParticles'),
                                                               JetTagDz       = cms.InputTag("flashggJets"),
                                                               StringTag      = cms.string("PF"),
                                                           )

process.flashggJetValidationTreeMakerPFCHS0   = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                               GenParticleTag = cms.untracked.InputTag('prunedGenParticles'),
                                                               JetTagDz       = cms.InputTag("flashggJetsPFCHS0"),
                                                               StringTag      = cms.string("PFCHS0"),
                                                           )

process.flashggJetValidationTreeMakerPFCHSLeg = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                               GenParticleTag = cms.untracked.InputTag('prunedGenParticles'),
                                                               JetTagDz       = cms.InputTag("flashggJetsPFCHSLeg"),
                                                               StringTag      = cms.string("PFCHSLeg"),
                                                           )


#pfColl checkers

process.flashggPFCollAnalyzer = cms.EDAnalyzer('FlashggFlashggPFCollAnalyzer',
                                               CollTagPF         = cms.InputTag("packedPFCandidates"),
                                               CollTagPFPFCHS0   = cms.InputTag("pfNoElectronsCHS0"),
                                               CollTagPFPFCHSLeg = cms.InputTag("pfNoElectronsCHSLeg"),
                                           )

#process.out = cms.OutputModule("PoolOutputModule", 
#                               #fileName = cms.untracked.string('myMicroAODOutputFile.root'),
#                               fileName = cms.untracked.vstring(),
#                               outputCommands = cms.untracked.vstring())
#
process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)

process.p = cms.Path(
#    process.flashggPFCollAnalyzer +
    process.flashggJetValidationTreeMaker +
    process.flashggJetValidationTreeMakerPFCHS0 +
    process.flashggJetValidationTreeMakerPFCHSLeg 
)

#process.e = cms.EndPath(process.out)
