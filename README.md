RCT Standalone DQM Package
==========================

This package provides quick turn-around DQM at the expense of not being integrated into CMS Offline DQM Web GUI.
It is mostly for RCT experts to investigate issues in the hardware.

Installation
------------

From new CMSSW_7_4_6:

```bash
git cms-merge-topic --unsafe nsmith-:RCTStandaloneDQMv3
cd DQM
git clone https://github.com/nsmith-/RCTStandaloneDQM.git
cd ..
scram b -j 4
```

[Link to Branch](https://github.com/nsmith-/cmssw/tree/RCTStandaloneDQMv3)

How I got to that branch:

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
git checkout -b RCTStandaloneDQMv3 from-CMSSW_7_4_6
git merge RCTToDigiBackport # should be fast-forward
git merge updatedDQMForFED1350V2
```

__This recipe will make a CMSSW branch unsuitable for a pull request__

All the rebasing was necessary because the unpacker was at 76X and the updated DQM code was at 745, so it was making some nasty merge commits.


Running Test Setup
------------------

From `test`.  The manual run entry will change _soon_!

Put some runs in `newRuns.txt` however you like. For example:
```
251250
251251
251252
```

Then do (from an lxplus machine):
```bash
./batchSubmit.sh
# watch bjobs till done
bjobs
./batchOutputToWeb.sh
```
You will need a web directory of some sort, change `batchOutputToWeb.sh` to have the web directory you want.

Running a single job
--------------------
You can skip batchSubmit and go straight to cmsRun, and optionally specify a list of lumis, e.g.
```bash
cmsRun testRCToffline.py runNumber=251718 lumis=170-190
# There is also dataStream=... option, defaults to /ExpressPhysics/Run2015B-Express-v1/FEVT
```
