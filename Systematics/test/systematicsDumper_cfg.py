#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
from FWCore.ParameterSet.VarParsing import VarParsing

from flashgg.MetaData.samples_utils import SamplesManager

## CMD LINE OPTIONS ##
options = VarParsing('analysis')
print options

targetMass = 120.

# maxEvents is the max number of events processed of each file, not globally
options.maxEvents = 1000
options.inputFiles = "file:myTagOutputFile_%i.root" % int(targetMass)
options.outputFile = "ValidationTagsDump_%i.root" % int(targetMass)
print "before parseArguments()"
options.parseArguments()
print "after parseArguments()"

## I/O SETUP ##
process = cms.Process("ValidationTagsDumper")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1 )
process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(options.inputFiles),
                             eventsToProcess = cms.untracked.VEventRange('1:1-1:'+str(options.maxEvents)))

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(options.outputFile))

process.load("flashgg.Taggers.diphotonTagDumper_cfi") ##  import diphotonTagDumper 
import flashgg.Taggers.dumperConfigTools as cfgTools

process.diphotonDumper.className = "DiPhotonTagDumper"
process.diphotonDumper.src = "flashggTagSorter"
process.diphotonDumper.dumpTrees = True
process.diphotonDumper.dumpWorkspace = True
process.diphotonDumper.dumpHistos = True

process.diphotonDumper.quietRooFit = True


## define categories and associated objects to dump
cfgTools.addCategory(process.diphotonDumper,
                    "flashggDiPhotonUntaggedCategory",
                          subcats=5,
                                  variables=["CMS_hgg_mass[320,100,180]:=diPhoton().mass", 
                                  "leadPt                   :=diPhoton().leadingPhoton.pt",
                                  "subleadPt                :=diPhoton().subLeadingPhoton.pt",
                                             "diphoMVA                 :=diPhotonMVA().result",    
                                  "maxEta                   :=max(abs(diPhoton().leadingPhoton.superCluster.eta),abs(diPhoton().leadingPhoton.superCluster.eta))",
                                  ],
                       histograms=["CMS_hgg_mass>>mass(320,100,180)",
                                   "subleadPt:leadPt>>ptLeadvsSub(180,20,200:180,20,200)",
                                   "diphoMVA>>diphoMVA(50,0,1)",
                                   "maxEta>>maxEta[0.,0.1,0.2,0.3,0.4,0.6,0.8,1.0,1.2,1.4442,1.566,1.7,1.8,2.,2.2,2.3,2.5]"
                                   ]
                    )

cfgTools.addCategory(process.diphotonDumper,
                    "flashggVBFTag",
                          subcats=3,
                                  variables=["CMS_hgg_mass[320,100,180]:=diPhoton().mass", 
                                  "leadPt                   :=diPhoton().leadingPhoton.pt",
                                  "subleadPt                :=diPhoton().subLeadingPhoton.pt",
                                             "diphoMVA                 :=diPhotonMVA().result",    
                                  "maxEta                   :=max(abs(diPhoton().leadingPhoton.superCluster.eta),abs(diPhoton().leadingPhoton.superCluster.eta))",
                                               "leadJetPt                :=leadingJet().pt",
                                               "subleadJetPt             :=subLeadingJet().pt",
                                               "VBFMVA                   :=VBFMVA().VBFMVAValue()",
                                  ],
                       histograms=["CMS_hgg_mass>>mass(320,100,180)",
                                   "subleadPt:leadPt>>ptLeadvsSub(180,20,200:180,20,200)",
                                   "diphoMVA>>diphoMVA(50,0,1)",
                                   "maxEta>>maxEta[0.,0.1,0.2,0.3,0.4,0.6,0.8,1.0,1.2,1.4442,1.566,1.7,1.8,2.,2.2,2.3,2.5]",
                                      "subleadJetPt:leadJetPt>>JetptLeadvsSub(8,20,100:8,20,100)",
                                   "VBFMVA>>VBFMVA(50,0,1)"
                                   ]
                    )

cfgTools.addCategories(process.diphotonDumper,
                    [("flashggVHtightTag",0),
                          ("flashggVHlooseTag",0),
                         ],
                                    variables=["CMS_hgg_mass[320,100,180]:=diPhoton().mass", 
                                  "leadPt                   :=diPhoton().leadingPhoton.pt",
                                  "subleadPt                :=diPhoton().subLeadingPhoton.pt",
                                               "diphoMVA                 :=diPhotonMVA().result",    
                                  "maxEta                   :=max(abs(diPhoton().leadingPhoton.superCluster.eta),abs(diPhoton().leadingPhoton.superCluster.eta))",
                                                 "nMuons                   :=getMuons().size()",
                                                 "nElectrons               :=getElectrons().size()",
                                                 "nJets                    :=getJets().size()",
                                  ],
                       histograms=["CMS_hgg_mass>>mass(320,100,180)",
                                   "subleadPt:leadPt>>ptLeadvsSub(180,20,200:180,20,200)",
                                   "diphoMVA>>diphoMVA(50,0,1)",
                                   "maxEta>>maxEta[0.,0.1,0.2,0.3,0.4,0.6,0.8,1.0,1.2,1.4442,1.566,1.7,1.8,2.,2.2,2.3,2.5]",
                                   "nMuons:nElectrons>>nElectronsVsMuons(2,0,2:2,0,2)",
                                   "nJets>>nJets(5,0,5)"
                                   ]
                       )


cfgTools.addCategory(process.diphotonDumper,
                    "flashggVHhadronicTag",
                                  variables=["CMS_hgg_mass[320,100,180]:=diPhoton().mass", 
                                  "leadPt                   :=diPhoton().leadingPhoton.pt",
                                  "subleadPt                :=diPhoton().subLeadingPhoton.pt",
                                             "diphoMVA                 :=diPhotonMVA().result",    
                                  "maxEta                   :=max(abs(diPhoton().leadingPhoton.superCluster.eta),abs(diPhoton().leadingPhoton.superCluster.eta))",
                                               "leadJetPt                :=leadingJet().pt",
                                               "subleadJetPt             :=subLeadingJet().pt",
                                  ],
                       ## histograms to be plotted. 
                       ## the variables need to be defined first
                       histograms=["CMS_hgg_mass>>mass(320,100,180)",
                                   "subleadPt:leadPt>>ptLeadvsSub(180,20,200:180,20,200)",
                                   "diphoMVA>>diphoMVA(50,0,1)",
                                   "maxEta>>maxEta[0.,0.1,0.2,0.3,0.4,0.6,0.8,1.0,1.2,1.4442,1.566,1.7,1.8,2.,2.2,2.3,2.5]",
                                      "subleadJetPt:leadJetPt>>JetptLeadvsSub(8,20,100:18,20,200)",
                                   ]
                    )

cfgTools.addCategory(process.diphotonDumper,
                    "flashggTTHleptonicTag",
                                 variables=["CMS_hgg_mass[320,100,180]:=diPhoton().mass", 
                                  "leadPt                   :=diPhoton().leadingPhoton.pt",
                                  "subleadPt                :=diPhoton().subLeadingPhoton.pt",
                                            "diphoMVA                 :=diPhotonMVA().result",    
                                  "maxEta                   :=max(abs(diPhoton().leadingPhoton.superCluster.eta),abs(diPhoton().leadingPhoton.superCluster.eta))",
                                              "nMuons                   :=getMuons().size()",
                                              "nElectrons               :=getElectrons().size()",
                                              "nJets                    :=getJets().size()",
                                              "nBJets                   :=getBJets().size()",
                                  ],
                       ## histograms to be plotted. 
                       ## the variables need to be defined first
                       histograms=["CMS_hgg_mass>>mass(320,100,180)",
                                   "subleadPt:leadPt>>ptLeadvsSub(180,20,200:180,20,200)",
                                   "diphoMVA>>diphoMVA(50,0,1)",
                                   "maxEta>>maxEta[0.,0.1,0.2,0.3,0.4,0.6,0.8,1.0,1.2,1.4442,1.566,1.7,1.8,2.,2.2,2.3,2.5]",
                                   "nMuons:nElectrons>>nElectronsVsMuons(2,0,2:2,0,2)",
                                   "nJets>>nJets(5,0,5)",
                                   "nBJets>>nBJets(5,0,5)"
                                   ]
                    )

cfgTools.addCategory(process.diphotonDumper,
                    "flashggTTHhadronicTag",
                                   variables=["CMS_hgg_mass[320,100,180]:=diPhoton().mass", 
                                  "leadPt                   :=diPhoton().leadingPhoton.pt",
                                  "subleadPt                :=diPhoton().subLeadingPhoton.pt",
                                              "diphoMVA                 :=diPhotonMVA().result",    
                                  "maxEta                   :=max(abs(diPhoton().leadingPhoton.superCluster.eta),abs(diPhoton().leadingPhoton.superCluster.eta))",
                                                "nJets                    :=getJetVector().size()",
                                                "nBJets                   :=getBJetVector().size()",
                                  ],
                       ## histograms to be plotted. 
                       ## the variables need to be defined first
                       histograms=["CMS_hgg_mass>>mass(320,100,180)",
                                   "subleadPt:leadPt>>ptLeadvsSub(180,20,200:180,20,200)",
                                   "diphoMVA>>diphoMVA(50,0,1)",
                                   "maxEta>>maxEta[0.,0.1,0.2,0.3,0.4,0.6,0.8,1.0,1.2,1.4442,1.566,1.7,1.8,2.,2.2,2.3,2.5]",
                                   "nJets>>nJets(5,0,5)",
                                   "nBJets>>nBJets(5,0,5)"
                                   ]
                    )

process.p1 = cms.Path(
    process.diphotonDumper
    )
