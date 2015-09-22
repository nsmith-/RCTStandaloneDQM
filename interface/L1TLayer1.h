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
    edm::EDGetTokenT<EcalTrigPrimDigiCollection> ecalTPSourceData_;
    std::string ecalTPSourceDataLabel_;
    edm::EDGetTokenT<HcalTrigPrimDigiCollection> hcalTPSourceData_;
    std::string hcalTPSourceDataLabel_;
    edm::EDGetTokenT<EcalTrigPrimDigiCollection> ecalTPSourceEmul_;
    std::string ecalTPSourceEmulLabel_;
    edm::EDGetTokenT<HcalTrigPrimDigiCollection> hcalTPSourceEmul_;
    std::string hcalTPSourceEmulLabel_;

    std::string histFolder_;
    int tpFillThreshold_;

    MonitorElement * ecalTPOccupancyData2D_;
    MonitorElement * hcalTPOccupancyData2D_;
    MonitorElement * ecalTPOccupancyEmul2D_;
    MonitorElement * hcalTPOccupancyEmul2D_;
};

#endif
