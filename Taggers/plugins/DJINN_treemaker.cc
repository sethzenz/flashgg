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
        bool expectMultiples_;

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
        expectMultiples_( iConfig.getUntrackedParameter<bool>( "ExpectMultiples", false) )
    {
//        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
//            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
//            tokenJets_.push_back(token);
//        }
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

        /*
        JetCollectionVector Jets( inputTagJets_.size() );
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
            iEvent.getByToken( tokenJets_[j], Jets[j] );
        }

        unsigned jetCollectionIndex = 0;

        for( UInt_t jetLoop = 0; jetLoop < Jets[jetCollectionIndex]->size() ; jetLoop++ ) {

            Ptr<flashgg::Jet> jet  = Jets[jetCollectionIndex]->ptrAt( jetLoop );
            std::cout << jetLoop << std::endl;
            std::cout << jet->pt() << " " << jet->eta() << " " << jet->phi() << std::endl;
            std::cout << jet->hasConstituentInfo() << std::endl;
            
            std::vector<float> infoVec = jet->getConstituentInfo();
            unsigned count = 0;
            for (unsigned i=0;i<infoVec.size();i++){
                std::cout << infoVec[i] << " ";
                count++;
                if (count == 4){
                    count = 0;
                    std::cout << std::endl;
                }
            }

        }
        */





        if (!expectMultiples_) {
            assert (TagSorter.product()->size() <= 1);
            if ( TagSorter.product()->size() == 0) std::cout << "[NO TAG]" << std::endl;
        } else {
            std::cout << "Multiple tags allowed and we have a total of " << TagSorter.product()->size() << std::endl;
        }

        for ( auto tag = TagSorter.product()->begin() ; tag != TagSorter.product()->end() ; tag++ ) {
            const flashgg::DiPhotonTagBase *chosenTag = &*( tag );

            const	VBFTag *vbftag = dynamic_cast<const VBFTag *>( chosenTag );
            if( vbftag != NULL ) {
                std::cout << "[VBF] Category " << vbftag->categoryNumber() << " with lead jet pt eta "
                          << vbftag->leadingJet().pt() << " " << vbftag->leadingJet().eta()
                          << " and sublead jet eta " << vbftag->subLeadingJet().pt() << " " << vbftag->subLeadingJet().eta() << " mass=" << vbftag->diPhoton()->mass()
                          << ", systLabel " << vbftag->systLabel() << std::endl;
                std::cout << "    VBFDiPhoDiJetMVA=" << vbftag->VBFDiPhoDiJetMVA().VBFDiPhoDiJetMVAValue()
                          << " VBFMVA=" << vbftag->VBFMVA().VBFMVAValue()
                          << std::endl;


            }


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

