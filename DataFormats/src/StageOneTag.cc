#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/StageOneTag.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

using namespace flashgg;

StageOneTag::StageOneTag() {}

StageOneTag::~StageOneTag() {}

StageOneTag::StageOneTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes ) :
    StageOneTag::StageOneTag( diPho, *mvaRes ) {}

StageOneTag::StageOneTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares ) :
    DiPhotonTagBase::DiPhotonTagBase( dipho, mvares ) {
}

StageOneTag::StageOneTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes, edm::Ptr<VBFDiPhoDiJetMVAResult> vbfDiPhoDiJet_mvaRes ) :
    StageOneTag::StageOneTag( diPho, *mvaRes, *vbfDiPhoDiJet_mvaRes ) {}

StageOneTag::StageOneTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares, VBFDiPhoDiJetMVAResult vbfDiPhoDiJet_mvaRes ) :
    StageOneTag::StageOneTag( dipho, mvares )
{
    vbfDiPhoDiJet_mva_result_ = vbfDiPhoDiJet_mvaRes;
}

const VBFDiPhoDiJetMVAResult StageOneTag::VBFDiPhoDiJetMVA() const
{
    return vbfDiPhoDiJet_mva_result_;
}
const VBFMVAResult StageOneTag::VBFMVA() const
{
    return vbfDiPhoDiJet_mva_result_.vbfMvaResult;
}

void StageOneTag::computeStage1Kinematics( const edm::Handle<edm::View<flashgg::Jet> >& jets, float ptV, float lepeta1, float lepphi1, float lepeta2, float lepphi2 ) {
    stage1recoTag_ = stage1recoTag::LOGICERROR;
    float ptH = this->diPhoton()->pt();
    unsigned int nJ = 0;
    float dEta = 0.;
    float mjj = 0.;
    float ptHjj = 0.;
    edm::Ptr<flashgg::Jet> j0;
    edm::Ptr<flashgg::Jet> j1;
    for ( unsigned int i = 0 ; i < jets->size(); i++ ) {
        edm::Ptr<flashgg::Jet> jet = jets->ptrAt(i);

        //        std::cout << " Jet " << i << " pt=" << jet->pt() << " eta=" << jet->eta() << std::endl;                                                                                                                                    

        if ( jet->pt() < 30. ) continue;
        if ( fabs(jet->eta()) > 4.7 ) continue;
        bool _useJetID = true; // temporary                                                                                                                                                                                                  
        std::string _JetIDLevel = "Tight"; // temporary                                                                                                                                                                                      
        float _drJetPhoton = 0.4; // Temporary                                                                                                                                                                                               
        float _drJetLepton = 0.4; // Temporary                                                                                                                                                                                               
        // Temporary: No PU JET ID                                                                                                                                                                                                           
        if( _useJetID ){
            if( _JetIDLevel == "Loose" && !jet->passesJetID  ( flashgg::Loose ) ) continue;
            if( _JetIDLevel == "Tight" && !jet->passesJetID  ( flashgg::Tight ) ) continue;

            // close to lead photon?
            float dPhi = deltaPhi( jet->phi(), this->diPhoton()->leadingPhoton()->phi() );
            float dEta = jet->eta() - this->diPhoton()->leadingPhoton()->eta();
            if( sqrt( dPhi * dPhi + dEta * dEta ) < _drJetPhoton ) { continue; }

            // close to sublead photon?
            dPhi = deltaPhi( jet->phi(), this->diPhoton()->subLeadingPhoton()->phi() );
            dEta = jet->eta() - this->diPhoton()->subLeadingPhoton()->eta();
            if( sqrt( dPhi * dPhi + dEta * dEta ) < _drJetPhoton ) { continue; }

            // close to lepton1 (if any)                                                                                                                                                                                                         
            dPhi = deltaPhi( jet->phi(), lepphi1 );
            dEta = jet->eta() - lepeta1;
            if( sqrt( dPhi * dPhi + dEta * dEta ) < _drJetLepton ) { continue; }

            // close to lepton2 (if any)                                                                                                                                                                                                         
            dPhi = deltaPhi( jet->phi(), lepphi2 );
            dEta = jet->eta() - lepeta2;
            if( sqrt( dPhi * dPhi + dEta * dEta ) < _drJetLepton ) { continue; }

            nJ++;

            if ( j0.isNull() ) {
                //            std::cout << " Save jet " << i << " as j0" << std::endl;                                                                                                                                                           
                j0 = jet;
            } else if ( j1.isNull() ) {
                //            std::cout << " Save jet " << i << " as j1" << std::endl;                                                                                                                                                           
                j1 = jet;
            } else {
                //            std::cout << " Not saving jet " << i << " - two jets already " << std::endl;                                                                                                                                       
            }
        }
        //    std::cout << " nJ=" << nJ << " ptV=" << ptV << " ptH=" << ptH << std::endl;                                                                                                                                                        

        unsigned nlep = 0;
        if (lepphi1 > -998. ) nlep++;
        if (lepphi2 > -998. ) nlep++;
        string nlepstring = std::to_string(nlep)+"LEP";

        if ( nJ >= 2 ) {
            dEta = fabs( j0->eta() - j1->eta() );
            mjj = ( j0->p4() + j1->p4() ).mass();
            ptHjj = ( j0->p4() + j1->p4() + this->diPhoton()->p4() ).pt();
            //        std::cout << " dEta=" << dEta << " mjj=" << mjj << " ptHjj=" << ptHjj << std::endl;                                                                                                                                        
        }
        if ( ptV < -0.5 ) {
            if (nJ == 0) {
                stage1recoTag_ = stage1recoTag::RECO_0J;
            } else if ( nJ == 1 ) {
                if ( ptH > 200 ) {
                    stage1recoTag_ = stage1recoTag::RECO_1J_PTH_GT200;
                } else if ( ptH > 120. ) {
                    stage1recoTag_ = stage1recoTag::RECO_1J_PTH_120_200;
                } else if ( ptH > 60. ) {
                    stage1recoTag_ = stage1recoTag::RECO_1J_PTH_60_120;
                } else {
                    stage1recoTag_ = stage1recoTag::RECO_1J_PTH_0_60;
                }
            } else { // 2 jets
                if ( ptH > 200 ) {
                    stage1recoTag_ = stage1recoTag::RECO_GE2J_PTH_GT200;
                } else if ( mjj > 400. && dEta > 2.8 ) {
                    if ( ptHjj < 25. ) {
                        stage1recoTag_ = stage1recoTag::RECO_VBFTOPO_JET3VETO;
                    } else {
                        stage1recoTag_ = stage1recoTag::RECO_VBFTOPO_JET3;
                    }
                } else if ( mjj > 60. && mjj < 120. ) {
                    stage1recoTag_ = stage1recoTag::RECO_VH2JET;
                } else if ( ptH > 120. ) {
                    stage1recoTag_ = stage1recoTag::RECO_GE2J_PTH_120_200;
                } else if ( ptH > 60. ) {
                    stage1recoTag_ = stage1recoTag::RECO_GE2J_PTH_60_120;
                } else {
                    stage1recoTag_ = stage1recoTag::RECO_GE2J_PTH_0_60;
                }
            }
        } else { // Leptonic vector boson assigned                                                                                                                                                                                               
            if ( ptV <  150. ) {
                if(nlep == 0) stage1recoTag_ = stage1recoTag::RECO_0LEP_PTV_0_150;
                else if(nlep == 1) stage1recoTag_ = stage1recoTag::RECO_1LEP_PTV_0_150;
                else if(nlep == 2) stage1recoTag_ = stage1recoTag::RECO_2LEP_PTV_0_150;
            } else if ( ptV < 250. ) {
                if ( nJ >= 1 ) {
                    if(nlep == 0) stage1recoTag_ = stage1recoTag::RECO_0LEP_PTV_150_250_GE1J;
                    else if(nlep == 1) stage1recoTag_ = stage1recoTag::RECO_1LEP_PTV_150_250_GE1J;
                    else if(nlep == 2) stage1recoTag_ = stage1recoTag::RECO_2LEP_PTV_150_250_GE1J;
                } else {
                    if(nlep == 0) stage1recoTag_ = stage1recoTag::RECO_0LEP_PTV_150_250_0J;
                    else if(nlep == 1) stage1recoTag_ = stage1recoTag::RECO_1LEP_PTV_150_250_0J;
                    else if(nlep == 2) stage1recoTag_ = stage1recoTag::RECO_2LEP_PTV_150_250_0J;
                }
            } else {
                if(nlep == 0) stage1recoTag_ = stage1recoTag::RECO_0LEP_PTV_GT250;
                else if(nlep == 1) stage1recoTag_ = stage1recoTag::RECO_1LEP_PTV_GT250;
                else if(nlep == 2) stage1recoTag_ = stage1recoTag::RECO_2LEP_PTV_GT250;
            }
        }
        //    std::cout << " Final label " << stage1KinematicLabel_ << std::endl;                                                                                                                                                                
    }
}

string StageOneTag::stage1KinematicLabel() const { 
    enum stage1recoTag { LOGICERROR = -1, NOTAG = 0, RECO_0J, RECO_1J_PTH_0_60, RECO_1J_PTH_60_120, RECO_1J_PTH_120_200, RECO_1J_PTH_GT200,
                         RECO_GE2J_PTH_0_60, RECO_GE2J_PTH_60_120, RECO_GE2J_PTH_120_200, RECO_GE2J_PTH_GT200, RECO_VBFTOPO_JET3VETO, RECO_VBFTOPO_JET3, RECO_VH2JET,
                         RECO_0LEP_PTV_0_150, RECO_0LEP_PTV_150_250_0J, RECO_0LEP_PTV_150_250_GE1J, RECO_0LEP_PTV_GT250,
                         RECO_1LEP_PTV_0_150, RECO_1LEP_PTV_150_250_0J, RECO_1LEP_PTV_150_250_GE1J, RECO_1LEP_PTV_GT250,
                         RECO_2LEP_PTV_0_150, RECO_2LEP_PTV_150_250_0J, RECO_2LEP_PTV_150_250_GE1J, RECO_2LEP_PTV_GT250,
                         RECO_TTH_LEP, RECO_TTH_HAD };

    switch(stage1recoTag_) {
    case stage1recoTag::LOGICERROR:
        return string("LOGICERROR");
    case stage1recoTag::NOTAG:
        return string("NOTAG");
    case stage1recoTag::RECO_0J:
        return string("RECO_0J");
    case stage1recoTag::RECO_1J_PTH_0_60:
        return string("RECO_1J_PTH_0_60");
    case stage1recoTag::RECO_1J_PTH_60_120:
        return string("RECO_1J_PTH_60_120");
    case stage1recoTag::RECO_1J_PTH_120_200:
        return string("RECO_1J_PTH_120_200");
    case stage1recoTag::RECO_1J_PTH_GT200:
        return string("RECO_1J_PTH_GT200");
    case stage1recoTag::RECO_GE2J_PTH_0_60:
        return string("RECO_GE2J_PTH_0_60");
    case stage1recoTag::RECO_GE2J_PTH_60_120:
        return string("RECO_GE2J_PTH_60_120");
    case stage1recoTag::RECO_GE2J_PTH_120_200:
        return string("RECO_GE2J_PTH_120_200");
    case stage1recoTag::RECO_GE2J_PTH_GT200:
        return string("RECO_GE2J_PTH_GT200");
    case stage1recoTag::RECO_VBFTOPO_JET3VETO:
        return string("RECO_VBFTOPO_JET3VETO");
    case stage1recoTag::RECO_VBFTOPO_JET3:
        return string("RECO_VBFTOPO_JET3");
    case stage1recoTag::RECO_VH2JET:
        return string("RECO_VH2JET");
    case stage1recoTag::RECO_0LEP_PTV_0_150:
        return string("RECO_0LEP_PTV_0_150");
    case stage1recoTag::RECO_0LEP_PTV_150_250_0J:
        return string("RECO_0LEP_PTV_150_250_0J");
    case stage1recoTag::RECO_0LEP_PTV_150_250_GE1J:
        return string("RECO_0LEP_PTV_150_250_GE1J");
    case stage1recoTag::RECO_0LEP_PTV_GT250:
        return string("RECO_0LEP_PTV_GT250");
    case stage1recoTag::RECO_1LEP_PTV_0_150:
        return string("RECO_1LEP_PTV_0_150");
    case stage1recoTag::RECO_1LEP_PTV_150_250_0J:
        return string("RECO_1LEP_PTV_150_250_0J");
    case stage1recoTag::RECO_1LEP_PTV_150_250_GE1J:
        return string("RECO_1LEP_PTV_150_250_GE1J");
    case stage1recoTag::RECO_1LEP_PTV_GT250:
        return string("RECO_1LEP_PTV_GT250");
    case stage1recoTag::RECO_2LEP_PTV_0_150:
        return string("RECO_2LEP_PTV_0_150");
    case stage1recoTag::RECO_2LEP_PTV_150_250_0J:
        return string("RECO_2LEP_PTV_150_250_0J");
    case stage1recoTag::RECO_2LEP_PTV_150_250_GE1J:
        return string("RECO_2LEP_PTV_150_250_GE1J");
    case stage1recoTag::RECO_2LEP_PTV_GT250:
        return string("RECO_2LEP_PTV_GT250");
    case stage1recoTag::RECO_TTH_LEP:
        return string("RECO_TTH_LEP");
    case stage1recoTag::RECO_TTH_HAD:
        return string("RECO_TTH_HAD");
    default:
        break;
    }
    return string("TAG NOT ON LIST");
}


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

