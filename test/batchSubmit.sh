#!/bin/bash

stream='/ExpressPhysics/Run2015B-Express-v1/FEVT'

for run in 25116{0..8}; do
bsub -q 8nh -J RCTDQMRun${run} -o run${run}.log <<EOF
cd $(pwd)
source /afs/cern.ch/cms/cmsset_default.sh
cmsenv
cmsRun testRCToffline.py runNumber=${run} dataStream='${stream}' 2>/dev/null
EOF
done
