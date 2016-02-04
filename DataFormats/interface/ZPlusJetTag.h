#ifndef FLASHgg_ZPlusJetTag_h
#define FLASHgg_ZPlusJetTag_h


#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/DataFormats/interface/Jet.h"

namespace flashgg {

    class ZPlusJetTag: public DiPhotonTagBase
    {
    public:
        ZPlusJetTag();
        ~ZPlusJetTag();

        ZPlusJetTag *clone() const { return ( new ZPlusJetTag( *this ) ); }

        ZPlusJetTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult>, edm::Ptr<Jet>, unsigned );

        edm::Ptr<Jet> jet() const { return theJet_; }
        unsigned nJets() const { return njets_; }

    private:
        edm::Ptr<Jet> theJet_;
        unsigned njets_;
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

