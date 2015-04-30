#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "flashgg/DataFormats/interface/VBFTag.h"
#include "flashgg/DataFormats/interface/VBFTruth.h"

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
        genPartToken_ (consumes<View<reco::GenParticle> >(iConfig.getUntrackedParameter<InputTag> ("GenParticleTag", InputTag("prunedGenParticles")))),
        genJetToken_ (consumes<View<reco::GenJet> >(iConfig.getUntrackedParameter<InputTag> ("GenJetTag", InputTag("ak4GenJets")))),
        vbfTagToken_( consumes<View<VBFTag> >( iConfig.getUntrackedParameter<InputTag> ( "VbfTag", InputTag( "flashggVBFTag")))),
        debug_( iConfig.getUntrackedParameter<bool>( "Debug", false ) )
    {
        produces<vector<VBFTruth> >();
    }

    void VBFTruthProducer::produce( Event &evt, const EventSetup & )
    {

        Handle<View<reco::GenParticle> > genParticles;
        evt.getByToken(genPartToken_,genParticles);

        Handle<View<reco::GenJet> > genJets;
        evt.getByToken(genJetToken_,genJets);

        Handle<View<VBFTag> > vbfTags;
        evt.getByToken(vbfTagToken_,vbfTags);

        std::auto_ptr<vector<VBFTruth> > truths( new vector<VBFTruth> );

        if (debug_) {
            std::cout << setw(12) << "Pt" << setw(12) << "Eta" << setw(12) << "Phi"; 
            std::cout << setw(12) << "Status" << setw(12) << "PDG id" << setw(12) << "Rapidity";
            std::cout << setw(12) << "nMothers" << setw(12) << "nDaughters" << std::endl;
        }
        
        for( unsigned int genLoop =0 ; genLoop < genParticles->size(); genLoop++){
            if (genParticles->ptrAt(genLoop)->status() == 3) {
                if (debug_) {
                    std::cout << setw(12) << genParticles->ptrAt(genLoop)->pt();
                    std::cout << setw(12) << genParticles->ptrAt(genLoop)->eta();
                    std::cout << setw(12) << genParticles->ptrAt(genLoop)->phi();
                    std::cout << setw(12) << genParticles->ptrAt(genLoop)->status();
                    std::cout << setw(12) << genParticles->ptrAt(genLoop)->pdgId();
                    std::cout << setw(12) << genParticles->ptrAt(genLoop)->rapidity();
                    std::cout << setw(12) << genParticles->ptrAt(genLoop)->numberOfMothers();
                    std::cout << setw(12) << genParticles->ptrAt(genLoop)->numberOfDaughters();
                    std::cout << std::endl;
                }
            }
        }

        evt.put(truths);
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

