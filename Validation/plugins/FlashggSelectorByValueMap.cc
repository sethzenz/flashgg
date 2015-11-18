#include "PhysicsTools/TagAndProbe/plugins/SelectorByValueMap.h"
#include "flashgg/DataFormats/interface/Photon.h"

typedef SelectorByValueMap<flashgg::Photon, float> FlashggPhotonSelectorByDoubleValueMap;
DEFINE_FWK_MODULE(FlashggPhotonSelectorByDoubleValueMap);

typedef SelectorByValueMap<flashgg::Photon, bool> FlashggPhotonSelectorByValueMap;
DEFINE_FWK_MODULE(FlashggPhotonSelectorByValueMap);

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
