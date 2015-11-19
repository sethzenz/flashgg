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
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<reco::GenParticle> > genPartToken_;
        EDGetTokenT<View<reco::GenJet> > genJetToken_;
        
        float photonIdCut_;
        float loosePhotonIdCut_;
        unsigned nevt_total;
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
        unsigned nevt_dijet;
        unsigned nevt_tightenpho;
        unsigned nevt_dijetpresel;
        unsigned nevt_dijetmultiple;
        unsigned nevt_dijetallsame;
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
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        genPartToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        genJetToken_( consumes<View<reco::GenJet> >( iConfig.getParameter<InputTag> ( "GenJetTag" ) ) )
    {
        photonIdCut_ = 0.; //non-trivial value
        loosePhotonIdCut_ = -0.2;
        nevt_total = 0;
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
        nevt_dijet = 0;
        nevt_tightenpho = 0;
        nevt_dijetpresel = 0;
        nevt_dijetmultiple = 0;
        nevt_dijetallsame = 0;
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
        std::cout << " Number of diphotons: " << diPhotons->size() << std::endl;
        unsigned passptpho1 = 0;
        unsigned passptpho2 = 0;
        unsigned passmgg = 0;
        unsigned prompt0 = 0;
        unsigned prompt1 = 0;
        unsigned prompt2 = 0;
        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {
            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt(candIndex);
            unsigned np = 0;
            if ( dipho->leadingPhoton()->genMatchType() == flashgg::Photon::kPrompt ) np++;
            if ( dipho->subLeadingPhoton()->genMatchType() == flashgg::Photon::kPrompt ) np++;
            if ( np == 2 ) prompt2++;
            if ( np == 1 ) prompt1++;
            if ( np == 0 ) prompt0++;
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
        std::cout << " (unpresel, not in cutflow) number diphotons with 2 prompt photons: " << prompt2 << std::endl;
        std::cout << " (unpresel, not in cutflow) number diphotons with 1 prompt photons: " << prompt1 << std::endl;
        std::cout << " (unpresel, not in cutflow) number diphotons with 0 prompt photons: " << prompt0 << std::endl;
        std::cout << " Number of preselected diphotons: " << preselectedDiPhotons->size() << std::endl;

        nevt_total++;
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
            if (i0 && i1) {
                edm::Ptr<flashgg::VBFMVAResult> vbfmvares = vbfMvaResults->ptrAt( candIndex );
                edm::Ptr<flashgg::VBFMVAResult> vbfmvaresOpposite = vbfMvaResultsOpposite->ptrAt( candIndex );
                edm::Ptr<flashgg::VBFMVAResult> vbfmvaresHighest = vbfMvaResultsHighest->ptrAt( candIndex );
                if (vbfmvares->dijet_Mjj > -0.1 && vbfmvares->dijet_LeadJPt > 30. && vbfmvares->dijet_SubJPt > 20. ) {
                    valid_dijet++;
                    if ( dipho->leadingPhoton()->pt() > dipho->mass()/2 ) {
                        dijet_tighten_pho++;
                        if ( vbfmvares->dijet_Mjj > 250. ) {
                            mjjs.push_back(vbfmvares->dijet_Mjj);
                            mjjsOpposite.push_back(vbfmvaresOpposite->dijet_Mjj);
                            mjjsHighest.push_back(vbfmvaresHighest->dijet_Mjj);
                            pths.push_back(dipho->sumPt());
                        }
                    }
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
        if (valid_dijet) {
            nevt_dijet++;
            std::cout <<  "  With chosen dijet pt0,1 > 30,20, |eta| < 4.7: " << valid_dijet << std::endl;
        }
        if (dijet_tighten_pho) {
            nevt_tightenpho++;
            std::cout << "   With lead photon pt > mgg/2: " << dijet_tighten_pho << std::endl;
            std::cout << "   With mjj > 250 (preselected): " << mjjs.size() << std::endl;
        }
        if (mjjs.size() > 0) {
            nevt_dijetpresel++;
            if (mjjs.size() > 1) nevt_dijetmultiple++;
            if (mjjs[0] == mjjsOpposite[0] && mjjs[0] == mjjsHighest[0]) {
                nevt_dijetallsame++;
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
        
        std::cout << std::endl << std::endl << "  FINAL VBFWORKFLOWANALYZER NUMBERS" << std::endl << std::endl;
        
        std::cout << std::setprecision(3);
        std::cout << "Events: " << nevt_total << std::endl;
        std::cout << "Events with a diphoton: " << nevt_dipho << " (" << (float(nevt_dipho)/nevt_total) << ") " << std::endl;
        std::cout << "Events with a preselected diphoton: " << nevt_presel << " (" << (float(nevt_presel)/nevt_total) << ") " << std::endl;
        std::cout << "  with pt0 > mgg/3: " << nevt_leadpt << " (" << (float(nevt_leadpt)/nevt_total) << ") " << std::endl;
        std::cout << "   and pt1 > mgg/4: " << nevt_bothpt << " (" << (float(nevt_bothpt)/nevt_total) << ") " << std::endl;
        std::cout << "   and 100 < mgg < 180: " << nevt_mgg << " (" << (float(nevt_mgg)/nevt_total) << ") " << std::endl;
        std::cout << "   and >=1 ele veto: " << nevt_leadeleveto << " (" << (float(nevt_leadeleveto)/nevt_total) << ") " << std::endl;
        std::cout << "  and both ele veto: " << nevt_botheleveto << " (" << (float(nevt_botheleveto)/nevt_total) << ") " << std::endl;
        std::cout << "  and mva0 > " << loosePhotonIdCut_ << ": " << nevt_leadlooseMVA << " (" << (float(nevt_leadlooseMVA)/nevt_total) << ") " << std::endl;
        std::cout << "  and mva1 > " << loosePhotonIdCut_ <<": " << nevt_bothlooseMVA << " (" << (float(nevt_bothlooseMVA)/nevt_total) << ") " << std::endl;
        std::cout << "  and mva0 > " << photonIdCut_ <<": " << nevt_leadMVA << " (" << (float(nevt_leadMVA)/nevt_total) << ") " << std::endl;
        std::cout << "  and mva1 > " << photonIdCut_ <<": " << nevt_bothMVA << " (" << (float(nevt_bothMVA)/nevt_total) << ") " << std::endl;
        std::cout << "  and a dijet (pt0 > 30, pt1 > 20): " << nevt_dijet << " (" << (float(nevt_dijet)/nevt_total) << ") " << std::endl;
        std::cout << "  and photon pt0 > mgg/2: " << nevt_tightenpho << " (" << (float(nevt_tightenpho)/nevt_total) << ") " << std::endl;
        std::cout << "  and mgg > 250 (dijet presel): " << nevt_dijetpresel << " (" << (float(nevt_dijetpresel)/nevt_total) << ") " << std::endl;

        std::cout << std::endl << "END OF CUTFLOW, TWO FURTHER SEPARATEQUESTIONS, with full list of cuts (i.e. dijet presel) as denominator:" << std::endl << std::endl;
        std::cout << "    Fraction with multiple dipho/dijets to consider: " << nevt_dijetmultiple 
                  << nevt_dijetmultiple << " (" << (float(nevt_dijetmultiple)/nevt_dijetpresel) << ") " << std::endl;
        //        std::cout << "    Fraction with all 3 sublead jet methods giving same answer [highest pT(H) dijet only]: " << nevt_dijetallsame << " (" << (float(nevt_dijetallsame)/nevt_dijetpresel) << ") " << std::endl;
        
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

