#!/bin/bash

stream='/ExpressPhysics/Run2015B-Express-v1/FEVT'

if [ ! -f newRuns.txt ]; then
  echo 'No newRuns.txt file present!'
  exit
fi

for run in `cat newRuns.txt`; do
bsub -q 8nh -J RCTDQMRun${run} -o run${run}.log <<EOF
cd $(pwd)
source /afs/cern.ch/cms/cmsset_default.sh
cmsenv
cmsRun testRCToffline.py runNumber=${run} dataStream='${stream}' 2>/dev/null | grep -v '^1111$'
EOF
done

rm newRuns.txt
