import FWCore.ParameterSet.Config as cms

scaleBins = cms.PSet( 
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet( 
        cms.PSet (lowBounds = cms.vdouble(0,-999.), upBounds =  cms.vdouble(1.0,0.94), values = cms.vdouble( 0. ), uncertainties = cms.vdouble( 0.0005 )),
        cms.PSet (lowBounds = cms.vdouble(0,0.94), upBounds =  cms.vdouble(1.0,999.), values = cms.vdouble( 0. ), uncertainties = cms.vdouble( 0.0005 )),
        cms.PSet (lowBounds = cms.vdouble(1.0,-999.), upBounds =  cms.vdouble(1.5,0.94), values = cms.vdouble( 0. ), uncertainties = cms.vdouble( 0.0012 )),
        cms.PSet (lowBounds = cms.vdouble(1.0,0.94), upBounds =  cms.vdouble(1.5,999.), values = cms.vdouble( 0. ), uncertainties = cms.vdouble( 0.0006 )),
        cms.PSet (lowBounds = cms.vdouble(1.5,-999.), upBounds =  cms.vdouble(2.0,0.94), values = cms.vdouble( 0. ), uncertainties = cms.vdouble( 0.003 )),
        cms.PSet (lowBounds = cms.vdouble(1.5,0.94), upBounds =  cms.vdouble(2.0,999.), values = cms.vdouble( 0. ), uncertainties = cms.vdouble( 0.002 )),
        cms.PSet (lowBounds = cms.vdouble(2.0,-999.), upBounds =  cms.vdouble(3.0,0.94), values = cms.vdouble( 0. ), uncertainties = cms.vdouble( 0.003 )),
        cms.PSet (lowBounds = cms.vdouble(2.0,0.94), upBounds =  cms.vdouble(3.0,999.), values = cms.vdouble( 0. ), uncertainties = cms.vdouble( 0.002 ))
        )
    )


flashggSmearDiPhoton = cms.EDProducer('FlashggDiPhotonSystematicProducer',
		src = cms.InputTag('flashggDiPhotons'),
                SystMethods2D = cms.VPSet(),                      
		SystMethods = cms.VPSet(
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleString"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleHighR9EB"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9>0.94&&abs(eta)<1.5"),
                                                  BinList = scaleBins,
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleString"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleLowR9EB"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9<0.94&&abs(eta)<1.5"),
                                                  BinList = scaleBins,
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleString"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleHighR9EE"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9>0.94&&abs(eta)>=1.5"),
                                                  BinList = scaleBins,
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleString"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleLowR9EE"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9<0.94&&abs(eta)>=1.5"),
                                                  BinList = scaleBins,
                                                  Debug = cms.untracked.bool(False)
                                                  )
                                        )
                                      )

