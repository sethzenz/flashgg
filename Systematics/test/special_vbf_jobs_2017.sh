# NB this command is specific to the configuration on lxplus and is not gaurenteed elsewhere
#outdir="/afs/cern.ch/work/s/sethzenz/ws/" # can't set absolute path on lsf because we're expecting to stage
queue="1nd"
useAAA=0
version="1058b"
fggRunJobs.py --load special_vbf_jobs_2017.json -d sig_jobs_$version -x cmsRun workspaceStd_VBFyCut.py maxEvents=-1 -n 500 -q $queue -D -P useAAA=$useAAA doHTXS=True doFiducial=False tthTagsOnly=True
