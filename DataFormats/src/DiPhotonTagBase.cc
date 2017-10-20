#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

using namespace flashgg;

DiPhotonTagBase::DiPhotonTagBase()
{
    category_number_ = -1;
    isGold_ = -1;
}

DiPhotonTagBase::DiPhotonTagBase( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes )
    : DiPhotonTagBase::DiPhotonTagBase( diPho, *mvaRes ) {}

DiPhotonTagBase::DiPhotonTagBase( edm::Ptr<flashgg::DiPhotonCandidate> diPho, DiPhotonMVAResult mvaRes )
{
    mva_result_ = mvaRes;
    category_number_ = -1;
    dipho_ = diPho;
    isGold_ = -1;
    stage1KinematicLabel_ = "";
}

DiPhotonTagBase::~DiPhotonTagBase()
{
}


bool DiPhotonTagBase::operator <( const DiPhotonTagBase &b ) const
{
    // For choosing which of two tags OF THE SAME TYPE are of higher priority
    // Comparison of different tags not currently supported - is it ever needed?
    // Overloading may be appropriate if different tags have different priorities

    if( categoryNumber() == b.categoryNumber() ) {
        return ( sumPt() < b.sumPt() );
    } else {
        return ( categoryNumber() < b.categoryNumber() );
    }
}

void DiPhotonTagBase::setIsGold( int runNumber ) {
    // Below is the subtraction between
    // https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_Silver_v2.txt
    // and
    // https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt
    //
    // They were last changed on 18 December and still the most recent (as of 28 January)
    // I have confirmed that none of these runs are in the Gold at all, so checking runNumber without lumiSection suffices
    // Of course the result may be wrong for events not even in the Silver JSON
    // I am hard coding this to make dumping and comparing events faster, because this issue now needs intensive study
    //
    /*
      {"256729": [[1, 331], [346, 598], [600, 755], [758, 760], [765, 1165], [1167, 1292], [1295, 1327], [1329, 1732]],
 "256734": [[1, 57], [60, 213]],
 "257394": [[41, 72]],
 "257395": [[1, 13]],
 "257396": [[1, 216]],
 "257397": [[1, 119]],
 "257399": [[1, 271]],
 "257400": [[1, 291], [295, 819], [1011, 1418]],
 "257487": [[50, 102], [104, 202], [204, 1124]],
 "257490": [[1, 591]],
 "257822": [[1, 719], [721, 1389]],
 "257823": [[1, 171]],
 "258443": [[1, 291]]}
    */
    isGold_ = 1;
    if ( runNumber == 256729 ) { isGold_ = 0; }
    if ( runNumber == 256734 ) { isGold_ = 0; }
    if ( (runNumber >= 257394) && (runNumber <= 257397) ) { isGold_ = 0; }
    if ( (runNumber >= 257399) && (runNumber <=257400)) { isGold_ = 0; }
    if ( runNumber == 257487 ) { isGold_ = 0; }
    if ( runNumber == 257490 ) { isGold_ = 0; }
    if ( (runNumber >= 257822) && (runNumber <=257823)) { isGold_ = 0; }
    if ( runNumber == 258443 ) { isGold_ = 0; }
}

float DiPhotonTagBase::ggHweightCentralised( std::string weightName ) const {
    float weightCentral  = centralWeight();
    float weightOriginal = tagTruth()->HTXSggHweight( weightName );
    return weightCentral * weightOriginal;
}

void DiPhotonTagBase::computeStage1Kinematics( const edm::Handle<edm::View<flashgg::Jet> >& jets, float ptV ) {
    stage1KinematicLabel_ = "LOGICERROR";
    float ptH = this->diPhoton()->pt();
    unsigned int nJ = 0;
    float dEta = 0.;
    float mjj = 0.;
    float ptHjj = 0.;
    for ( unsigned int i = 0 ; i < jets->size(); i++ ) {
        if ( jets->ptrAt(i)->pt() > 30. ) nJ++;
    }
    if ( nJ >= 2 ) {
        dEta = fabs( jets->ptrAt(0)->eta() - jets->ptrAt(1)->eta() );
        mjj = ( jets->ptrAt(0)->p4() + jets->ptrAt(1)->p4() ).mass();
        ptHjj = ( jets->ptrAt(0)->p4() + jets->ptrAt(1)->p4() + this->diPhoton()->p4() ).pt();
    }
    if ( ptV < -0.5 ) {
        if (nJ == 0) {
            stage1KinematicLabel_ = "RECO_0J";
        } else if ( nJ == 1 ) {
            if ( ptH > 200 ) {
                stage1KinematicLabel_ = "RECO_1J_PTH_GT200";
            } else if ( ptH > 120. ) {
                stage1KinematicLabel_ = "RECO_1J_PTH_120_200";
            } else if ( ptH > 60. ) {
                stage1KinematicLabel_ = "RECO_1J_PTH_60_120";
            } else {
                stage1KinematicLabel_ = "RECO_1J_PTH_0_60";
            }
        } else { // 2 jets
            if ( ptH > 200 ) {
                stage1KinematicLabel_ ="RECO_1J_PTH_GT200";
            } else if ( mjj > 400. && dEta > 2.8 ) {
                if ( ptHjj < 25. ) {
                    stage1KinematicLabel_ = "RECO_VBFTOPO_JET3VETO";
                } else {
                    stage1KinematicLabel_ = "RECO_VBFTOPO_JET3";
                }
            } else if ( ptH > 120. ) {
                stage1KinematicLabel_ = "RECO_GE2J_PTH_120_200";
            } else if ( ptH > 60. ) {
                stage1KinematicLabel_ = "RECO_GE2J_PTH_60_120";
            } else {
                stage1KinematicLabel_ = "RECO_GE2J_PTH_0_60";
            }
        }
    } else { // Leptonic vector boson assigned
        if ( ptV <  150. ) {
            stage1KinematicLabel_ = "RECOLEP_PTV_0_150";
        } else if ( ptV < 250. ) {
            if ( nJ >= 1 ) {
                stage1KinematicLabel_ = "RECOLEP_PTV_150_250_GE1J";
            } else {
                stage1KinematicLabel_ = "RECOLEP_PTV_150_250_0J";
            }
        } else {
            stage1KinematicLabel_ = "RECOLEP_PTV_GT250";
        }
    }
}


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

