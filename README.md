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

Running a single job
--------------------
You can specify a run, and optionally specify a list of lumis, e.g.
```bash
cmsRun testRCToffline.py runNumber=251718 lumis=170-190
```
There is also `dataStream=...` option, defaults to `/ExpressPhysics/Run2015C-Express-v1/FEVT`
The default number of events processed is 60k, but one can safely Ctrl-C these processes if impatient.

Running Automated Setup
-----------------------
From `test` on an lxplus machine (lxplus needed for `bsub`):
   * Set up the web directory in `batchOutputToWeb.sh`
   * Run `getNewRuns.py`
   * Execute `batchSubmit.sh`
   * Wait for jobs to finish (check with `bjobs`)
   * Run `batchOutputToWeb.sh`

This automated setup is installed on `rctcmstr` under `/afs/cern.ch/work/r/rctcmstr/`.

In the future, it will run a cron job equivalent to the above instructions.

