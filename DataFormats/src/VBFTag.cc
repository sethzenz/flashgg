#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/VBFTag.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

using namespace flashgg;

VBFTag::VBFTag() {}

VBFTag::~VBFTag() {}

VBFTag::VBFTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes, edm::Ptr<VBFDiPhoDiJetMVAResult> vbfDiPhoDiJet_mvaRes ) :
    VBFTag::VBFTag( diPho, *mvaRes, *vbfDiPhoDiJet_mvaRes ) {}

VBFTag::VBFTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares, VBFDiPhoDiJetMVAResult vbfDiPhoDiJet_mvaRes ) :
    DiPhotonTagBase::DiPhotonTagBase( dipho, mvares )
{
    vbfDiPhoDiJet_mva_result_ = vbfDiPhoDiJet_mvaRes;
}


const VBFDiPhoDiJetMVAResult VBFTag::VBFDiPhoDiJetMVA() const
{
    return vbfDiPhoDiJet_mva_result_;
}
const VBFMVAResult VBFTag::VBFMVA() const
{
    return vbfDiPhoDiJet_mva_result_.vbfMvaResult;
}

const reco::Candidate::LorentzVector VBFTag::leadingJet() const
{
    return vbfDiPhoDiJet_mva_result_.vbfMvaResult.leadJet;
}

const reco::Candidate::LorentzVector  VBFTag::subLeadingJet() const
{
    return vbfDiPhoDiJet_mva_result_.vbfMvaResult.subleadJet;
}

const reco::Candidate::LorentzVector  VBFTag::subSubLeadingJet() const
{
    //! adding a third jets for the VBF studies
    return vbfDiPhoDiJet_mva_result_.vbfMvaResult.subsubleadJet;
}

const edm::Ptr<Jet> VBFTag::leadingJet_ptr() const
{
    return vbfDiPhoDiJet_mva_result_.vbfMvaResult.leadJet_ptr;
}

const edm::Ptr<Jet> VBFTag::subLeadingJet_ptr() const
{
    return vbfDiPhoDiJet_mva_result_.vbfMvaResult.subleadJet_ptr;
}

const edm::Ptr<Jet> VBFTag::subSubLeadingJet_ptr() const
{
    //! adding a third jets for the VBF studies
    return vbfDiPhoDiJet_mva_result_.vbfMvaResult.subsubleadJet_ptr;
}

const bool VBFTag::hasValidVBFTriJet() const
{
    return vbfDiPhoDiJet_mva_result_.vbfMvaResult.hasValidVBFTriJet;
}

const float VBFTag::jetEtaCorrectionWeight() const 
{
    if (!hasValidVBFDiJet() ) return 1.;

    float pt1 = leadingJet_ptr()->pt();
    float eta1 = leadingJet_ptr()->eta();
    float pt2 = subLeadingJet_ptr()->pt();
    float eta2 = subLeadingJet_ptr()->eta();

    float weight = 1.;
    if (pt1 > 30.0 && pt1 < 40.0 && fabs(eta1) > 0.0 && fabs(eta1) < 2.5) weight *= 1.00;
    if (pt1 > 30.0 && pt1 < 40.0 && fabs(eta1) > 2.5 && fabs(eta1) < 2.8) weight *= 0.85;
    if (pt1 > 30.0 && pt1 < 40.0 && fabs(eta1) > 2.8 && fabs(eta1) < 3.1) weight *= 0.54;
    if (pt1 > 30.0 && pt1 < 40.0 && fabs(eta1) > 3.1 && fabs(eta1) < 3.4) weight *= 0.71;
    if (pt1 > 30.0 && pt1 < 40.0 && fabs(eta1) > 3.4 && fabs(eta1) < 3.7) weight *= 0.96;
    if (pt1 > 30.0 && pt1 < 40.0 && fabs(eta1) > 3.7 && fabs(eta1) < 4.0) weight *= 0.75;
    if (pt1 > 30.0 && pt1 < 40.0 && fabs(eta1) > 4.0 && fabs(eta1) < 4.7) weight *= 0.62;
    if (pt1 > 40.0 && pt1 < 50.0 && fabs(eta1) > 0.0 && fabs(eta1) < 2.5) weight *= 1.00;
    if (pt1 > 40.0 && pt1 < 50.0 && fabs(eta1) > 2.5 && fabs(eta1) < 2.8) weight *= 0.88;
    if (pt1 > 40.0 && pt1 < 50.0 && fabs(eta1) > 2.8 && fabs(eta1) < 3.1) weight *= 0.64;
    if (pt1 > 40.0 && pt1 < 50.0 && fabs(eta1) > 3.1 && fabs(eta1) < 3.4) weight *= 0.67;
    if (pt1 > 40.0 && pt1 < 50.0 && fabs(eta1) > 3.4 && fabs(eta1) < 3.7) weight *= 0.86;
    if (pt1 > 40.0 && pt1 < 50.0 && fabs(eta1) > 3.7 && fabs(eta1) < 4.0) weight *= 0.77;
    if (pt1 > 40.0 && pt1 < 50.0 && fabs(eta1) > 4.0 && fabs(eta1) < 4.7) weight *= 0.56;
    if (pt1 > 50.0 && pt1 < 999999.0 && fabs(eta1) > 0.0 && fabs(eta1) < 2.5) weight *= 1.00;
    if (pt1 > 50.0 && pt1 < 999999.0 && fabs(eta1) > 2.5 && fabs(eta1) < 2.8) weight *= 0.77;
    if (pt1 > 50.0 && pt1 < 999999.0 && fabs(eta1) > 2.8 && fabs(eta1) < 3.1) weight *= 0.46;
    if (pt1 > 50.0 && pt1 < 999999.0 && fabs(eta1) > 3.1 && fabs(eta1) < 3.4) weight *= 0.84;
    if (pt1 > 50.0 && pt1 < 999999.0 && fabs(eta1) > 3.4 && fabs(eta1) < 3.7) weight *= 0.92;
    if (pt1 > 50.0 && pt1 < 999999.0 && fabs(eta1) > 3.7 && fabs(eta1) < 4.0) weight *= 0.84;
    if (pt1 > 50.0 && pt1 < 999999.0 && fabs(eta1) > 4.0 && fabs(eta1) < 4.7) weight *= 0.53;

    if (pt2 > 20.0 && pt2 < 25.0 && fabs(eta2) > 0.0 && fabs(eta2) < 2.5) weight *= 1.00;
    if (pt2 > 20.0 && pt2 < 25.0 && fabs(eta2) > 2.5 && fabs(eta2) < 2.8) weight *= 0.70;
    if (pt2 > 20.0 && pt2 < 25.0 && fabs(eta2) > 2.8 && fabs(eta2) < 3.1) weight *= 0.48;
    if (pt2 > 20.0 && pt2 < 25.0 && fabs(eta2) > 3.1 && fabs(eta2) < 3.4) weight *= 0.56;
    if (pt2 > 20.0 && pt2 < 25.0 && fabs(eta2) > 3.4 && fabs(eta2) < 3.7) weight *= 0.73;
    if (pt2 > 20.0 && pt2 < 25.0 && fabs(eta2) > 3.7 && fabs(eta2) < 4.0) weight *= 0.64;
    if (pt2 > 20.0 && pt2 < 25.0 && fabs(eta2) > 4.0 && fabs(eta2) < 4.7) weight *= 0.56;
    if (pt2 > 25.0 && pt2 < 30.0 && fabs(eta2) > 0.0 && fabs(eta2) < 2.5) weight *= 1.00;
    if (pt2 > 25.0 && pt2 < 30.0 && fabs(eta2) > 2.5 && fabs(eta2) < 2.8) weight *= 0.77;
    if (pt2 > 25.0 && pt2 < 30.0 && fabs(eta2) > 2.8 && fabs(eta2) < 3.1) weight *= 0.55;
    if (pt2 > 25.0 && pt2 < 30.0 && fabs(eta2) > 3.1 && fabs(eta2) < 3.4) weight *= 0.59;
    if (pt2 > 25.0 && pt2 < 30.0 && fabs(eta2) > 3.4 && fabs(eta2) < 3.7) weight *= 0.80;
    if (pt2 > 25.0 && pt2 < 30.0 && fabs(eta2) > 3.7 && fabs(eta2) < 4.0) weight *= 0.66;
    if (pt2 > 25.0 && pt2 < 30.0 && fabs(eta2) > 4.0 && fabs(eta2) < 4.7) weight *= 0.68;
    if (pt2 > 30.0 && pt2 < 40.0 && fabs(eta2) > 0.0 && fabs(eta2) < 2.5) weight *= 1.00;
    if (pt2 > 30.0 && pt2 < 40.0 && fabs(eta2) > 2.5 && fabs(eta2) < 2.8) weight *= 0.92;
    if (pt2 > 30.0 && pt2 < 40.0 && fabs(eta2) > 2.8 && fabs(eta2) < 3.1) weight *= 0.63;
    if (pt2 > 30.0 && pt2 < 40.0 && fabs(eta2) > 3.1 && fabs(eta2) < 3.4) weight *= 0.69;
    if (pt2 > 30.0 && pt2 < 40.0 && fabs(eta2) > 3.4 && fabs(eta2) < 3.7) weight *= 0.94;
    if (pt2 > 30.0 && pt2 < 40.0 && fabs(eta2) > 3.7 && fabs(eta2) < 4.0) weight *= 0.78;
    if (pt2 > 30.0 && pt2 < 40.0 && fabs(eta2) > 4.0 && fabs(eta2) < 4.7) weight *= 0.73;
    if (pt2 > 40.0 && pt2 < 50.0 && fabs(eta2) > 0.0 && fabs(eta2) < 2.5) weight *= 1.00;
    if (pt2 > 40.0 && pt2 < 50.0 && fabs(eta2) > 2.5 && fabs(eta2) < 2.8) weight *= 0.93;
    if (pt2 > 40.0 && pt2 < 50.0 && fabs(eta2) > 2.8 && fabs(eta2) < 3.1) weight *= 0.67;
    if (pt2 > 40.0 && pt2 < 50.0 && fabs(eta2) > 3.1 && fabs(eta2) < 3.4) weight *= 0.86;
    if (pt2 > 40.0 && pt2 < 50.0 && fabs(eta2) > 3.4 && fabs(eta2) < 3.7) weight *= 0.75;
    if (pt2 > 40.0 && pt2 < 50.0 && fabs(eta2) > 3.7 && fabs(eta2) < 4.0) weight *= 0.94;
    if (pt2 > 40.0 && pt2 < 50.0 && fabs(eta2) > 4.0 && fabs(eta2) < 4.7) weight *= 0.63;
    if (pt2 > 50.0 && pt2 < 999999.0 && fabs(eta2) > 0.0 && fabs(eta2) < 2.5) weight *= 1.00;
    if (pt2 > 50.0 && pt2 < 999999.0 && fabs(eta2) > 2.5 && fabs(eta2) < 2.8) weight *= 0.89;
    if (pt2 > 50.0 && pt2 < 999999.0 && fabs(eta2) > 2.8 && fabs(eta2) < 3.1) weight *= 0.58;
    if (pt2 > 50.0 && pt2 < 999999.0 && fabs(eta2) > 3.1 && fabs(eta2) < 3.4) weight *= 0.94;
    if (pt2 > 50.0 && pt2 < 999999.0 && fabs(eta2) > 3.4 && fabs(eta2) < 3.7) weight *= 1.05;
    if (pt2 > 50.0 && pt2 < 999999.0 && fabs(eta2) > 3.7 && fabs(eta2) < 4.0) weight *= 0.85;
    if (pt2 > 50.0 && pt2 < 999999.0 && fabs(eta2) > 4.0 && fabs(eta2) < 4.7) weight *= 0.68;

    return weight;
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

