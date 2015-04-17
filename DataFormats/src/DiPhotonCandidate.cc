#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/SinglePhotonView.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

using namespace flashgg;

DiPhotonCandidate::DiPhotonCandidate() {}

DiPhotonCandidate::~DiPhotonCandidate() {}

DiPhotonCandidate::DiPhotonCandidate( edm::Ptr<flashgg::Photon> photon1, edm::Ptr<flashgg::Photon> photon2, edm::Ptr<reco::Vertex> vertex )
{
//    addDaughter( *photon1 );
//    addDaughter( *photon2 );
//    vertex_ = vertex;
    // Adding momenta
    // Needs its own object - but why?
    // Copied from example
//    AddFourMomenta addP4;
//    addP4.set( *this );
	
	if( photon1->pt() > photon2->pt()) {
		phoView1_ = flashgg::SinglePhotonView(photon1, vertex);
		phoView2_ = flashgg::SinglePhotonView(photon2, vertex);
	} else {
		phoView1_ = flashgg::SinglePhotonView(photon2, vertex);
		phoView2_ = flashgg::SinglePhotonView(photon1, vertex);
	}
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

