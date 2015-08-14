#!/bin/bash

webDirectory=~/www/RCTStandaloneDQM

if [ ! -d $webDirectory ]; then
  echo "Web directory ${webDirectory} doesn't exist!"
  exit
fi

mkdir -p processed

for file in outputs/*.root
do
  [[ $file == 'outputs/*.root' ]] && break
  run=$(echo ${file}|sed -n 's:outputs/run\(.*\).root:\1:p')
  mkdir run${run}
  root -b -q -l "newRct.C++(\"${run}\", \"L1TdeRCT\")"
  root -b -q -l "newRct.C++(\"${run}\", \"L1TdeRCT_FromRCT\")"
  ./dumpPUMplots.py ${run}
  mv $file processed/
  mkdir -p dqmAnalysis
  mv run${run} dqmAnalysis
done
./updateRctDqmWeb.sh dqmAnalysis ${webDirectory}
