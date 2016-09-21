#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Common/interface/Handle.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

using namespace std;
using namespace edm;

// **********************************************************************                                                                                                                                             

namespace flashgg {

    class TriggerTestAnalyzer : public EDAnalyzer
    {
    public:
        explicit TriggerTestAnalyzer( const ParameterSet & );
        ~TriggerTestAnalyzer();
        
        static void fillDescriptions( ConfigurationDescriptions &descriptions );
        
        
    private:
        
        virtual void beginJob() override;
        virtual void analyze( const Event &, const EventSetup & ) override;
        virtual void endJob() override;
        
        EDGetTokenT<View<flashgg::DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<TriggerResults> triggerResultToken_;
        vector<string> triggerPaths_;
        
        map<string, unsigned int> trigger_indices;
    };
    
    TriggerTestAnalyzer::TriggerTestAnalyzer( const ParameterSet &iConfig ):
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        triggerResultToken_( consumes<TriggerResults>( iConfig.getParameter<InputTag>( "TriggerResult" ) ) ),
        triggerPaths_( iConfig.getParameter<std::vector<std::string> >("TriggerPaths"))
    {
        for (unsigned i = 0 ; i < triggerPaths_.size() ; i++) {
            triggerPaths_[i] = HLTConfigProvider::removeVersion( triggerPaths_[i] );
        }
    }
    
    TriggerTestAnalyzer::~TriggerTestAnalyzer()
    {
    }
    
    void
    TriggerTestAnalyzer::analyze( const Event &iEvent, const EventSetup &iSetup )
    {
        Handle<TriggerResults> triggerResult;
        iEvent.getByToken( triggerResultToken_, triggerResult );
        const edm::TriggerNames &triggerNames = iEvent.triggerNames( *triggerResult );
        
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        iEvent.getByToken( diPhotonToken_, diPhotons );

        if ( diPhotons->size() == 0 ) {
        
            std::cout << "*** Number of diphotons: " << diPhotons->size() << " *** " << std::endl;
            
            for( unsigned int i = 0; i < triggerNames.triggerNames().size(); i++ ) {
                std::string trimmedName = HLTConfigProvider::removeVersion( triggerNames.triggerName( i ) );
                //            std::cout << triggerNames.triggerName( i ) << std::endl;
                trigger_indices[trimmedName] = triggerNames.triggerIndex( triggerNames.triggerName( i ) );
            }
            
            std::cout << " *** LIST OF PASSING TRIGGERS IN DoubleEG PD *** " << std::endl;
            
            unsigned tot_in_list = 0;
            //        unsigned tot = 0;
            
            for ( auto && path : triggerPaths_) {
                //            std::cout << path;
                //            bool found = false;
                for (std::map<std::string, unsigned int>::const_iterator cit = trigger_indices.begin(); cit != trigger_indices.end(); cit++) {
                    if (path == cit->first) {
                        //                    std::cout << " " << triggerResult->accept(cit->second) << std::endl;
                        if ( triggerResult->accept(cit->second) ) {
                            std::cout << "     " << path << std::endl;
                            tot_in_list++;
                        }
                        //                    found = true;
                        break;
                    }
                }
                //            if (!found) {
                //                std::cout << " NOT FOUND " << std::endl;
                //            }
            }

            std::cout << " * Number passed: " << tot_in_list << std::endl;
            //        std::cout << " * Length of trigger list: " << triggerNames.triggerNames().size() << std::endl;
            //        std::cout << " *** EVERYTHING THAT PASSED *** " << std::endl;
            
            //        for (std::map<std::string, unsigned int>::const_iterator cit = trigger_indices.begin(); cit != trigger_indices.end(); cit++) {
            //            if ( triggerResult->accept(cit->second) ) {
            //                std::cout << cit->first << std::endl;
            //                tot++;
            //            }
            //        }
            
            //        std::cout << " * Passed overall: " << tot << std::endl;
            
            std::cout << "******************************" << std::endl;
        } // if 0 diphotons

    } // analyze                                                                                                                                                                                                      
    void
    TriggerTestAnalyzer::beginJob()
    {
    }
    
    void
    TriggerTestAnalyzer::endJob()
    {
    }
    
    void
    TriggerTestAnalyzer::fillDescriptions( edm::ConfigurationDescriptions &descriptions )
    {
        //The following says we do not know what parameters are allowed so do no validation
        // Please change this to state exactly what you do use, even if it is no parameters
        edm::ParameterSetDescription desc;
        desc.setUnknown();
        descriptions.addDefault( desc );
    }
    
} // namespace flashgg

typedef flashgg::TriggerTestAnalyzer FlashggTriggerTestAnalyzer;
DEFINE_FWK_MODULE( FlashggTriggerTestAnalyzer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4


