RCT Standalone DQM Package
==========================

This package provides quick turn-around DQM at the expense of not being integrated into CMS Offline DQM Web GUI.
It is mostly for RCT experts to investigate issues in the hardware.

Installation
------------

From new CMSSW_7_4_6:

_If rebase fails, try `git rebase --abort` and then try again_

```bash
git cms-addpkg EventFilter/RctRawToDigi 
git cms-addpkg DQM/L1TMonitor
git cms-addpkg L1Trigger/RegionalCaloTrigger
git remote add isobelojalvo https://github.com/isobelojalvo/cmssw.git
git fetch isobelojalvo RCTToDigi:RCTToDigiBackport
git rebase --onto from-CMSSW_7_4_6 f9bbfeec1dd49f3a673ab36634f93178084cfc84 RCTToDigiBackport
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

__This recipe will make a CMSSW branch unsuitable for a pull request__

All the rebasing was necessary because the unpacker was at 76X and the updated DQM code was at 745, so it was making some nasty merge commits.


Running Test Setup
------------------

From `test`.  The errors about GCT unpacker are plentiful and to be ignored for now.

```bash
cmsRun testRCToffline.py
mv DQM_V0001_R000249951__L1TMonitor__Calo__RCTOffline.root run249951.root
root -b -q -l "newRct.C++(\"249951\")"
```

The plotting script appears to crash pretty hard, probably a ROOT 6 issue.
