#ifndef L1TPUM_H
#define L1TPUM_H

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

#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"

class L1TPUM : public DQMEDAnalyzer {
  public:
    L1TPUM(const edm::ParameterSet& ps);
    virtual ~L1TPUM();
  
  protected:
    void analyze(const edm::Event& e, const edm::EventSetup& c);
    virtual void bookHistograms(DQMStore::IBooker &ibooker, const edm::Run&, const edm::EventSetup&) override;
    virtual void dqmBeginRun(const edm::Run&, const edm::EventSetup&);
    void beginLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&);
  
  private:
    edm::EDGetTokenT<L1CaloRegionCollection> regionSource_;
    std::string histFolder_;

    std::vector<MonitorElement*> regionsPUMEta_;
};

#endif
