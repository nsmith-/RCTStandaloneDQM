#!/usr/bin/env python
import DQM.RCTStandaloneDQM.util as util
import json, datetime, os

# Datasets for run types
datasets = {
    'Cosmics' : '/ExpressCosmics/Run2015B-Express-v1/FEVT',
    'Collisions' : '/ExpressPhysics/Run2015B-Express-v1/FEVT',
    'Commissioning' : '/ExpressPhysics/Run2015B-Express-v1/FEVT',
}

# Default start run if no runCache present
# One less than first run of /ExpressPhysics/Run2015B-Express-v1/FEVT
startRun = 250984

runCache = {}
if os.path.exists('runCache.json') :
    with open('runCache.json') as cacheFile :
        runCache = json.load(cacheFile)

    startRun = int(max(runCache.keys()))

newRuns = util.getRunsNewer(startRun)
for runDict in newRuns.values() :
    runDict['batchSubmitted'] = True
    for name, dataset in datasets.iteritems() :
        if name in runDict['runClassName'] :
            runDict['datasetUsed'] = dataset

print 'Found %d new runs!' % len(newRuns)

# Guard against runs being too new for DAS
for runNo in sorted(newRuns.keys(), reverse=True) :
    files = util.getFilesForRun(runNo, newRuns[runNo]['datasetUsed'])
    if len(files) > 0 :
        break
    print 'Removing run %d due to no files present in DAS yet' % runNo
    newRuns.pop(runNo)

runCache.update(newRuns)

with open('runCache.json', 'w') as cacheFile :
    json.dump(runCache, cacheFile, indent=4)

with open('batchSubmit.sh', 'w') as outFile :
    outFile.write('''#!/bin/bash

# Batch file for submission. Will be overwritten by getNewRuns.py!
mkdir -p logs
mkdir -p outputs
''')
    for runDict in newRuns.values() :
        string = '''
bsub -q 8nh -J RCTDQMRun{number} -o logs/run{number}.log <<EOF
cd $(pwd)
source /afs/cern.ch/cms/cmsset_default.sh
cmsenv
cmsRun testRCToffline.py runNumber={number} dataStream='{datasetUsed}' 2>/dev/null | grep -v '^1111$'
mv DQM_V0001_R{number:0>9}__L1TMonitor__Calo__RCTOffline.root outputs/run{number}.root
EOF
'''.format(**runDict)
        outFile.write(string)

os.chmod('batchSubmit.sh', 0744)
# exec batchSubmit.sh
