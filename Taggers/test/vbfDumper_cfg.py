#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
from FWCore.ParameterSet.VarParsing import VarParsing

from flashgg.MetaData.samples_utils import SamplesManager

## CMD LINE OPTIONS ##
options = VarParsing('analysis')
print options

# maxEvents is the max number of events processed of each file, not globally
options.maxEvents = -1
options.inputFiles = "file:myTagOutputFile.root" 
options.outputFile = "VBFTagsDump.root" 
options.parseArguments()

## I/O SETUP ##
process = cms.Process("VBFTagsDumper")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1 )
process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(options.inputFiles))

if options.maxEvents > 0:
    process.source.eventsToProcess = cms.untracked.VEventRange('1:1-1:'+str(options.maxEvents))

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(options.outputFile))

process.load("flashgg.Taggers.VBFTagDumper_cfi") ##  import diphotonTagDumper 
import flashgg.Taggers.dumperConfigTools as cfgTools

process.diphotonDumper.className = "DiPhotonTagDumper"
process.diphotonDumper.src = "flashggVBFTag"
process.diphotonDumper.processId = "test"
process.diphotonDumper.dumpTrees = True
process.diphotonDumper.dumpWorkspace = False
process.diphotonDumper.dumpHistos = True

process.diphotonDumper.quietRooFit = True

## define categories and associated objects to dump
cfgTools.addCategory(process.diphotonDumper,
                    "flashggVBFTag",
                          subcats=3,
                                  variables=["CMS_hgg_mass[160,100,180]:=diPhoton().mass", 
                                  "leadPt                   :=diPhoton().leadingPhoton.pt",
                                  "subleadPt                :=diPhoton().subLeadingPhoton.pt",
                                             "diphoMVA                 :=diPhotonMVA().result",    
                                  "maxEta                   :=max(abs(diPhoton().leadingPhoton.superCluster.eta),abs(diPhoton().leadingPhoton.superCluster.eta))",
                                               "leadJetPt                :=leadingJet().pt",
                                               "subleadJetPt             :=subLeadingJet().pt",
                                               "VBFMVA                   :=VBFMVA().VBFMVAValue()",
                                             "genZ           :=tagTruth().genPV().z",
                                             "vtxZ           :=diPhoton().vtx().z",
                                             "dZ            :=abs(tagTruth().genPV().z-diPhoton().vtx().z)",
                                             "leadQuarkPt                  := tagTruth().pt_Q1",
                                             "matchingGenJetToLeadPt    := tagTruth().pt_genPartMatchingToJ1"
                                  ],
                       histograms=["CMS_hgg_mass>>mass(160,100,180)",
                                   "subleadPt:leadPt>>ptLeadvsSub(180,20,200:180,20,200)",
                                   "diphoMVA>>diphoMVA(50,0,1)",
                                   "maxEta>>maxEta[0.,0.1,0.2,0.3,0.4,0.6,0.8,1.0,1.2,1.4442,1.566,1.7,1.8,2.,2.2,2.3,2.5]",
                                      "subleadJetPt:leadJetPt>>JetptLeadvsSub(8,20,100:8,20,100)",
                                   "VBFMVA>>VBFMVA(50,0,1)"
                                   ]
                    )
process.p1 = cms.Path(
    process.diphotonDumper
    )

print process.p1
