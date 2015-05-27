import FWCore.ParameterSet.Config as cms

scaleBins = cms.VPSet(cms.PSet( Range = cms.string("pt>10.&&pt<15."), Shift = cms.double(0.003), Uncertainty = cms.double(0.001) ),
                      cms.PSet( Range = cms.string("pt>15.&&pt<20."), Shift = cms.double(-0.001), Uncertainty = cms.double(0.0005) ),
                      cms.PSet( Range = cms.string("pt>20."), Shift = cms.double(0.002), Uncertainty = cms.double(0.0004) )
                      )

flashggSmearPhoton = cms.EDProducer('FlashggDiPhotonSystematicProducer',
		src = cms.untracked.InputTag('flashggDiPhotons'),
		SystMethods = cms.VPSet(
        				# cms.PSet( PhotonMethodName = cms.string("FlashggSimplePhotonSmear"),
                                        #          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                        #          Label = cms.string("exampleSystAlpha"),
                                        #          Sigma = cms.double(3.0),
                                        #          NSigmas = cms.vint32(-1,1)),
                                        # cms.PSet( PhotonMethodName = cms.string("FlashggSimplePhotonSmear"),
                                        #          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                        #          Label = cms.string("exampleSystBravo"),
                                        #          Sigma = cms.double(1.0),
                                        #          NSigmas = cms.vint32(-2,-1,4,7)),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleString"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("exampleSystCharlieBarrel"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("pt > 10.&&abs(eta)<1.479"),
                                                  Bins = scaleBins,
                                                  Debug = cms.untracked.bool(True)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleString"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("exampleSystCharlieEndcap"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("pt > 10.&&abs(eta)>1.653"),
                                                  Bins = scaleBins,
                                                  Debug = cms.untracked.bool(True)
                                                  )
                                        )
                                    )
