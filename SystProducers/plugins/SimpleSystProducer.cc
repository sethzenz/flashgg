#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TrackingTools/IPTools/interface/IPTools.h"

#include "flashgg/SystAlgos/interface/BaseSystMethods.h"
//#include "flashgg/SystAlgos/interface/ApplySystVariations.h"

using namespace edm;
using namespace std;

namespace flashgg {

	class PhotonSystematicProducer : public edm::EDProducer {
		public:

			PhotonSystematicProducer(const edm::ParameterSet&);

		private:
	  		
			void produce(edm::Event&, const edm::EventSetup&) override;
			edm::EDGetTokenT<View<flashgg::Photon> > PhotonToken_; 	
			std::vector<shared_ptr<BaseSystMethods> > Corrections_;
			std::vector<std::vector<double> > sigmas_;
			unsigned int nOutputsNonCentral_;

			void ApplyCorrections(flashgg::Photon & y, shared_ptr<BaseSystMethods> CorrToShift, double syst_shift);
	  //			std::vector<std::vector<std::string> > labels_;

	  //			unsigned int Index2Np1(unsigned int indN, unsigned int indSigma);
	  //			unsigned int IndexSigma(unsigned int ind2Np1);
	  //	  		unsigned int IndexN(unsigned int ind2Np1);

	};

	PhotonSystematicProducer::PhotonSystematicProducer(const ParameterSet & iConfig) :
		PhotonToken_(consumes<View<flashgg::Photon> >(iConfig.getUntrackedParameter<InputTag>("PhotonTag", InputTag("flashggPhotons"))))
	{
		produces<std::vector<flashgg::Photon> >("Central"); // Central value
		std::vector<edm::ParameterSet> vpset = iConfig.getParameter<std::vector<edm::ParameterSet> >("SystMethodNames");

		std::vector<double> default_sigmas; 
		default_sigmas.push_back(-1.); 
		default_sigmas.push_back(1.);

		// TODO Set size of Corrections_ here!!! (is labels_ needed too???)
		Corrections_.resize(vpset.size());

		nOutputsNonCentral_ = 0;
		unsigned int ipset = 0;
		for( const auto& pset : vpset ) {
			std::string methodName = pset.getParameter<string>("MethodName");
			sigmas_.push_back(pset.getUntrackedParameter<vector<double> >("NSigmas",default_sigmas));
			std::vector<double> nsigmas = pset.getUntrackedParameter<vector<double> >("NSigmas",default_sigmas);
			nsigmas.erase(std::remove(nsigmas.begin(), nsigmas.end(), 0.), nsigmas.end()); // remove 0 values
			// TODO remove 0 if present
			sigmas_.push_back(nsigmas);
			Corrections_.at(ipset).reset(FlashggSystematicPhotonMethodsFactory::get()->create(methodName,vpset[s]));
			for (const auto &sig : sigmas_.at(ipset)) {
				if (sig < 0.) {
					std::string label = Form("%sUp%.2d_sigma",methodName.c_str(),sig);
				} else {
					std::string label = Form("%sDown%.2d_sigma",methodName.c_str(),sig);
				}
				produces<vector<flashgg::Photon> >(label);
				nOutputsNonCentral_++;
			}
			ipset++;
		}
	}
  
	///fucntion takes in the current corection one is looping through and compares with its own internal loop, given that this will be within the corr and sys loop it takes care of the 2n+1 collection number////

	void PhotonSystematicProducer::ApplyCorrections(flashgg::Photon & y, shared_ptr<BaseSystMethods> CorrToShift, double syst_shift)
	{
		for( unsigned int shift = 0; shift < Corrections_.size(); shift++){
			if(CorrToShift == Corrections_.at(shift)){
				CorrToShift->applyCorrection(y,syst_shift);	
			}else{
				CorrToShift->applyCorrection(y,0.);
			}
		}
	}

	void PhotonSystematicProducer::produce( Event & evt, const EventSetup & ) {

		Handle<View<flashgg::Photon> > photons;
		evt.getByToken(PhotonToken_,photons);
		const PtrVector<flashgg::Photon>& photonPointers = photons->ptrVector();

		std::vector<std::vector<flashgg::Photon> > all_collections;
		all_collections.resize(nOutputsNonCentral_);
		
		int photon_size = photonPointers.size();
`
		// Build central collection
		auto_ptr<vector<flashgg::Photon> > centralPhotonColl(new vector<flashgg::Photon>);
		for(int i = 0; i < photon_size; i++) {
			for (unsigned int ncorr = 0 ; ncorr < Corrections_.size() ; ncorr++) {
				flashgg::Photon pho = flashgg::Photon(*photonPointers[i]);
				ApplyCorrections(pho, Corrections_.at(ncorr), 0.);
				centralPhotonColl->push_back(pho);
			}
		}
		event.put(centralPhotonColl); // put central collection in event
		      
		// build shifted collections
		for(int i = 0; i < photon_size; i++) {
			unsigned int ncoll = 0;  
			for (unsigned int ncorr = 0 ; ncorr < Corrections_.size() ; ncorr++) {
				for (  const auto & sig : sigmas_at(ncorr)) {
					flashgg::Photon pho = flashgg::Photon(*photonPointers[i]);
					ApplyCorrections(pho, Corrections_.at(ncorr), sig);
					all_collections[ncoll++].push_back(pho);
				}
			}
		}

		for (unsigned int ncoll = 0 ; ncoll < all_collections.size() ; ncoll++) {
			auto_ptr<vector<flashgg::Photon> > nonCentralPhotonColl (all_collections[ncoll]);
			event.put(nonCentralPhotonColl)
		}
	} // event of event
}


typedef flashgg::PhotonSystematicProducer FlashggPhotonSystematicProducer;
DEFINE_FWK_MODULE(FlashggPhotonSystematicProducer);
