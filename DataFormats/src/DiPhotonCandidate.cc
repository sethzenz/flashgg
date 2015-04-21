#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/SinglePhotonView.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"
#include "flashgg/MicroAOD/interface/PhotonIdUtils.h"


using namespace flashgg;

DiPhotonCandidate::DiPhotonCandidate() {}

DiPhotonCandidate::~DiPhotonCandidate() {}

DiPhotonCandidate::DiPhotonCandidate( edm::Ptr<flashgg::Photon> photon1, edm::Ptr<flashgg::Photon> photon2, edm::Ptr<reco::Vertex> vertex)
{
    vertex_ = vertex;
	if(photon1->pt() > photon2->pt()){
		Pho1_ = photon1;
		Pho2_ = photon2;
	} else {
		Pho1_ = photon2;
		Pho2_ = photon1;
	}
	math::XYZTLorentzVector pho1corr = DiPhotonCandidate::Pho1P4Corr();
	math::XYZTLorentzVector pho2corr = DiPhotonCandidate::Pho2P4Corr();
	double thisX = pho1corr.px() + pho2corr.Px();
	double thisY = pho1corr.py() + pho2corr.Py();
	double thisZ = pho1corr.pz() + pho2corr.Pz();
	double thisE = pho1corr.energy() + pho2corr.energy();
//	math::XYZTLorentzVector diphocorr;
//	diphocorr.SetPxPyPzE(thisX, thisY, thisZ, thisE);
//  this->setP4(diphocorr);
	this->SetPxPyPzE(thisX, thisY, thisZ, thisE);

    // Adding momenta
    // Needs its own object - but why?
    // Copied from example
//    AddFourMomenta addP4;
//    addP4.set( *this );
	
//	flashgg::PhotonIdUtils phoIDutils;
//	corrPho1_ = flashgg::PhotonIdUtils::pho4MomCorrectionTLVector(photon1, vertex);
//	corrPho1_ = phoIDutils.pho4MomCorrectionTLVector(photon1, vertex);
//	corrPho2_ = phoIDutils.pho4MomCorrectionTLVector(photon2, vertex);
}

DiPhotonCandidate::DiPhotonCandidate( const flashgg::Photon &photon1, const flashgg::Photon &photon2, edm::Ptr<reco::Vertex> vertex )
{
//    addDaughter( photon1 );
//    addDaughter( photon2 );
//    vertex_ = vertex;

    // Adding momenta
    // Needs its own object - but why?
    // Copied from example
//    AddFourMomenta addP4;
//    addP4.set( *this );
}

const edm::Ptr<flashgg::Photon> DiPhotonCandidate::leadingPhotonPtr() const
{
    // if( daughter( 0 )->pt() > daughter( 1 )->pt() ) {
    //     return dynamic_cast<flashgg::Photon &>( *daughter( 0 ) );
    // } else {
    //     return dynamic_cast<flashgg::Photon &>( *daughter( 1 ) );
    // }
	return Pho1_;
}

const edm::Ptr<flashgg::Photon> DiPhotonCandidate::subLeadingPhotonPtr() const
{
    // if( daughter( 0 )->pt() > daughter( 1 )->pt() ) {
    //     return dynamic_cast<flashgg::Photon &>( *daughter( 1 ) );
    // } else {
    //     return dynamic_cast<flashgg::Photon &>( *daughter( 0 ) );
    // }
	return Pho2_;
}

const flashgg::Photon *DiPhotonCandidate::leadingPhoton() const
{
    // if( daughter( 0 )->pt() > daughter( 1 )->pt() ) {
    //     return dynamic_cast<const flashgg::Photon *>( daughter( 0 ) );
    // } else {
    //     return dynamic_cast<const flashgg::Photon *>( daughter( 1 ) );
    // }
	return dynamic_cast<const flashgg::Photon *>( Pho1_.get() );
}

const flashgg::Photon *DiPhotonCandidate::subLeadingPhoton() const
{
    // if( daughter( 0 )->pt() > daughter( 1 )->pt() ) {
    //     return dynamic_cast<const flashgg::Photon *>( daughter( 1 ) );
    // } else {
    //     return dynamic_cast<const flashgg::Photon *>( daughter( 0 ) );
    // }
	return dynamic_cast<const flashgg::Photon *>( Pho2_.get() );
}

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

math::XYZTLorentzVector DiPhotonCandidate::Pho1P4Corr() const
{
    /// vtx should be the chosen vtx, not primary vtx ///
    float vtx_X = vertex_->x();
    float vtx_Y = vertex_->y();
    float vtx_Z = vertex_->z();

    float sc_X = Pho1_->superCluster()->x();
    float sc_Y = Pho1_->superCluster()->y();
    float sc_Z = Pho1_->superCluster()->z();

    math::XYZVector vtx_Pos( vtx_X, vtx_Y, vtx_Z );
    math::XYZVector sc_Pos( sc_X, sc_Y, sc_Z );

    math::XYZVector direction = sc_Pos - vtx_Pos;
    math::XYZVector p = ( direction.Unit() ) * ( Pho1_->energy() );
    math::XYZTLorentzVector corrected_p4( p.x(), p.y(), p.z(), Pho1_->energy() );
	return corrected_p4;
}

math::XYZTLorentzVector DiPhotonCandidate::Pho2P4Corr() const
{
    /// vtx should be the chosen vtx, not primary vtx ///
    float vtx_X = vertex_->x();
    float vtx_Y = vertex_->y();
    float vtx_Z = vertex_->z();

    float sc_X = Pho2_->superCluster()->x();
    float sc_Y = Pho2_->superCluster()->y();
    float sc_Z = Pho2_->superCluster()->z();

    math::XYZVector vtx_Pos( vtx_X, vtx_Y, vtx_Z );
    math::XYZVector sc_Pos( sc_X, sc_Y, sc_Z );

    math::XYZVector direction = sc_Pos - vtx_Pos;
    math::XYZVector p = ( direction.Unit() ) * ( Pho2_->energy() );
    math::XYZTLorentzVector corrected_p4( p.x(), p.y(), p.z(), Pho2_->energy() );
	return corrected_p4;
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

