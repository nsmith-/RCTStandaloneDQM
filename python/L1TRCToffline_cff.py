import FWCore.ParameterSet.Config as cms

#add'n
from  Configuration.Geometry.GeometryIdeal_cff import *

#unpacking
from Configuration.StandardSequences.RawToDigi_Data_cff import *

#emulator/comparator
from L1Trigger.HardwareValidation.L1HardwareValidation_cff import *
from L1Trigger.Configuration.L1Config_cff import *

from DQM.L1TMonitor.L1TRCT_cfi import *
l1tRct.rctSource = 'l1RctHwDigis'
l1tRct.gctSource = 'gctDigis'

l1tRctfromGCT = l1tRct.clone()
l1tRctfromGCT.rctSource = 'gctDigis'
l1tRctfromGCT.HistFolder = cms.untracked.string('L1T/L1TRCT_FromGCT')

from DQM.L1TMonitor.L1TdeRCT_cfi import *
l1TdeRCT.rctSourceData = 'l1RctHwDigis'
l1TdeRCT.gctSourceData = 'gctDigis'
l1TdeRCT.rctSourceEmul = 'valRctDigis'

l1TdeRCTfromGCT = l1TdeRCT.clone()
l1TdeRCTfromGCT.rctSourceData = 'gctDigis'
l1TdeRCTfromGCT.HistFolder = cms.untracked.string('L1TEMU/L1TdeRCT_FromGCT')

rctdqm = cms.Sequence(
    RawToDigi
    *L1HardwareValidation
    *(l1tRct + l1tRctfromGCT)
    *(l1TdeRCT + l1TdeRCTfromGCT)
)
