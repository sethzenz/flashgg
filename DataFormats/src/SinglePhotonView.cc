#include "flashgg/DataFormats/interface/SinglePhotonView.h"

namespace flashgg {
	/*
	SinglePhotonView::SinglePhotonView( const DiPhotonCandidate *dipho, int daughter )
	{
		hasPhoton_ = 1;
		hasVtx_ = 1;
		vtxRef_ = dipho->vtx(); //const_cast< edm::Ptr<reco::Vertex> >(dipho->vtx());
		if(daughter == 0){
			phoRef_ = dipho->leadingView()->photonRef();
		} else  if (daughter == 1){
			phoRef_ = dipho->subLeadingView()->photonRef();
		}
	}
	
	SinglePhotonView::SinglePhotonView( dipho_ptr_type dipho, int daughter )
	{
		hasPhoton_ = 1;
		hasVtx_ = 1;
		vtxRef_ = dipho->vtx();
		if(daughter == 0){
			phoRef_ = dipho->leadingView()->photonRef();
		} else  if (daughter == 1){
			phoRef_ = dipho->subLeadingView()->photonRef();
		}
	}
*/
    bool SinglePhotonView::MakePhoton() const
    {
		if ( hasPhoton_ ) return false;
		else if ( hasVtx_ ){
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
			
            pho_ =  flashgg::Photon(*phoRef_);
//            pho_ = new flashgg::Photon(*phoRef_);
//			pho_ = const_cast<flashgg::Photon *>(phoRef_.get());
			pho_.setP4(corrected_p4);
            hasPhoton_ = true;

            /*
            std::cout << " IGNORE " << p.x() << std::endl;
            //		    math::XYZTLorentzVector corrected_p4( p.x(), p.y(), p.z(), phoRef_->energy() ); 
            math::XYZTLorentzVector corrected_p4( 999., 0., 0., 999. ); // artificial for testing obvs

            std::cout << "--- TESTING A PHOTON WITH VECTOR LENGTH " << persistVec_.size() << " ---" << std::endl;
            std::cout << " Before building pho: phoRef_.get() = " << phoRef_.get() << std::endl;
            std::cout << " Before building pho: phoRef_->pt() = " << phoRef_->pt() << std::endl;
            std::cout << " Before building pho: pho_ = " << pho_ << std::endl;
            std::cout << " Before building pho: corrected_p4.pt() = " << corrected_p4.pt() << std::endl;
			pho_ = const_cast<flashgg::Photon *>(phoRef_.get());
        	std::cout << " After building pho: phoRef_.get() = " << phoRef_.get() << std::endl;
	        std::cout << " After building pho: phoRef_->pt() = " << phoRef_->pt() << std::endl;
            std::cout << " After building pho: pho_ = " << pho_ << std::endl;
            std::cout << " After building pho: pho_->pt() = " << pho_->pt() << std::endl;
			pho_->setP4(corrected_p4);
            std::cout << " After correcting p4: phoRef_.get() = " << phoRef_.get() << std::endl;
            std::cout << " After correcting p4: phoRef_->pt() = " << phoRef_->pt() << std::endl;
            std::cout << " After correcting p4: pho_ = " << pho_ << std::endl;
            std::cout << " After correcting p4: pho_->pt() = " << pho_->pt() << std::endl;
            std::cout << "--- DONE TESTING THE PHOTON WITH VECTOR LENGTH " << persistVec_.size() << " ---" << std::endl;
            */
		} else {
            pho_ = flashgg::Photon(*phoRef_);
//			pho_ = const_cast<flashgg::Photon *>(phoRef_.get());
            hasPhoton_ = true;
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

