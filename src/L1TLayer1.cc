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
    if(((ecalTp.sample(0).raw()>>13) & 0x7)==0) {
      ecalTPCompressedEtRecd_->Fill(ecalTp.compressedEt());
      if ( ecalTp.compressedEt() > tpFillThreshold_ ) {
	float etaBin = ecalTp.id().ieta() + ((ecalTp.id().ieta() > 0) ? -0.5 : 0.5);
	ecalTPOccupancyRecd2D_->Fill(etaBin, ecalTp.id().iphi());
      }
      for ( const auto& ecalTps : *ecalTPsSent ) {
	if(ecalTp.id().ieta()==ecalTps.id().ieta() && ecalTp.id().iphi()==ecalTps.id().iphi())
	  {
	    if ( ecalTp.compressedEt() > tpFillThreshold_ && ecalTps.compressedEt() > tpFillThreshold_)
	      {
		if(ecalTp.compressedEt()!=ecalTps.compressedEt())
		  {
		    ecalTPCompressedEtdiff_->Fill(ecalTp.compressedEt()-ecalTps.compressedEt());
		    ecalTPCompressedEtRecdnomatch_->Fill(ecalTp.compressedEt());
		    ecalTPCompressedEtSentnomatch_->Fill(ecalTps.compressedEt());
		    float etaBin = ecalTp.id().ieta() + ((ecalTp.id().ieta() > 0) ? -0.5 : 0.5);
		    ecalTPOccupancy2DNoMatch_->Fill(etaBin, ecalTp.id().iphi());
		  }
		if(ecalTp.compressedEt()==ecalTps.compressedEt())
		  {
		    ecalTPCompressedEtRecdmatch_->Fill(ecalTp.compressedEt());
		    ecalTPCompressedEtSentmatch_->Fill(ecalTps.compressedEt());
		    float etaBin = ecalTp.id().ieta() + ((ecalTp.id().ieta() > 0) ? -0.5 : 0.5);
		    ecalTPOccupancy2DMatch_->Fill(etaBin, ecalTp.id().iphi());
		  }
	      }
	      
	  }
      }
    }
    
    
    if(ecalTp.fineGrain()==1){
      float etaBin = ecalTp.id().ieta() + ((ecalTp.id().ieta() > 0) ? -0.5 : 0.5);
      ecalTPOccupancyRecd2D_isFineGrainVB_->Fill(etaBin, ecalTp.id().iphi());
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

  for ( const auto& ecalTp : *ecalTPsSent ) {
    ecalTPCompressedEtSent_->Fill(ecalTp.compressedEt());
    if ( ecalTp.compressedEt() > tpFillThreshold_ ) {
      float etaBin = 1.*ecalTp.id().ieta() + ((ecalTp.id().ieta() > 0) ? -0.5 : 0.5);
      ecalTPOccupancySent2D_->Fill(etaBin, ecalTp.id().iphi());
    }
    if(ecalTp.fineGrain()==1){
      float etaBin = ecalTp.id().ieta() + ((ecalTp.id().ieta() > 0) ? -0.5 : 0.5);
      ecalTPOccupancySent2D_isFineGrainVB_->Fill(etaBin, ecalTp.id().iphi());
    }
  }

  for ( const auto& hcalTp : *hcalTPsRecd ) {
    if(((hcalTp.sample(0).raw()>>13) & 0x7)==0) {
      hcalTPCompressedEtRecd_->Fill(hcalTp.SOI_compressedEt());
      if ( hcalTp.SOI_compressedEt() > tpFillThreshold_ ) {
	float etaBin = hcalTp.id().ieta() + ((hcalTp.id().ieta() > 0) ? -0.5 : 0.5);
	hcalTPOccupancyRecd2D_->Fill(etaBin, hcalTp.id().iphi());
      }
      for ( const auto& hcalTps : *hcalTPsSent ) {
	if(hcalTp.id().ieta()==hcalTps.id().ieta() && hcalTp.id().iphi()==hcalTps.id().iphi())
	  {
	    if ( hcalTp.SOI_compressedEt() > tpFillThreshold_ && hcalTps.SOI_compressedEt() > tpFillThreshold_)
	      {
		if(hcalTp.SOI_compressedEt()!=hcalTps.SOI_compressedEt())
		  {
		    hcalTPCompressedEtdiff_->Fill(hcalTp.SOI_compressedEt()-hcalTps.SOI_compressedEt());
		    hcalTPCompressedEtRecdnomatch_->Fill(hcalTp.SOI_compressedEt());
		    hcalTPCompressedEtSentnomatch_->Fill(hcalTps.SOI_compressedEt());
		    float etaBin = hcalTp.id().ieta() + ((hcalTp.id().ieta() > 0) ? -0.5 : 0.5);
		    hcalTPOccupancy2DNoMatch_->Fill(etaBin, hcalTp.id().iphi());
		  }
		if(hcalTp.SOI_compressedEt()==hcalTps.SOI_compressedEt())
		  {
		    hcalTPCompressedEtRecdmatch_->Fill(hcalTp.SOI_compressedEt());
		    hcalTPCompressedEtSentmatch_->Fill(hcalTps.SOI_compressedEt());
		    float etaBin = hcalTp.id().ieta() + ((hcalTp.id().ieta() > 0) ? -0.5 : 0.5);
		    hcalTPOccupancy2DMatch_->Fill(etaBin, hcalTp.id().iphi());
		  }
	      }
	  }
      
      }
    }
    if(hcalTp.SOI_fineGrain()){
      float etaBin = hcalTp.id().ieta() + ((hcalTp.id().ieta() > 0) ? -0.5 : 0.5);
      hcalTPOccupancyRecd2D_isFineGrainVB_->Fill(etaBin, hcalTp.id().iphi());
    }
  
    if(((hcalTp.sample(0).raw()>>13)&1)==1){
      float etaBin = hcalTp.id().ieta() + ((hcalTp.id().ieta() > 0) ? -0.5 : 0.5);
      hcalTPOccupancyRecd2D_isHCALTowerMasked_->Fill(etaBin, hcalTp.id().iphi());
    }
  
    if(((hcalTp.sample(0).raw()>>14)&1)==1){
      float etaBin = hcalTp.id().ieta() + ((hcalTp.id().ieta() > 0) ? -0.5 : 0.5);
      hcalTPOccupancyRecd2D_isHCALLinkMasked_->Fill(etaBin, hcalTp.id().iphi());
    }
  
    if(((hcalTp.sample(0).raw()>>15)&1)==1){
      float etaBin = hcalTp.id().ieta() + ((hcalTp.id().ieta() > 0) ? -0.5 : 0.5);
      hcalTPOccupancyRecd2D_isHCALLinkInError_->Fill(etaBin, hcalTp.id().iphi());
    }
    if ( hcalTp.SOI_compressedEt() > tpFillThreshold_ ) {
      float etaBin = hcalTp.id().ieta() + ((hcalTp.id().ieta() > 0) ? -0.5 : 0.5);
      hcalTPOccupancyRecd2D_->Fill(etaBin, hcalTp.id().iphi());
    }
  }

  for ( const auto& hcalTp : *hcalTPsSent ) {
    hcalTPCompressedEtSent_->Fill(hcalTp.SOI_compressedEt());
    if ( hcalTp.SOI_compressedEt() > tpFillThreshold_ ) {
      float etaBin = 1.*hcalTp.id().ieta() + ((hcalTp.id().ieta() > 0) ? -0.5 : 0.5);
      hcalTPOccupancySent2D_->Fill(etaBin, hcalTp.id().iphi());
    }
    if(hcalTp.SOI_fineGrain()==1){
      float etaBin = hcalTp.id().ieta() + ((hcalTp.id().ieta() > 0) ? -0.5 : 0.5);
      hcalTPOccupancySent2D_isFineGrainVB_->Fill(etaBin, hcalTp.id().iphi());
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


  ecalTPCompressedEtRecdnomatch_ = ibooker.book1D("ecalTPCompressedEtRecd_nomatch",
						  "ECal Compressed Et received for (received - sent !=0)"+sourceString(ecalTPSourceRecdLabel_),
						  TPGEtbins, TPGEtMIN, TPGEtMAX);

  ecalTPCompressedEtSentnomatch_ = ibooker.book1D("ecalTPCompressedEtSent_nomatch",
						  "ECal Compressed Et sent for (received - sent !=0)"+sourceString(ecalTPSourceRecdLabel_),
						  TPGEtbins, TPGEtMIN, TPGEtMAX);



  ecalTPCompressedEtRecdmatch_ = ibooker.book1D("ecalTPCompressedEtRecd_match",
						"ECal Compressed Et received for (received - sent =0)"+sourceString(ecalTPSourceRecdLabel_),
						TPGEtbins, TPGEtMIN, TPGEtMAX);

  ecalTPCompressedEtSentmatch_ = ibooker.book1D("ecalTPCompressedEtSent_match",
						"ECal Compressed Et sent for (received - sent =0)"+sourceString(ecalTPSourceRecdLabel_),
						TPGEtbins, TPGEtMIN, TPGEtMAX);


  ecalTPCompressedEtdiff_ = ibooker.book1D("ecalTPCompressedEtdiff",
					   "ECal Compressed Et difference (received - sent)"+sourceString(ecalTPSourceRecdLabel_),
					   TPGEtbins1, TPGEtMIN1, TPGEtMAX1);


  ecalTPCompressedEtSent_ = ibooker.book1D("ecalTPCompressedEtSent",
					   "ECal Compressed Et sent"+sourceString(ecalTPSourceSentLabel_),
					   TPGEtbins, TPGEtMIN, TPGEtMAX);


  ecalTPOccupancyRecd2D_isECALTowerMasked_ = ibooker.book2D("ecalTPOccupancyRecd2D_isECALTowerMasked", 
							    "ECal TP Occupancy received for the ECAL Masked towers"+sourceString(ecalTPSourceRecdLabel_),
							    TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);


  ecalTPOccupancyRecd2D_isFineGrainVB_ = ibooker.book2D("ecalTPOccupancyRecd2D_isFineGrainVB", 
							"ECal TP Occupancy received for the fine grain veto"+sourceString(ecalTPSourceRecdLabel_),
							TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);

  ecalTPOccupancySent2D_isFineGrainVB_ = ibooker.book2D("ecalTPOccupancySent2D_isFineGrainVB", 
							"ECal TP Occupancy sent for the fine grain veto"+sourceString(ecalTPSourceSentLabel_),
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
					   "ECal TP Occupancy when compressed ET  match between recieved/sent links"+sourceString(ecalTPSourceRecdLabel_),
					   TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);

  ecalTPOccupancySent2D_ = ibooker.book2D("ecalTPOccupancySent2D", 
					  "ECal TP Occupancy sent"+sourceString(ecalTPSourceSentLabel_),
					  TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);


  hcalTPCompressedEtRecd_ = ibooker.book1D("hcalTPCompressedEtRecd",
					   "Hcal Compressed Et received"+sourceString(hcalTPSourceRecdLabel_),
					   TPGEtbins, TPGEtMIN, TPGEtMAX);


  hcalTPCompressedEtRecdnomatch_ = ibooker.book1D("hcalTPCompressedEtRecd_nomatch",
						  "Hcal Compressed Et received for (received - sent !=0)"+sourceString(hcalTPSourceRecdLabel_),
						  TPGEtbins, TPGEtMIN, TPGEtMAX);

  hcalTPCompressedEtSentnomatch_ = ibooker.book1D("hcalTPCompressedEtSent_nomatch",
						  "Hcal Compressed Et sent for (received - sent !=0)"+sourceString(hcalTPSourceRecdLabel_),
						  TPGEtbins, TPGEtMIN, TPGEtMAX);



  hcalTPCompressedEtRecdmatch_ = ibooker.book1D("hcalTPCompressedEtRecd_match",
						"Hcal Compressed Et received for (received - sent =0)"+sourceString(hcalTPSourceRecdLabel_),
						TPGEtbins, TPGEtMIN, TPGEtMAX);

  hcalTPCompressedEtSentmatch_ = ibooker.book1D("hcalTPCompressedEtSent_match",
						"Hcal Compressed Et sent for (received - sent =0)"+sourceString(hcalTPSourceRecdLabel_),
						TPGEtbins, TPGEtMIN, TPGEtMAX);


  hcalTPCompressedEtdiff_ = ibooker.book1D("hcalTPCompressedEtdiff",
					   "Hcal Compressed Et difference (received - sent)"+sourceString(hcalTPSourceRecdLabel_),
					   TPGEtbins1, TPGEtMIN1, TPGEtMAX1);


  hcalTPCompressedEtSent_ = ibooker.book1D("hcalTPCompressedEtSent",
					   "Hcal Compressed Et sent"+sourceString(hcalTPSourceSentLabel_),
					   TPGEtbins, TPGEtMIN, TPGEtMAX);


  hcalTPOccupancyRecd2D_isHCALTowerMasked_ = ibooker.book2D("hcalTPOccupancyRecd2D_isHCALTowerMasked", 
							    "Hcal TP Occupancy received for the HCAL Masked towers"+sourceString(hcalTPSourceRecdLabel_),
							    TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);


  hcalTPOccupancyRecd2D_isFineGrainVB_ = ibooker.book2D("hcalTPOccupancyRecd2D_isFineGrainVB", 
							"Hcal TP Occupancy received for the fine grain veto"+sourceString(hcalTPSourceRecdLabel_),
							TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);

  hcalTPOccupancySent2D_isFineGrainVB_ = ibooker.book2D("hcalTPOccupancySent2D_isFineGrainVB", 
							"Hcal TP Occupancy sent for the fine grain veto"+sourceString(hcalTPSourceSentLabel_),
							TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);


  hcalTPOccupancyRecd2D_isHCALLinkMasked_ = ibooker.book2D("hcalTPOccupancyRecd2D_isHCALLinkMasked", 
							   "Hcal TP Occupancy received for the HCAL Masked Links"+sourceString(hcalTPSourceRecdLabel_),
							   TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);

  hcalTPOccupancyRecd2D_isHCALLinkInError_ = ibooker.book2D("hcalTPOccupancyRecd2D_isHCALLinkInError", 
							    "Hcal TP Occupancy received for the HCAL Misaligned, Inerror and Down Links"+sourceString(hcalTPSourceRecdLabel_),
							    TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);

  hcalTPOccupancyRecd2D_ = ibooker.book2D("hcalTPOccupancyRecd2D", 
					  "Hcal TP Occupancy received"+sourceString(hcalTPSourceRecdLabel_),
					  TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);

  hcalTPOccupancy2DNoMatch_ = ibooker.book2D("hcalTPOccupancy2DNoMatch", 
					     "Hcal TP Occupancy when compressed ET don't match between recieved/sent links "+sourceString(hcalTPSourceRecdLabel_),
					     TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);

  hcalTPOccupancy2DMatch_ = ibooker.book2D("hcalTPOccupancy2DMatch", 
					   "Hcal TP Occupancy when compressed ET  match between recieved/sent links"+sourceString(hcalTPSourceRecdLabel_),
					   TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);

  hcalTPOccupancySent2D_ = ibooker.book2D("hcalTPOccupancySent2D", 
					  "Hcal TP Occupancy sent"+sourceString(hcalTPSourceSentLabel_),
					  TPGETABINS, TPGETAMIN, TPGETAMAX, TPGPHIBINS, TPGPHIMIN, TPGPHIMAX);

}

void L1TLayer1::beginLuminosityBlock(const edm::LuminosityBlock& ls,const edm::EventSetup& es)
{
}

