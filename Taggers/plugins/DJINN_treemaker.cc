// DJINNTreeMaker.cc by J. Wright
//
// * Produces dataset trees for DJINN training/evaluation
//
// Adapted from TagTestAnalyzer code by S. Zenz, which was
// Adapted from globelikeTreeMakerWithTagSorter code by L. D. Corpe, which was
// Adapted from the flashggCommissioning tree maker code  by C. Favaro et al.

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Common/interface/Handle.h"

#include "flashgg/DataFormats/interface/VBFTag.h"
#include "flashgg/DataFormats/interface/UntaggedTag.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/TTHHadronicTag.h"
#include "flashgg/DataFormats/interface/TTHLeptonicTag.h"
#include "flashgg/DataFormats/interface/VHTightTag.h"
#include "flashgg/DataFormats/interface/VHEtTag.h"
#include "flashgg/DataFormats/interface/VHLooseTag.h"
#include "flashgg/DataFormats/interface/VHHadronicTag.h"
#include "flashgg/DataFormats/interface/VBFTagTruth.h"
#include "flashgg/DataFormats/interface/ZPlusJetTag.h"

#include "flashgg/DataFormats/interface/Jet.h"

using namespace std;
using namespace edm;

// **********************************************************************

namespace flashgg {

    class DJINNTreeMaker : public edm::EDAnalyzer
    {
    public:
        explicit DJINNTreeMaker( const edm::ParameterSet & );
        ~DJINNTreeMaker();

        static void fillDescriptions( edm::ConfigurationDescriptions &descriptions );


    private:

        edm::Service<TFileService> fs_;

        virtual void beginJob() override;
        virtual void analyze( const edm::Event &, const edm::EventSetup & ) override;
        virtual void endJob() override;

        edm::EDGetTokenT<edm::View<flashgg::DiPhotonTagBase> > TagSorterToken_;

        EDGetTokenT<View<DiPhotonCandidate>> diphotonToken_;
        EDGetTokenT<View<DiPhotonMVAResult>> mvaResultToken_;
        std::vector<edm::InputTag> inputTagJets_;
        std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;

        bool expectMultiples_;

        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;
    };

// ******************************************************************************************
// ******************************************************************************************

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
    DJINNTreeMaker::DJINNTreeMaker( const edm::ParameterSet &iConfig ):
        TagSorterToken_( consumes<edm::View<flashgg::DiPhotonTagBase> >( iConfig.getParameter<InputTag> ( "TagSorter" ) ) ),
        diphotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        inputTagJets_ ( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        expectMultiples_( iConfig.getUntrackedParameter<bool>( "ExpectMultiples", false) )
    {
        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }
    }

    DJINNTreeMaker::~DJINNTreeMaker()
    {

    }

    void
    DJINNTreeMaker::analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup )
    {

        // ********************************************************************************
        // access edm objects

        Handle<edm::View<flashgg::DiPhotonTagBase> > TagSorter;
        iEvent.getByToken( TagSorterToken_, TagSorter );

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        iEvent.getByToken( mvaResultToken_, mvaResults );

        Handle<View<flashgg::DiPhotonCandidate> > diphotons;
        iEvent.getByToken( diphotonToken_, diphotons );

        JetCollectionVector jetCollection( inputTagJets_.size() );
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
            iEvent.getByToken( tokenJets_[j], jetCollection[j] );
        }
        unsigned jetCollectionIndex = 0;
        for ( auto tag = TagSorter.product()->begin(); tag != TagSorter.product()->end(); tag++) {
            jetCollectionIndex = tag->diPhoton()->jetCollectionIndex();
        }
        edm::Handle<edm::View<flashgg::Jet>> jets = jetCollection[jetCollectionIndex]; 

        // ********************************************************************************
        // do stuff with dem objects

        std::cout << "-----Diphoton Info" << std::endl;
        for (unsigned i=0;i<diphotons->size();i++){
            edm::Ptr<flashgg::DiPhotonCandidate> diphoton = diphotons->ptrAt(i);
            std::cout << i << std::endl;
            std::cout << "\t" << diphoton->mass() << std::endl; 
            std::cout << "\t" << diphoton->eta() << std::endl; 
            std::cout << "\t" << diphoton->phi() << std::endl; 
            std::cout << "\t" << diphoton->pt() << std::endl; 
            std::cout << "\t" << diphoton->leadingPhoton()->eta() << std::endl;
            std::cout << "\t" << diphoton->subLeadingPhoton()->eta() << std::endl;
        }

        std::cout << "-----Diphoton MVA Info" << std::endl;
        for (unsigned i=0;i<mvaResults->size();i++){
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt(i);
            std::cout << i << std::endl;
            std::cout << "\t" << mvares->leadptom << std::endl;
            std::cout << "\t" << mvares->subleadptom << std::endl;
            std::cout << "\t" << mvares->subleadmva << std::endl;
            std::cout << "\t" << mvares->leadmva << std::endl;
            std::cout << "\t" << mvares->leadeta << std::endl;
            std::cout << "\t" << mvares->subleadeta << std::endl;
            std::cout << "\t" << mvares->sigmarv << std::endl;
            std::cout << "\t" << mvares->decorrSigmarv << std::endl;
            std::cout << "\t" << mvares->sigmawv << std::endl;
            std::cout << "\t" << mvares->CosPhi << std::endl;
            std::cout << "\t" << mvares->vtxprob << std::endl;
            std::cout << "\t" << mvares->result << std::endl;
        }
        

        std::pair<int,int> dijet_indices(-999,-999);
        std::pair<float,float> dijet_pts(0.0,0.0);

        for (unsigned i=0;i<jets->size();i++){
            if (jets->ptrAt(i)->pt() > dijet_pts.first){
                dijet_indices.second = dijet_indices.first;
                dijet_pts.second = dijet_pts.first;
                dijet_indices.first = i;
                dijet_pts.first = jets->ptrAt(i)->pt();
            }else if (jets->ptrAt(i)->pt() > dijet_pts.second){
                dijet_indices.second = i;
                dijet_pts.second = jets->ptrAt(i)->pt();
            }
        }

        std::cout << "-----Dijet Info" << std::endl;
        if (dijet_indices.first >= 0 && dijet_indices.second >= 0){
            std::cout << "\t" << jets->ptrAt(dijet_indices.first)->pt();
            std::cout << "  " << jets->ptrAt(dijet_indices.first)->eta();
            std::cout << "  " << jets->ptrAt(dijet_indices.first)->phi();
            std::cout << "  " << std::endl;
            std::cout << "\t" << jets->ptrAt(dijet_indices.second)->pt();
            std::cout << "  " << jets->ptrAt(dijet_indices.second)->eta();
            std::cout << "  " << jets->ptrAt(dijet_indices.second)->phi();
            std::cout << std::endl;
        }else{
            std::cout << "No valid dijet!" << std::endl;
        }


         
    } // analyze

    void
    DJINNTreeMaker::beginJob()
    {
    }

    void
    DJINNTreeMaker::endJob()
    {
    }

    void
    DJINNTreeMaker::fillDescriptions( edm::ConfigurationDescriptions &descriptions )
    {
        //The following says we do not know what parameters are allowed so do no validation
        // Please change this to state exactly what you do use, even if it is no parameters
        edm::ParameterSetDescription desc;
        desc.setUnknown();
        descriptions.addDefault( desc );
    }

} // namespace flashgg

typedef flashgg::DJINNTreeMaker FlashggDJINNTreeMaker;
DEFINE_FWK_MODULE( FlashggDJINNTreeMaker );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

