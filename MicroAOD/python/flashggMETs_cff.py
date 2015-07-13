import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMETCorrectionsAndUncertainties

def runMETs(process,metTuple=False):
    runMETCorrectionsAndUncertainties(process, metType="PF",
                                      correctionLevel=["T1"],
                                      computeUncertainties=True,
                                      produceIntermediateCorrections=False,
                                      addToPatDefaultSequence=False,
                                      jetCollection="selectedPatJets",
                                      electronCollection="slimmedElectrons",
                                      muonCollection="slimmedMuons",
                                      tauCollection="slimmedTaus",
#                                      photonCollection="slimmedPhotons", # SCZ added
                                      reclusterJets = True,
                                      pfCandCollection = "packedPFCandidates",
                                      onMiniAOD=True,
                                      postfix="",
                                      )
    if metTuple:
        runMETCorrectionsAndUncertainties(process, metType="PF",
                                          correctionLevel=["T1","Txy"],
                                          computeUncertainties=False,
                                          produceIntermediateCorrections=False,
                                          addToPatDefaultSequence=False,
                                          jetCollection="selectedPatJets",
                                          electronCollection="slimmedElectrons",
                                          muonCollection="slimmedMuons",
                                          tauCollection="slimmedTaus",
                                          reclusterJets = True,
                                          pfCandCollection = "packedPFCandidates",
                                          onMiniAOD=True,
                                          postfix="",
                                          )
        process.METReader = cms.EDAnalyzer("METReader",
                                           originalSlimmedMetlabel = cms.InputTag("slimmedMETs","","PAT"),
                                           newCorrectedSlimmedMetLabel = cms.InputTag("slimmedMETs","","FLASHggMicroAOD"),
                                           rootOutputFile = cms.string("met.root"),
                                           T1TxyMETLabel = cms.InputTag("patPFMetT1Txy"),
                                           )

        process.flashggMicroAODSequence += process.METReader
