#!/bin/bash

export SOURCE=${CMSSW_BASE}/flashgg/Taggers/test/VBFProduction
today=`date +%F`
outdir=/vols/cms/yhaddad/vbf-trees-RunII16Fall-80x/prod-zee-withmva-${today}

fggRunJobs.py --load control.json -d ${outdir} \
	      -x cmsRun standard_vbf_dumper_cfg.py maxEvents=-1 runOnZee=True \
	      -q hepmedium.q --no-use-tarball useAAA=1 targetLumi=1.00e+3 \
	      -n 100 lumiMask=/home/hep/szenz/fromscratch26/CMSSW_8_0_8_patch1/src/flashgg/MetaData/work/jsons/Cert_271036-274421_13TeV_PromptReco_Collisions16_JSON.txt \
	      puTarget=2.18e+03,2.4e+04,7e+04,1.98e+05,3.61e+05,6.19e+05,1.29e+06,9e+06,2.33e+07,3.11e+07,4.09e+07,5.77e+07,8.37e+07,1.16e+08,1.51e+08,1.84e+08,2.06e+08,2.11e+08,2.01e+08,1.82e+08,1.56e+08,1.28e+08,9.8e+07,7.05e+07,4.72e+07,2.94e+07,1.71e+07,9.23e+06,4.68e+06,2.24e+06,1.02e+06,4.36e+05,1.78e+05,6.87e+04,2.51e+04,8.62e+03,2.79e+03,851,244,66.2,17.1,4.25,1.04,0.257,0.0658,0.0178,0.00504,0.00146,0.000424,0.000121


#puTarget=6.87,6.3e+03,2.79e+04,4.2e+04,7.25e+04,1.22e+05,1.46e+05,3.3e+05,8.76e+05,2.47e+06,6.24e+06,1.24e+07,2e+07,2.9e+07,3.88e+07,4.56e+07,4.74e+07,4.23e+07,3.17e+07,2.07e+07,1.3e+07,8.45e+06,5.67e+06,3.6e+06,2.03e+06,9.96e+05,4.24e+05,1.59e+05,5.65e+04,2.31e+04,1.37e+04,1.12e+04,1.01e+04,9.27e+03,8.45e+03,7.71e+03,7.08e+03,6.6e+03,6.25e+03,6.01e+03,5.85e+03,5.73e+03,5.61e+03,5.49e+03,5.34e+03,5.15e+03,4.94e+03,4.69e+03,4.41e+03,4.12e+03

#lumiMask=/home/hep/szenz/fromscratch26/CMSSW_8_0_8_patch1/src/flashgg/MetaData/work/jsons/Cert_271036-274443_13TeV_PromptReco_Collisions16_JSON.txt

#puTarget=2.18e+03,2.4e+04,7e+04,1.98e+05,3.61e+05,6.19e+05,1.29e+06,9e+06,2.33e+07,3.11e+07,4.09e+07,5.77e+07,8.37e+07,1.16e+08,1.51e+08,1.84e+08,2.06e+08,2.11e+08,2.01e+08,1.82e+08,1.56e+08,1.28e+08,9.8e+07,7.05e+07,4.72e+07,2.94e+07,1.71e+07,9.23e+06,4.68e+06,2.24e+06,1.02e+06,4.36e+05,1.78e+05,6.87e+04,2.51e+04,8.62e+03,2.79e+03,851,244,66.2,17.1,4.25,1.04,0.257,0.0658,0.0178,0.00504,0.00146,0.000424,0.000121

#puTarget=6.87,6.3e+03,2.79e+04,4.2e+04,7.25e+04,1.22e+05,1.46e+05,3.3e+05,8.76e+05,2.47e+06,6.24e+06,1.24e+07,2e+07,2.9e+07,3.88e+07,4.56e+07,4.74e+07,4.23e+07,3.17e+07,2.07e+07,1.3e+07,8.45e+06,5.67e+06,3.6e+06,2.03e+06,9.96e+05,4.24e+05,1.59e+05,5.65e+04,2.31e+04,1.37e+04,1.12e+04,1.01e+04,9.27e+03,8.45e+03,7.71e+03,7.08e+03,6.6e+03,6.25e+03,6.01e+03,5.85e+03,5.73e+03,5.61e+03,5.49e+03,5.34e+03,5.15e+03,4.94e+03,4.69e+03,4.41e+03,4.12e+03

#1.34e+05,6.34e+05,8.42e+05,1.23e+06,2.01e+06,4.24e+06,1.26e+07,4.88e+07,1.56e+08,3.07e+08,4.17e+08,4.48e+08,4.04e+08,3.05e+08,1.89e+08,9.64e+07,4.19e+07,1.71e+07,7.85e+06,4.2e+06,2.18e+06,9.43e+05,3.22e+05,8.9e+04,2.16e+04,5.43e+03,1.6e+03,551,206,80.1,31.2,11.9,4.38,1.54,0.518,0.165,0.0501,0.0144,0.00394,0.00102,0.000251,5.87e-05,1.3e-05,2.74e-06,5.47e-07,1.04e-07,1.86e-08,3.18e-09,5.16e-10,9.35e-11,0,0