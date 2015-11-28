#!/bin/bash

cd /afs/cern.ch/work/r/rctcmstr/CMSSW_7_5_7_patch1/src/
source /afs/cern.ch/cms/cmsset_default.sh
eval `scramv1 runtime -sh`
cd DQM/RCTStandaloneDQM
git checkout master
git reset --hard HEAD
git pull
pushd $CMSSW_BASE/src
scram b
popd
cd test
./getNewRuns.py
./batchSubmit.sh
./batchOutputToWeb.sh
