import FWCore.ParameterSet.Config as cms
import DQM.RCTStandaloneDQM.util as util
from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing()
options.register('runNumber', 0, VarParsing.multiplicity.singleton, VarParsing.varType.int, 'Run to analyze')
options.register('lumis', '1-max', VarParsing.multiplicity.singleton, VarParsing.varType.string, 'Lumis')
options.register('dataStream', '/ExpressPhysics/Run2015B-Express-v1/FEVT', VarParsing.multiplicity.singleton, VarParsing.varType.string, 'Dataset to look for run in')
options.register('inputFiles', [], VarParsing.multiplicity.list, VarParsing.varType.string, 'Manual file list input, will query DAS if empty')
options.parseArguments()

def formatLumis(lumistring, run) :
    lumis = (lrange.split('-') for lrange in lumistring.split(','))
    runlumis = (['%d:%s' % (run,lumi) for lumi in lrange] for lrange in lumis)
    return ['-'.join(l) for l in runlumis]

print 'Getting files for run %d...' % options.runNumber
if len(options.inputFiles) is 0 :
    inputFiles = util.getFilesForRun(options.runNumber, options.dataStream)
else :
    inputFiles = cms.untracked.vstring(options.inputFiles)
if len(inputFiles) is 0 :
    raise Exception('No files found for dataset %s run %d' % (options.dataStream, options.runNumber))
print 'Ok, time to analyze'
process = cms.Process("RCTofflineTEST")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport = cms.untracked.PSet( reportEvery = cms.untracked.int32(1000) )

process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = 'GR_H_V58C'
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource', 'GlobalTag')

# Due to Stage 1 layer 2 not having O2O yet
process.load("L1Trigger.L1TCalorimeter.caloStage1Params_cfi")

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")
process.DQMStore = cms.Service("DQMStore")
process.dqmSaver.workflow = cms.untracked.string('/L1TMonitor/Calo/RCTOffline')

process.load("DQM.RCTStandaloneDQM.L1TRCToffline_cff")

# This must be a bug!
process.simRctUpgradeFormatDigis.regionTag = cms.InputTag("caloStage1Digis")
process.simRctUpgradeFormatDigis.emTag = cms.InputTag("caloStage1Digis")

process.qTester = cms.EDAnalyzer("QualityTester",
    qtList          = cms.untracked.FileInPath('DQM/RCTStandaloneDQM/data/L1TdeRCTQualityTests.xml'),
    prescaleFactor  = cms.untracked.int32     (1),
    testInEventloop = cms.untracked.bool      (True),
    verboseQT       = cms.untracked.bool      (True),
)

process.p = cms.Path(process.rctdqm*process.qTester*process.dqmSaver)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        inputFiles
    ),
    lumisToProcess = cms.untracked.VLuminosityBlockRange(formatLumis(options.lumis, options.runNumber))
)
