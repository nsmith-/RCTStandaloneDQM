#!/bin/bash

if pwd|grep -q '/r/rctcmstr/'; then
  echo "Manual DQM conflicts with automatic DQM! Please run this in your own user area"
  exit
fi

if [ ! -d outputs ]; then
  echo "outputs directory with run root files doesn't exist!"
  exit
fi

webDirectory=~/www/RCTManualDQM
if [ ! -d $webDirectory ]; then
  echo "Web directory ${webDirectory} doesn't exist!"
  exit
fi

for file in outputs/run*.root
do
  [[ $file == 'DQM_V0001_R*__L1TMonitor__Calo__RCTOffline.root' ]] && break
  run=$(basename ${file} |sed 's/run//g' | sed 's/\.root//g')
  mkdir run${run}
  root -b -q -l "newRct.C++(\"${run}\", \"L1TdeRCT\")"
  root -b -q -l "newRct.C++(\"${run}\", \"L1TdeRCT_FromRCT\")"
  root -b -q -l "newRct.C++(\"${run}\", \"L1TdeRCT_FromGCT\")"
  root -b -q -l "newRct.C++(\"${run}\", \"L1TdeRCT_FromLayer1\")"
  ./dumpPUMplots.py ${run}
  ./dumpL1TLayer1plots.py ${run}
  mkdir -p manualDQM
  mv run${run} manualDQM
  rm -rf ${webDirectory}/run${run}
done

./updateRctDqmWeb.sh manualDQM ${webDirectory}
