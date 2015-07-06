import FWCore.ParameterSet.Config as cms
import DQM.RCTStandaloneDQM.util as util

run = 249951
#print 'Getting files for run %d...' % run
#inputFiles = util.getFilesForRun(run, '/ExpressCosmics/Run2015A-Express-v1/FEVT')
#print 'Ok, time to analyze'
inputFiles = ['/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/0085560F-BB1D-E511-BEAA-02163E01370E.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/06030077-C11D-E511-8D45-02163E011A34.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/089F0320-C71D-E511-9DEC-02163E011A34.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/08C84629-C51D-E511-A81C-02163E011A34.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/0C4AE022-C51D-E511-B373-02163E01239B.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/143D5AD8-C71D-E511-9540-02163E011A34.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/200712A3-BF1D-E511-A4C1-02163E012180.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/2872286D-C11D-E511-983C-02163E012B4B.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/565EA64B-C31D-E511-A3BC-02163E012180.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/5C686AFC-BB1D-E511-AF0E-02163E011F80.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/8CEC1035-C41D-E511-B676-02163E01370E.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/A258E87E-C11D-E511-B882-02163E011D0D.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/A64BFED8-C81D-E511-92E0-02163E01239B.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/A6B109D3-BC1D-E511-8F56-02163E012973.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/AA64A24B-C21D-E511-B609-02163E012B4B.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/B606F81A-C71D-E511-B9AB-02163E01370E.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/B68AEDC0-CA1D-E511-9622-02163E011A34.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/C0EE798F-C01D-E511-BA69-02163E011CD9.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/C4576F48-C31D-E511-9190-02163E011CD9.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/CC80CDC7-C81D-E511-B167-02163E012B4B.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/CEFF9BB8-BF1D-E511-BFD4-02163E012973.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/D0DE759A-BE1D-E511-ACB6-02163E012973.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/D20956B6-BD1D-E511-8F00-02163E011F80.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/DCEEE4CC-C91D-E511-ACEA-02163E012973.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/E4A30DA6-BF1D-E511-87B5-02163E012B4B.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/E64B840C-BB1D-E511-9A97-02163E011A34.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/E6900D30-C41D-E511-B3F2-02163E011A34.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/E6CB0127-C61D-E511-A7BD-02163E011F80.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/EA7490B7-BD1D-E511-9981-02163E011CD9.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/EC8174C7-C91D-E511-9EB3-02163E012B4B.root', '/store/express/Run2015A/ExpressCosmics/FEVT/Express-v1/000/249/951/00000/FCD18A49-C21D-E511-84BC-02163E01239B.root']
process = cms.Process("RCTofflineTEST")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport = cms.untracked.PSet( reportEvery = cms.untracked.int32(1000) )

process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = 'GR_H_V58C'
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource', 'GlobalTag')

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")
process.DQMStore = cms.Service("DQMStore")
process.dqmSaver.workflow = cms.untracked.string('/L1TMonitor/Calo/RCTOffline')

process.load("DQM.RCTStandaloneDQM.L1TRCToffline_cff")

process.load( "EventFilter.RctRawToDigi.l1RctHwDigis_cfi" )
process.l1RctHwDigis.inputLabel = cms.InputTag( "rawDataCollector" )
process.l1RctHwDigis.verbose = cms.untracked.bool( True )
process.l1RctHwDigis.rctFedId = cms.untracked.int32( 1350 )

process.qTester = cms.EDAnalyzer("QualityTester",
    qtList          = cms.untracked.FileInPath('DQM/RCTStandaloneDQM/data/L1TdeRCTQualityTests.xml'),
    prescaleFactor  = cms.untracked.int32     (1),
    testInEventloop = cms.untracked.bool      (True),
    verboseQT       = cms.untracked.bool      (True),
)

process.p = cms.Path(process.l1RctHwDigis*process.rctdqm*process.qTester*process.dqmSaver)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(60000)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        inputFiles
    )
)
