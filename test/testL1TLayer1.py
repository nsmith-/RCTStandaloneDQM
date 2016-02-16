#
#  testL1TLayer1.py  configurable test of L1T Layer1 plot making
#
#  Example Usage:
#
#     cmsRun testL1TLayer1.py runNumber=xx dataStream=yy
#


import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
import os
import sys
import commands

import EventFilter.L1TXRawToDigi.util as util

from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing()
options.register('runNumber', 0, VarParsing.multiplicity.singleton, VarParsing.varType.int, 'Run to analyze')
options.register('lumis', '1-max', VarParsing.multiplicity.singleton, VarParsing.varType.string, 'Lumis')
options.register('dataStream', '/ExpressPhysics/Run2015D-Express-v4/FEVT', VarParsing.multiplicity.singleton, VarParsing.varType.string, 'Dataset to look for run in')
options.register('inputFiles', [], VarParsing.multiplicity.list, VarParsing.varType.string, 'Manual file list input, will query DAS if empty')
options.register('inputFileList', '', VarParsing.multiplicity.singleton, VarParsing.varType.string, 'Manual file list input, will query DAS if empty')
options.register('useORCON', False, VarParsing.multiplicity.singleton, VarParsing.varType.bool, 'Use ORCON for conditions.  This is necessary for very recent runs where conditions have not propogated to Frontier')
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

process = cms.Process("L1TCaloLayer1Plots")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(50)
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(False))


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(inputFiles)
)

process.load('Configuration.Geometry.GeometryExtended2015Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2015_cff')

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

process.load('Configuration.StandardSequences.RawToDigi_cff')

process.dumpED = cms.EDAnalyzer("EventContentAnalyzer")
process.dumpES = cms.EDAnalyzer("PrintEventSetupContent")

process.load('EventFilter.L1TXRawToDigi.caloLayer1Stage2Digis_cfi')

process.load('L1Trigger.L1TCaloLayer1.simCaloStage2Layer1Digis_cfi')

process.simCaloStage2Layer1Digis.useLUT = cms.bool(True)
process.simCaloStage2Layer1Digis.verbose = cms.bool(False)
process.simCaloStage2Layer1Digis.ecalToken = cms.InputTag("l1tCaloLayer1Digis")
process.simCaloStage2Layer1Digis.hcalToken = cms.InputTag("l1tCaloLayer1Digis")

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")
process.DQMStore = cms.Service("DQMStore")
process.dqmSaver.workflow = cms.untracked.string('/L1TMonitor/Calo/RCTOffline')

process.load('DQM.RCTStandaloneDQM.L1TLayer1_cfi')

process.L1TSeq = cms.Sequence(process.RawToDigi
                              + process.l1tCaloLayer1Digis
                              + process.simCaloStage2Layer1Digis
                              + process.l1tLayer1
                              + process.dqmSaver
                              )

process.L1TPath = cms.Path(process.L1TSeq)

process.schedule = cms.Schedule(process.L1TPath)

process.L1TSeq.remove(process.csctfDigis)
process.L1TSeq.remove(process.dttfDigis)
process.L1TSeq.remove(process.gctDigis)
process.L1TSeq.remove(process.gtDigis)
process.L1TSeq.remove(process.gtEvmDigis)
process.L1TSeq.remove(process.siPixelDigis)
process.L1TSeq.remove(process.siStripDigis)
# process.L1TSeq.remove(process.caloStage1Digis)
# process.L1TSeq.remove(process.caloStage2Digis)
# process.L1TSeq.remove(process.caloStage1FinalDigis)
# process.L1TSeq.remove(process.caloStage1LegacyFormatDigis)
process.L1TSeq.remove(process.ecalPreshowerDigis)
process.L1TSeq.remove(process.muonCSCDigis)
process.L1TSeq.remove(process.muonDTDigis)
process.L1TSeq.remove(process.muonRPCDigis)
process.L1TSeq.remove(process.castorDigis)
# process.L1TSeq.remove(process.gmtStage2Digis)
# process.L1TSeq.remove(process.gtStage2Digis)
# process.L1TSeq.remove(process.siPixelDigis)
# process.L1TSeq.remove(process.siStripDigis)
# process.L1TSeq.remove(process.castorDigis)
process.L1TSeq.remove(process.scalersRawToDigi)
process.L1TSeq.remove(process.tcdsDigis)

print process.L1TSeq


