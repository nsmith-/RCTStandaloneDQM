/*
 * \file L1TPUM.cc
 *
 * N. Smith <nick.smith@cern.ch>
 */


// TODO: if we move to online DQM, switch
//#include "DQM/L1TMonitor/interface/L1TPUM.h"
#include "DQM/RCTStandaloneDQM/interface/L1TPUM.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Provenance/interface/EventAuxiliary.h"

#include "CondFormats/RunInfo/interface/RunInfo.h"
#include "CondFormats/DataRecord/interface/RunSummaryRcd.h"

// TODO: move to configuration?
namespace {
  const unsigned int R10BINS = 1024;
  const float R10MIN = -0.5;
  const float R10MAX = 1023.5;

  const unsigned int PUMETABINS = 22;
  const unsigned int PUMNORMALIZE = 22;

  const unsigned int PUMBINS = 18;
  const float PUMMIN = -0.5;
  const float PUMMAX = 17.5;
}

L1TPUM::L1TPUM(const edm::ParameterSet & ps) :
   regionSource_(consumes<L1CaloRegionCollection>(ps.getParameter<edm::InputTag>("regionSource"))),
   histFolder_(ps.getParameter<std::string>("histFolder"))
{
}

L1TPUM::~L1TPUM()
{
}

void L1TPUM::dqmBeginRun(const edm::Run&, const edm::EventSetup&)
{
}

void L1TPUM::analyze(const edm::Event & event, const edm::EventSetup & es)
{
  edm::Handle<L1CaloRegionCollection> regionCollection;
  event.getByToken(regionSource_, regionCollection);

  int nonzeroRegionsBx0{0};
  int nonzeroRegionsBxP2{0};
  int nonzeroRegionsBxM2{0};
  for (const auto& region : *regionCollection) {
    if ( region.et() > 0 ) {
      if ( region.bx() == 0 )
        nonzeroRegionsBx0++;
      else if ( region.bx() == 2 )
        nonzeroRegionsBxP2++;
      else if ( region.bx() == -2 )
        nonzeroRegionsBxM2++;
    }
  }

  for (const auto& region : *regionCollection) {
    size_t etaBin = region.gctEta();
    if ( region.bx() == 0 )
      regionsPUMEtaBx0_[etaBin]->Fill(nonzeroRegionsBx0/PUMNORMALIZE, region.et());
    else if ( region.bx() == 2 )
      regionsPUMEtaBxP2_[etaBin]->Fill(nonzeroRegionsBxP2/PUMNORMALIZE, region.et());
    else if ( region.bx() == -2 )
      regionsPUMEtaBxM2_[etaBin]->Fill(nonzeroRegionsBxM2/PUMNORMALIZE, region.et());
  }
}

void L1TPUM::bookHistograms(DQMStore::IBooker &ibooker, const edm::Run& run , const edm::EventSetup& es) 
{
  ibooker.setCurrentFolder(histFolder_+"/BX0");
  regionsPUMEtaBx0_.resize(PUMETABINS);
  for (size_t ieta=0; ieta<PUMETABINS; ++ieta) {
    regionsPUMEtaBx0_[ieta] = ibooker.book2D("regionsPUMEta"+std::to_string(ieta), "PUM Bin rank distribution", PUMBINS, PUMMIN, PUMMAX, R10BINS, R10MIN, R10MAX);
  }

  ibooker.setCurrentFolder(histFolder_+"/BXP2");
  regionsPUMEtaBxP2_.resize(PUMETABINS);
  for (size_t ieta=0; ieta<PUMETABINS; ++ieta) {
    regionsPUMEtaBxP2_[ieta] = ibooker.book2D("regionsPUMEta"+std::to_string(ieta), "PUM Bin rank distribution", PUMBINS, PUMMIN, PUMMAX, R10BINS, R10MIN, R10MAX);
  }

  ibooker.setCurrentFolder(histFolder_+"/BXM2");
  regionsPUMEtaBxM2_.resize(PUMETABINS);
  for (size_t ieta=0; ieta<PUMETABINS; ++ieta) {
    regionsPUMEtaBxM2_[ieta] = ibooker.book2D("regionsPUMEta"+std::to_string(ieta), "PUM Bin rank distribution", PUMBINS, PUMMIN, PUMMAX, R10BINS, R10MIN, R10MAX);
  }
}

void L1TPUM::beginLuminosityBlock(const edm::LuminosityBlock& ls,const edm::EventSetup& es)
{
}

