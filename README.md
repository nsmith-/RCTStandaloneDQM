RCT Standalone DQM Package
==========================

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
git checkout -b RCTStandaloneDQM
git merge RCTToDigiBackport # should be fast-forward
git merge updatedDQMForFED1350V2
```
