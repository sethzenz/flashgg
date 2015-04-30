#ifndef FLASHgg_VBFTruth_h
#define FLASHgg_VBFTruth_h

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/Ptr.h"

namespace flashgg {

    struct VBFTruth {
        edm::Ptr<reco::GenJet> closestGenJetToLeadingJet;
        edm::Ptr<reco::GenJet> closestGenJetToSubLeadingJet;
        edm::Ptr<reco::GenParticle> closestParticleToLeadingJet;
        edm::Ptr<reco::GenParticle> closestParticleToSubLeadingJet;
        edm::Ptr<reco::GenParticle> closestParticleToLeadingPhoton;
        edm::Ptr<reco::GenParticle> closestParticleToSubLeadingPhoton;
        edm::Ptr<reco::GenParticle> leadingQuark;
        edm::Ptr<reco::GenParticle> subLeadingQuark;
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

