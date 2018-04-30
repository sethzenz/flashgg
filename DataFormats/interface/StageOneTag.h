#ifndef FLASHgg_StageOneTag_h
#define FLASHgg_StageOneTag_h

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/VBFDiPhoDiJetMVAResult.h"
#include "flashgg/DataFormats/interface/Jet.h"

namespace flashgg {
    enum stage1recoTag { LOGICERROR = -1, NOTAG = 0, RECO_0J, RECO_1J_PTH_0_60, RECO_1J_PTH_60_120, RECO_1J_PTH_120_200, RECO_1J_PTH_GT200,
                         RECO_GE2J_PTH_0_60, RECO_GE2J_PTH_60_120, RECO_GE2J_PTH_120_200, RECO_GE2J_PTH_GT200, RECO_VBFTOPO_JET3VETO, RECO_VBFTOPO_JET3, RECO_VH2JET,
                         RECO_0LEP_PTV_0_150, RECO_0LEP_PTV_150_250_0J, RECO_0LEP_PTV_150_250_GE1J, RECO_0LEP_PTV_GT250,
                         RECO_1LEP_PTV_0_150, RECO_1LEP_PTV_150_250_0J, RECO_1LEP_PTV_150_250_GE1J, RECO_1LEP_PTV_GT250,
                         RECO_2LEP_PTV_0_150, RECO_2LEP_PTV_150_250_0J, RECO_2LEP_PTV_150_250_GE1J, RECO_2LEP_PTV_GT250,
                         RECO_TTH_LEP, RECO_TTH_HAD };

    class StageOneTag: public DiPhotonTagBase
    {
    public:
        StageOneTag();
        ~StageOneTag();

        StageOneTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );
        StageOneTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult );
        StageOneTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult>, edm::Ptr<VBFDiPhoDiJetMVAResult> );
        StageOneTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult, VBFDiPhoDiJetMVAResult );

        StageOneTag *clone() const override { return ( new StageOneTag( *this ) ); }

        DiPhotonTagBase::tag_t tagEnum() const override {return DiPhotonTagBase::kStageOne; }

        int stage1recoEnum() const { return stage1recoTag_; }

        void computeStage1Kinematics( const edm::Handle<edm::View<flashgg::Jet> > & jets, float ptV = -1., float lepphi1 = -999., float lepeta1 = -999., float lepphi2 = -999., float lepeta2 = -999. );
        string stage1KinematicLabel() const;
        void setStage1recoTag( const int tag ) { stage1recoTag_ = tag; }

        const VBFDiPhoDiJetMVAResult VBFDiPhoDiJetMVA() const;
        const VBFMVAResult VBFMVA() const ;


    private:
        int stage1recoTag_;
        VBFDiPhoDiJetMVAResult vbfDiPhoDiJet_mva_result_;

    };

}

#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

