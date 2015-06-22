#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

namespace flashgg {

    class PhotonSmearString: public ObjectSystMethodBinnedByFunctor<flashgg::Photon, std::pair<int, int> >
    {

    public:
        typedef StringCutObjectSelector<Photon, true> selector_type;

        PhotonSmearString( const edm::ParameterSet &conf );
        void applyCorrection( flashgg::Photon &y, std::pair<int, int> syst_shift ) override;
        std::string shiftLabel( std::pair<int, int> ) const override;

        const std::string &firstParameterName() const { return label1_; }
        const std::string &secondParameterName() const { return label2_; }

    private:
        selector_type overall_range_;
        const std::string label1_;
        const std::string label2_;
    };

    PhotonSmearString::PhotonSmearString( const edm::ParameterSet &conf ) :
        ObjectSystMethodBinnedByFunctor( conf ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),
        label1_( conf.getUntrackedParameter<std::string>( "FirstParameterName", "Par1" ) ),
        label2_( conf.getUntrackedParameter<std::string>( "SecondParameterName", "Par2" ) )
    {
    }

    std::string PhotonSmearString::shiftLabel( std::pair<int, int> syst_value ) const
    {
        std::string result = label();
        if( syst_value.first == 0 && syst_value.second == 0 ) {
            result += "Central";
        } else {
            if( syst_value.first > 0 ) { result += Form( "%sUp%.2dsigma", firstParameterName().c_str(), syst_value.first ); }
            if( syst_value.first < 0 ) { result += Form( "%sDown%.2dsigma", firstParameterName().c_str(), -1 * syst_value.first ); }
            if( syst_value.second > 0 ) { result += Form( "%sUp%.2dsigma", secondParameterName().c_str(), syst_value.second ); }
            if( syst_value.second < 0 ) { result += Form( "%sDown%.2dsigma", secondParameterName().c_str(), -1 * syst_value.second ); }
        }
        return result;
    }

    void PhotonSmearString::applyCorrection( flashgg::Photon &y, std::pair<int, int> syst_shift )
    {
        if( overall_range_( y ) ) {
            auto val_err = binContents( y );
            if( val_err.first.size() == 2 && val_err.second.size() == 2 ) { // otherwise no-op because we don't have an entry
                float a_val = val_err.first[0];
                float a_err = val_err.second[0];
                float b_val = val_err.first[0];
                float b_err = val_err.second[0];
                if( debug_ ) {
                    //                    std::cout << "  " << shiftLabel( syst_shift ) << ": Photon has pt= " << y.pt() << " eta=" << y.eta()
                    //        << " and we apply a multiplicative correction of " << scale << std::endl;
                    std::cout << "  " << shiftLabel( syst_shift ) << ": Photon has pt= " << y.pt() << " eta=" << y.eta()
                              << " and we are testing the interface so we just note: " << firstParameterName() << " = " << a_val << " +/- " << a_err
                              << " " << secondParameterName() << " = " << b_val << " +/- " << b_err << std::endl;
                }
                //                y.updateEnergy( shiftLabel( syst_shift ), scale * y.energy() );
            }
        }
    }
}

DEFINE_EDM_PLUGIN( FlashggSystematicPhotonMethodsFactory2D,
                   flashgg::PhotonSmearString,
                   "FlashggPhotonSmearString" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

