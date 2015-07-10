RCT Standalone DQM Package
==========================

This package provides quick turn-around DQM at the expense of not being integrated into CMS Offline DQM Web GUI.
It is mostly for RCT experts to investigate issues in the hardware.

Installation
------------

From new CMSSW_7_4_9:

```bash
# This pull request should be merged soon
git cms-merge-topic --unsafe 10381
cd DQM
git clone https://github.com/nsmith-/RCTStandaloneDQM.git
cd ..
scram b -j 4
```

Running Test Setup
------------------
From `test` on an lxplus machine (for `bsub`):
   * Set up the web directory in `batchOutputToWeb.sh`
   * Double-check datasets for run conditions in `getNewRuns.py`
   * Run `getNewRuns.py`
   * Double-check the file produced - `batchSubmit.sh` looks ok
   * Execute `batchSubmit.sh`
   * Wait for jobs to finish (check with `bjobs`)
   * Run `batchOutputToWeb.sh`

This will be set up on `rctcmstr` under `/afs/cern.ch/work/r/rctcmstr/` soon.

Running a single job
--------------------
You can skip batchSubmit and go straight to cmsRun, and optionally specify a list of lumis, e.g.
```bash
cmsRun testRCToffline.py runNumber=251718 lumis=170-190
```
There is also `dataStream=...` option, defaults to `/ExpressPhysics/Run2015C-Express-v1/FEVT`
