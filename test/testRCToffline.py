import FWCore.ParameterSet.Config as cms
import DQM.RCTStandaloneDQM.util as util
from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing()
options.register('runNumber', 0, VarParsing.multiplicity.singleton, VarParsing.varType.int, 'Run to analyze')
options.register('lumis', '1-max', VarParsing.multiplicity.singleton, VarParsing.varType.string, 'Lumis')
options.register('dataStream', '/ExpressPhysics/Run2015D-Express-v3/FEVT', VarParsing.multiplicity.singleton, VarParsing.varType.string, 'Dataset to look for run in')
options.register('inputFiles', [], VarParsing.multiplicity.list, VarParsing.varType.string, 'Manual file list input, will query DAS if empty')
options.register('inputFileList', '', VarParsing.multiplicity.singleton, VarParsing.varType.string, 'Manual file list input, will query DAS if empty')
options.register('useORCON', False, VarParsing.multiplicity.singleton, VarParsing.varType.bool, 'Use ORCON for conditions.  This is necessary for very recent runs where conditions have not propogated to Frontier')
options.register('HIrun', False, VarParsing.multiplicity.singleton, VarParsing.varType.bool, 'True if run is HI, i.e. different FED raw data collection')
options.parseArguments()

def formatLumis(lumistring, run) :
    lumis = (lrange.split('-') for lrange in lumistring.split(','))
    runlumis = (['%d:%s' % (run,lumi) for lumi in lrange] for lrange in lumis)
    return ['-'.join(l) for l in runlumis]

print 'Getting files for run %d...' % options.runNumber
if len(options.inputFiles) is 0 and options.inputFileList is '' :
    inputFiles = util.getFilesForRun(options.runNumber, options.dataStream)
elif len(options.inputFileList) > 0 :
    with open(options.inputFileList) as f :
        inputFiles = list((line.strip() for line in f))
else :
    inputFiles = cms.untracked.vstring(options.inputFiles)
if len(inputFiles) is 0 :
    raise Exception('No files found for dataset %s run %d' % (options.dataStream, options.runNumber))
print 'Ok, time to analyze'
process = cms.Process("RCTofflineTEST")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport = cms.untracked.PSet( reportEvery = cms.untracked.int32(1000) )
process.MessageLogger.cerr.WARNING = cms.untracked.PSet( limit = cms.untracked.int32(100) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = '74X_dataRun2_Express_v1'
if options.useORCON :
    # Channel mask record does not propogate to FrontierProd as fast as we'd like
    # Hopefully this doesn't qualify as abuse
    process.GlobalTag.connect = 'oracle://cms_orcon_adg/CMS_CONDITIONS'
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

if options.HIrun :
    process.castorDigis.InputLabel = cms.InputTag("rawDataRepacker")
    process.csctfDigis.producer = cms.InputTag("rawDataRepacker")
    process.dttfDigis.DTTF_FED_Source = cms.InputTag("rawDataRepacker")
    process.ecalDigis.InputLabel = cms.InputTag("rawDataRepacker")
    process.ecalPreshowerDigis.sourceTag = cms.InputTag("rawDataRepacker")
    process.gctDigis.inputLabel = cms.InputTag("rawDataRepacker")
    process.gtDigis.DaqGtInputTag = cms.InputTag("rawDataRepacker")
    process.gtEvmDigis.EvmGtInputTag = cms.InputTag("rawDataRepacker")
    process.hcalDigis.InputLabel = cms.InputTag("rawDataRepacker")
    process.l1compare.FEDsourceEmul = cms.untracked.InputTag("rawDataRepacker")
    process.l1compare.FEDsourceData = cms.untracked.InputTag("rawDataRepacker")
    process.muonCSCDigis.InputObjects = cms.InputTag("rawDataRepacker")
    process.muonDTDigis.inputLabel = cms.InputTag("rawDataRepacker")
    process.muonRPCDigis.InputLabel = cms.InputTag("rawDataRepacker")
    process.scalersRawToDigi.scalersInputTag = cms.InputTag("rawDataRepacker")
    process.siPixelDigis.InputLabel = cms.InputTag("rawDataRepacker")
    process.siStripDigis.ProductLabel = cms.InputTag("rawDataRepacker")
    process.caloStage1Digis.InputLabel = cms.InputTag("rawDataRepacker")
    process.rctDigis.inputLabel = cms.InputTag("rawDataRepacker")
    process.load("L1Trigger.L1TCalorimeter.caloConfigStage1HI_cfi")
    process.GlobalTag.globaltag = '75X_dataRun2_HLTHI_v4'
    process.l1tCaloLayer1Digis.fedRawDataLabel = cms.InputTag("rawDataRepacker")

