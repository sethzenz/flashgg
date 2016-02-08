# ================================================
#   MicroAOD and Jet Tree Producer
#   Y. Haddad 01/2015
# ================================================

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
from flashgg.MicroAOD.flashggJets_cfi import flashggBTag, maxJetCollections
process = cms.Process("FLASHggJetValidation")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '76X_mcRun2_asymptotic_v13')

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService")
process.RandomNumberGeneratorService.flashggRandomizedPhotons = cms.PSet(
          initialSeed = cms.untracked.uint32(16253245)
        )

# +++++ the number of processed events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( 50000 ) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 100 )

# +++++ the source file
process.source = cms.Source("PoolSource",
                            fileNames=cms.untracked.vstring(#"/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-JetUpdate-BetaV7J1/Spring15BetaV7J1/VBFHToGG_M-125_13TeV_powheg_pythia8/RunIISpring15-JetUpdate-BetaV7J1-Spring15BetaV7J1-v0-RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/151127_155759/0000/myMicroAODOutputFile_1.root")
#"root://eoscms.cern.ch//eos/cms/store/mc/RunIIFall15MiniAODv2/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/60000/E67F894B-99C1-E511-9DFA-0025905C95FA.root"
"file:E67F894B-99C1-E511-9DFA-0025905C95FA.root"
))


process.MessageLogger.cerr.threshold = 'ERROR'

process.TFileService = cms.Service("TFileService",fileName  = cms.string("jetValidationTrees_VBF_HToGG_test.root"))

# process.flashggJetValidationTreeMaker = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
#                                                        GenParticleTag           = cms.untracked.InputTag('prunedGenParticles'),
#                                                        JetTagDz                 = cms.InputTag("flashggJets"),
#                                                        StringTag		= cms.string("PF"),
#                                                    )
#

#JetCollectionVInputTagPFCHS = cms.VInputTag()
#JetCollectionVInputTagPUPPI = cms.VInputTag()
#for i in range(0,5):
#    JetCollectionVInputTagPFCHS.append(cms.InputTag('flashggPFCHSJets' + str(i)))
    #JetCollectionVInputTagPUPPI.append(cms.InputTag('flashggPUPPIJets' + str(i)))

process.flashggUnpackedJets = cms.EDProducer("FlashggVectorVectorJetUnpacker",
                                             JetsTag = cms.InputTag("flashggFinalJets"),
                                             NCollections = cms.uint32(maxJetCollections)
                                     )

UnpackedJetCollectionVInputTag = cms.VInputTag()
for i in range(0,maxJetCollections):
    UnpackedJetCollectionVInputTag.append(cms.InputTag('flashggUnpackedJets',str(i)))
    
process.flashggJetValidationTreeMakerPFCHS = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                            GenParticleTag  = cms.untracked.InputTag('flashggPrunedGenParticles'),
                                                            DiPhotonTag     = cms.InputTag('flashggDiPhotons'),
                                                            inputTagJets    = UnpackedJetCollectionVInputTag,
                                                            StringTag	    = cms.string("PFCHS"),
                                                            PFCandidatesTag=cms.InputTag('packedPFCandidates'),
                                                            )
#
#process.flashggJetValidationTreeMakerPUPPI = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
#                                                            GenParticleTag = cms.untracked.InputTag('prunedGenParticles'),
#                                                            DiPhotonTag     = cms.InputTag('flashggDiPhotons'),
#                                                            inputTagJets   = JetCollectionVInputTagPUPPI,
#                                                            StringTag      = cms.string("PUPPI"),
#                                                        )

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('testetstestst.root'),
                               outputCommands = cms.untracked.vstring())

process.options = cms.untracked.PSet( allowUnscheduled = cms.untracked.bool(True) )

process.load("flashgg/MicroAOD/flashggMicroAODSequence_cff")

process.p = cms.Path( 
    process.flashggMicroAODSequence +
    process.flashggUnpackedJets +  
    process.flashggJetValidationTreeMakerPFCHS 
    )
#process.e = cms.EndPath(process.out)

from flashgg.MicroAOD.MicroAODCustomize import customize
customize(process)
