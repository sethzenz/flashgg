#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"
#include "flashgg/MicroAOD/interface/PhotonIdUtils.h"


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
    // if( daughter( 0 )->pt() > daughter( 1 )->pt() ) {
    //     return dynamic_cast<const flashgg::Photon *>( daughter( 0 ) );
    // } else {
    //     return dynamic_cast<const flashgg::Photon *>( daughter( 1 ) );
    // }
//	return dynamic_cast<const flashgg::Photon *>( Pho1_.get() );
//	return dynamic_cast<const flashgg::Photon *>( viewPho1_->() );
	return viewPho1_->photonPtr();

}

const flashgg::Photon *DiPhotonCandidate::subLeadingPhoton() const
{
    // if( daughter( 0 )->pt() > daughter( 1 )->pt() ) {
    //     return dynamic_cast<const flashgg::Photon *>( daughter( 1 ) );
    // } else {
    //     return dynamic_cast<const flashgg::Photon *>( daughter( 0 ) );
    // }
//	return dynamic_cast<const flashgg::Photon *>( Pho2_.get() );
//	return dynamic_cast<const flashgg::Photon *>( viewPho2_->() );
	return viewPho2_->photonPtr();
}

/*
 flashgg::Photon &DiPhotonCandidate::getLeadingPhoton()
{
    // if( daughter( 0 )->pt() > daughter( 1 )->pt() ) {
    //     return dynamic_cast<flashgg::Photon &>( *daughter( 0 ) );
    // } else {
    //     return dynamic_cast<flashgg::Photon &>( *daughter( 1 ) );
    // }
	const flashgg::Photon &tempPho = dynamic_cast<const flashgg::Photon &>( *Pho1_.get() );
	return const_cast<flashgg::Photon &>( tempPho );
}

 flashgg::Photon &DiPhotonCandidate::getSubLeadingPhoton()
{
    // if( daughter( 0 )->pt() > daughter( 1 )->pt() ) {
    //     return dynamic_cast<flashgg::Photon &>( *daughter( 1 ) );
    // } else {
    //     return dynamic_cast<flashgg::Photon &>( *daughter( 0 ) );
    // }
	
	const flashgg::Photon &tempPho = dynamic_cast<const flashgg::Photon &>( *Pho2_.get() );
	return const_cast<flashgg::Photon &>( tempPho );
}
*/

// SinglePhotonView DiPhotonCandidate::leadingView() const
// {
//     // if( daughter( 0 )->pt() > daughter( 1 )->pt() ) {
//     //     return SinglePhotonView( this, 0 );
//     // } else {
//     //     return SinglePhotonView( this, 1 );
//     // }
// 	return 0;
// }
//
// SinglePhotonView DiPhotonCandidate::subLeadingView() const
// {
//     // if( daughter( 0 )->pt() > daughter( 1 )->pt() ) {
//     //     return SinglePhotonView( this, 1 );
//     // } else {
//     //     return SinglePhotonView( this, 0 );
//     // }
// 	return 0;
// }

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

