#include "flashgg/DataFormats/interface/SinglePhotonView.h"

namespace flashgg {

    bool SinglePhotonView::MakePhoton() const
    {
		if ( pho_ ) return false;
		else{
		    float vtx_X = vtxRef_->x();
		    float vtx_Y = vtxRef_->y();
		    float vtx_Z = vtxRef_->z();

		    float sc_X = phoRef_->superCluster()->x();
		    float sc_Y = phoRef_->superCluster()->y();
		    float sc_Z = phoRef_->superCluster()->z();

		    math::XYZVector vtx_Pos( vtx_X, vtx_Y, vtx_Z );
		    math::XYZVector sc_Pos( sc_X, sc_Y, sc_Z );

		    math::XYZVector direction = sc_Pos - vtx_Pos;
		    math::XYZVector p = ( direction.Unit() ) * ( phoRef_->energy() );
		    math::XYZTLorentzVector corrected_p4( p.x(), p.y(), p.z(), phoRef_->energy() );
			
			pho_ = const_cast<flashgg::Photon *>(phoRef_.get());
			pho_->setP4(corrected_p4);
			}
		return true;
    }
}

/*
flashgg::Photon SinglePhotonView::pho4MomCorrection( edm::Ptr<flashgg::Photon> photon, edm::Ptr<reco::Vertex> vtx ) const
{

    /// vtx should be the chosen vtx, not primary vtx ///
    float vtx_X = vtx->x();
    float vtx_Y = vtx->y();
    float vtx_Z = vtx->z();

    float sc_X = photon->superCluster()->x();
    float sc_Y = photon->superCluster()->y();
    float sc_Z = photon->superCluster()->z();

    math::XYZVector vtx_Pos( vtx_X, vtx_Y, vtx_Z );
    math::XYZVector sc_Pos( sc_X, sc_Y, sc_Z );

    math::XYZVector direction = sc_Pos - vtx_Pos;
    math::XYZVector p = ( direction.Unit() ) * ( photon->energy() );
    math::XYZTLorentzVector corrected_p4( p.x(), p.y(), p.z(), photon->energy() );
    flashgg::Photon p4CorrPho = *photon;
    p4CorrPho.setP4( corrected_p4 );
    return p4CorrPho;
}
*/

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

