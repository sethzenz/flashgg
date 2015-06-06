
import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggMicroAOD")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = 'PLS170_V7AN1::All'
#process.GlobalTag.globaltag = 'auto:run2_mc'
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( 10) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

# PHYS14 Files
#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Phys14DR/TTbarH_HToGG_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/10000/7CD4EFBC-9C70-E411-94A0-002590200828.root"))
process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Phys14DR/GluGluToHToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/3C2EFAB1-B16F-E411-AB34-7845C4FC39FB.root", "/store/mc/Phys14DR/GluGluToHToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/E270C596-4F70-E411-9EB9-7845C4FC347F.root"))

'''
process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring(
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_1.root", 
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_2.root", 
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_3.root", 
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_4.root", 
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_5.root", 
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_6.root", 
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_7.root", 
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_8.root", 
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_9.root", 
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_10.root", 
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_11.root", 
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_12.root", 
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_13.root", 
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_14.root", 
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_15.root", 
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_16.root", 
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_17.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_18.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_19.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_20.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_21.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_22.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_23.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_24.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_25.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_26.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_27.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_28.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_29.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_30.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_31.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_32.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_33.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_34.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_35.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_36.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_37.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_38.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_39.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_40.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_41.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_42.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_43.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_44.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_45.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_46.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_47.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_48.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_49.root",
	"/store/user/rateixei/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Fall13-POSTLS162_V1-v2_PU20bx25_Phys14_miniAOD/150423_200112/0000/HH2BBGG-Phys14DR_MINIAOD_50.root"))
'''

#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Phys14DR/VBF_HToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/4A8E0BD1-026C-E411-8760-00266CFFA418.root")
#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Phys14DR/WH_ZH_HToGG_M-125_13TeV_pythia6/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/24B70163-5769-E411-93CA-002590200A28.root"))
### process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Phys14DR/GJet_Pt40_doubleEMEnriched_TuneZ2star_13TeV-pythia6/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/101611CC-026E-E411-B8D7-00266CFFBF88.root"))
## process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Phys14DR/GJets_HT-100to200_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/00D67F78-2873-E411-B3BB-0025907DC9C0.root"))

# 740 RelVal
#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/relval/CMSSW_7_4_0_pre9_ROOT6/RelValH130GGgluonfusion_13/MINIAODSIM/MCRUN2_74_V7-v1/00000/0A35F6D-DAD1-E411-A8CC-0026189438CC.root"))                                                                                                                                             


process.MessageLogger.cerr.threshold = 'ERROR' # can't get suppressWarning to work: disable all warnings for now
# process.MessageLogger.suppressWarning.extend(['SimpleMemoryCheck','MemoryCheck']) # this would have been better...

# Uncomment the following if you notice you have a memory leak
# This is a lightweight tool to digg further
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#                                        ignoreTotal = cms.untracked.int32(1),
#                                        monitorPssAndPrivate = cms.untracked.bool(True)
#                                       )

process.load("flashgg/MicroAOD/flashggMicroAODSequence_cff")

from flashgg.MicroAOD.flashggMicroAODOutputCommands_cff import microAODDefaultOutputCommand,microAODDebugOutputCommand
process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myMicroAODOutputFile_hgg_legacyvtx_sizeTest_1k.root'),
                               outputCommands = microAODDefaultOutputCommand
                               )
process.out.outputCommands += microAODDebugOutputCommand # extra items for debugging, CURRENTLY REQUIRED

# need to allow unscheduled processes otherwise reclustering function will fail
# this is because of the jet clustering tool, and we have to live with it for now.
process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
    )
# import function which takes care of reclustering the jets using legacy vertex		
from flashgg.MicroAOD.flashggJets_cfi import addFlashggPFCHSLegJets 
# call the function, it takes care of everything else.
addFlashggPFCHSLegJets(process)


process.p = cms.Path(process.flashggMicroAODSequence)
process.e = cms.EndPath(process.out)

# Uncomment these lines to run the example commissioning module and send its output to root
#process.commissioning = cms.EDAnalyzer('flashggCommissioning',
#                                       PhotonTag=cms.untracked.InputTag('flashggPhotons'),
#                                       DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
#                                       VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices')
#)
#process.TFileService = cms.Service("TFileService",
#                                   fileName = cms.string("commissioningTree.root")
#)
#process.p *= process.commissioning


from flashgg.MicroAOD.MicroAODCustomize import customize
customize(process)
