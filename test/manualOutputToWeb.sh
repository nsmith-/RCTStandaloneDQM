#!/bin/bash

webDirectory=~/www/RCTManualDQM

if pwd|grep -q '/r/rctcmstr/'; then
  echo "Manual DQM conflicts with automatic DQM! Please run this in your own user area"
  exit
fi

mkdir -p outputs

if [ ! -d $webDirectory ]; then
  echo "Web directory ${webDirectory} doesn't exist!"
  exit
fi

for file in DQM_V0001_R*__L1TMonitor__Calo__RCTOffline.root
do
  [[ $file == 'DQM_V0001_R*__L1TMonitor__Calo__RCTOffline.root' ]] && break
  run=$(echo ${file}|sed -n 's:.*R0*\([0-9]*\)__.*:\1:p')
  mv $file outputs/run${run}.root
  mkdir run${run}
  root -b -q -l "newRct.C++(\"${run}\", \"L1TdeRCT\")"
  root -b -q -l "newRct.C++(\"${run}\", \"L1TdeRCT_FromRCT\")"
  root -b -q -l "newRct.C++(\"${run}\", \"L1TdeRCT_FromGCT\")"
  ./dumpPUMplots.py ${run}
  mkdir -p manualDQM
  mv run${run} manualDQM
  rm -rf ${webDirectory}/run${run}
done

./updateRctDqmWeb.sh manualDQM ${webDirectory}
