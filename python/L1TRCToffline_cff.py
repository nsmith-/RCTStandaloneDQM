import FWCore.ParameterSet.Config as cms

#add'n
from  Configuration.Geometry.GeometryIdeal_cff import *

#unpacking
from Configuration.StandardSequences.RawToDigi_Data_cff import *

#emulator/comparator
from L1Trigger.HardwareValidation.L1HardwareValidation_cff import *
from L1Trigger.Configuration.L1Config_cff import *

# For the GT
from L1TriggerConfig.L1GtConfigProducers.L1GtConfig_cff import *

#dqm
rctEmulDigis = cms.EDProducer("L1RCTProducer",
    useEcal = cms.bool(True),
    useHcal = cms.bool(True),
    ecalDigis = cms.VInputTag(cms.InputTag("ecalDigis:EcalTriggerPrimitives")),
    hcalDigis = cms.VInputTag(cms.InputTag("hcalDigis")),
    queryDelayInLS = cms.uint32(10),
    queryIntervalInLS = cms.uint32(100),
    getFedsFromOmds = cms.bool(False),
    BunchCrossings = cms.vint32(0),
    conditionsLabel = cms.string("")
)

l1tderct = cms.EDAnalyzer("L1TdeRCT",
    rctSourceData = cms.InputTag("l1RctHwDigis"),
    rctSourceEmul = cms.InputTag("rctEmulDigis"),
    gctSourceData = cms.InputTag("gctDigis"),
    ecalTPGData = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
    hcalTPGData = cms.InputTag("hcalDigis"),
    gtDigisLabel = cms.InputTag("gtDigis"),
    gtEGAlgoName = cms.string("L1_SingleEG1"),
    doubleThreshold = cms.int32(3),
    filterTriggerType = cms.int32(1),
    selectBX = cms.untracked.int32(2),
)

l1trct = cms.EDAnalyzer("L1TRCT",
    rctSource = cms.InputTag("l1RctHwDigis"),
    gctSource = cms.InputTag("gctDigis"),
    filterTriggerType = cms.int32(1),
    selectBX = cms.untracked.int32(2),
)

rctdqm = cms.Sequence(
     cms.SequencePlaceholder("RawToDigi")
    *cms.SequencePlaceholder("rctEmulDigis")
    *cms.SequencePlaceholder("l1trct")
    *cms.SequencePlaceholder("l1tderct")
)
