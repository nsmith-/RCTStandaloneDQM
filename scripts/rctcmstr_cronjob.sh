#!/bin/bash

cd /afs/cern.ch/work/r/rctcmstr/CMSSW_7_4_9/src/
eval `scramv1 runtime -csh`
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
