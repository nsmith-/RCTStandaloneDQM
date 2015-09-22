import FWCore.ParameterSet.Config as cms

l1tLayer1 = cms.EDAnalyzer("L1TLayer1",
    ecalTPSourceData = cms.InputTag("ecalDigis","EcalTriggerPrimitives"),
    hcalTPSourceData = cms.InputTag("hcalDigis"),
    ecalTPSourceEmul = cms.InputTag("valEcalTriggerPrimitiveDigis"),
    hcalTPSourceEmul = cms.InputTag("valHcalTriggerPrimitiveDigis"),
    histFolder = cms.string('L1TEMU/L1TLayer1'),
)
