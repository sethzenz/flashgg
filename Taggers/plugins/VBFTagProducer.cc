#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/VBFDiPhoDiJetMVAResult.h"
#include "flashgg/DataFormats/interface/VBFMVAResult.h"
#include "flashgg/DataFormats/interface/VBFTag.h"

#include <vector>
#include <algorithm>

using namespace std;
using namespace edm;

namespace flashgg {

    class VBFTagProducer : public EDProducer
    {

    public:
        VBFTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        int chooseCategory( float );

        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<View<VBFDiPhoDiJetMVAResult> > vbfDiPhoDiJetMvaResultToken_;
        EDGetTokenT<View<VBFMVAResult> > vbfMvaResultToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;

        vector<double> boundaries;

    };

    VBFTagProducer::VBFTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getUntrackedParameter<InputTag> ( "DiPhotonTag", InputTag( "flashggDiPhotons" ) ) ) ),
        vbfDiPhoDiJetMvaResultToken_( consumes<View<flashgg::VBFDiPhoDiJetMVAResult> >( iConfig.getUntrackedParameter<InputTag> ( "VBFDiPhoDiJetMVAResultTag",
                                      InputTag( "flashggVBFDiPhoDiJetMVA" ) ) ) ),
        //    vbfMvaResultToken_(consumes<View<flashgg::VBFMVAResult> >(iConfig.getUntrackedParameter<InputTag> ("VBFMVAResultTag", InputTag("flashggVBFMVA")))),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getUntrackedParameter<InputTag> ( "MVAResultTag", InputTag( "flashggDiPhotonMVA" ) ) ) )

    {
        vector<double> default_boundaries;
        default_boundaries.push_back( 0.52 );
        default_boundaries.push_back( 0.85 );
        default_boundaries.push_back( 0.915 );
        default_boundaries.push_back( 1 ); // from here
        //https://github.com/h2gglobe/h2gglobe/blob/master/AnalysisScripts/massfac_mva_binned/massfactorizedmvaanalysis.dat#L32

        // getUntrackedParameter<vector<float> > has no library, so we use double transiently
        boundaries = iConfig.getUntrackedParameter<vector<double > >( "Boundaries", default_boundaries );

        assert( is_sorted( boundaries.begin(), boundaries.end() ) ); // we are counting on ascending order - update this to give an error message or exception

        produces<vector<VBFTag> >();
    }

    int VBFTagProducer::chooseCategory( float mvavalue )
    {
        // should return 0 if mva above all the numbers, 1 if below the first, ..., boundaries.size()-N if below the Nth, ...
        int n;
        for( n = 0 ; n < ( int )boundaries.size() ; n++ ) {
            if( ( double )mvavalue > boundaries[boundaries.size() - n - 1] ) { return n; }
        }
        return -1; // Does not pass, object will not be produced
    }

    void VBFTagProducer::produce( Event &evt, const EventSetup & )
    {

        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );

        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {
            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( candIndex );

            cout << "dipho->mass() " << dipho->mass() << endl;

        }
//        evt.put( tags );
    }
}

typedef flashgg::VBFTagProducer FlashggVBFTagProducer;
DEFINE_FWK_MODULE( FlashggVBFTagProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

