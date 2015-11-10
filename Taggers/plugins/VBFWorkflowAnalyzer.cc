#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/VBFDiPhoDiJetMVAResult.h"
#include "flashgg/DataFormats/interface/VBFMVAResult.h"
#include "flashgg/DataFormats/interface/VBFTag.h"
#include "flashgg/DataFormats/interface/VBFTagTruth.h"

#include "DataFormats/Common/interface/RefToPtr.h"

#include <vector>
#include <algorithm>

using namespace std;
using namespace edm;

namespace flashgg {

    class VBFWorkflowAnalyzer : public EDProducer
    {

    public:
        typedef math::XYZPoint Point;

        VBFWorkflowAnalyzer( const ParameterSet & );

    private:
        void produce( Event &, const EventSetup & ) override;
        void endJob() override;

        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<View<DiPhotonCandidate> > preselectedDiPhotonToken_;
        EDGetTokenT<View<VBFDiPhoDiJetMVAResult> > vbfDiPhoDiJetMvaResultToken_;
        EDGetTokenT<View<VBFMVAResult> > vbfMvaResultToken_;
        EDGetTokenT<View<VBFMVAResult> > vbfMvaResultTokenOpposite_;
        EDGetTokenT<View<VBFMVAResult> > vbfMvaResultTokenHighest_;
        EDGetTokenT<View<VBFMVAResult> > vbfMvaResultTokenThirdJetHighPt_;
        EDGetTokenT<View<VBFMVAResult> > vbfMvaResultTokenThirdJetMaxMjjj_;
        EDGetTokenT<View<VBFMVAResult> > vbfMvaResultTokenThirdJetMinMjjj_;
        EDGetTokenT<View<VBFMVAResult> > vbfMvaResultTokenThirdJetMinDr_;

        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<reco::GenParticle> > genPartToken_;
        EDGetTokenT<View<reco::GenJet> > genJetToken_;
        
        bool requireAcceptance_;
        bool useGenJets_;
        
        float photonIdCut_;
        float loosePhotonIdCut_;
        unsigned nevt_total;
        unsigned nevt_photon_acc;
        unsigned nevt_dipho;
        unsigned nevt_presel;
        unsigned nevt_leadpt;
        unsigned nevt_bothpt;
        unsigned nevt_mgg;
        unsigned nevt_leadeleveto;
        unsigned nevt_botheleveto;
        unsigned nevt_leadlooseMVA;
        unsigned nevt_bothlooseMVA;
        unsigned nevt_leadMVA;
        unsigned nevt_bothMVA;
        unsigned nevt_jet_acc;
        unsigned nevt_dijet;
        unsigned nevt_tightenpho;
        unsigned nevt_dijetpresel;
        unsigned nevt_dijetmultiple;
        unsigned nevt_dijetallsame;
        unsigned nevt_dijet_correct;
        unsigned nevt_dijet_correctOpposite;
        unsigned nevt_dijet_correctHighest;
        unsigned nevt_dijet_correctThirdJetHighPt;
        unsigned nevt_dijet_correctThirdJetMinDr;
        unsigned nevt_dijet_correctThirdJetMinMjjj;
        unsigned nevt_dijet_correctThirdJetMaxMjjj;
        
        unsigned nevt_j2; 
        unsigned nevt_j2_dijet;
        unsigned nevt_j2_tightenpho;
        unsigned nevt_j2_dijetpresel;
        unsigned nevt_j2_dijetmultiple;
        unsigned nevt_j2_dijetallsame;
        unsigned nevt_j2_dijet_correct;
        unsigned nevt_j2_dijet_correctOpposite;
        unsigned nevt_j2_dijet_correctHighest;
        unsigned nevt_j2_dijet_correctThirdJetHighPt;
        unsigned nevt_j2_dijet_correctThirdJetMinDr;
        unsigned nevt_j2_dijet_correctThirdJetMinMjjj;
        unsigned nevt_j2_dijet_correctThirdJetMaxMjjj;

        unsigned nevt_j3;
        unsigned nevt_j3_dijet;
        unsigned nevt_j3_tightenpho;
        unsigned nevt_j3_dijetpresel;
        unsigned nevt_j3_dijetmultiple;
        unsigned nevt_j3_dijetallsame;
        unsigned nevt_j3_dijet_correct;
        unsigned nevt_j3_dijet_correctOpposite;
        unsigned nevt_j3_dijet_correctHighest;
        unsigned nevt_j3_dijet_correctThirdJetHighPt;
        unsigned nevt_j3_dijet_correctThirdJetMinDr;
        unsigned nevt_j3_dijet_correctThirdJetMinMjjj;
        unsigned nevt_j3_dijet_correctThirdJetMaxMjjj;
        

        /*
 Number of diphotons: 3
 Number of preselected diphotons: 1
 With lead photon passing pt cut: 1
 With both photons passing pt cut: 1
 With mgg in mass range also: 1
   With >=1 photon passing electron veto: 1
   With 2 photons passing electron veto: 1
   With >=1 photon having phoIdMVA > -0.2: 1
   With 2 photons having phoIdMVA > -0.2: 1
   With >=1 photon having phoIdMVA > 0: 1
   With 2 photons having phoIdMVA > 0: 1
   With a valid dijet: 1
        */
    };

    VBFWorkflowAnalyzer::VBFWorkflowAnalyzer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        preselectedDiPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "PreselectedDiPhotonTag" ) ) ),
        vbfMvaResultToken_( consumes<View<flashgg::VBFMVAResult> >( iConfig.getParameter<InputTag> ( "VBFMVAResultTag" ) ) ),
        vbfMvaResultTokenOpposite_( consumes<View<flashgg::VBFMVAResult> >( iConfig.getParameter<InputTag> ( "VBFMVAResultTagOpposite" ) ) ),
        vbfMvaResultTokenHighest_( consumes<View<flashgg::VBFMVAResult> >( iConfig.getParameter<InputTag> ( "VBFMVAResultTagHighest" ) ) ),
        vbfMvaResultTokenThirdJetHighPt_( consumes<View<flashgg::VBFMVAResult> >( iConfig.getParameter<InputTag> ( "VBFMVAResultTagThirdJetHighPt" ) ) ),
        vbfMvaResultTokenThirdJetMaxMjjj_( consumes<View<flashgg::VBFMVAResult> >( iConfig.getParameter<InputTag> ( "VBFMVAResultTagThirdJetMaxMjjj" ) ) ),
        vbfMvaResultTokenThirdJetMinMjjj_( consumes<View<flashgg::VBFMVAResult> >( iConfig.getParameter<InputTag> ( "VBFMVAResultTagThirdJetMinMjjj" ) ) ),
        vbfMvaResultTokenThirdJetMinDr_( consumes<View<flashgg::VBFMVAResult> >( iConfig.getParameter<InputTag> ( "VBFMVAResultTagThirdJetMinDr" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        genPartToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        genJetToken_( consumes<View<reco::GenJet> >( iConfig.getParameter<InputTag> ( "GenJetTag" ) ) ),
        requireAcceptance_(  iConfig.getUntrackedParameter<bool> ( "RequireAcceptance" , true ) ),
        useGenJets_( iConfig.getUntrackedParameter<bool>( "UseGenJets", true ) )
    {
        photonIdCut_ = 0.; //non-trivial value
        loosePhotonIdCut_ = -0.2;
        nevt_total = 0;
        nevt_photon_acc = 0;
        nevt_dipho = 0;
        nevt_presel = 0;
        nevt_leadpt = 0;
        nevt_bothpt = 0;
        nevt_mgg = 0;
        nevt_leadeleveto = 0;
        nevt_botheleveto = 0;
        nevt_leadlooseMVA = 0;
        nevt_bothlooseMVA = 0;
        nevt_leadMVA = 0;
        nevt_bothMVA = 0;
        nevt_jet_acc = 0;
        nevt_dijet = 0;
        nevt_tightenpho = 0;
        nevt_dijetpresel = 0;
        nevt_dijetmultiple = 0;
        nevt_dijetallsame = 0;
        nevt_dijet_correct = 0;
        nevt_dijet_correctOpposite = 0;
        nevt_dijet_correctHighest = 0;
        nevt_dijet_correctThirdJetHighPt = 0;
        nevt_dijet_correctThirdJetMinDr = 0;
        nevt_dijet_correctThirdJetMinMjjj = 0;
        nevt_dijet_correctThirdJetMaxMjjj = 0;

        nevt_j2 = 0;
        nevt_j2_dijet = 0;
        nevt_j2_tightenpho = 0;
        nevt_j2_dijetpresel = 0;
        nevt_j2_dijetmultiple = 0;
        nevt_j2_dijetallsame = 0;
        nevt_j2_dijet_correct = 0;
        nevt_j2_dijet_correctOpposite = 0;
        nevt_j2_dijet_correctHighest = 0;
        nevt_j2_dijet_correctThirdJetHighPt = 0;
        nevt_j2_dijet_correctThirdJetMinDr = 0;
        nevt_j2_dijet_correctThirdJetMinMjjj = 0;
        nevt_j2_dijet_correctThirdJetMaxMjjj = 0;

        nevt_j3 = 0;
        nevt_j3_dijet = 0;
        nevt_j3_tightenpho = 0;
        nevt_j3_dijetpresel = 0;
        nevt_j3_dijetmultiple = 0;
        nevt_j3_dijetallsame = 0;
        nevt_j3_dijet_correct = 0;
        nevt_j3_dijet_correctOpposite = 0;
        nevt_j3_dijet_correctHighest = 0;
        nevt_j3_dijet_correctThirdJetHighPt = 0;
        nevt_j3_dijet_correctThirdJetMinDr = 0;
        nevt_j3_dijet_correctThirdJetMinMjjj = 0;
        nevt_j3_dijet_correctThirdJetMaxMjjj = 0;
    }

    void VBFWorkflowAnalyzer::produce( Event &evt, const EventSetup & )
    {

        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );

        Handle<View<flashgg::DiPhotonCandidate> > preselectedDiPhotons;
        evt.getByToken( preselectedDiPhotonToken_, preselectedDiPhotons );

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );

        Handle<View<flashgg::VBFMVAResult> > vbfMvaResults;
        evt.getByToken( vbfMvaResultToken_, vbfMvaResults );

        Handle<View<flashgg::VBFMVAResult> > vbfMvaResultsOpposite;
        evt.getByToken( vbfMvaResultTokenOpposite_, vbfMvaResultsOpposite );

        Handle<View<flashgg::VBFMVAResult> > vbfMvaResultsHighest;
        evt.getByToken( vbfMvaResultTokenHighest_, vbfMvaResultsHighest );

        Handle<View<flashgg::VBFMVAResult> > vbfMvaResultsThirdJetHighPt;
        evt.getByToken( vbfMvaResultTokenThirdJetHighPt_, vbfMvaResultsThirdJetHighPt );
        Handle<View<flashgg::VBFMVAResult> > vbfMvaResultsThirdJetMaxMjjj;
        evt.getByToken( vbfMvaResultTokenThirdJetMaxMjjj_, vbfMvaResultsThirdJetMaxMjjj );
        Handle<View<flashgg::VBFMVAResult> > vbfMvaResultsThirdJetMinMjjj;
        evt.getByToken( vbfMvaResultTokenThirdJetMinMjjj_, vbfMvaResultsThirdJetMinMjjj );
        Handle<View<flashgg::VBFMVAResult> > vbfMvaResultsThirdJetMinDr;
        evt.getByToken( vbfMvaResultTokenThirdJetMinDr_, vbfMvaResultsThirdJetMinDr );

        //        Handle<View<flashgg::VBFDiPhoDiJetMVAResult> > vbfDiPhoDiJetMvaResults;
        //        evt.getByToken( vbfDiPhoDiJetMvaResultToken_, vbfDiPhoDiJetMvaResults );

        Handle<View<reco::GenParticle> > genParticles;
        evt.getByToken( genPartToken_, genParticles );

        Handle<View<reco::GenJet> > genJets;
        evt.getByToken( genJetToken_, genJets );

        assert( preselectedDiPhotons->size() == vbfMvaResults->size() );
        assert( preselectedDiPhotons->size() == vbfMvaResultsOpposite->size() );
        assert( preselectedDiPhotons->size() == vbfMvaResultsHighest->size() );


        std::cout << std::endl;
        std::cout << " --- start of event --- " << std::endl;
        nevt_total++;

        bool pass_acceptance_leadpho = 0;
        unsigned npho_pass_acceptance = 0;
        bool pass_phoacceptance = 0;
        bool pass_acceptance_leadjet = 0;

        vector<reco::Candidate::LorentzVector> jetList;

        if ( useGenJets_ ) {
            for ( unsigned int gjLoop = 0 ; gjLoop < genJets->size() ; gjLoop++ ) {
                edm::Ptr<reco::GenJet> gj = genJets->ptrAt( gjLoop );
                std::cout << " Gen jet pt eta " << gj->pt() << " " << gj->eta() << std::endl;
                bool ok = true;
                for( unsigned int gpLoop = 0 ; gpLoop < genParticles->size() ; gpLoop++ ) {
                    edm::Ptr<reco::GenParticle> gp = genParticles->ptrAt( gpLoop );
                    if ( gp->pdgId() == 22  && fabs( gp->phi() ) < 2.5 && deltaR( gp->eta(), gp->phi(), gj->eta(), gj->phi() ) < 0.4 ) {
                        std::cout << "  ... Rejecting due to overlap with photon!";
                        ok = false;
                        break;
                    }
                }
                if ( ok ) {
                    if ( gj->pt() > 30. && fabs( gj->eta() ) < 4.7 ) pass_acceptance_leadjet = 1;
                    if ( gj->pt() > 20. && fabs( gj->eta() ) < 4.7) jetList.push_back( gj->p4() );
                }
            }
        } // otherwise use partons, see next section
 
        for( unsigned int gpLoop = 0 ; gpLoop < genParticles->size() ; gpLoop++ ) {
            edm::Ptr<reco::GenParticle> gp = genParticles->ptrAt( gpLoop );
            if ( gp->pdgId() == 22  && fabs( gp->phi() ) < 2.5 ) {
                std::cout << "Gen Photon status pt eta phi " << gp->status() << " " << gp->pt() << " " << gp->eta() << " " << gp->phi() << std::endl;
                if ( gp->pt() > 30. ) pass_acceptance_leadpho = 1;
                if ( gp->pt() > 20. ) npho_pass_acceptance += 1;
            }
            if ( (!useGenJets_) && abs( gp->pdgId() ) <= 9 && fabs( gp->eta() ) < 4.7 && gp->pt() > 20. ) {
                std::cout << "Gen parton pdgId status pt eta phi " << gp->pdgId() << " " << gp->status() << " " << gp->pt() << " " << gp->eta() << " " << gp->phi() << std::endl;
                if ( gp->pt() > 30. ) pass_acceptance_leadjet = 1;
                float dr_sofar = 999.;
                unsigned match_index = 999;
                for ( unsigned nj = 0 ; nj < jetList.size() ; nj++ ) {
                    float dr = deltaR ( jetList[nj].eta(), jetList[nj].phi(), gp->eta(), gp->phi() );
                    if ( dr < dr_sofar ) {
                        dr_sofar = dr;
                        match_index = nj;
                    }
                }
                if ( dr_sofar >= 0.4 ) {
                    jetList.push_back( gp->p4() );
                } else {
                    // merge
                    jetList[match_index] = (jetList[match_index] + gp->p4());
                    std::cout << " Merge into object with pt eta phi " << jetList[match_index].pt() << " " << jetList[match_index].eta() 
                              << " " << jetList[match_index].phi() << std::endl;
                }
            }
        }

        if ( npho_pass_acceptance >= 2 && pass_acceptance_leadpho ) { 
            pass_phoacceptance = 1;
            nevt_photon_acc++;
        }

        std::cout << " Passed photon acceptance? " << pass_phoacceptance << std::endl;

        if ( requireAcceptance_ && ! pass_phoacceptance ) {
            std::cout << " FAILED PHOTON ACCEPTANCE, SKIPPING EVENT" << std::endl << std::endl;
            return;
        }

        std::cout << " Number of diphotons: " << diPhotons->size() << std::endl;
        unsigned passptpho1 = 0;
        unsigned passptpho2 = 0;
        unsigned passmgg = 0;
        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {
            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt(candIndex);
            if ( dipho->leadingPhoton()->pt() > dipho->mass()/3 ) {
                passptpho1++;
                if ( dipho->subLeadingPhoton()->pt() >dipho->mass()/4) {
                    passptpho2++;
                    if ( dipho->mass() < 180. && dipho->mass() > 100. ) {
                        passmgg++;
                    }
                }
            }
        }
        std::cout << " (unpresel) number of diphotons with lead photon passing pt cut: " << passptpho1 << std::endl;
        std::cout << " (unpresel) number of diphotons with both photons passing pt cut: " << passptpho2 << std::endl;
        std::cout << " (unpresel) number of diphotons with mgg in mass range also: " << passmgg << std::endl;
        std::cout << " Number of preselected diphotons: " << preselectedDiPhotons->size() << std::endl;

        if (diPhotons->size() > 0) nevt_dipho++;
        if (preselectedDiPhotons->size() > 0) nevt_presel++;

        unsigned int nvpass1 = 0;
        unsigned int nvpass2 = 0;
        unsigned int nipass1 = 0;
        unsigned int nipass2 = 0;
        unsigned int nlipass1 = 0;
        unsigned int nlipass2 = 0;
        vector<float> pths;
        vector<float> mjjs;
        vector<float> mjjsOpposite;
        vector<float> mjjsHighest;
        vector<bool> correct;
        vector<bool> correctOpposite;
        vector<bool> correctHighest;
        vector<bool> correctThirdJetHighPt;
        vector<bool> correctThirdJetMinDr;
        vector<bool> correctThirdJetMinMjjj;
        vector<bool> correctThirdJetMaxMjjj;

        unsigned preselpassptpho1 = 0;
        unsigned preselpassptpho2 = 0;
        unsigned preselpassmgg = 0;
        unsigned int valid_dijet = 0;
        unsigned int dijet_tighten_pho = 0;
        for( unsigned int candIndex = 0; candIndex < preselectedDiPhotons->size() ; candIndex++ ) {
            edm::Ptr<flashgg::DiPhotonCandidate> dipho = preselectedDiPhotons->ptrAt(candIndex);
            if ( dipho->leadingPhoton()->pt() > dipho->mass()/3 ) {
                preselpassptpho1++;
                if ( dipho->subLeadingPhoton()->pt() >dipho->mass()/4) {
                    preselpassptpho2++;
                    if ( dipho->mass() < 180. && dipho->mass() > 100. ) {
                        preselpassmgg++;
                    }
                }
            }
            bool v0 = 0, v1 = 0;
            if ( preselpassmgg ) {
                v0 = dipho->leadingPhoton()->passElectronVeto();
                v1 = dipho->subLeadingPhoton()->passElectronVeto();
            }
            if (v0 || v1) nvpass1++;
            if (v0 && v1) nvpass2++;
            bool li0 = 0, li1 = 0;
            if (v0 && v1) {
                li0 = ( dipho->leadPhotonId() > loosePhotonIdCut_ );
                li1 = ( dipho->subLeadPhotonId() > loosePhotonIdCut_ );
                if (li0 || li1) nlipass1++;
                if (li0 && li1) nlipass2++;
            }
            bool i0 = 0, i1 = 0;
            if (li0 && li1) {
                i0 = ( dipho->leadPhotonId() > photonIdCut_ );
                i1 = ( dipho->subLeadPhotonId() > photonIdCut_ );
                if (i0 || i1) nipass1++;
                if (i0 && i1) nipass2++;
            }
            if (i0 && i1 && ((!requireAcceptance_) || (jetList.size() >= 2 && pass_acceptance_leadjet) ) ) {
                edm::Ptr<flashgg::VBFMVAResult> vbfmvares = vbfMvaResults->ptrAt( candIndex );
                edm::Ptr<flashgg::VBFMVAResult> vbfmvaresOpposite = vbfMvaResultsOpposite->ptrAt( candIndex );
                edm::Ptr<flashgg::VBFMVAResult> vbfmvaresHighest = vbfMvaResultsHighest->ptrAt( candIndex );
                edm::Ptr<flashgg::VBFMVAResult> vbfmvaresThirdJetHighPt = vbfMvaResultsThirdJetHighPt->ptrAt( candIndex );
                edm::Ptr<flashgg::VBFMVAResult> vbfmvaresThirdJetMinMjjj = vbfMvaResultsThirdJetMinMjjj->ptrAt( candIndex );
                edm::Ptr<flashgg::VBFMVAResult> vbfmvaresThirdJetMaxMjjj = vbfMvaResultsThirdJetMaxMjjj->ptrAt( candIndex );
                edm::Ptr<flashgg::VBFMVAResult> vbfmvaresThirdJetMinDr = vbfMvaResultsThirdJetMinDr->ptrAt( candIndex );
                if (vbfmvares->dijet_Mjj > -0.1 && vbfmvares->dijet_LeadJPt > 30. && vbfmvares->dijet_SubJPt > 20. ) {
                    valid_dijet++;
                    if ( dipho->leadingPhoton()->pt() > dipho->mass()/2 ) {
                        dijet_tighten_pho++;
                        if ( vbfmvares->dijet_Mjj > 250. ) {
                            mjjs.push_back(vbfmvares->dijet_Mjj);
                            mjjsOpposite.push_back(vbfmvaresOpposite->dijet_Mjj);
                            mjjsHighest.push_back(vbfmvaresHighest->dijet_Mjj);
                            bool okj1 = 0, okj2 = 0;
                            for ( unsigned nj = 0 ; nj < jetList.size() ; nj++ ) {
                                if ( deltaR ( jetList[nj].eta(), jetList[nj].phi(), vbfmvares->leadJet.eta(), vbfmvares->leadJet.phi() ) < 0.4 ) okj1 = 1;
                                if ( deltaR ( jetList[nj].eta(), jetList[nj].phi(), vbfmvares->subleadJet.eta(), vbfmvares->subleadJet.phi() ) < 0.4 ) okj2 = 1;
                            }
                            correct.push_back(okj1&&okj2);
                            pths.push_back(dipho->sumPt());
                        }
                    }
                }
                if (vbfmvaresOpposite->dijet_Mjj > -0.1 && vbfmvaresOpposite->dijet_LeadJPt > 30. && vbfmvaresOpposite->dijet_SubJPt > 20. 
                    && dipho->leadingPhoton()->pt() > dipho->mass()/2 && vbfmvaresOpposite->dijet_Mjj > 250.) {
                    bool okj1 = 0, okj2 = 0;
                    for ( unsigned nj = 0 ; nj < jetList.size() ; nj++ ) {
                        if ( deltaR ( jetList[nj].eta(), jetList[nj].phi(), vbfmvaresOpposite->leadJet.eta(), vbfmvaresOpposite->leadJet.phi() ) < 0.4 ) okj1 = 1;
                        if ( deltaR ( jetList[nj].eta(), jetList[nj].phi(), vbfmvaresOpposite->subleadJet.eta(), vbfmvaresOpposite->subleadJet.phi() ) < 0.4 ) okj2 = 1;
                    }
                    correctOpposite.push_back(okj1&&okj2);
                }
                if (vbfmvaresHighest->dijet_Mjj > -0.1 && vbfmvaresHighest->dijet_LeadJPt > 30. && vbfmvaresHighest->dijet_SubJPt > 20.
                    && dipho->leadingPhoton()->pt() > dipho->mass()/2 && vbfmvaresHighest->dijet_Mjj > 250.) {
                    bool okj1 = 0, okj2 = 0;
                    for ( unsigned nj = 0 ; nj < jetList.size() ; nj++ ) {
                        if ( deltaR ( jetList[nj].eta(), jetList[nj].phi(), vbfmvaresHighest->leadJet.eta(), vbfmvaresHighest->leadJet.phi() ) < 0.4 ) okj1 = 1;
                        if ( deltaR ( jetList[nj].eta(), jetList[nj].phi(), vbfmvaresHighest->subleadJet.eta(), vbfmvaresHighest->subleadJet.phi() ) < 0.4 ) okj2 = 1;
                    }
                    correctHighest.push_back(okj1&&okj2);
                    okj1 = 0; okj2 = 0; bool okj3 = 0;
                    for ( unsigned nj = 0 ; nj < jetList.size() ; nj++ ) {
                        if ( deltaR ( jetList[nj].eta(), jetList[nj].phi(), vbfmvaresThirdJetHighPt->leadJet.eta(), vbfmvaresThirdJetHighPt->leadJet.phi() ) < 0.4 ) okj1 = 1;
                        if ( deltaR ( jetList[nj].eta(), jetList[nj].phi(), vbfmvaresThirdJetHighPt->subleadJet.eta(), vbfmvaresThirdJetHighPt->subleadJet.phi() ) < 0.4 ) okj2 = 1;
                        if ( deltaR ( jetList[nj].eta(), jetList[nj].phi(), vbfmvaresThirdJetHighPt->subsubleadJet.eta(), 
                                      vbfmvaresThirdJetHighPt->subsubleadJet.phi() ) < 0.4 ) okj3 = 1;
                    }
                    correctThirdJetHighPt.push_back(okj1&&okj2&&okj3);
                    okj1 = 0; okj2 = 0; okj3 = 0;
                    for ( unsigned nj = 0 ; nj < jetList.size() ; nj++ ) {
                        if ( deltaR ( jetList[nj].eta(), jetList[nj].phi(), vbfmvaresThirdJetMinMjjj->leadJet.eta(), vbfmvaresThirdJetMinMjjj->leadJet.phi() ) < 0.4 ) okj1 = 1;
                        if ( deltaR ( jetList[nj].eta(), jetList[nj].phi(), vbfmvaresThirdJetMinMjjj->subleadJet.eta(), vbfmvaresThirdJetMinMjjj->subleadJet.phi() ) < 0.4 ) okj2 = 1;
                        if ( deltaR ( jetList[nj].eta(), jetList[nj].phi(), vbfmvaresThirdJetMinMjjj->subsubleadJet.eta(),
                                      vbfmvaresThirdJetMinMjjj->subsubleadJet.phi() ) < 0.4 ) okj3 = 1;
                    }
                    correctThirdJetMinMjjj.push_back(okj1&&okj2&&okj3);
                    okj1 = 0; okj2 = 0; okj3 = 0;
                    for ( unsigned nj = 0 ; nj < jetList.size() ; nj++ ) {
                        if ( deltaR ( jetList[nj].eta(), jetList[nj].phi(), vbfmvaresThirdJetMaxMjjj->leadJet.eta(), vbfmvaresThirdJetMaxMjjj->leadJet.phi() ) < 0.4 ) okj1 = 1;
                        if ( deltaR ( jetList[nj].eta(), jetList[nj].phi(), 
                                      vbfmvaresThirdJetMaxMjjj->subleadJet.eta(), vbfmvaresThirdJetMaxMjjj->subleadJet.phi() ) < 0.4 ) okj2 = 1;
                        if ( deltaR ( jetList[nj].eta(), jetList[nj].phi(), vbfmvaresThirdJetMaxMjjj->subsubleadJet.eta(),
                                      vbfmvaresThirdJetMaxMjjj->subsubleadJet.phi() ) < 0.4 ) okj3 = 1;
                    }
                    correctThirdJetMaxMjjj.push_back(okj1&&okj2&&okj3);
                    okj1 = 0; okj2 = 0; okj3 = 0;
                    for ( unsigned nj = 0 ; nj < jetList.size() ; nj++ ) {
                        if ( deltaR ( jetList[nj].eta(), jetList[nj].phi(), vbfmvaresThirdJetMinDr->leadJet.eta(), vbfmvaresThirdJetMinDr->leadJet.phi() ) < 0.4 ) okj1 = 1;
                        if ( deltaR ( jetList[nj].eta(), jetList[nj].phi(),
                                      vbfmvaresThirdJetMinDr->subleadJet.eta(), vbfmvaresThirdJetMinDr->subleadJet.phi() ) < 0.4 ) okj2 = 1;
                        if ( deltaR ( jetList[nj].eta(), jetList[nj].phi(), vbfmvaresThirdJetMinDr->subsubleadJet.eta(),
                                      vbfmvaresThirdJetMinDr->subsubleadJet.phi() ) < 0.4 ) okj3 = 1;
                    }
                    correctThirdJetMinDr.push_back(okj1&&okj2&&okj3);
                }
            }
        }
        if ( preselectedDiPhotons->size() > 0 ) {
            std::cout << " With lead photon passing pt cut: " << preselpassptpho1 << std::endl;
            std::cout << " With both photons passing pt cut: " << preselpassptpho2 << std::endl;
            std::cout << " With mgg in mass range also: " << preselpassmgg << std::endl;
            if (preselpassptpho1) nevt_leadpt++;
            if (preselpassptpho2) nevt_bothpt++;
        }
        if ( preselpassmgg > 0 ) {
            nevt_mgg++;
            if (nvpass1) nevt_leadeleveto++;
            std::cout << "   With >=1 photon passing electron veto: " << nvpass1 << std::endl;
            std::cout << "   With 2 photons passing electron veto: " << nvpass2 << std::endl;
        }
        if (nvpass2 > 0 ) {
            nevt_botheleveto++;
            if (nlipass1) nevt_leadlooseMVA++;
            std::cout << "   With >=1 photon having phoIdMVA > " << loosePhotonIdCut_ << ": " << nlipass1 << std::endl;
            std::cout << "   With 2 photons having phoIdMVA > " << loosePhotonIdCut_ << ": " << nlipass2 << std::endl;
        }
        if (nlipass2 > 0) {
            nevt_bothlooseMVA++;
            if (nipass1) nevt_leadMVA++;
            if (nipass2 > 0)  nevt_bothMVA++;
            std::cout << "   With >=1 photon having phoIdMVA > " << photonIdCut_ << ": " << nipass1 << std::endl;
            std::cout << "   With 2 photons having phoIdMVA > " << photonIdCut_ << ": " << nipass2 << std::endl;
        }
        if ( nipass1 && nipass2 && requireAcceptance_) { 
            if ( jetList.size() >= 2 && pass_acceptance_leadjet ) {
                nevt_jet_acc++;
                if (useGenJets_) std::cout << "  Passed gen jet acceptance!  Count: " << jetList.size() << std::endl;
                if (!useGenJets_) std::cout << "  Passed parton jet acceptance!  Count: " << jetList.size() << std::endl;
                if ( jetList.size() == 2) nevt_j2++;
                if ( jetList.size() == 3) nevt_j3++;
            }
        }
        if (valid_dijet) {
            nevt_dijet++;
            std::cout <<  "  With chosen dijet pt0,1 > 30,20, |eta| < 4.7: " << valid_dijet << std::endl;
            if ( jetList.size() == 2) nevt_j2_dijet++;
            if ( jetList.size() == 3) nevt_j3_dijet++;
        }
        if (dijet_tighten_pho) {
            nevt_tightenpho++;
            if ( jetList.size() == 2) nevt_j2_tightenpho++;
            if ( jetList.size() == 3) nevt_j3_tightenpho++;
            std::cout << "   With lead photon pt > mgg/2: " << dijet_tighten_pho << std::endl;
            std::cout << "   With mjj > 250 (preselected): " << mjjs.size() << std::endl;
        }
        if (mjjs.size() > 0) {
            nevt_dijetpresel++;
            if ( jetList.size() == 2) nevt_j2_dijetpresel++;
            if ( jetList.size() == 3) nevt_j3_dijetpresel++;
            if (mjjs.size() > 1) {
                nevt_dijetmultiple++;
                if ( jetList.size() == 2) nevt_j2_dijetmultiple++;
                if ( jetList.size() == 3) nevt_j3_dijetmultiple++;
            }
            if (mjjs[0] == mjjsOpposite[0] && mjjs[0] == mjjsHighest[0]) {
                nevt_dijetallsame++;
                if ( jetList.size() == 2) nevt_j2_dijetallsame++;
                if ( jetList.size() == 3) nevt_j3_dijetallsame++;
            }
            if (correct[0]) {
                nevt_dijet_correct++;
                if ( jetList.size() == 2) nevt_j2_dijet_correct++;
                if ( jetList.size() == 3) nevt_j3_dijet_correct++;
            }
            if (correctOpposite.size() > 0 && correctOpposite[0]) {
                nevt_dijet_correctOpposite++;
                if ( jetList.size() == 2) nevt_j2_dijet_correctOpposite++;
                if ( jetList.size() == 3) nevt_j3_dijet_correctOpposite++;
            }
            if (correctHighest.size() > 0 && correctHighest[0]) {
                nevt_dijet_correctHighest++;
                if ( jetList.size() == 2) nevt_j2_dijet_correctHighest++;
                if ( jetList.size() == 3) nevt_j3_dijet_correctHighest++;
            }
            if (correctThirdJetHighPt.size() > 0 && correctThirdJetHighPt[0]) {
                nevt_dijet_correctThirdJetHighPt++;
                if ( jetList.size() == 2) nevt_j2_dijet_correctThirdJetHighPt++;
                if ( jetList.size() == 3) nevt_j3_dijet_correctThirdJetHighPt++;
            }
            if (correctThirdJetMinDr.size() > 0 && correctThirdJetMinDr[0]) {
                nevt_dijet_correctThirdJetMinDr++;
                if ( jetList.size() == 2) nevt_j2_dijet_correctThirdJetMinDr++;
                if ( jetList.size() == 3) nevt_j3_dijet_correctThirdJetMinDr++;
            }
            if (correctThirdJetMinMjjj.size() > 0 && correctThirdJetMinMjjj[0]) {
                nevt_dijet_correctThirdJetMinMjjj++;
                if ( jetList.size() == 2) nevt_j2_dijet_correctThirdJetMinMjjj++;
                if ( jetList.size() == 3) nevt_j3_dijet_correctThirdJetMinMjjj++;
            }
            if (correctThirdJetMaxMjjj.size() > 0 && correctThirdJetMaxMjjj[0]) {
                nevt_dijet_correctThirdJetMaxMjjj++;
                if ( jetList.size() == 2) nevt_j2_dijet_correctThirdJetMaxMjjj++;
                if ( jetList.size() == 3) nevt_j3_dijet_correctThirdJetMaxMjjj++;
            }

            std::cout << std::endl << "     pT(H) list: ";
            for (unsigned int i = 0 ; i < pths.size() ; i++) {
                std::cout << pths[i] << " ";
            }
            std::cout << "      Mjj list (top 2 in pT): ";
            for (unsigned int i = 0 ; i < mjjs.size() ; i++) {
                std::cout << mjjs[i] << " ";
            }
            std::cout << "      Mjj list (2nd jet opposite eta): ";
            for (unsigned int i = 0 ; i < mjjsOpposite.size() ; i++) {
                std::cout << mjjsOpposite[i] << " ";
            }
            std::cout << "      Mjj list (2nd jet highest mjj): ";
            for (unsigned int i = 0 ; i < mjjsHighest.size() ; i++) {
                std::cout << mjjsHighest[i] << " ";
            }
            std::cout << std::endl;
        }
            

        std::cout << " --- end of event --- " << std::endl;
        std::cout << std::endl;

        /*
        unsigned int index_leadq = std::numeric_limits<unsigned int>::max(), index_subleadq = std::numeric_limits<unsigned int>::max();
        float pt_leadq = 0., pt_subleadq = 0.;
        Point higgsVtx;

        if( ! evt.isRealData() ) {
            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                int pdgid = genParticles->ptrAt( genLoop )->pdgId();
                if( pdgid == 25 || pdgid == 22 ) {
                    higgsVtx = genParticles->ptrAt( genLoop )->vertex();
                    break;
                }
            }
            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                edm::Ptr<reco::GenParticle> part = genParticles->ptrAt( genLoop );
                if( part->isHardProcess() ) {
                    if( abs( part->pdgId() ) <= 5 ) {
                        if( part->pt() > pt_leadq ) {
                            index_subleadq = index_leadq;
                            pt_subleadq = pt_leadq;
                            index_leadq = genLoop;
                            pt_leadq = part->pt();
                        } else if( part->pt() > pt_subleadq ) {
                            index_subleadq = genLoop;
                            pt_subleadq = part->pt();
                        }
                    }
                }
            }
        }
        assert( diPhotons->size() == vbfDiPhoDiJetMvaResults->size() ); // We are relying on corresponding sets - update this to give an error/exception
        assert( diPhotons->size() ==
                mvaResults->size() ); // We are relying on corresponding sets - update this to give an error/exception

        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {
            edm::Ptr<flashgg::VBFDiPhoDiJetMVAResult> vbfdipho_mvares = vbfDiPhoDiJetMvaResults->ptrAt( candIndex );
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( candIndex );
            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( candIndex );

            VBFTag tag_obj( dipho, mvares, vbfdipho_mvares );
            tag_obj.setDiPhotonIndex( candIndex );

            tag_obj.setSystLabel( systLabel_ );

            int catnum = chooseCategory( vbfdipho_mvares->vbfDiPhoDiJetMvaResult );
            tag_obj.setCategoryNumber( catnum );
            unsigned int index_gp_leadjet = std::numeric_limits<unsigned int>::max();
            unsigned int index_gp_subleadjet = std::numeric_limits<unsigned int>::max();
            unsigned int index_gp_leadphoton = std::numeric_limits<unsigned int>::max();
            unsigned int index_gp_subleadphoton = std::numeric_limits<unsigned int>::max();
            unsigned int index_gj_leadjet = std::numeric_limits<unsigned int>::max();
            unsigned int index_gj_subleadjet = std::numeric_limits<unsigned int>::max();
            float dr_gp_leadjet = 999.;
            float dr_gp_subleadjet = 999.;
            float dr_gp_leadphoton = 999.;
            float dr_gp_subleadphoton = 999.;
            float dr_gj_leadjet = 999.;
            float dr_gj_subleadjet = 999.;
            VBFTagTruth truth_obj;
            if( ! evt.isRealData() ) {
                for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                    edm::Ptr<reco::GenParticle> part = genParticles->ptrAt( genLoop );
                    if( part->isHardProcess() ) {
                        float dr = deltaR( tag_obj.leadingJet().eta(), tag_obj.leadingJet().phi(), part->eta(), part->phi() );
                        if( dr < dr_gp_leadjet ) {
                            dr_gp_leadjet = dr;
                            index_gp_leadjet = genLoop;
                        }
                        dr = deltaR( tag_obj.subLeadingJet().eta(), tag_obj.subLeadingJet().phi(), part->eta(), part->phi() );
                        if( dr < dr_gp_subleadjet ) {
                            dr_gp_subleadjet = dr;
                            index_gp_subleadjet = genLoop;
                        }
                        dr = deltaR( tag_obj.diPhoton()->leadingPhoton()->eta(), tag_obj.diPhoton()->leadingPhoton()->phi(), part->eta(), part->phi() );
                        if( dr < dr_gp_leadphoton ) {
                            dr_gp_leadphoton = dr;
                            index_gp_leadphoton = genLoop;
                        }
                        dr = deltaR( tag_obj.diPhoton()->subLeadingPhoton()->eta(), tag_obj.diPhoton()->subLeadingPhoton()->phi(), part->eta(), part->phi() );
                        if( dr < dr_gp_subleadphoton ) {
                            dr_gp_subleadphoton = dr;
                            index_gp_subleadphoton = genLoop;
                        }
                    }
                }

                if( index_gp_leadjet < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestParticleToLeadingJet( genParticles->ptrAt( index_gp_leadjet ) ); }
                if( index_gp_subleadjet < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestParticleToSubLeadingJet( genParticles->ptrAt( index_gp_subleadjet ) ); }
                if( index_gp_leadphoton < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestParticleToLeadingPhoton( genParticles->ptrAt( index_gp_leadphoton ) ); }
                if( index_gp_subleadphoton < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestParticleToSubLeadingPhoton( genParticles->ptrAt( index_gp_subleadphoton ) ); }
                for( unsigned int gjLoop = 0 ; gjLoop < genJets->size() ; gjLoop++ ) {
                    edm::Ptr<reco::GenJet> gj = genJets->ptrAt( gjLoop );
                    float dr = deltaR( tag_obj.leadingJet().eta(), tag_obj.leadingJet().phi(), gj->eta(), gj->phi() );
                    if( dr < dr_gj_leadjet ) {
                        dr_gj_leadjet = dr;
                        index_gj_leadjet = gjLoop;
                    }
                    dr = deltaR( tag_obj.subLeadingJet().eta(), tag_obj.subLeadingJet().phi(), gj->eta(), gj->phi() );
                    if( dr < dr_gj_subleadjet ) {
                        dr_gj_subleadjet = dr;
                        index_gj_subleadjet = gjLoop;
                    }
                }
                if( index_gj_leadjet < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestGenJetToLeadingJet( genJets->ptrAt( index_gj_leadjet ) ); }
                if( index_gj_subleadjet < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestGenJetToSubLeadingJet( genJets->ptrAt( index_gj_subleadjet ) ); }

                if( index_leadq < std::numeric_limits<unsigned int>::max() ) { truth_obj.setLeadingQuark( genParticles->ptrAt( index_leadq ) ); }
                if( index_subleadq < std::numeric_limits<unsigned int>::max() ) { truth_obj.setSubLeadingQuark( genParticles->ptrAt( index_subleadq ) ); }
            }
            truth_obj.setGenPV( higgsVtx );
            if( tag_obj.categoryNumber() >= 0 ) {
                tags->push_back( tag_obj );
                if( ! evt.isRealData() ) {
                    truths->push_back( truth_obj );
                    tags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<VBFTagTruth> >( rTagTruth, idx++ ) ) );
                }
            }
        }

        evt.put( tags );
        evt.put( truths );
*/
    }

    void VBFWorkflowAnalyzer::endJob() {
        
        char genString[20] = "parton";
        if (useGenJets_) strcpy(genString,"jet");

        std::cout << std::endl << std::endl << "  FINAL VBFWORKFLOWANALYZER NUMBERS" << std::endl << std::endl;
        
        std::cout << std::setprecision(3);
        unsigned nevt_denom = nevt_total;
        if (requireAcceptance_) nevt_denom = nevt_photon_acc;
        std::cout << "Events: " << nevt_total << std::endl;
        std::cout << "Events passing truth-level diphoton acceptance (|eta| < 2.5, pt > 30, 20): " << nevt_photon_acc << " (" << (float(nevt_photon_acc)/nevt_total) << ") " << std::endl;
        std::cout << "Denominator for next fractions is: " << nevt_denom << std::endl;
        std::cout << " Events with a diphoton: " << nevt_dipho << " (" << (float(nevt_dipho)/nevt_denom) << ") " << std::endl;
        std::cout << " Events with a preselected diphoton: " << nevt_presel << " (" << (float(nevt_presel)/nevt_denom) << ") " << std::endl;
        std::cout << "  with pt0 > mgg/3: " << nevt_leadpt << " (" << (float(nevt_leadpt)/nevt_denom) << ") " << std::endl;
        std::cout << "   and pt1 > mgg/4: " << nevt_bothpt << " (" << (float(nevt_bothpt)/nevt_denom) << ") " << std::endl;
        std::cout << "   and 100 < mgg < 180: " << nevt_mgg << " (" << (float(nevt_mgg)/nevt_denom) << ") " << std::endl;
        std::cout << "   and >=1 ele veto: " << nevt_leadeleveto << " (" << (float(nevt_leadeleveto)/nevt_denom) << ") " << std::endl;
        std::cout << "   and both ele veto: " << nevt_botheleveto << " (" << (float(nevt_botheleveto)/nevt_denom) << ") " << std::endl;
        std::cout << "   and mva0 > " << loosePhotonIdCut_ << ": " << nevt_leadlooseMVA << " (" << (float(nevt_leadlooseMVA)/nevt_denom) << ") " << std::endl;
        std::cout << "   and mva1 > " << loosePhotonIdCut_ <<": " << nevt_bothlooseMVA << " (" << (float(nevt_bothlooseMVA)/nevt_denom) << ") " << std::endl;
        std::cout << "   and mva0 > " << photonIdCut_ <<": " << nevt_leadMVA << " (" << (float(nevt_leadMVA)/nevt_denom) << ") " << std::endl;
        std::cout << "   and mva1 > " << photonIdCut_ <<": " << nevt_bothMVA << " (" << (float(nevt_bothMVA)/nevt_denom) << ") " << std::endl;
        if (requireAcceptance_) {
            std::cout << "   and >= 2 gen-level " << genString << "s (pt > 30, pt > 20): " 
                                          << nevt_jet_acc << " (" << (float(nevt_jet_acc)/nevt_denom) << " ) " << std::endl;
        }
        std::cout << "   and a dijet (pt0 > 30, pt1 > 20): " << nevt_dijet << " (" << (float(nevt_dijet)/nevt_denom) << ") " << std::endl;
        std::cout << "   and photon pt0 > mgg/2: " << nevt_tightenpho << " (" << (float(nevt_tightenpho)/nevt_denom) << ") " << std::endl;
        std::cout << "   and mgg > 250 (dijet presel): " << nevt_dijetpresel << " (" << (float(nevt_dijetpresel)/nevt_denom) << ") " << std::endl;

        std::cout << std::endl << "END OF CUTFLOW, TWO FURTHER SEPARATE QUESTIONS, with full list of cuts (i.e. dijet presel) as denominator:" << std::endl << std::endl;
        std::cout << "    Fraction with multiple dipho/dijets to consider: " << nevt_dijetmultiple 
                  << nevt_dijetmultiple << " (" << (float(nevt_dijetmultiple)/nevt_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with all 3 sublead jet methods giving same answer [highest pT(H) dijet only]: " 
                  << nevt_dijetallsame << " (" << (float(nevt_dijetallsame)/nevt_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with Default method giving jets that match gen-level " << genString << "s: " << nevt_dijet_correct 
                  << " (" << (float(nevt_dijet_correct)/nevt_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with Opposite method giving jets that match gen-level " << genString << "s: " << nevt_dijet_correctOpposite
                  << " (" << (float(nevt_dijet_correctOpposite)/nevt_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with Highest method giving jets that match gen-level " << genString << "s: " << nevt_dijet_correctHighest
                  << " (" << (float(nevt_dijet_correctHighest)/nevt_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with 3-jet Highest Pt method giving jets that match gen-level " << genString << "s: " << nevt_dijet_correctThirdJetHighPt
                  << " (" << (float(nevt_dijet_correctThirdJetHighPt)/nevt_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with 3-jet min(mjjj) method giving jets that match gen-level " << genString << "s: " << nevt_dijet_correctThirdJetMinMjjj
                  << " (" << (float(nevt_dijet_correctThirdJetMinMjjj)/nevt_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with 3-jet max(mjjj) method giving jets that match gen-level " << genString << "s: " << nevt_dijet_correctThirdJetMaxMjjj
                  << " (" << (float(nevt_dijet_correctThirdJetMaxMjjj)/nevt_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with 3-jet min(dr) method giving jets that match gen-level " << genString << "s: " << nevt_dijet_correctThirdJetMinDr
                  << " (" << (float(nevt_dijet_correctThirdJetMinDr)/nevt_dijetpresel) << ") " << std::endl;



        std::cout << std::endl;
        std::cout << " Number of events passing gen " << genString << " acceptance with exactly 2 " << genString << "s: " 
                  << nevt_j2 << " (" << (float(nevt_j2)/nevt_denom) << " ) " << std::endl;
        std::cout << "                 (Using this as the denominator for next fractions)" << std::endl;
        std::cout << "   and a dijet (pt0 > 30, pt1 > 20): " << nevt_j2_dijet << " (" << (float(nevt_j2_dijet)/nevt_j2) << ") " << std::endl;
        std::cout << "   and photon pt0 > mgg/2: " << nevt_j2_tightenpho << " (" << (float(nevt_j2_tightenpho)/nevt_j2) << ") " << std::endl;
        std::cout << "   and mgg > 250 (dijet presel): " << nevt_j2_dijetpresel << " (" << (float(nevt_j2_dijetpresel)/nevt_j2) << ") " << std::endl;
        std::cout << " And the two further questions, with exactly 2 gen " << genString << "s + presel as denominator:" << std::endl;
        std::cout << "    Fraction with multiple dipho/dijets to consider: " << nevt_j2_dijetmultiple
                  << nevt_j2_dijetmultiple << " (" << (float(nevt_j2_dijetmultiple)/nevt_j2_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with all 3 sublead jet methods giving same answer [highest pT(H) dijet only]: "
                  << nevt_j2_dijetallsame << " (" << (float(nevt_j2_dijetallsame)/nevt_j2_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with Default method giving jets that match gen " << genString << "s: " << nevt_j2_dijet_correct
                  << " (" << (float(nevt_j2_dijet_correct)/nevt_j2_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with Opposite method giving jets that match gen " << genString << "s: " << nevt_j2_dijet_correctOpposite
                  << " (" << (float(nevt_j2_dijet_correctOpposite)/nevt_j2_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with Highest method giving jets that match gen " << genString << "s: " << nevt_j2_dijet_correctHighest
                  << " (" << (float(nevt_j2_dijet_correctHighest)/nevt_j2_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with 3-jet Highest Pt method giving jets that match gen-level " << genString << "s: " << nevt_j2_dijet_correctThirdJetHighPt
                  << " (" << (float(nevt_j2_dijet_correctThirdJetHighPt)/nevt_j2_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with 3-jet min(mjjj) method giving jets that match gen-level " << genString << "s: " << nevt_j2_dijet_correctThirdJetMinMjjj
                  << " (" << (float(nevt_j2_dijet_correctThirdJetMinMjjj)/nevt_j2_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with 3-jet max(mjjj) method giving jets that match gen-level " << genString << "s: " << nevt_j2_dijet_correctThirdJetMaxMjjj
                  << " (" << (float(nevt_j2_dijet_correctThirdJetMaxMjjj)/nevt_j2_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with 3-jet min(dr) method giving jets that match gen-level " << genString << "s: " << nevt_j2_dijet_correctThirdJetMinDr
                  << " (" << (float(nevt_j2_dijet_correctThirdJetMinDr)/nevt_j2_dijetpresel) << ") " << std::endl;


        std::cout << std::endl;
        std::cout << " Number of events passing gen " << genString << " acceptance with exactly 3 " << genString << "s: " << nevt_j3 << " (" << (float(nevt_j3)/nevt_denom) << " ) " << std::endl;
        std::cout << "                 (Using this as the denominator for next fractions)" << std::endl;
        std::cout << "   and a dijet (pt0 > 30, pt1 > 20): " << nevt_j3_dijet << " (" << (float(nevt_j3_dijet)/nevt_j3) << ") " << std::endl;
        std::cout << "   and photon pt0 > mgg/2: " << nevt_j3_tightenpho << " (" << (float(nevt_j3_tightenpho)/nevt_j3) << ") " << std::endl;
        std::cout << "   and mgg > 250 (dijet presel): " << nevt_j3_dijetpresel << " (" << (float(nevt_j3_dijetpresel)/nevt_j3) << ") " << std::endl;
        std::cout << " And the two further questions, with exactly 3 gen " << genString << "s + presel as denominator:" << std::endl;
        std::cout << "    Fraction with multiple dipho/dijets to consider: " << nevt_j3_dijetmultiple
                  << nevt_j3_dijetmultiple << " (" << (float(nevt_j3_dijetmultiple)/nevt_j3_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with all 3 sublead jet methods giving same answer [highest pT(H) dijet only]: "
                  << nevt_j3_dijetallsame << " (" << (float(nevt_j3_dijetallsame)/nevt_j3_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with Default method giving jets that match gen " << genString << "s: " << nevt_j3_dijet_correct
                  << " (" << (float(nevt_j3_dijet_correct)/nevt_j3_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with Opposite method giving jets that match gen " << genString << "s: " << nevt_j3_dijet_correctOpposite
                  << " (" << (float(nevt_j3_dijet_correctOpposite)/nevt_j3_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with Highest method giving jets that match gen " << genString << "s: " << nevt_j3_dijet_correctHighest
                  << " (" << (float(nevt_j3_dijet_correctHighest)/nevt_j3_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with 3-jet Highest Pt method giving jets that match gen-level " << genString << "s: " << nevt_j3_dijet_correctThirdJetHighPt
                  << " (" << (float(nevt_j3_dijet_correctThirdJetHighPt)/nevt_j3_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with 3-jet min(mjjj) method giving jets that match gen-level " << genString << "s: " << nevt_j3_dijet_correctThirdJetMinMjjj
                  << " (" << (float(nevt_j3_dijet_correctThirdJetMinMjjj)/nevt_j3_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with 3-jet max(mjjj) method giving jets that match gen-level " << genString << "s: " << nevt_j3_dijet_correctThirdJetMaxMjjj
                  << " (" << (float(nevt_j3_dijet_correctThirdJetMaxMjjj)/nevt_j3_dijetpresel) << ") " << std::endl;
        std::cout << "    Fraction with 3-jet min(dr) method giving jets that match gen-level " << genString << "s: " << nevt_j3_dijet_correctThirdJetMinDr
                  << " (" << (float(nevt_j3_dijet_correctThirdJetMinDr)/nevt_j3_dijetpresel) << ") " << std::endl;

        
        std::cout << std::endl << std::endl;
    }

}


typedef flashgg::VBFWorkflowAnalyzer FlashggVBFWorkflowAnalyzer;
DEFINE_FWK_MODULE( FlashggVBFWorkflowAnalyzer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

