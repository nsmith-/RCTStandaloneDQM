#include "FWCore/PluginManager/interface/ModuleDef.h"

#include "FWCore/Framework/interface/MakerMacros.h"

// TODO: if we move to online DQM, switch
//#include "DQM/L1TMonitor/interface/L1TLayer1.h"
#include "DQM/RCTStandaloneDQM/interface/L1TLayer1.h"
DEFINE_FWK_MODULE(L1TLayer1);

