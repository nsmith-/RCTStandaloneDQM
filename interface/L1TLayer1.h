#ifndef L1TLayer1_H
#define L1TLayer1_H

// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/Core/interface/DQMEDAnalyzer.h"

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

class L1TLayer1 : public DQMEDAnalyzer {
  public:
    L1TLayer1(const edm::ParameterSet& ps);
    virtual ~L1TLayer1();
  
  protected:
    void analyze(const edm::Event& e, const edm::EventSetup& c);
    virtual void bookHistograms(DQMStore::IBooker &ibooker, const edm::Run&, const edm::EventSetup&) override;
    virtual void dqmBeginRun(const edm::Run&, const edm::EventSetup&);
    void beginLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&);
  
  private:
    edm::EDGetTokenT<EcalTrigPrimDigiCollection> ecalTPSourceRecd_;
    std::string ecalTPSourceRecdLabel_;
    edm::EDGetTokenT<HcalTrigPrimDigiCollection> hcalTPSourceRecd_;
    std::string hcalTPSourceRecdLabel_;
    edm::EDGetTokenT<EcalTrigPrimDigiCollection> ecalTPSourceSent_;
    std::string ecalTPSourceSentLabel_;
    edm::EDGetTokenT<HcalTrigPrimDigiCollection> hcalTPSourceSent_;
    std::string hcalTPSourceSentLabel_;

    std::string histFolder_;
    int tpFillThreshold_;

    MonitorElement * ecalTPOccupancyRecd2D_;
    MonitorElement * hcalTPOccupancyRecd2D_;
    MonitorElement * ecalTPOccupancySent2D_;
    MonitorElement * hcalTPOccupancySent2D_;
    MonitorElement * ecalTPCompressedEtRecd_;
    MonitorElement * hcalTPCompressedEtRecd_;
    MonitorElement * hcalTPCompressedEtSent_;
    MonitorElement * ecalTPCompressedEtSent_;
    MonitorElement * ecalTPOccupancyRecd2D_isECALTowerMasked_;
    MonitorElement * ecalTPOccupancyRecd2D_isECALLinkMasked_;
    MonitorElement * ecalTPOccupancyRecd2D_isECALLinkInError_;
    MonitorElement * ecalTPOccupancy2DNoMatch_;
    
};

#endif
