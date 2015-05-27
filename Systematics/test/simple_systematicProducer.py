import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggSyst")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )
#process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 100 )

# Uncomment the following if you notice you have a memory leak
# This is a lightweight tool to digg further
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#                                        ignoreTotal = cms.untracked.int32(1),
#                                        monitorPssAndPrivate = cms.untracked.bool(True)
#                                       )

process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring("file:myMicroAODOutputFile.root"))

process.load("flashgg.Systematics.flashggPhotonSmear_cfi")

# Code to artificially scale photon energies to make different mass points for signal fit tests
srcMass = 125.
targetMass = 130.
process.flashggSmearPhoton.SystMethods.append(cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleString"),
                                                        MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                        Label = cms.string("FakeMassScale"),
                                                        NSigmas = cms.vint32(0),
                                                        OverallRange = cms.string("1"),
                                                        Bins = cms.VPSet(cms.PSet( Range = cms.string("pt>20."), Shift = cms.double(targetMass/srcMass - 1.), Uncertainty = cms.double(0.0004))),
                                                        Debug = cms.untracked.bool(True)
                                                        )
                                              )


# add new code in here

# DEPRECATED
for pset in process.flashggSmearPhoton.SystMethods:
    if pset.MethodName == "FlashggDiPhotonFromPhoton" and pset.PhotonMethodName == "FlashggPhotonScaleString":
        print pset

from flashgg.Taggers.flashggTagOutputCommands_cff import tagDefaultOutputCommand

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('mySystOutputFile.root'),
                               outputCommands = tagDefaultOutputCommand			       
                               )

process.p = cms.Path(process.flashggSmearPhoton)

process.e = cms.EndPath(process.out)
