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
        bool _isLegacyMVA;
        bool _usePuJetID;
        bool _requireOppositeEta;
        bool _useHighestDijetMinv;
        double _minDrToPhoton;
        double _minDijetMinv;
        double _minJetPt;

        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;

        float dijet_leadEta_;
        float dijet_subleadEta_;
        float dijet_abs_dEta_;
        float dijet_LeadJPt_;
        float dijet_SubJPt_;
        float dijet_Zep_;
        float dijet_dPhi_trunc_;
        float dijet_Mjj_;
        float dipho_PToM_;
        float leadPho_PToM_;
        float sublPho_PToM_;


    };

    VBFMVAProducer::VBFMVAProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        //jetTokenDz_( consumes<View<flashgg::Jet> >( iConfig.getParameter<InputTag>( "JetTag" ) ) ),
        inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        _isLegacyMVA( iConfig.getUntrackedParameter<bool>( "UseLegacyMVA" , false ) ),
        _usePuJetID( iConfig.getUntrackedParameter<bool>( "UsePuJetID" , false ) ),
        _requireOppositeEta( iConfig.getUntrackedParameter<bool>( "RequireOppositeEta", false ) ),        
        _useHighestDijetMinv( iConfig.getUntrackedParameter<bool>( "UseHighestDijetMinv", false ) ),
        _minDrToPhoton( iConfig.getUntrackedParameter<double>( "MinDrToPhoton", 0.4) ),
        _minDijetMinv( iConfig.getParameter<double>( "MinDijetMinv" ) ),
        _minJetPt( iConfig.getUntrackedParameter<double>( "MinJetPt", 0.) )
        /*
        bool _requireOppositeEta;
        bool _useHighestDijetMinv;
        double _minDrToPhoton;
        double _minDijetMinv;
        double _minJetPt;
        */
    {

        vbfMVAweightfile_ = iConfig.getParameter<edm::FileInPath>( "vbfMVAweightfile" );

        dijet_leadEta_ = -999.;
        dijet_subleadEta_ = -999.;
        dijet_abs_dEta_ = -999.;
        dijet_LeadJPt_ = -999.;
        dijet_SubJPt_ = -999.;
        dijet_Zep_ = -999.;
        dijet_dPhi_trunc_ = -999.;
        dijet_Mjj_ = -999.;
        dipho_PToM_ = -999.;
        leadPho_PToM_ = -999.;
        sublPho_PToM_ = -999.;


        VbfMva_.reset( new TMVA::Reader( "!Color:Silent" ) );

        if( _isLegacyMVA ) {
            // legacy input var
            VbfMva_->AddVariable( "dijet_leadEta", &dijet_leadEta_ );
            VbfMva_->AddVariable( "dijet_subleadEta", &dijet_subleadEta_ );
            VbfMva_->AddVariable( "dijet_LeadJPt", &dijet_LeadJPt_ );
            VbfMva_->AddVariable( "dijet_SubJPt", &dijet_SubJPt_ );
            VbfMva_->AddVariable( "dijet_Zep", &dijet_Zep_ );
            VbfMva_->AddVariable( "min(dijet_dPhi,2.916)", &dijet_dPhi_trunc_ );
            VbfMva_->AddVariable( "dijet_Mjj", &dijet_Mjj_ );
            VbfMva_->AddVariable( "dipho_pt/mass", &dipho_PToM_ );
            VbfMva_->BookMVA( "BDTG", vbfMVAweightfile_.fullPath() );
        } else {
            // new flashgg var
            VbfMva_->AddVariable( "dijet_LeadJPt" , &dijet_LeadJPt_ );
            VbfMva_->AddVariable( "dijet_SubJPt" , &dijet_SubJPt_ );
            VbfMva_->AddVariable( "dijet_abs_dEta" , &dijet_abs_dEta_ );
            VbfMva_->AddVariable( "dijet_Mjj" , &dijet_Mjj_ );
            VbfMva_->AddVariable( "dijet_Zep" , &dijet_Zep_ );
            VbfMva_->AddVariable( "dijet_dPhi_trunc", &dijet_dPhi_trunc_ );
            //VbfMva_->AddVariable("dipho_pt/mass", &dipho_PToM_);
            VbfMva_->AddVariable( "leadPho_PToM", &leadPho_PToM_ );
            VbfMva_->AddVariable( "sublPho_PToM", &sublPho_PToM_ );
            VbfMva_->BookMVA( "BDT", vbfMVAweightfile_.fullPath() );
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

            dijet_leadEta_ = -999.;
            dijet_subleadEta_ = -999.;
            dijet_abs_dEta_ = -999.;
            dijet_LeadJPt_ = -999.;
            dijet_SubJPt_ = -999.;
            dijet_Zep_ = -999.;
            dijet_dPhi_trunc_ = -999.;
            dijet_Mjj_ = -999.;
            dipho_PToM_ = -999.;
            leadPho_PToM_ = -999.;
            sublPho_PToM_ = -999.;


            // First find dijet by looking for highest-pt jets...
            std::pair <int, int> dijet_indices( -1, -1 );
            std::pair <float, float> dijet_pts( -1., -1. );
            float mjj_so_far = -999.;
            //			float PuIDCutoff = 0.8;
            //            float dr2pho = _minDrToPhoton;

            float phi1 = diPhotons->ptrAt( candIndex )->leadingPhoton()->phi();
            float eta1 = diPhotons->ptrAt( candIndex )->leadingPhoton()->eta();
            float phi2 = diPhotons->ptrAt( candIndex )->subLeadingPhoton()->phi();
            float eta2 = diPhotons->ptrAt( candIndex )->subLeadingPhoton()->eta();

            bool hasValidVBFDijet = 0;

            // take the jets corresponding to the diphoton candidate
            unsigned int jetCollectionIndex = diPhotons->ptrAt( candIndex )->jetCollectionIndex();

            if ( Jets[jetCollectionIndex]->size() < 2 ) continue;

            for( UInt_t jetLoop = 0; jetLoop < Jets[jetCollectionIndex]->size() ; jetLoop++ ) {

                Ptr<flashgg::Jet> jet  = Jets[jetCollectionIndex]->ptrAt( jetLoop );

                std::cout << "  Jet " << jetLoop << " pt eta _requireOppositeEta _useHighestDijetMinv " << jet->pt() << " " << jet->eta() 
                          << " " << _requireOppositeEta << " " << _useHighestDijetMinv << std::endl;

                //pass PU veto??
                //if (jet->puJetId(diPhotons[candIndex]) <  PuIDCutoff) {continue;}
                if( _usePuJetID && !jet->passesPuJetId( diPhotons->ptrAt( candIndex ) ) ) { continue; }
                // within eta 4.7?

                if( fabs( jet->eta() ) > 4.7 ) { continue; }
                // close to lead photon?

                float dPhi = deltaPhi( jet->phi(), phi1 );
                float dEta = jet->eta() - eta1;
                if( sqrt( dPhi * dPhi + dEta * dEta ) < _minDrToPhoton ) { std::cout << " Jet " << jetLoop << " near photon" << std::endl; continue; }

                // close to sublead photon?
                dPhi = deltaPhi( jet->phi(), phi2 );
                dEta = jet->eta() - eta2;
                if( sqrt( dPhi * dPhi + dEta * dEta ) < _minDrToPhoton ) { std::cout << " Jet " << jetLoop << " near photon" << std::endl; continue; }

                if ( jet->pt() < _minJetPt ) { continue; }

                if (dijet_indices.first == -1) {
                    std::cout << " Lead jet has pt=" << jet->pt() << " eta=" << jet->eta() << " " << jetLoop << std::endl;
                    dijet_indices.first = jetLoop;
                    dijet_pts.first = jet->pt();
                } else {
                    if ( _requireOppositeEta ) {
                        float eta_product = (jet->eta())*(Jets[jetCollectionIndex]->ptrAt( dijet_indices.first )->eta());
                        std::cout << " RequireOppositeEta is on!" << std::endl;
                        if ( eta_product > 0. ) { std::cout << " Skipping this jet!" << std::endl; continue; } else { std::cout << " But this dijet is fine!" << std::endl; }
                    } else {
                        std::cout << " RequireOppositeEta is off!" << std::endl;
                    }
                    if ( _useHighestDijetMinv ) {
                        std::cout << " Use highest DijetMinv is on!" << std::endl;
                        auto lead_p4 = Jets[jetCollectionIndex]->ptrAt( dijet_indices.first )->p4();
                        auto sublead_p4 = jet->p4();
                        double mjj = (lead_p4+sublead_p4).M();
                        if (mjj > mjj_so_far) {
                            std::cout << " Improving old mjj=" << mjj_so_far << " to " << mjj << " with index " << jetLoop << std::endl;
                            dijet_indices.second = jetLoop;
                            dijet_pts.second = jet->pt();
                            mjj_so_far = mjj;
                        }
                    } else {
                        std::cout << " Use highest DijetMinv is off so we take the second jet and call if a day jetLoop=" << jetLoop <<std::endl;
                        dijet_indices.second = jetLoop;
                        dijet_pts.second = jet->pt();
                        break;
                    }
                }

                // OLD CODE
                // jets are sorted descending in pt (enforced by JetProducer)
                // So it is not possible for a later jet to have higher pt
                /*
                if( jet->pt() > dijet_pts.first ) {
                    // if pt of this jet is higher than the one currently in lead position
                    // then shift back lead jet into sublead position...
                    dijet_indices.second = dijet_indices.first;
                    dijet_pts.second = dijet_pts.first;
                    // .. and put the new jet as the lead jet.
                    dijet_indices.first = jetLoop;
                    dijet_pts.first = jet->pt();
                } else if( jet->pt() > dijet_pts.second ) {
                    // if the jet's pt isn't as high as the lead jet's but i higher than the sublead jet's
                    // The replace the sublead jet by this new jet.
                    dijet_indices.second = jetLoop;
                    dijet_pts.second = jet->pt();
                }
                */
                // if the jet's pt is neither higher than the lead jet or sublead jet, then forget it!
                //                if( dijet_indices.first != -1 && dijet_indices.second != -1 ) {hasValidVBFDijet = 1;}

            }

            hasValidVBFDijet = ( dijet_indices.first != -1 && dijet_indices.second != -1 );

            //std::cout << "[VBF] has valid VBF Dijet ? "<< hasValidVBFDijet<< std::endl;
            if( hasValidVBFDijet ) {

                std::pair < Ptr<flashgg::Jet>, Ptr<flashgg::Jet> > dijet;
                // fill dijet pair with lead jet as first, sublead as second.
                dijet.first =  Jets[jetCollectionIndex]->ptrAt( dijet_indices.first );
                dijet.second =  Jets[jetCollectionIndex]->ptrAt( dijet_indices.second );

                dijet_leadEta_ = dijet.first->eta();
                dijet_subleadEta_ = dijet.second->eta();
                dijet_abs_dEta_ = std::fabs( dijet.first->eta() - dijet.second->eta() );
                dijet_LeadJPt_ = dijet.first->pt();
                dijet_SubJPt_ = dijet.second->pt();

                auto leadPho_p4 = diPhotons->ptrAt( candIndex )->leadingPhoton()->p4();
                auto sublPho_p4 =  diPhotons->ptrAt( candIndex )->subLeadingPhoton()->p4();
                auto leadJet_p4 =  dijet.first->p4();
                auto sublJet_p4 =  dijet.second->p4();

                auto diphoton_p4 = leadPho_p4 + sublPho_p4;
                auto dijet_p4 = leadJet_p4 + sublJet_p4;

                dijet_Zep_ = fabs( diphoton_p4.Eta() - 0.5 * ( leadJet_p4.Eta() + sublJet_p4.Eta() ) );
                float dijet_dPhi_ = fabs( deltaPhi( dijet_p4.Phi(), diphoton_p4.Phi() ) );
                dijet_dPhi_trunc_ = std::min( dijet_dPhi_, (float) 2.916 );
                //                std::cout << " dphi (trunc): " << dijet_dPhi_ << " " << dijet_dPhi_trunc_ << std::endl;
                dijet_Mjj_ = dijet_p4.M();
                dipho_PToM_ = diphoton_p4.Pt() / diphoton_p4.M();
                leadPho_PToM_ = diPhotons->ptrAt( candIndex )->leadingPhoton()->pt() / diphoton_p4.M();
                sublPho_PToM_ = diPhotons->ptrAt( candIndex )->subLeadingPhoton()->pt() / diphoton_p4.M();

                //debug stuff
                //	std::cout<<"numbr of jets " <<  Jets[jetCollectionIndex]->size() << std::endl;
                //	std::cout<<"jet indices: " <<  dijet_indices.first << "	" << dijet_indices.second << std::endl;
                mvares.leadJet    = *Jets[jetCollectionIndex]->ptrAt( dijet_indices.first );
                mvares.subleadJet = *Jets[jetCollectionIndex]->ptrAt( dijet_indices.second );


                //debug stuff
                //std::cout << mvares.leadJet.eta() << std::endl;
                //std::cout << mvares.subleadJet.eta() << std::endl;

            }

            if( _isLegacyMVA ) {
                mvares.vbfMvaResult_value = VbfMva_->EvaluateMVA( "BDTG" );
            } else {
                mvares.vbfMvaResult_value = VbfMva_->EvaluateMVA( "BDT" );
            }

            //	mvares.vbfMvaResult_value = VbfMva_->EvaluateMVA("BDT");
            //std::cout <<" debug mva " <<  mvares.vbfMvaResult_value << std::endl;
            mvares.dijet_leadEta = dijet_leadEta_ ;
            mvares.dijet_subleadEta = dijet_subleadEta_ ;
            mvares.dijet_abs_dEta = dijet_abs_dEta_ ;
            mvares.dijet_LeadJPt = dijet_LeadJPt_ ;
            mvares.dijet_SubJPt = dijet_SubJPt_ ;
            mvares.dijet_Zep =    dijet_Zep_ ;
            mvares.dijet_dPhi_trunc = dijet_dPhi_trunc_ ;
            mvares.dijet_Mjj =    dijet_Mjj_ ;
            mvares.dipho_PToM =   dipho_PToM_ ;
            mvares.sublPho_PToM = sublPho_PToM_ ;
            mvares.leadPho_PToM = leadPho_PToM_ ;

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

