#!/bin/bash

run=249951
stream='/ExpressCosmics/Run2015A-Express-v1/FEVT'

bsub -q 8nh -J RCTDQMRun${run} -o run${run}.log <<EOF
cd $(pwd)
source /afs/cern.ch/cms/cmsset_default.sh
cmsenv
cmsRun testRCToffline.py runNumber=${run} dataStream='${stream}' 2>/dev/null
EOF
