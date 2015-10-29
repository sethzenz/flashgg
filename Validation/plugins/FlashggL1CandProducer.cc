#include "PhysicsTools/TagAndProbe/plugins/MiniAODL1CandProducer.h"
#include "flashgg/DataFormats/interface/Photon.h"

typedef MiniAODL1CandProducer<flashgg::Photon> FlashggPhotonL1CandProducer;
DEFINE_FWK_MODULE(FlashggPhotonL1CandProducer);

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
