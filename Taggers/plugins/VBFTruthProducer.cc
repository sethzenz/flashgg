#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "flashgg/DataFormats/interface/VBFTag.h"
#include "flashgg/DataFormats/interface/VBFTruth.h"

#include "DataFormats/Math/interface/deltaR.h"

#include <vector>

using namespace std;
using namespace edm;

namespace flashgg {

    class VBFTruthProducer : public EDProducer
    {

    public:
        VBFTruthProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        EDGetTokenT<View<reco::GenParticle> > genPartToken_;
        EDGetTokenT<View<reco::GenJet> > genJetToken_;
        EDGetTokenT<View<VBFTag> > vbfTagToken_;

        bool debug_;
    };

    VBFTruthProducer::VBFTruthProducer( const ParameterSet &iConfig ) :
        genPartToken_( consumes<View<reco::GenParticle> >( iConfig.getUntrackedParameter<InputTag> ( "GenParticleTag", InputTag( "prunedGenParticles" ) ) ) ),
        genJetToken_( consumes<View<reco::GenJet> >( iConfig.getUntrackedParameter<InputTag> ( "GenJetTag", InputTag( "ak4GenJets" ) ) ) ),
        vbfTagToken_( consumes<View<VBFTag> >( iConfig.getUntrackedParameter<InputTag> ( "VbfTag", InputTag( "flashggVBFTag" ) ) ) ),
        debug_( iConfig.getUntrackedParameter<bool>( "Debug", false ) )
    {
        produces<vector<VBFTruth> >();
    }

    void VBFTruthProducer::produce( Event &evt, const EventSetup & )
    {

        Handle<View<reco::GenParticle> > genParticles;
        evt.getByToken( genPartToken_, genParticles );

        Handle<View<reco::GenJet> > genJets;
        evt.getByToken( genJetToken_, genJets );

        Handle<View<VBFTag> > vbfTags;
        evt.getByToken( vbfTagToken_, vbfTags );

        std::auto_ptr<vector<VBFTruth> > truths( new vector<VBFTruth> );

        if( debug_ ) {
            std::cout << setw( 12 ) << "Pt" << setw( 12 ) << "Eta" << setw( 12 ) << "Phi";
            std::cout << setw( 12 ) << "Status" << setw( 12 ) << "PDG id" << setw( 12 ) << "Rapidity";
            std::cout << setw( 12 ) << "nMothers" << setw( 12 ) << "nDaughters" << std::endl;
        }

        for( unsigned int vbfLoop = 0 ; vbfLoop < vbfTags->size() ; vbfLoop++ ) {
            unsigned int index_gp_leadjet = 999, index_gp_subleadjet = 999, index_gp_leadphoton = 999, index_gp_subleadphoton = 999, index_gj_leadjet = 999,
                         index_gj_subleadjet = 999;
            float dr_gp_leadjet = 999., dr_gp_subleadjet = 999., dr_gp_leadphoton = 999., dr_gp_subleadphoton = 999., dr_gj_leadjet = 999., dr_gj_subleadjet = 999.;
            edm::Ptr<VBFTag> tag = vbfTags->ptrAt( vbfLoop );
            VBFTruth truth;
            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                edm::Ptr<reco::GenParticle> part = genParticles->ptrAt( genLoop );
                if( part->status() == 3 ) {
                    float dr = deltaR( tag->leadingJet().eta(), tag->leadingJet().phi(), part->eta(), part->phi() );
                    if( dr < dr_gp_leadjet ) {
                        dr_gp_leadjet = dr;
                        index_gp_leadjet = genLoop;
                    }
                    dr = deltaR( tag->subLeadingJet().eta(), tag->subLeadingJet().phi(), part->eta(), part->phi() );
                    if( dr < dr_gp_subleadjet ) {
                        dr_gp_subleadjet = dr;
                        index_gp_subleadjet = genLoop;
                    }
                    dr = deltaR( tag->diPhoton()->leadingPhoton()->eta(), tag->diPhoton()->leadingPhoton()->phi(), part->eta(), part->phi() );
                    if( dr < dr_gp_leadphoton ) {
                        dr_gp_leadphoton = dr;
                        index_gp_leadphoton = genLoop;
                    }
                    dr = deltaR( tag->diPhoton()->subLeadingPhoton()->eta(), tag->diPhoton()->subLeadingPhoton()->phi(), part->eta(), part->phi() );
                    if( dr < dr_gp_subleadphoton ) {
                        dr_gp_subleadphoton = dr;
                        index_gp_subleadphoton = genLoop;
                    }
                }
            }
            truth.closestParticleToLeadingJet = genParticles->ptrAt( index_gp_leadjet );
            truth.closestParticleToSubLeadingJet = genParticles->ptrAt( index_gp_subleadjet );
            truth.closestParticleToLeadingPhoton = genParticles->ptrAt( index_gp_leadphoton );
            truth.closestParticleToSubLeadingPhoton = genParticles->ptrAt( index_gp_subleadphoton );
            for( unsigned int gjLoop = 0 ; gjLoop < genJets->size() ; gjLoop++ ) {
                edm::Ptr<reco::GenJet> gj = genJets->ptrAt( gjLoop );
                float dr = deltaR( tag->leadingJet().eta(), tag->leadingJet().phi(), gj->eta(), gj->phi() );
                if( dr < dr_gj_leadjet ) {
                    dr_gj_leadjet = dr;
                    index_gj_leadjet = gjLoop;
                }
                dr = deltaR( tag->subLeadingJet().eta(), tag->subLeadingJet().phi(), gj->eta(), gj->phi() );
                if( dr < dr_gj_subleadjet ) {
                    dr_gj_subleadjet = dr;
                    index_gj_subleadjet = gjLoop;
                }
            }
            truth.closestGenJetToLeadingJet = genJets->ptrAt( index_gj_leadjet );
            truth.closestGenJetToSubLeadingJet = genJets->ptrAt( index_gj_subleadjet );
            truths->push_back( truth );
        }

        unsigned int index_leadq = 999, index_subleadq = 999;
        float pt_leadq = 0., pt_subleadq = 0.;

        for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
            edm::Ptr<reco::GenParticle> part = genParticles->ptrAt( genLoop );
            if( part->status() == 3 ) {
                if( abs( part->pdgId() ) <= 5 ) {
                    if( part->pt() > pt_leadq ) {
                        index_subleadq = index_leadq;
                        pt_subleadq = pt_leadq;
                        index_leadq = genLoop;
                        pt_leadq = part->pt();
                    } else if( part->pt() > pt_subleadq ) {
                        index_subleadq = genLoop;
                        pt_subleadq = part->pt();
                    }
                }
                if( debug_ ) {
                    std::cout << setw( 12 ) << part->pt();
                    std::cout << setw( 12 ) << part->eta();
                    std::cout << setw( 12 ) << part->phi();
                    std::cout << setw( 12 ) << part->status();
                    std::cout << setw( 12 ) << part->pdgId();
                    std::cout << setw( 12 ) << part->rapidity();
                    std::cout << setw( 12 ) << part->numberOfMothers();
                    std::cout << setw( 12 ) << part->numberOfDaughters();
                    std::cout << std::endl;
                }
            }
        }

        for( unsigned int i = 0 ; i < truths->size(); i++ ) {
            truths->at( i ).leadingQuark = genParticles->ptrAt( index_leadq );
            truths->at( i ).subLeadingQuark = genParticles->ptrAt( index_subleadq );
        }

        evt.put( truths );
    }
}

typedef flashgg::VBFTruthProducer FlashggVBFTruthProducer;
DEFINE_FWK_MODULE( FlashggVBFTruthProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

