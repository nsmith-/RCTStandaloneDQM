#!/usr/bin/env python
import DQM.RCTStandaloneDQM.util as util
import json, datetime, os

runCache = {}
# One less than first run of /ExpressPhysics/Run2015B-Express-v1/FEVT
startRun = 250984

if os.path.exists('runCache.json') :
    with open('runCache.json') as cacheFile :
        runCache = json.load(cacheFile)

    startRun = int(max(runCache.keys()))

newRuns = util.getRunsNewer(startRun)
for runDict in newRuns.values() :
    runDict['batchSubmitted'] = True

runCache.update(newRuns)

with open('runCache.json', 'w') as cacheFile :
    json.dump(runCache, cacheFile, indent=4)

with open('newRuns.txt', 'w') as runFile :
    for run in newRuns.keys() :
        runFile.write('%d\n' % run)

# exec batchSubmit.sh
