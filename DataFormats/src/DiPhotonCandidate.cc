#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"


using namespace flashgg;

DiPhotonCandidate::DiPhotonCandidate() {}

DiPhotonCandidate::~DiPhotonCandidate() {}

DiPhotonCandidate::DiPhotonCandidate( edm::Ptr<flashgg::Photon> photon1, edm::Ptr<flashgg::Photon> photon2, edm::Ptr<reco::Vertex> vertex)
{
    vertex_ = vertex;
	if(photon1->pt() > photon2->pt()){
		viewPho1_ = new flashgg::SinglePhotonView(photon1, vertex);
		viewPho2_ = new flashgg::SinglePhotonView(photon2, vertex);
	} else {
		viewPho1_ = new flashgg::SinglePhotonView(photon2, vertex);
		viewPho2_ = new flashgg::SinglePhotonView(photon1, vertex);
	}

    std::cout << "SETTING VIEWS: \t VIEW1 pt " << viewPho1_->photonPtr()->pt() << " \t VIEW 2 pt " << viewPho2_->photonPtr()->pt() << std::endl; 

	math::XYZTLorentzVector pho1corr = DiPhotonCandidate::PhoP4Corr(photon1);
	math::XYZTLorentzVector pho2corr = DiPhotonCandidate::PhoP4Corr(photon2);
	double thisX = pho1corr.px() + pho2corr.Px();
	double thisY = pho1corr.py() + pho2corr.Py();
	double thisZ = pho1corr.pz() + pho2corr.Pz();
	double thisE = pho1corr.energy() + pho2corr.energy();
	this->SetPxPyPzE(thisX, thisY, thisZ, thisE);

    // Adding momenta
    // Needs its own object - but why?
    // Copied from example
//    AddFourMomenta addP4;
//    addP4.set( *this );
}


const flashgg::Photon *DiPhotonCandidate::leadingPhoton() const
{
	return viewPho1_->photonPtr();
}

const flashgg::Photon *DiPhotonCandidate::subLeadingPhoton() const
{
	return viewPho2_->photonPtr();
}

flashgg::Photon &DiPhotonCandidate::getLeadingPhoton()
{
	return const_cast<flashgg::Photon &>(*viewPho1_->photonPtr());
//	return remove_const<flashgg::Photon>(viewPho1_->photon());
}

flashgg::Photon &DiPhotonCandidate::getSubLeadingPhoton()
{
	return const_cast<flashgg::Photon &>(*viewPho2_->photonPtr());
//	return const_cast<flashgg::Photon>(viewPho2_->photon());
}


math::XYZTLorentzVector DiPhotonCandidate::PhoP4Corr(edm::Ptr<flashgg::Photon> photon1) const
{
    /// vtx should be the chosen vtx, not primary vtx ///
    float vtx_X = vertex_->x();
    float vtx_Y = vertex_->y();
    float vtx_Z = vertex_->z();

    float sc_X = photon1->superCluster()->x();
    float sc_Y = photon1->superCluster()->y();
    float sc_Z = photon1->superCluster()->z();

    math::XYZVector vtx_Pos( vtx_X, vtx_Y, vtx_Z );
    math::XYZVector sc_Pos( sc_X, sc_Y, sc_Z );

    math::XYZVector direction = sc_Pos - vtx_Pos;
    math::XYZVector p = ( direction.Unit() ) * ( photon1->energy() );
    math::XYZTLorentzVector corrected_p4( p.x(), p.y(), p.z(), photon1->energy() );
	return corrected_p4;
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

