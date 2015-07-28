#include "FWCore/PluginManager/interface/ModuleDef.h"

#include "FWCore/Framework/interface/MakerMacros.h"

// TODO: if we move to online DQM, switch
//#include "DQM/L1TMonitor/interface/L1TPUM.h"
#include "DQM/RCTStandaloneDQM/interface/L1TPUM.h"
DEFINE_FWK_MODULE(L1TPUM);

