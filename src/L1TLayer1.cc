/*
 * \file L1TLayer1.cc
 *
 * N. Smith <nick.smith@cern.ch>
 */
//Modified by Bhawna Gomber <bhawna.gomber@cern.ch>

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

  const unsigned int TPGEtbins = 255;
  const float TPGEtMIN = 0.0;
  const float TPGEtMAX = 255.0;

  const unsigned int TPGEtbins1 = 510;
  const float TPGEtMIN1 = -255.0;
  const float TPGEtMAX1 = 255.0;
};

L1TLayer1::L1TLayer1(const edm::ParameterSet & ps) :
   ecalTPSourceRecd_(consumes<EcalTrigPrimDigiCollection>(ps.getParameter<edm::InputTag>("ecalTPSourceRecd"))),
   ecalTPSourceRecdLabel_(ps.getParameter<edm::InputTag>("ecalTPSourceRecd").label()),
   hcalTPSourceRecd_(consumes<HcalTrigPrimDigiCollection>(ps.getParameter<edm::InputTag>("hcalTPSourceRecd"))),
   hcalTPSourceRecdLabel_(ps.getParameter<edm::InputTag>("hcalTPSourceRecd").label()),
   ecalTPSourceSent_(consumes<EcalTrigPrimDigiCollection>(ps.getParameter<edm::InputTag>("ecalTPSourceSent"))),
   ecalTPSourceSentLabel_(ps.getParameter<edm::InputTag>("ecalTPSourceSent").label()),
   hcalTPSourceSent_(consumes<HcalTrigPrimDigiCollection>(ps.getParameter<edm::InputTag>("hcalTPSourceSent"))),
   hcalTPSourceSentLabel_(ps.getParameter<edm::InputTag>("hcalTPSourceSent").label()),
   histFolder_(ps.getParameter<std::string>("histFolder")),
   tpFillThreshold_(ps.getUntrackedParameter<int>("occupancyFillThreshold", 0))
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
  edm::Handle<EcalTrigPrimDigiCollection> ecalTPsRecd;
  event.getByToken(ecalTPSourceRecd_, ecalTPsRecd);
  edm::Handle<HcalTrigPrimDigiCollection> hcalTPsRecd;
  event.getByToken(hcalTPSourceRecd_, hcalTPsRecd);

  edm::Handle<EcalTrigPrimDigiCollection> ecalTPsSent;
  event.getByToken(ecalTPSourceSent_, ecalTPsSent);
  edm::Handle<HcalTrigPrimDigiCollection> hcalTPsSent;
  event.getByToken(hcalTPSourceSent_, hcalTPsSent);

  for ( const auto& ecalTp : *ecalTPsRecd ) {
    if ((ecalTp.sample(0).raw()) >= 4096) {std::cout<<"raw : "<<ecalTp.sample(0).raw()<<" and its condition check :"<< ((ecalTp.sample(0).raw()>>13) & 0x7)<<std::endl;}
    
    if(((ecalTp.sample(0).raw()>>13) & 0x7)==0)
      {
	ecalTPCompressedEtRecd_->Fill(ecalTp.compressedEt());
      }
    
    if(((ecalTp.sample(0).raw()>>13) & 0x7)==0)
      { 
	for ( const auto& ecalTps : *ecalTPsSent ) {
	  if(ecalTp.id().ieta()==ecalTps.id().ieta() && ecalTp.id().iphi()==ecalTps.id().iphi())
	    {
	      //std::cout<<"ecalTp.id().ieta() "<<ecalTp.id().ieta()<<"ecalTps.id().ieta(): "<<ecalTps.id().ieta()<<std::endl;
	      if ( ecalTp.compressedEt() > tpFillThreshold_ && ecalTps.compressedEt() > tpFillThreshold_)
		{
		  ecalTPCompressedEtdiff_->Fill(ecalTp.compressedEt()-ecalTps.compressedEt());
		  if(ecalTp.compressedEt()!=ecalTps.compressedEt())
		    {
		      float etaBin = ecalTp.id().ieta() + ((ecalTp.id().ieta() > 0) ? -0.5 : 0.5);
		      ecalTPOccupancy2DNoMatch_->Fill(etaBin, ecalTp.id().iphi());
		    }
		  if(ecalTp.compressedEt()==ecalTps.compressedEt())
		    {
		      float etaBin = ecalTp.id().ieta() + ((ecalTp.id().ieta() > 0) ? -0.5 : 0.5);
		      ecalTPOccupancy2DMatch_->Fill(etaBin, ecalTp.id().iphi());
		    }
		}
	      
	    }
	}
      }
  
  
    if(((ecalTp.sample(0).raw()>>13)&1)==1){
      float etaBin = ecalTp.id().ieta() + ((ecalTp.id().ieta() > 0) ? -0.5 : 0.5);
      ecalTPOccupancyRecd2D_isECALTowerMasked_->Fill(etaBin, ecalTp.id().iphi());
    }
    
    if(((ecalTp.sample(0).raw()>>14)&1)==1){
      float etaBin = ecalTp.id().ieta() + ((ecalTp.id().ieta() > 0) ? -0.5 : 0.5);
      ecalTPOccupancyRecd2D_isECALLinkMasked_->Fill(etaBin, ecalTp.id().iphi());
    }

    if(((ecalTp.sample(0).raw()>>15)&1)==1){
      float etaBin = ecalTp.id().ieta() + ((ecalTp.id().ieta() > 0) ? -0.5 : 0.5);
      ecalTPOccupancyRecd2D_isECALLinkInError_->Fill(etaBin, ecalTp.id().iphi());
    }
    if ( ecalTp.compressedEt() > tpFillThreshold_ ) {
      float etaBin = ecalTp.id().ieta() + ((ecalTp.id().ieta() > 0) ? -0.5 : 0.5);
      ecalTPOccupancyRecd2D_->Fill(etaBin, ecalTp.id().iphi());
    }
  }

  
  for ( const auto& hcalTp : *hcalTPsRecd ) {
    hcalTPCompressedEtRecd_->Fill(hcalTp.SOI_compressedEt());
    if ( hcalTp.SOI_compressedEt() > tpFillThreshold_ ) {
      float etaBin = hcalTp.id().ieta() + ((hcalTp.id().ieta() > 0) ? -0.5 : 0.5);
      hcalTPOccupancyRecd2D_->Fill(etaBin, hcalTp.id().iphi());
    }
  }
  for ( const auto& ecalTp : *ecalTPsSent ) {
    ecalTPCompressedEtSent_->Fill(ecalTp.compressedEt());
    if ( ecalTp.compressedEt() > tpFillThreshold_ ) {
      float etaBin = 1.*ecalTp.id().ieta() + ((ecalTp.id().ieta() > 0) ? -0.5 : 0.5);
      ecalTPOccupancySent2D_->Fill(etaBin, ecalTp.id().iphi());
    }
  }
  for ( const auto& hcalTp : *hcalTPsSent ) {
    hcalTPCompressedEtSent_->Fill(hcalTp.SOI_compressedEt());
    if ( hcalTp.SOI_compressedEt() > tpFillThreshold_ ) {
      float etaBin = 1.*hcalTp.id().ieta() + ((hcalTp.id().ieta() > 0) ? -0.5 : 0.5);
      hcalTPOccupancySent2D_->Fill(etaBin, hcalTp.id().iphi());
    }
  }
}

void L1TLayer1::bookHistograms(DQMStore::IBooker &ibooker, const edm::Run& run , const edm::EventSetup& es) 
{
  auto sourceString = [](std::string label){return " (source: "+label+")";};

  ibooker.setCurrentFolder(histFolder_+"/Occupancies");

  ecalTPCompressedEtRecd_ = ibooker.book1D("ecalTPCompressedEtRecd",
				       "ECal Compressed Et received"+sourceString(ecalTPSourceRecdLabel_),
				       TPGEtbins, TPGEtMIN, TPGEtMAX);


  ecalTPCompressedEtdiff_ = ibooker.book1D("ecalTPCompressedEtdiff",
				       "ECal Compressed Et difference (received - sent)"+sourceString(ecalTPSourceRecdLabel_),
				       TPGEtbins1, TPGEtMIN1, TPGEtMAX1);


  hcalTPCompressedEtRecd_ = ibooker.book1D("hcalTPCompressedEtRecd",
					   "HCal Compressed Et received"+sourceString(hcalTPSourceRecdLabel_),
					   TPGEtbins, TPGEtMIN, TPGEtMAX);



  ecalTPCompressedEtSent_ = ibooker.book1D("ecalTPCompressedEtSent",
				       "ECal Compressed Et sent"+sourceString(ecalTPSourceSentLabel_),
				       TPGEtbins, TPGEtMIN, TPGEtMAX);


  hcalTPCompressedEtSent_ = ibooker.book1D("hcalTPCompressedEtSent",
					   "HCal Compressed Et sent"+sourceString(hcalTPSourceSentLabel_),
					   TPGEtbins, TPGEtMIN, TPGEtMAX);
  

  

  
  ecalTPOccupancyRecd2D_isECALTowerMasked_ = ibooker.book2D("ecalTPOccupancyRecd2D_isECALTowerMasked", 
      "ECal TP Occupancy received for the ECAL Masked towers"+sourceString(ecalTPSourceRecdLabel_),
      TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);


  ecalTPOccupancyRecd2D_isECALLinkMasked_ = ibooker.book2D("ecalTPOccupancyRecd2D_isECALLinkMasked", 
      "ECal TP Occupancy received for the ECAL Masked Links"+sourceString(ecalTPSourceRecdLabel_),
      TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);

  ecalTPOccupancyRecd2D_isECALLinkInError_ = ibooker.book2D("ecalTPOccupancyRecd2D_isECALLinkInError", 
      "ECal TP Occupancy received for the ECAL Misaligned, Inerror and Down Links"+sourceString(ecalTPSourceRecdLabel_),
      TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);

  ecalTPOccupancyRecd2D_ = ibooker.book2D("ecalTPOccupancyRecd2D", 
      "ECal TP Occupancy received"+sourceString(ecalTPSourceRecdLabel_),
      TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);

  ecalTPOccupancy2DNoMatch_ = ibooker.book2D("ecalTPOccupancy2DNoMatch", 
      "ECal TP Occupancy when compressed ET don't match between recieved/sent links "+sourceString(ecalTPSourceRecdLabel_),
      TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);

  ecalTPOccupancy2DMatch_ = ibooker.book2D("ecalTPOccupancy2DMatch", 
      "ECal TP Occupancy when compressed ET don't match between recieved/sent links"+sourceString(ecalTPSourceRecdLabel_),
      TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);



  hcalTPOccupancyRecd2D_ = ibooker.book2D("hcalTPOccupancyRecd2D", 
      "HCal TP Occupancy received"+sourceString(hcalTPSourceRecdLabel_),
      TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);

  ecalTPOccupancySent2D_ = ibooker.book2D("ecalTPOccupancySent2D", 
      "ECal TP Occupancy sent"+sourceString(ecalTPSourceSentLabel_),
      TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);

  hcalTPOccupancySent2D_ = ibooker.book2D("hcalTPOccupancySent2D", 
      "HCal TP Occupancy sent"+sourceString(hcalTPSourceSentLabel_),
      TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);
}

void L1TLayer1::beginLuminosityBlock(const edm::LuminosityBlock& ls,const edm::EventSetup& es)
{
}

