import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggTag")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = 'POSTLS170_V5::All'
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc')
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
#process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 100 )

# Uncomment the following if you notice you have a memory leak
# This is a lightweight tool to digg further
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#                                        ignoreTotal = cms.untracked.int32(1),
#                                        monitorPssAndPrivate = cms.untracked.bool(True)
#                                       )

process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring("/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-ReMiniAOD-BetaV7-25ns/Spring15BetaV7/VBFHToGG_M-120_13TeV_powheg_pythia8/RunIISpring15-ReMiniAOD-BetaV7-25ns-Spring15BetaV7-v0-RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/151021_152730/0000/myMicroAODOutputFile_1.root"))
#process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring("/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-ReMiniAOD-BetaV7-25ns/Spring15BetaV7/GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8/RunIISpring15-ReMiniAOD-BetaV7-25ns-Spring15BetaV7-v0-RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/151021_160300/0000/myMicroAODOutputFile_1.root"))
#process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring("/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-ReMiniAOD-BetaV7-25ns/Spring15BetaV7/DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa/RunIISpring15-ReMiniAOD-BetaV7-25ns-Spring15BetaV7-v0-RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/151021_151551/0000/myMicroAODOutputFile_10.root"))
#process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring("/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-ReMiniAOD-BetaV7-25ns/Spring15BetaV7/GluGluHToGG_M-120_13TeV_powheg_pythia8/RunIISpring15-ReMiniAOD-BetaV7-25ns-Spring15BetaV7-v0-RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/151021_152027/0000/myMicroAODOutputFile_2.root"))
process.maxEvents.input = 10

process.load("flashgg/Taggers/flashggTagSequence_cfi")
#process.load("flashgg/Taggers/flashggTagTester_cfi")

from PhysicsTools.PatAlgos.tools.helpers import massSearchReplaceAnyInputTag                                                                                                       
massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggDiPhotons"),cms.InputTag("flashggPreselectedDiPhotons"))                                              

process.flashggVBFMVAOpposite = process.flashggVBFMVA.clone(RequireOppositeEta=cms.untracked.bool(True))
process.flashggVBFMVAHighest = process.flashggVBFMVA.clone(UseHighestDijetMinv=cms.untracked.bool(True),MinJetPt=cms.untracked.double(20.))

#        _requireOppositeEta( iConfig.getUntrackedParameter<bool>( "RequireOppositeEta", false ) ),
#        _useHighestDijetMinv( iConfig.getUntrackedParameter<bool>( "UseHighestDijetMinv", false ) ),


# Run MVAs etc on preselected diphotons only
#from PhysicsTools.PatAlgos.tools.helpers import massSearchReplaceAnyInputTag
#massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggDiPhotons"),cms.InputTag("flashggPreselectedDiPhotons"))

process.flashggVBFWorkflowTest = cms.EDProducer("FlashggVBFWorkflowAnalyzer",
                                                DiPhotonTag=cms.InputTag('flashggDiPhotons'),
                                                PreselectedDiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'),
                                                MVAResultTag=cms.InputTag('flashggDiPhotonMVA'),
                                                VBFMVAResultTagOpposite=cms.InputTag('flashggVBFMVAOpposite'),
                                                VBFMVAResultTagHighest = cms.InputTag('flashggVBFMVAHighest'),
#                                             VBFDiPhoDiJetMVAResultTag=cms.InputTag('flashggVBFDiPhoDiJetMVA'),
                                                VBFMVAResultTag=cms.InputTag('flashggVBFMVA'),
                                                GenParticleTag=cms.InputTag( "flashggPrunedGenParticles" ),
                                                GenJetTag = cms.InputTag("slimmedGenJets"),
                                                )

process.p = cms.Path(process.flashggUnpackedJets
                     * process.flashggVBFMVA
                     * process.flashggVBFMVAOpposite
                     * process.flashggVBFMVAHighest
                     * process.flashggVBFWorkflowTest)

