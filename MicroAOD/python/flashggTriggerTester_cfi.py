import FWCore.ParameterSet.Config as cms

DoubleEGTriggers = cms.vstring( # /cdaq/physics/Run2016/25ns10e33/v1.1.1/HLT/V3 from run 273158, 2016B
    'HLT_Diphoton30EB_18EB_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v1',
    'HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v1',
    'HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelSeedMatch_Mass70_v1',
    'HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass90_v1',
    'HLT_Diphoton30_18_Solid_R9Id_AND_IsoCaloId_AND_HE_R9Id_Mass55_v1',
    'HLT_DoubleEle24_22_eta2p1_WPLoose_Gsf_v2',
    'HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v3',
    'HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v3',
    'HLT_DoubleEle33_CaloIdL_MW_v1',
    'HLT_DoubleEle33_CaloIdL_v1',
    'HLT_DoubleEle37_Ele27_CaloIdL_GsfTrkIdVL_v1',
    'HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT250_v1',
    'HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_v4',
    'HLT_DoublePhoton60_v1',
    'HLT_DoublePhoton85_v2',
    'HLT_ECALHT800_v2',
    'HLT_Ele10_CaloIdM_TrackIdM_CentralPFJet30_BTagCSV_p13_v1',
    'HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v3',
    'HLT_Ele12_CaloIdL_TrackIdL_IsoVL_v3',
    'HLT_Ele12_CaloIdM_TrackIdM_PFJet30_v3',
    'HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL_v3',
    'HLT_Ele17_CaloIdL_GsfTrkIdVL_v1',
    'HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_v1',
    'HLT_Ele17_CaloIdL_TrackIdL_IsoVL_v2',
    'HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v1',
    'HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v3',
    'HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_v3',
    'HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v3',
    'HLT_Ele23_CaloIdL_TrackIdL_IsoVL_v3',
    'HLT_Ele23_CaloIdM_TrackIdM_PFJet30_v3',
    'HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v3',
    'HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v3',
    'HLT_Ele27_HighEta_Ele20_Mass55_v1',
    'HLT_Ele30WP60_Ele8_Mass55_v2',
    'HLT_Ele30WP60_SC4_Mass55_v3',
    'HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v1',
    'HLT_Ele8_CaloIdM_TrackIdM_PFJet30_v3',
    'HLT_Photon26_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon16_AND_HE10_R9Id65_Eta2_Mass60_v2',
    'HLT_Photon36_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon22_AND_HE10_R9Id65_Eta2_Mass15_v2',
    'HLT_Photon42_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon25_AND_HE10_R9Id65_Eta2_Mass15_v2' )

flashggTriggerTester = cms.EDAnalyzer('FlashggTriggerTestAnalyzer',
                                      DiPhotonTag    = cms.InputTag('flashggDiPhotons'),
                                      TriggerResult = cms.InputTag('TriggerResults::HLT'),
                                      TriggerPaths = cms.vstring(DoubleEGTriggers)
                                      )
