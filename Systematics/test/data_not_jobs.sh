# NB this command is specific to the configuration at IC and is not gaurenteed elsewhere
fggRunJobs.py --load data_76x.json -d /vols/cms04/szenz/data_not_jobs_643 -x cmsRun workspaceStd_trigNot.py maxEvents=-1 -n 1000 -q hepshort.q -D -P useAAA=1 --no-use-tarball 