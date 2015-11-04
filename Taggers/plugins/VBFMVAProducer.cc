#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/VBFMVAResult.h"

#include "TMVA/Reader.h"
#include "TMath.h"
#include "DataFormats/Math/interface/deltaR.h"
#include <string>

using namespace std;
using namespace edm;

namespace flashgg {

    class VBFMVAProducer : public EDProducer
    {

    public:
        VBFMVAProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        
        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        //EDGetTokenT<View<flashgg::Jet> > jetTokenDz_;
        std::vector<edm::InputTag> inputTagJets_;

        unique_ptr<TMVA::Reader>VbfMva_;
        FileInPath vbfMVAweightfile_;
        string     _MVAMethod;
        bool       _usePuJetID;
        bool       _useJetID;
        string     _JetIDLevel;
        bool _requireOppositeEta;
        bool _useHighestDijetMinv;
        double _minDrToPhoton;
        double _minDijetMinv;
        double _minJetPt;
        unsigned int _thirdJetMode;
        bool _verbose;

        
        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;
        
        float dijet_leadEta_   ;
        float dijet_subleadEta_;
        float dijet_abs_dEta_;
        float dijet_LeadJPt_ ;
        float dijet_SubJPt_  ;
        float dijet_Zep_     ;
        float dijet_dphi_trunc_;
        float dijet_dipho_dphi_;
        float dijet_Mjj_   ;
        float dijet_minDRJetPho_ ;
        float dijet_dy_    ;
        float dijet_leady_    ;
        float dijet_subleady_ ;
        float dijet_dipho_pt_ ;
        
        float dipho_PToM_  ;
        float leadPho_PToM_;
        float sublPho_PToM_;

    };
    
    VBFMVAProducer::VBFMVAProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        //jetTokenDz_( consumes<View<flashgg::Jet> >( iConfig.getParameter<InputTag>( "JetTag" ) ) ),
        inputTagJets_ ( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        _MVAMethod    ( iConfig.getUntrackedParameter<string>( "MVAMethod" , "BDT") ),
        _usePuJetID   ( iConfig.getUntrackedParameter<bool>  ( "UsePuJetID"  , false ) ),
        _useJetID     ( iConfig.getUntrackedParameter<bool>  ( "UseJetID"    , false ) ),
        _JetIDLevel   ( iConfig.getUntrackedParameter<string>( "JetIDLevel", "Loose" ) ), // Loose == 0, Tight == 1
        _requireOppositeEta( iConfig.getUntrackedParameter<bool>( "RequireOppositeEta", false ) ),        
        _useHighestDijetMinv( iConfig.getUntrackedParameter<bool>( "UseHighestDijetMinv", false ) ),
        _minDrToPhoton( iConfig.getUntrackedParameter<double>( "MinDrToPhoton", 0.4) ),
        _minDijetMinv( iConfig.getParameter<double>( "MinDijetMinv" ) ),
        _minJetPt( iConfig.getUntrackedParameter<double>( "MinJetPt", 0.) ),
        _thirdJetMode( iConfig.getUntrackedParameter<unsigned int>( "ThirdJetMode", 0) ),
        _verbose( iConfig.getUntrackedParameter<bool>( "Verbose", false) )
    {
        vbfMVAweightfile_ = iConfig.getParameter<edm::FileInPath>( "vbfMVAweightfile" );
        
        dijet_leadEta_    = -999.;
        dijet_subleadEta_ = -999.;
        dijet_abs_dEta_   = -999.;
        dijet_LeadJPt_    = -999.;
        dijet_SubJPt_     = -999.;
        dijet_Zep_        = -999.;
        dijet_dphi_trunc_ = -999.;
        dijet_dipho_dphi_ = -999.;
        dijet_Mjj_        = -999.;
        dijet_dy_         = -999.;
        dipho_PToM_       = -999.;
        leadPho_PToM_     = -999.;
        sublPho_PToM_     = -999.;
        dijet_minDRJetPho_= -999.;
        dijet_dipho_pt_   = -999.;
        dijet_leady_      = -999.;
        dijet_subleady_   = -999.;
        
        if (_MVAMethod != ""){
            VbfMva_.reset( new TMVA::Reader( "!Color:Silent" ) );
            // set of VBF variables
            VbfMva_->AddVariable( "dijet_LeadJPt"     , &dijet_LeadJPt_    );
            VbfMva_->AddVariable( "dijet_SubJPt"      , &dijet_SubJPt_     );
            VbfMva_->AddVariable( "dijet_abs_dEta"    , &dijet_abs_dEta_   );
            //VbfMva_->AddVariable( "dijet_dy"          , &dijet_dy_         );
            VbfMva_->AddVariable( "dijet_Mjj"         , &dijet_Mjj_        );
            VbfMva_->AddVariable( "dijet_Zep"         , &dijet_Zep_        );
            //VbfMva_->AddVariable( "dijet_minDRJetPho" , &dijet_minDRJetPho_);
            //VbfMva_->AddVariable( "dijet_dipho_dphi"  , &dijet_dipho_dphi_ );
            VbfMva_->AddVariable( "dijet_dPhi_trunc"  , &dijet_dipho_dphi_ );
            //VbfMva_->AddVariable( "dipho_PToM"        , &dipho_PToM_       );
            VbfMva_->AddVariable( "leadPho_PToM"      , &leadPho_PToM_);
            VbfMva_->AddVariable( "sublPho_PToM"      , &sublPho_PToM_);
            //new variables
            VbfMva_->BookMVA( _MVAMethod.c_str() , vbfMVAweightfile_.fullPath() );
        }
        produces<vector<VBFMVAResult> >();
        
    }
    
    void VBFMVAProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );
        
        JetCollectionVector Jets( inputTagJets_.size() );
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
            evt.getByLabel( inputTagJets_[j], Jets[j] );
        }
        
        std::auto_ptr<vector<VBFMVAResult> > vbf_results( new vector<VBFMVAResult> );
        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {
            
            flashgg::VBFMVAResult mvares;
            
            dijet_leadEta_    = -999.;
            dijet_subleadEta_ = -999.;
            dijet_abs_dEta_   = -999.;
            dijet_LeadJPt_    = -999.;
            dijet_SubJPt_     = -999.;
            dijet_Zep_        = -999.;
            dijet_dphi_trunc_ = -999.;
            dijet_dipho_dphi_ = -999.;
            dijet_Mjj_        = -999.;
            dijet_dy_         = -999.;
            dijet_minDRJetPho_= -999.;
            dijet_dipho_pt_   = -999.;
            dijet_leady_      = -999.;
            dijet_subleady_   = -999.;
            
            dipho_PToM_       = -999.;
            leadPho_PToM_     = -999.;
            sublPho_PToM_     = -999.;
            
            // First find dijet by looking for highest-pt jets...
            std::pair <int, int>     dijet_indices( -1, -1 );
            std::pair <float, float> dijet_pts( -1., -1. );
            
            float phi1 = diPhotons->ptrAt( candIndex )->leadingPhoton()->phi();
            float eta1 = diPhotons->ptrAt( candIndex )->leadingPhoton()->eta();
            float phi2 = diPhotons->ptrAt( candIndex )->subLeadingPhoton()->phi();
            float eta2 = diPhotons->ptrAt( candIndex )->subLeadingPhoton()->eta();

            bool hasValidVBFDijet  = 0;
            bool hasValidVBFTrijet = 0;
            float mjj_so_far = -999.;
            
            // take the jets corresponding to the diphoton candidate
            unsigned int jetCollectionIndex = diPhotons->ptrAt( candIndex )->jetCollectionIndex();
                        
            for( UInt_t jetLoop = 0; jetLoop < Jets[jetCollectionIndex]->size() ; jetLoop++ ) {
                Ptr<flashgg::Jet> jet  = Jets[jetCollectionIndex]->ptrAt( jetLoop );

                if ( _verbose ) std::cout << "  Jet " << jetLoop << " pt eta _requireOppositeEta _useHighestDijetMinv _thirdJetMode" << jet->pt() << " " << jet->eta() 
                          << " " << _requireOppositeEta << " " << _useHighestDijetMinv << " " << _thirdJetMode << std::endl;

                if( _usePuJetID && !jet->passesPuJetId(diPhotons->ptrAt( candIndex ))){ continue;}
                if( _useJetID ){
                    if( _JetIDLevel == "Loose" && !jet->passesJetID  ( flashgg::Loose ) ) continue;
                    if( _JetIDLevel == "Tight" && !jet->passesJetID  ( flashgg::Tight ) ) continue;
                }
                
                // within eta 4.7?
                if( fabs( jet->eta() ) > 4.7 ) { continue; }

                // close to lead photon?
                float dPhi = deltaPhi( jet->phi(), phi1 );
                float dEta = jet->eta() - eta1;
                if( sqrt( dPhi * dPhi + dEta * dEta ) < _minDrToPhoton ) { if ( _verbose ) std::cout << " Jet " << jetLoop << " near photon" << std::endl; continue; }

                // close to sublead photon?
                dPhi = deltaPhi( jet->phi(), phi2 );
                dEta = jet->eta() - eta2;
                if( sqrt( dPhi * dPhi + dEta * dEta ) < _minDrToPhoton ) { if ( _verbose ) std::cout << " Jet " << jetLoop << " near photon" << std::endl; continue; }

                if ( jet->pt() < _minJetPt ) { continue; }
                
                if (dijet_indices.first == -1) {
                    if ( _verbose ) std::cout << " Lead jet has pt=" << jet->pt() << " eta=" << jet->eta() << " " << jetLoop << std::endl;
                    dijet_indices.first = jetLoop;
                    dijet_pts.first = jet->pt();
                } else {
                    if ( _requireOppositeEta ) {
                        float eta_product = (jet->eta())*(Jets[jetCollectionIndex]->ptrAt( dijet_indices.first )->eta());
                        if ( _verbose ) std::cout << " RequireOppositeEta is on!" << std::endl;
                        if ( eta_product > 0. ) { if ( _verbose ) std::cout << " Skipping this jet!" << std::endl; continue; } else { if ( _verbose ) std::cout << " But this dijet is fine!" << std::endl; }
                    } else {
                        if ( _verbose ) std::cout << " RequireOppositeEta is off!" << std::endl;
                    }
                    if ( _useHighestDijetMinv ) {
                        if ( _verbose ) std::cout << " Use highest DijetMinv is on!" << std::endl;
                        auto lead_p4 = Jets[jetCollectionIndex]->ptrAt( dijet_indices.first )->p4();
                        auto sublead_p4 = jet->p4();
                        double mjj = (lead_p4+sublead_p4).M();
                        if (mjj > mjj_so_far) {
                            if ( _verbose ) std::cout << " Improving old mjj=" << mjj_so_far << " to " << mjj << " with index " << jetLoop << std::endl;
                            dijet_indices.second = jetLoop;
                            dijet_pts.second = jet->pt();
                            mjj_so_far = mjj;
                        }
                    } else {
                        if ( _verbose ) std::cout << " Use highest DijetMinv is off so we take the second jet and call it a day jetLoop=" << jetLoop <<std::endl;
                        dijet_indices.second = jetLoop;
                        dijet_pts.second = jet->pt();
                        break;
                    }
                }
            }

            hasValidVBFDijet = ( dijet_indices.first != -1 && dijet_indices.second != -1 );

            // Extra loop for 3rd jet                                                                                                                                                                                            
            int jet3_index = -1;
            float jet3_mjjj = -999.;
            if (_thirdJetMode == 3) {
                jet3_mjjj = 9999999.; // going for lowest
            }
            float jet3_dr = 999.;
            if ( hasValidVBFDijet ) {
                for( UInt_t jetLoop = 0; jetLoop < Jets[jetCollectionIndex]->size() ; jetLoop++ ) {
                    if ( (int)jetLoop == dijet_indices.first || (int)jetLoop == dijet_indices.second ) continue;
                    Ptr<flashgg::Jet> jet  = Jets[jetCollectionIndex]->ptrAt( jetLoop );

                    if( _usePuJetID && !jet->passesPuJetId( diPhotons->ptrAt( candIndex ) ) ) { continue; }
                    if( fabs( jet->eta() ) > 4.7 ) { continue; }
                    float dPhi = deltaPhi( jet->phi(), phi1 );
                    float dEta = jet->eta() - eta1;
                    if( sqrt( dPhi * dPhi + dEta * dEta ) < _minDrToPhoton ) { continue; }
                    dPhi = deltaPhi( jet->phi(), phi2 );
                    dEta = jet->eta() - eta2;
                    if( sqrt( dPhi * dPhi + dEta * dEta ) < _minDrToPhoton ) { continue; }
                    if ( jet->pt() < _minJetPt ) { continue; }

                    if ( _thirdJetMode == 0 /* lead pt */ ) {
                        if ( _verbose ) std::cout << " Setting 3rd jet to highest pt remaining: " << jetLoop << std::endl;
                        jet3_index = jetLoop;
                        break;
                    } else if ( _thirdJetMode == 1 /* closest to another jet */ ) {
                        auto j1 = Jets[jetCollectionIndex]->ptrAt( dijet_indices.first );
                        auto j2 = Jets[jetCollectionIndex]->ptrAt( dijet_indices.second );
                        dPhi = deltaPhi( jet->phi(), j1->phi() );
                        dEta = jet->eta() - j1->eta();
                        float dr = sqrt( dPhi * dPhi + dEta * dEta );
                        if( dr < jet3_dr ) {
                            jet3_index = jetLoop;
                            jet3_dr = dr;
                            if ( _verbose ) std::cout << " Setting 3rd jet to be the one closest to lead jet (not final): index=" << jetLoop << " dr=" << jet3_dr << " pt=" << jet->pt() << std::endl;
                        }
                        dPhi = deltaPhi( jet->phi(), j2->phi() );
                        dEta = jet->eta() - j2->eta();
                        dr = sqrt( dPhi * dPhi + dEta * dEta );
                        if( dr < jet3_dr ) {
                            jet3_index = jetLoop;
                            jet3_dr = dr;
                            if ( _verbose ) std::cout << " Setting 3rd jet to be the one closest to sublead jet (not final): index=" << jetLoop << " dr=" << jet3_dr << " pt=" << jet->pt() << std::endl;
                        }
                    } else if ( _thirdJetMode == 2 /* highest m_jjj */ ) {
                        auto dijet_p4 = ( Jets[jetCollectionIndex]->ptrAt( dijet_indices.first )->p4() + Jets[jetCollectionIndex]->ptrAt( dijet_indices.second )->p4());
                        float mjjj = (dijet_p4 + jet->p4()).M();
                        if (mjjj > jet3_mjjj) {
                            jet3_index = jetLoop;
                            jet3_mjjj = mjjj;
                            if ( _verbose ) std::cout << " Setting 3rd jet to be the one giving highest mjjj (not final): index=" << jetLoop << " mjjj=" << jet3_mjjj << " pt=" << jet->pt() << std::endl;
                        }
                    } else if ( _thirdJetMode == 3 /* lowest m_jjj */ ) {
                        auto dijet_p4 = ( Jets[jetCollectionIndex]->ptrAt( dijet_indices.first )->p4() + Jets[jetCollectionIndex]->ptrAt( dijet_indices.second )->p4());
                        float mjjj = (dijet_p4 + jet->p4()).M();
                        if (mjjj < jet3_mjjj) {
                            jet3_index = jetLoop;
                            jet3_mjjj = mjjj;
                            if ( _verbose ) std::cout << " Setting 3rd jet to be the one giving lowest mjjj (not final): index=" << jetLoop << " mjjj=" << jet3_mjjj << " pt=" << jet->pt() << std::endl;
                        }
                    }
                }
            }

            if( hasValidVBFDijet          && jet3_index != -1          ) {hasValidVBFTrijet = 1;}

            //std::cout << "[VBF] has valid VBF Dijet ? "<< hasValidVBFDijet<< std::endl;
            if( hasValidVBFDijet ) {
                
                std::pair < Ptr<flashgg::Jet>, Ptr<flashgg::Jet> > dijet;
                // fill dijet pair with lead jet as first, sublead as second.
                dijet.first  =  Jets[jetCollectionIndex]->ptrAt( dijet_indices.first );
                dijet.second =  Jets[jetCollectionIndex]->ptrAt( dijet_indices.second );
                
                dijet_leadEta_    = dijet.first->eta();
                dijet_subleadEta_ = dijet.second->eta();
                dijet_abs_dEta_   = std::fabs( dijet.first->eta() - dijet.second->eta() );
                dijet_LeadJPt_    = dijet.first->pt();
                dijet_SubJPt_     = dijet.second->pt();

                auto leadPho_p4 =  diPhotons->ptrAt( candIndex )->leadingPhoton()->p4();
                auto sublPho_p4 =  diPhotons->ptrAt( candIndex )->subLeadingPhoton()->p4();
                auto leadJet_p4 =  dijet.first->p4();
                auto sublJet_p4 =  dijet.second->p4();
                
                auto diphoton_p4  = leadPho_p4 + sublPho_p4;
                auto dijet_p4     = leadJet_p4 + sublJet_p4;
                
                dijet_dphi_trunc_ = std::min((float) abs(dijet.first->phi() - dijet.second->phi()), (float) 2.916);
                dijet_dipho_dphi_ = std::min((float) abs(dijet_p4.Phi() -diphoton_p4.Phi()), (float) 2.916 );
                
                dijet_dipho_pt_   = (dijet_p4 + diphoton_p4).Pt();
                dijet_Zep_        = fabs(diphoton_p4.Eta() - 0.5 * ( leadJet_p4.Eta() + sublJet_p4.Eta()));
                dijet_Mjj_        = dijet_p4.M();
                dipho_PToM_       = diphoton_p4.Pt()/diphoton_p4.M();
                leadPho_PToM_     = diPhotons->ptrAt(candIndex)->leadingPhoton()->pt()/diphoton_p4.M();
                sublPho_PToM_     = diPhotons->ptrAt(candIndex)->subLeadingPhoton()->pt()/diphoton_p4.M();
                
                dijet_minDRJetPho_= std::min( std::min(reco::deltaR( leadJet_p4,leadPho_p4 ),
                                                       reco::deltaR( sublJet_p4,leadPho_p4 )),
                                              std::min(reco::deltaR( leadJet_p4,sublPho_p4 ),
                                                       reco::deltaR( sublJet_p4,sublPho_p4 ))
                                              );
                
                dijet_dy_         = std::fabs(leadJet_p4.Rapidity() - sublPho_p4.Rapidity());
                dijet_leady_      = leadJet_p4.Rapidity();
                dijet_subleady_   = sublJet_p4.Rapidity();
                
                mvares.leadJet    = *Jets[jetCollectionIndex]->ptrAt( dijet_indices.first );
                mvares.subleadJet = *Jets[jetCollectionIndex]->ptrAt( dijet_indices.second );
                
                mvares.leadJet_ptr    = Jets[jetCollectionIndex]->ptrAt( dijet_indices.first );
                mvares.subleadJet_ptr = Jets[jetCollectionIndex]->ptrAt( dijet_indices.second );
                mvares.diphoton   = *diPhotons->ptrAt( candIndex );
            }else{
                mvares.leadJet_ptr    = edm::Ptr<flashgg::Jet>();
                mvares.subleadJet_ptr = edm::Ptr<flashgg::Jet>();
            }
            
            if ( hasValidVBFDijet && hasValidVBFTrijet){
                mvares.subsubleadJet     = *Jets[jetCollectionIndex]->ptrAt( jet3_index );
                mvares.subsubleadJet_ptr =  Jets[jetCollectionIndex]->ptrAt( jet3_index );
                mvares.hasValidVBFTriJet = 1;
            }else{
                mvares.subsubleadJet_ptr =  edm::Ptr<flashgg::Jet>();
            }
            

            if (_MVAMethod != "") 
                mvares.vbfMvaResult_value = VbfMva_->EvaluateMVA( _MVAMethod.c_str() );
            
            mvares.dijet_leadEta    = dijet_leadEta_ ;
            mvares.dijet_subleadEta = dijet_subleadEta_ ;
            mvares.dijet_abs_dEta   = dijet_abs_dEta_ ;
            mvares.dijet_LeadJPt    = dijet_LeadJPt_ ;
            mvares.dijet_SubJPt     = dijet_SubJPt_ ;
            mvares.dijet_Zep        = dijet_Zep_ ;
            mvares.dijet_dphi_trunc = dijet_dphi_trunc_ ;
            mvares.dijet_dipho_dphi = dijet_dipho_dphi_ ;
            mvares.dijet_Mjj        = dijet_Mjj_ ;
            mvares.dipho_PToM       = dipho_PToM_ ;
            mvares.sublPho_PToM     = sublPho_PToM_ ;
            mvares.leadPho_PToM     = leadPho_PToM_ ;
            mvares.dijet_minDRJetPho= dijet_minDRJetPho_;
            mvares.dijet_dy         = dijet_dy_;
            mvares.dijet_dipho_pt   = dijet_dipho_pt_ ;
            mvares.dijet_leady      = dijet_leady_   ;
            mvares.dijet_subleady   = dijet_subleady_;
            
            vbf_results->push_back( mvares );
        }
        evt.put( vbf_results );
    }
}

typedef flashgg::VBFMVAProducer FlashggVBFMVAProducer;
DEFINE_FWK_MODULE( FlashggVBFMVAProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

