/*
 * \file L1TLayer1.cc
 *
 * N. Smith <nick.smith@cern.ch>
 */


// TODO: if we move to online DQM, switch
//#include "DQM/L1TMonitor/interface/L1TLayer1.h"
#include "DQM/RCTStandaloneDQM/interface/L1TLayer1.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Provenance/interface/EventAuxiliary.h"

#include "CondFormats/RunInfo/interface/RunInfo.h"
#include "CondFormats/DataRecord/interface/RunSummaryRcd.h"

// Binning specification
namespace {
  const unsigned int TPGETABINS = 64;
  const float TPGETAMIN = -32.;
  const float TPGETAMAX = 32.;

  const unsigned int TPGPHIBINS = 72;
  const float TPGPHIMIN = -0.5;
  const float TPGPHIMAX = 71.5;
};

L1TLayer1::L1TLayer1(const edm::ParameterSet & ps) :
   ecalTPSourceData_(consumes<EcalTrigPrimDigiCollection>(ps.getParameter<edm::InputTag>("ecalTPSourceData"))),
   ecalTPSourceDataLabel_(ps.getParameter<edm::InputTag>("ecalTPSourceData").label()),
   hcalTPSourceData_(consumes<HcalTrigPrimDigiCollection>(ps.getParameter<edm::InputTag>("hcalTPSourceData"))),
   hcalTPSourceDataLabel_(ps.getParameter<edm::InputTag>("hcalTPSourceData").label()),
   ecalTPSourceEmul_(consumes<EcalTrigPrimDigiCollection>(ps.getParameter<edm::InputTag>("ecalTPSourceEmul"))),
   ecalTPSourceEmulLabel_(ps.getParameter<edm::InputTag>("ecalTPSourceEmul").label()),
   hcalTPSourceEmul_(consumes<HcalTrigPrimDigiCollection>(ps.getParameter<edm::InputTag>("hcalTPSourceEmul"))),
   hcalTPSourceEmulLabel_(ps.getParameter<edm::InputTag>("hcalTPSourceEmul").label()),
   histFolder_(ps.getParameter<std::string>("histFolder")),
   tpFillThreshold_(ps.getUntrackedParameter<int>("occupancyFillThreshold", 3))
{
}

L1TLayer1::~L1TLayer1()
{
}

void L1TLayer1::dqmBeginRun(const edm::Run&, const edm::EventSetup&)
{
}

void L1TLayer1::analyze(const edm::Event & event, const edm::EventSetup & es)
{
  edm::Handle<EcalTrigPrimDigiCollection> ecalTPsData;
  event.getByToken(ecalTPSourceData_, ecalTPsData);
  edm::Handle<HcalTrigPrimDigiCollection> hcalTPsData;
  event.getByToken(hcalTPSourceData_, hcalTPsData);

  edm::Handle<EcalTrigPrimDigiCollection> ecalTPsEmul;
  event.getByToken(ecalTPSourceEmul_, ecalTPsEmul);
  edm::Handle<HcalTrigPrimDigiCollection> hcalTPsEmul;
  event.getByToken(hcalTPSourceEmul_, hcalTPsEmul);

  for ( const auto& ecalTp : *ecalTPsData ) {
    if ( ecalTp.compressedEt() > tpFillThreshold_ ) {
      float etaBin = ecalTp.id().ieta() + ((ecalTp.id().ieta() > 0) ? -0.5 : 0.5);
      ecalTPOccupancyData2D_->Fill(etaBin, ecalTp.id().iphi());
    }
  }
  for ( const auto& hcalTp : *hcalTPsData ) {
    if ( hcalTp.SOI_compressedEt() > tpFillThreshold_ ) {
      float etaBin = hcalTp.id().ieta() + ((hcalTp.id().ieta() > 0) ? -0.5 : 0.5);
      hcalTPOccupancyData2D_->Fill(etaBin, hcalTp.id().iphi());
    }
  }
  for ( const auto& ecalTp : *ecalTPsEmul ) {
    if ( ecalTp.compressedEt() > tpFillThreshold_ ) {
      float etaBin = 1.*ecalTp.id().ieta() + ((ecalTp.id().ieta() > 0) ? -0.5 : 0.5);
      ecalTPOccupancyEmul2D_->Fill(etaBin, ecalTp.id().iphi());
    }
  }
  for ( const auto& hcalTp : *hcalTPsEmul ) {
    if ( hcalTp.SOI_compressedEt() > tpFillThreshold_ ) {
      float etaBin = 1.*hcalTp.id().ieta() + ((hcalTp.id().ieta() > 0) ? -0.5 : 0.5);
      hcalTPOccupancyEmul2D_->Fill(etaBin, hcalTp.id().iphi());
    }
  }
}

void L1TLayer1::bookHistograms(DQMStore::IBooker &ibooker, const edm::Run& run , const edm::EventSetup& es) 
{
  auto sourceString = [](std::string label){return " (source: "+label+")";};

  ibooker.setCurrentFolder(histFolder_+"/Occupancies");

  ecalTPOccupancyData2D_ = ibooker.book2D("ecalTPOccupancyData2D", 
      "ECal TP Occupancy from data"+sourceString(ecalTPSourceDataLabel_),
      TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);

  hcalTPOccupancyData2D_ = ibooker.book2D("hcalTPOccupancyData2D", 
      "HCal TP Occupancy from data"+sourceString(hcalTPSourceDataLabel_),
      TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);

  ecalTPOccupancyEmul2D_ = ibooker.book2D("ecalTPOccupancyEmul2D", 
      "ECal TP Occupancy from emulator"+sourceString(ecalTPSourceEmulLabel_),
      TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);

  hcalTPOccupancyEmul2D_ = ibooker.book2D("hcalTPOccupancyEmul2D", 
      "HCal TP Occupancy from emulator"+sourceString(hcalTPSourceEmulLabel_),
      TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);
}

void L1TLayer1::beginLuminosityBlock(const edm::LuminosityBlock& ls,const edm::EventSetup& es)
{
}

