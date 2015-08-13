import FWCore.ParameterSet.Config as cms

#required for ecal/hcal digis
from  Configuration.Geometry.GeometryIdeal_cff import *

#unpacking
from Configuration.StandardSequences.RawToDigi_Data_cff import *

#emulator/comparator
from L1Trigger.Configuration.L1Config_cff import *

from DQM.L1TMonitor.L1TEmulatorMonitor_cff import *
from DQM.L1TMonitor.L1TMonitor_cff import *

# Trim some unnecessary steps
RawToDigi = cms.Sequence(rctDigis+(caloStage1Digis*caloStage1LegacyFormatDigis)+gtDigis+ecalDigis+hcalDigis+scalersRawToDigi)
L1HardwareValidation = cms.Sequence(deEcal+deHcal+deRct+deStage1Layer2)

rctdqm = cms.Sequence(
    RawToDigi
    *L1HardwareValidation
    *(l1tRct + l1tRctfromRCT + l1tPUM)
    *(l1TdeRCT + l1TdeRCTfromRCT)
)
