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
			//std::vector<BaseSystMethods*> Corrections_;
	};

	PhotonSystematicProducer::PhotonSystematicProducer(const ParameterSet & iConfig) :

		PhotonToken_(consumes<View<flashgg::Photon> >(iConfig.getUntrackedParameter<InputTag>("PhotonTag", InputTag("flashggPhotons"))))
		{
			//const std::vector<std::string>& SystematicMethod = iConfig.getParameter<std::vector<string> >("SystMethodName");

			std::vector<edm::ParameterSet> vpset = iConfig.getParameter<std::vector<edm::ParameterSet> >("SystMethodNames");

			int size = vpset.size();

			std::vector<std::string> Names;

			for(std::vector<edm::ParameterSet>::const_iterator it = vpset.begin(); it < vpset.end() ; it++){


				std::string Name =  it->getParameter<std::string>("MethodName");
					
				Names.push_back(Name);	

			}


			for(int s = 0; s < size ; s++){

				//	std::string name = vpset.at(s).getUntrackedParameter<std::string>("name");//problem here at runtime, seems I am not actually doing this right. This parameter is needed for the constructor in the Base Class, BaseSystMethods.h line # 18

				//	names.push_back(name);
				//
				//
				std::string& MethodName = Names.at(s); 

				Corrections_.push_back(NULL);

				std::cout << Corrections_.size() << std::endl; 

				Corrections_.at(s).reset(FlashggSystematicPhotonMethodsFactory::get()->create(MethodName,vpset[s]));

				std::cout << "worked" << std::endl;

				produces<vector<flashgg::Photon> >(Form("collection%i,%i",s,s));

				//			int correction_num = SystematicMethod.size();

				//			for (int i = 0 ; i < correction_num; i++){
				//
				//				Corrections_.at(i).reset(FlashggSystematicPhotonMethodsFactory::get()->create(SystematicMethod.at(i),iConfig));
				//
				//				produces<vector<flashgg::Photon> >(Form("collection%i,%i",i,i));
				//
				//			}
			}
		}

	///fucntion takes in the current corection one is looping through and compares with its own internal loop, given that this will be within the corr and sys loop it takes care of the 2n+1 collection number////

	void ApplyCorrection(flashgg::Photon & y, std::vector<shared_ptr<BaseSystMethods> > Corr, shared_ptr<BaseSystMethods> CorrToShift, double syst_shift, int corr_size){


		for( int shift = 0; shift < corr_size; shift++){

			if(CorrToShift == Corr.at(shift)){

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

		int num_corr = Corrections_.size();

		std::vector<int> syst_shift = {-1,0,1};

		//				std::vector<std::vector<flashgg::Photon>* > collections;

		std::vector<flashgg::Photon> ** collections;////////create 2D array where rows are the # of systematci variations and colums are the # of corrections

		collections = new std::vector<flashgg::Photon> * [3];

		for(int l = 0; l<3; l++){

			collections[l] = new std::vector<flashgg::Photon>[num_corr];
		}


		//uses vector instead of an array, problem when running through the loop, there is no loop with 2n+1 indices to cover the whole vector///

		//				for(int l = 0; l<2*num_corr+1; l++){
		//		
		//					std::vector<flashgg::Photon> * vec = new std::vector<flashgg::Photon>;
		//		
		//					collections.push_back(vec);
		//				}

		int photon_size = photonPointers.size();

		for(int sys = 0; sys < 3 ; sys++){

			for(int corr = 0; corr < num_corr; corr++){

				for(int i = 0; i < photon_size; i++){

					flashgg::Photon pho = *photonPointers[i];
					//				ApplyCorrections(pho, Corrections_, sys , num_corr);
					ApplyCorrection(pho,Corrections_,Corrections_.at(corr),sys,num_corr);
					collections[sys][corr].push_back(pho);
					//	collections.at(i)->push_back(pho);

				}	

			}
		}


		for(int j = 0; j<3;j++){	
			for(int i = 0; i<num_corr; i++){

				auto_ptr<std::vector<flashgg::Photon> > p(new std::vector<flashgg::Photon>);	

				p.reset(&collections[j][i]);//transfers the owner ship of the pointer  "p"  //
				//p.reset(collections.at(i));
				evt.put(p,Form("%i_%i",i,j));////ran out of ideas on how exactly to dynamically name the collections, this doesn't compile///
			}
		}

	}
}


typedef flashgg::PhotonSystematicProducer FlashggPhotonSystematicProducer;
DEFINE_FWK_MODULE(FlashggPhotonSystematicProducer);
