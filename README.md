RCT Standalone DQM Package
==========================

This package provides quick turn-around DQM at the expense of not being integrated into CMS Offline DQM Web GUI.
It is mostly for RCT experts to investigate issues in the hardware.

Installation
------------

From new CMSSW_7_4_6:

```bash
git cms-addpkg EventFilter/RctRawToDigi 
git cms-addpkg DQM/L1TMonitor
git cms-addpkg L1Trigger/RegionalCaloTrigger
git remote add isobelojalvo https://github.com/isobelojalvo/cmssw.git
git fetch isobelojalvo RCTToDigi:RCTToDigiBackport
git rebase --onto from-CMSSW_7_4_6 f9bbfeec1dd49f3a673ab36634f93178084cfc84 RCTToDigiBackport
# if rebase fails, try `git rebase --abort` and then try again
git remote add mcepeda https://github.com/mcepeda/cmssw.git
git fetch mcepeda updatedDQMForFED1350V2:updatedDQMForFED1350V2
git rebase --onto from-CMSSW_7_4_6 CMSSW_7_4_5 updatedDQMForFED1350V2
git checkout -b RCTStandaloneDQM from-CMSSW_7_4_6
git merge RCTToDigiBackport # should be fast-forward
git merge updatedDQMForFED1350V2
cd DQM
git clone https://github.com/nsmith-/RCTStandaloneDQM.git
cd ..
scram b -j 4
```

All the rebasing was necessary because the unpacker was at 76X and the updated DQM code was at 745, so it was making some nasty merge commits.

