#ifndef FLASHgg_VBFTruth_h
#define FLASHgg_VBFTruth_h

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/Ptr.h"

namespace flashgg {

    struct VBFTruth {
        edm::Ptr<reco::GenJet> closestGenJetToLeadJet;
        edm::Ptr<reco::GenJet> closestGenJetToSubleadJet;
        edm::Ptr<reco::GenParticle> closestParticleToLeadJet; // status 3
        edm::Ptr<reco::GenParticle> closestParticleToSubleadingJet; // status 3
        edm::Ptr<reco::GenParticle> closestParticleToLeadPhoton; // status 3
        edm::Ptr<reco::GenParticle> closestParticleToSubleadingPhoton; // status 3
    };
}

#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

