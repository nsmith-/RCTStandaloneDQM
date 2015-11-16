import FWCore.ParameterSet.Config as cms

#required for ecal/hcal digis
from  Configuration.Geometry.GeometryIdeal_cff import *

#unpacking
from Configuration.StandardSequences.RawToDigi_Data_cff import *

#emulator/comparator
from L1Trigger.Configuration.L1Config_cff import *

from DQM.L1TMonitor.L1TEmulatorMonitor_cff import *
from DQM.L1TMonitor.L1TMonitor_cff import *

# TODO: if we move to online DQM, switch
# from DQM.L1TMonitor.L1TLayer1_cfi import *
from DQM.RCTStandaloneDQM.L1TLayer1_cfi import *

from EventFilter.L1TCaloLayer1RawToDigi.l1tCaloLayer1Digis_cfi import *

RctDigisLayer1=valRctDigis.clone()
RctDigisLayer1.ecalDigis = cms.VInputTag(cms.InputTag('l1tCaloLayer1Digis'))
RctDigisLayer1.hcalDigis = cms.VInputTag(cms.InputTag('l1tCaloLayer1Digis'))

l1tRctfromGCT = l1tRct.clone()
l1tRctfromGCT.rctSource = 'gctDigis'
l1tRctfromGCT.HistFolder = cms.untracked.string('L1T/L1TRCT_FromGCT')

l1TdeRCTfromGCT = l1TdeRCT.clone()
l1TdeRCTfromGCT.rctSourceData = 'gctDigis'
l1TdeRCTfromGCT.HistFolder = cms.untracked.string('L1TEMU/L1TdeRCT_FromGCT')

l1TdeRCTfromLayer1 = l1TdeRCTfromRCT.clone()
l1TdeRCTfromLayer1.ecalTPGData = cms.InputTag('l1tCaloLayer1Digis')
l1TdeRCTfromLayer1.hcalTPGData = cms.InputTag('l1tCaloLayer1Digis')
l1TdeRCTfromLayer1.rctSourceEmul = 'RctDigisLayer1'
l1TdeRCTfromLayer1.HistFolder = cms.untracked.string('L1TEMU/L1TdeRCT_FromLayer1')

# Disable the trigger type filters
l1TdeRCT.filterTriggerType = cms.int32(-1)
l1TdeRCTfromRCT.filterTriggerType = cms.int32(-1)
l1TdeRCTfromGCT.filterTriggerType = cms.int32(-1)
l1TdeRCTfromLayer1 .filterTriggerType = cms.int32(-1)

# Trim some unnecessary steps
RawToDigi = cms.Sequence(rctDigis+(caloStage1Digis*caloStage1LegacyFormatDigis)+gctDigis+gtDigis+ecalDigis+hcalDigis+scalersRawToDigi+l1tCaloLayer1Digis)
L1HardwareValidation = cms.Sequence(deEcal+deHcal+deRct+deStage1Layer2+RctDigisLayer1)

rctdqm = cms.Sequence(
    RawToDigi
    *L1HardwareValidation
    *(l1tRct + l1tRctfromRCT + l1tRctfromGCT + l1tPUM)
    *(l1TdeRCT + l1TdeRCTfromRCT + l1TdeRCTfromGCT + l1TdeRCTfromLayer1 + l1tLayer1)
)
