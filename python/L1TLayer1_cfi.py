import FWCore.ParameterSet.Config as cms

l1tLayer1 = cms.EDAnalyzer("L1TLayer1",
    ecalTPSourceRecd = cms.InputTag("ecalDigis","EcalTriggerPrimitives"),
    hcalTPSourceRecd = cms.InputTag("hcalDigis"),
    ecalTPSourceSent = cms.InputTag("ecalDigis","EcalTriggerPrimitives"),
    hcalTPSourceSent = cms.InputTag("hcalDigis"),
    histFolder = cms.string('L1TEMU/L1TLayer1'),
)
