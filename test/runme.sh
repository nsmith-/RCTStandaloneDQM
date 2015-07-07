cmsRun testRCToffline.py
mv DQM_V0001_R000249951__L1TMonitor__Calo__RCTOffline.root run249951.root
mkdir run249951
root -b -q -l "newRct.C++(\"249951\", \"L1TdeRCT\")"
root -b -q -l "newRct.C++(\"249951\", \"L1TdeRCT_FromGCT\")"
mkdir -p dqmAnalysis
mv run249951 dqmAnalysis
./updateRctDqmWeb dqmAnalysis ~/www/RCTStandaloneDQM
