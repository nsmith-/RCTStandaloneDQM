RCT Standalone DQM Package
==========================

This package provides quick turn-around DQM at the expense of not being integrated into CMS Offline DQM Web GUI.
It is mostly for RCT experts to investigate issues in the hardware.

Installation
------------

### 2016 Run Processing

From new CMSSW_8_0_0_pre6 

Use the recipe from: 
   https://github.com/cms-l1t-offline/cmssw/issues/147
   https://github.com/cms-l1t-offline/cmssw/issues/149

Then 

git cms-addpkg DQM/L1TMonitor
git cms-addpkg DQMOffline/L1Trigger
cd DQM
git clone https://github.com/nsmith-/RCTStandaloneDQM.git
cd ..
cmsenv
scram b -j 6
cd DQM/RCTStandaloneDQM/test/
cmsRun testL1TLayer1.py runNumber=264593 dataStream=/Cosmics/Commissioning2016-v1/RAW

### 2015 HI installation
From new CMSSW_7_5_7_patch1:

```bash
git cms-addpkg DQM/L1TMonitor
git cms-addpkg DQMOffline/L1Trigger
git cms-addpkg EventFilter/L1TRawToDigi
cd DQM
git clone https://github.com/nsmith-/RCTStandaloneDQM.git
cd ..
cd EventFilter
git clone https://github.com/SridharaDasu/L1TCaloLayer1RawToDigi.git
cd ..
scram b -j 4
```

### 2015 PP installation
From new CMSSW_7_4_12_patch4:

```bash
# This pull request should be merged soon
git cms-merge-topic --unsafe 10880
git cms-addpkg DQMOffline/L1Trigger
git cms-addpkg EventFilter/L1TRawToDigi
git apply ~ncsmith/public/patchFineGrain.patch
git commit -am 'MP7 unpacker patch'
cd DQM
git clone https://github.com/nsmith-/RCTStandaloneDQM.git
cd ..
cd EventFilter
git clone https://github.com/SridharaDasu/L1TCaloLayer1RawToDigi.git
cd ..
scram b -j 4
```

### Common installation
If you plan to analyze very recent runs, the conditions might not propogate to Frontier.
In that case, you must install the credentials to access ORCON.  This should only be used
for *critical* monitoring, more info at:
https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideORACLEProductionAndDevelopmentDatabaseSetup

Automated Setup
---------------
A cronjob is set up in `rctcmstr` to check for new runs every hour.
It runs off the `master` branch of this repository.

The outputs can be found at http://cmsdoc.cern.ch/~rctcmstr/RCTStandaloneDQM/

Running a single job
--------------------
From `test`, there is the CMSSW config `testRCToffline.py` and the helper script to make nice plots `manualOutputToWeb.sh`.
For the most part, the config can be left alone, but choose a web directory for the output script if you don't like the default.

In the cmsRun config, you can specify a run, and optionally specify a list of lumis, e.g.
```bash
cmsRun testRCToffline.py runNumber=251718 lumis=170-190
```
There is also `dataStream=...` option, defaults to `/ExpressPhysics/Run2015C-Express-v1/FEVT`
The default number of events processed is 60k, but one can safely Ctrl-C these processes if impatient.

After running some configs, execute `manualOutputToWeb.sh` to print plots to the chosen web directory.

Testing Automated Setup
-----------------------
From `test` on an lxplus machine (lxplus needed for `bsub`):
   * Set up the web directory in `batchOutputToWeb.sh`
   * Run `getNewRuns.py`
   * Execute `batchSubmit.sh`
   * Wait for jobs to finish (check with `bjobs`)
   * Run `batchOutputToWeb.sh`

This automated setup is installed on `rctcmstr` under `/afs/cern.ch/work/r/rctcmstr/`.
*Any modifications to this directory are automatically overwritten*
