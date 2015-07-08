#!/bin/bash

for file in DQM_V0001_R*__L1TMonitor__Calo__RCTOffline.root
do
  run=$(echo ${file}|sed -n 's:.*R0*\([0-9]*\)__.*:\1:p')
  mv $file run${run}.root
  mkdir run${run}
  root -b -q -l "newRct.C++(\"${run}\", \"L1TdeRCT\")"
  root -b -q -l "newRct.C++(\"${run}\", \"L1TdeRCT_FromGCT\")"
  mkdir -p dqmAnalysis
  mv run${run} dqmAnalysis
  ./updateRctDqmWeb.sh dqmAnalysis ~/www/RCTStandaloneDQM
done
