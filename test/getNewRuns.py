#!/usr/bin/env python
import DQM.RCTStandaloneDQM.util as util
import json, datetime, os, re

# Default start run if no runCache present
# One less than first run of /ExpressPhysics/Run2015C-Express-v1/FEVT
startRun = 253628-1

runCache = {}
if os.path.exists('runCache.json') :
    with open('runCache.json') as cacheFile :
        runCache = json.load(cacheFile)

    startRun = int(max(runCache.keys()))

newRuns = util.getRunsNewer(startRun)

runsNoDataset = []
for runNo, runDict in newRuns.iteritems() :
    runDict['batchSubmitted'] = True
    availableDatasets = util.runGetDatasetsAvailable(runNo)
    useableDatasets = filter(lambda d : re.match('/Express.*/FEVT', d), availableDatasets)
    if len(useableDatasets) > 0 :
        runDict['datasetUsed'] = useableDatasets[0]
    else :
        print 'Removing run %d due to no Express FEVT datasets available' % runNo
        runsNoDataset.append(runNo)

map(newRuns.pop, runsNoDataset)

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
    json.dump(runCache, cacheFile, indent=4, sort_keys=True)

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
cmsRun testRCToffline.py useORCON=true runNumber={number} dataStream='{datasetUsed}' 2>/dev/null
mv DQM_V0001_R{number:0>9}__L1TMonitor__Calo__RCTOffline.root outputs/run{number}.root
EOF
'''.format(**runDict)
        outFile.write(string)

os.chmod('batchSubmit.sh', 0744)
# exec batchSubmit.sh
