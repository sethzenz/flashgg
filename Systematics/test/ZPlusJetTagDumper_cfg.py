#!/usr/bin/env cmsRun

doSystematics = False
requireTriggerOnData = True
skipShifted = True

import os
import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
from FWCore.ParameterSet.VarParsing import VarParsing
from flashgg.MetaData.samples_utils import SamplesManager
from PhysicsTools.PatAlgos.tools.helpers import massSearchReplaceAnyInputTag,cloneProcessingSnippet
from flashgg.MetaData.JobConfig import customize

process = cms.Process("ZPlusJetTagsDumper")

if doSystematics:
    process.load("Configuration.StandardSequences.GeometryDB_cff")
    process.load("Configuration.StandardSequences.MagneticField_cff")
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
    from Configuration.AlCa.GlobalTag import GlobalTag
    process.GlobalTag.globaltag = '76X_mcRun2_asymptotic_v13'

    
process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )
process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(
#        "file:myMicroAODOutputFile.root"
#"root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIIFall15DR76-1_3_0-25ns_ext1/1_3_1/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15DR76-1_3_0-25ns_ext1-1_3_1-v0-RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160127_112132/0000/myMicroAODOutputFile_6.root"
#"root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIIFall15DR76-1_3_0-25ns/1_3_0/DoubleEG/RunIIFall15DR76-1_3_0-25ns-1_3_0-v0-Run2015D-16Dec2015-v2/160116_222234/0000/myMicroAODOutputFile_1.root"
#"root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIIFall15DR76-1_3_0-25ns_ext1/1_3_1/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15DR76-1_3_0-25ns_ext1-1_3_1-v0-RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160127_112132/0000/myMicroAODOutputFile_101.root"
"file:myMicroAODOutputFile_101.root"

                             ))

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("ZPlusJetTagsDump.root"),
                                   closeFileFast = cms.untracked.bool(True))


from flashgg.Taggers.flashggTagOutputCommands_cff import tagDefaultOutputCommand
import flashgg.Taggers.dumperConfigTools as cfgTools
from  flashgg.Taggers.tagsDumpers_cfi import createTagDumper

if doSystematics:
    from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
    from flashgg.Systematics.flashggJetSystematics_cfi import createJetSystematics
    process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService")
    jetSystematicsInputTags = createJetSystematics(process,UnpackedJetCollectionVInputTag)

process.load("flashgg.Taggers.flashggTagSequence_cfi")
process.load("flashgg.Taggers.flashggTagTester_cfi")

if doSystematics:
    process.flashggTagSequence.remove(process.flashggUnpackedJets)
    for i in range(len(UnpackedJetCollectionVInputTag)):
        massSearchReplaceAnyInputTag(process.flashggTagSequence,UnpackedJetCollectionVInputTag[i],jetSystematicsInputTags[i])
    process.flashggZPlusJetTagMerger = cms.EDProducer("ZPlusJetTagMerger",src=cms.VInputTag("flashggZPlusJetTag"))

process.systematicsTagSequences = cms.Sequence()

from flashgg.MetaData.JobConfig import customize
customize.parse()
print "customize.processId:",customize.processId

# Do systematics on all MC (n.b. different from ordinary workspaces) but not on data
if doSystematics:
    process.load("flashgg.Systematics.flashggDiPhotonSystematics_cfi")
    massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggDiPhotons"),cms.InputTag("flashggDiPhotonSystematics"))
    systlabels = [""]
    jetsystlabels = []
    if customize.processId == "Data":
        systprodlist = [getattr(process,"flashggJetSystematics%i"%i) for i in range(len(UnpackedJetCollectionVInputTag))]
        for systprod in systprodlist:
            # systprod.SystMethods = cms.VPSet() # do no systematics
            # For any MicroAOD up to 1_3_0 the JEC in Data MicroAOD were bugged and this line makes sure they are fixed
            # It should be a noop in cases where they are already correct
            newvpset = cms.VPSet()
            for pset in systprod.SystMethods:
                if pset.Label.value().count("JEC"):
                    pset.NSigmas = cms.vint32() # Do not perform shifts, central value only
                    pset.Debug = False
                    newvpset += [pset]
            systprod.SystMethods = newvpset        
            if os.environ["CMSSW_VERSION"].count("CMSSW_7_4"):
                systprod.DoCentralJEC = True
                systprod.JECLabel = "ak4PFCHSL1FastL2L3Residual"
                process.load("JetMETCorrections/Configuration/JetCorrectionServices_cff")
        newvpset = cms.VPSet()
        for pset in process.flashggDiPhotonSystematics.SystMethods:
            if pset.Label.value().count("Scale"):
                pset.NoCentralShift = cms.bool(False) # Turn on central shift for data (it is off for MC)
                pset.NSigmas = cms.vint32() # Do not perform shift
                newvpset += [pset]
        process.flashggDiPhotonSystematics.SystMethods = newvpset
    else:
        if not skipShifted:
            for direction in ["Up","Down"]:
                jetsystlabels.append("JEC%s01sigma" % direction)
                jetsystlabels.append("JER%s01sigma" % direction)
            systlabels += jetsystlabels
        for pset in process.flashggDiPhotonSystematics.SystMethods:
            pset.NSigmas = cms.vint32() # Central value only
    for systlabel in systlabels:
        if systlabel == "":
            continue
        newseq = cloneProcessingSnippet(process,process.flashggTagSequence,systlabel)
        if systlabel in jetsystlabels:    
            for i in range(len(jetSystematicsInputTags)):
                massSearchReplaceAnyInputTag(newseq,jetSystematicsInputTags[i],cms.InputTag(jetSystematicsInputTags[i].moduleLabel,systlabel))
        for name in newseq.moduleNames():
            module = getattr(process,name)
            if hasattr(module,"SystLabel"):
                module.SystLabel = systlabel
        process.systematicsTagSequences += newseq
        process.flashggZPlusJetTagMerger.src.append(cms.InputTag("flashggZPlusJetTag" + systlabel))
        
        
# set the ZPlusJetTag dumper
process.ZPlusJetTagDumper = createTagDumper("ZPlusJetTag")
process.ZPlusJetTagDumper.dumpTrees     = True
process.ZPlusJetTagDumper.dumpHistos    = True
process.ZPlusJetTagDumper.dumpWorkspace = False
       

if doSystematics:
    process.ZPlusJetTagDumper.src = "flashggZPlusJetTagMerger"
else:
    # use the trigger-diphoton-preselection
    massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggDiPhotons"),cms.InputTag("flashggPreselectedDiPhotons"))
    process.flashggPreselectedDiPhotons.src=cms.InputTag("flashggDiPhotons")

process.flashggPreselectedDiPhotons.variables =  cms.vstring('pfPhoIso03', 
                                                             'trkSumPtHollowConeDR03', 
                                                             'full5x5_sigmaIetaIeta', 
                                                             'full5x5_r9', 
                                                             '1-passElectronVeto')
# get the variable list
import flashgg.Taggers.VBFTagVariables as var
new_variables = [
    "n_jets         := nJets",
    "jetPt          := jetPt"
    ]
matching_photon = [
    "prompt_pho_1   := diPhoton.leadingPhoton.genMatchType()",
    "prompt_pho_2   := diPhoton.subLeadingPhoton.genMatchType()"
    ] 
all_variables = var.dipho_variables + new_variables 
if customize.processId != "Data":
#    all_variables += var.truth_variables + matching_photon
    all_variables += matching_photon

#cats = []
#if doSystematics:
#    for syst in jetsystlabels:
#        systcutstring = "hasSyst(\"%s\") "%syst
#        cats += [
#            #("VBFDiJet_%s"%syst,"leadingJet.pt>0&&%s"%systcutstring,0)]#,
#            ("VBFDiJet_%s"%syst,"%s"%systcutstring,0)]#,
#                 #("excluded_%s"%syst,systcutstring,0)]
#else:
#    cats = [
#        #("VBFDiJet","leadingJet.pt>0",0)#,
#        ("VBFDiJet","1",0)#,
#        #("excluded","1",0)
#        ]
#
cats = [
    #("VBFDiJet","leadingJet.pt>0",0)#,
    ("ZPlusJet","1",0)#,
    #("excluded","1",0)
    ]

cfgTools.addCategories(process.ZPlusJetTagDumper,
                       cats,
                       variables  = all_variables,
                       histograms = []
)

print cats

process.ZPlusJetTagDumper.nameTemplate = "$PROCESS_$SQRTS_$CLASSNAME_$SUBCAT_$LABEL"

customize.setDefault("maxEvents" ,  5000        ) # max-number of events
customize.setDefault("targetLumi",  1.00e+3  ) # define integrated lumi
customize(process)

from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
#process.hltHighLevel = hltHighLevel.clone(HLTPaths = cms.vstring("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass95_v*") )
process.hltHighLevel = hltHighLevel.clone(HLTPaths = cms.vstring("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*") )
process.options      = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.hltRequirement = cms.Sequence()
if customize.processId == "Data" and requireTriggerOnData:
    process.hltRequirement += process.hltHighLevel

if doSystematics:
    process.p1 = cms.Path(
        process.hltRequirement*
        process.flashggPreselectedDiPhotons*
        process.flashggDiPhotonSystematics*
        (process.flashggUnpackedJets*process.jetSystematicsSequence)*
        (process.flashggTagSequence+process.systematicsTagSequences)*
        process.flashggZPlusJetTagMerger*
        process.ZPlusJetTagDumper
        )
else:
    process.p1 = cms.Path(
        process.hltRequirement*
        process.flashggTagSequence*
        # process.flashggTagTester* # Uncommment if you what to test VBFtag
        process.ZPlusJetTagDumper
        )

print process.p1
