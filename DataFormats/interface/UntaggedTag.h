#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"

namespace flashgg {

    class UntaggedTag: public DiPhotonTagBase
    {
    public:
        UntaggedTag();
        ~UntaggedTag();

        UntaggedTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult );
        UntaggedTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );
        virtual UntaggedTag *clone() const;


    private:



    };
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

