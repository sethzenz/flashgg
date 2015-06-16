import FWCore.ParameterSet.Config as cms

scaleBins = cms.VPSet(
          cms.PSet( Range = cms.string("abs(superCluster.eta)>0.000&&abs(superCluster.eta)<1.000&&r9>0.94&&r9<999.00"), Shift = cms.double(0.00000), Uncertainty = cms.double(0.00050) ),
          cms.PSet( Range = cms.string("abs(superCluster.eta)>0.000&&abs(superCluster.eta)<1.000&&r9>-999.00&&r9<0.94"), Shift = cms.double(0.00000), Uncertainty = cms.double(0.00050) ),
          cms.PSet( Range = cms.string("abs(superCluster.eta)>1.000&&abs(superCluster.eta)<1.500&&r9>0.94&&r9<999.00"), Shift = cms.double(0.00000), Uncertainty = cms.double(0.00060) ),
          cms.PSet( Range = cms.string("abs(superCluster.eta)>1.000&&abs(superCluster.eta)<1.500&&r9>-999.00&&r9<0.94"), Shift = cms.double(0.00000), Uncertainty = cms.double(0.00120) ),
          cms.PSet( Range = cms.string("abs(superCluster.eta)>1.500&&abs(superCluster.eta)<2.000&&r9>-999.00&&r9<0.94"), Shift = cms.double(0.00000), Uncertainty = cms.double(0.00200) ),
          cms.PSet( Range = cms.string("abs(superCluster.eta)>1.500&&abs(superCluster.eta)<2.000&&r9>0.94&&r9<999.00"), Shift = cms.double(0.00000), Uncertainty = cms.double(0.00300) ),
          cms.PSet( Range = cms.string("abs(superCluster.eta)>2.000&&abs(superCluster.eta)<3.000&&r9>-999.00&&r9<0.94"), Shift = cms.double(0.00000), Uncertainty = cms.double(0.00200) ),
          cms.PSet( Range = cms.string("abs(superCluster.eta)>2.000&&abs(superCluster.eta)<3.000&&r9>0.94&&r9<999.00"), Shift = cms.double(0.00000), Uncertainty = cms.double(0.00300) )
                      )

scaleBinsNew = cms.PSet( 
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
		SystMethods = cms.VPSet(
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleStringNew"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleHighR9EBNew"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9>0.94&&abs(eta)<1.5"),
                                                  BinList = scaleBinsNew,
                                                  Debug = cms.untracked.bool(True)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleString"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleHighR9EB"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9>0.94&&abs(eta)<1.5"),
                                                  Bins = scaleBins,
                                                  Debug = cms.untracked.bool(True)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleString"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleHighR9EE"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9>0.94&&abs(eta)>1.5"),
                                                  Bins = scaleBins,
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleString"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleLowR9EB"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9<=0.94&&abs(eta)<1.5"),
                                                  Bins = scaleBins,
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleString"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleLowR9EE"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9<0.94&&abs(eta)>1.5"),
                                                  Bins = scaleBins,
                                                  Debug = cms.untracked.bool(False)
                                                  )
                                        )
                                    )
