#include "flashgg/SystAlgos/interface/BaseSystMethods.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"

namespace flashgg {

	//	class SimplePhotonSmear: public BaseSystMethods<flashgg::Photon,float> {
	class SimplePhotonSmear: public BaseSystMethods {


		public:
			SimplePhotonSmear(const edm::ParameterSet& conf):
				BaseSystMethods(conf){}
			//	SimplePhotonSmear();


			void applyCorrection( flashgg::Photon & y, float syst_shift) override;
	};

	void SimplePhotonSmear::applyCorrection( flashgg::Photon & y, float syst_shift)
	{
		if(syst_shift == 0. ){
			
			std::cout << "Nominal correction" << std::endl;

		}else{
			std::cout <<"momentum before correction  " << y.pt() << std::endl; 
			y.updateEnergy("smearedEnergy",y.energy()+syst_shift);
			std::cout << "corrected momentum value  " << y.pt() << std::endl;
		}
	}
}

DEFINE_EDM_PLUGIN(FlashggSystematicPhotonMethodsFactory,
		flashgg::SimplePhotonSmear,
		"FlashggSimplePhotonSmear");	
