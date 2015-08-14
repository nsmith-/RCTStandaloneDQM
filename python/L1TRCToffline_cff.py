import FWCore.ParameterSet.Config as cms

#required for ecal/hcal digis
from  Configuration.Geometry.GeometryIdeal_cff import *

#unpacking
from Configuration.StandardSequences.RawToDigi_Data_cff import *

#emulator/comparator
from L1Trigger.Configuration.L1Config_cff import *

from DQM.L1TMonitor.L1TEmulatorMonitor_cff import *
from DQM.L1TMonitor.L1TMonitor_cff import *

l1tRctfromGCT = l1tRct.clone()
l1tRctfromGCT.rctSource = 'gctDigis'
l1tRctfromGCT.HistFolder = cms.untracked.string('L1T/L1TRCT_FromGCT')

l1TdeRCTfromGCT = l1TdeRCT.clone()
l1TdeRCTfromGCT.rctSourceData = 'gctDigis'
l1TdeRCTfromGCT.HistFolder = cms.untracked.string('L1TEMU/L1TdeRCT_FromGCT')

# Trim some unnecessary steps
RawToDigi = cms.Sequence(rctDigis+(caloStage1Digis*caloStage1LegacyFormatDigis)+gctDigis+gtDigis+ecalDigis+hcalDigis+scalersRawToDigi)
L1HardwareValidation = cms.Sequence(deEcal+deHcal+deRct+deStage1Layer2)

rctdqm = cms.Sequence(
    RawToDigi
    *L1HardwareValidation
    *(l1tRct + l1tRctfromRCT + l1tRctfromGCT + l1tPUM)
    *(l1TdeRCT + l1TdeRCTfromRCT + l1TdeRCTfromGCT)
)
