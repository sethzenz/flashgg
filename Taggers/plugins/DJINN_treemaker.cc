// DJINNTreeMaker.cc by J. Wright
//
// * Produces dataset trees for DJINN training/evaluation
//
// Adapted from TagTestAnalyzer code by S. Zenz, which was
// Adapted from globelikeTreeMakerWithTagSorter code by L. D. Corpe, which was
// Adapted from the flashggCommissioning tree maker code  by C. Favaro et al.

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Common/interface/Handle.h"

#include "flashgg/DataFormats/interface/VBFTag.h"
#include "flashgg/DataFormats/interface/UntaggedTag.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/TTHHadronicTag.h"
#include "flashgg/DataFormats/interface/TTHLeptonicTag.h"
#include "flashgg/DataFormats/interface/VHTightTag.h"
#include "flashgg/DataFormats/interface/VHEtTag.h"
#include "flashgg/DataFormats/interface/VHLooseTag.h"
#include "flashgg/DataFormats/interface/VHHadronicTag.h"
#include "flashgg/DataFormats/interface/VBFTagTruth.h"
#include "flashgg/DataFormats/interface/ZPlusJetTag.h"

#include "TTree.h"

#include "flashgg/DataFormats/interface/Jet.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

using namespace std;
using namespace edm;

// ******************************************************************************************
// ******************************************************************************************

//Define the structs for filling the trees
struct JetStruct {
    std::vector<int> pdgIds;
    vector<float> constituents;
};

struct EventStruct {

    float weight;

    float lead_pt_m;
    float sublead_pt_m;
    float total_pt_m;
    float mass_gg;
    float mass_jj;
    float abs_dEta;
    float centrality;
    float dPhi_jj;
    float dPhi_ggjj;
    float minDR;

    float leadPhoton_eta;
    float subleadPhoton_eta;
    float cos_dPhi_gg;
    float leadPhotonID;
    float subleadPhotonID;
    float sigma_rv;
    float sigma_wv;
    float prob_vtx;

    float diphoton_score;

    float leadJetPt;
    float leadJetEta;
    float leadJetPhi;

    float subleadJetPt;
    float subleadJetEta;
    float subleadJetPhi;

    float leadJetRMS;
    float subleadJetRMS;

    float leadJetPUJID;
    float subleadJetPUJID;

    const TString eventVariableString = TString("weight/F:lead_pt_m/F:sublead_pt_m/F:total_pt_m/F:mass_gg/F:"
                                                "mass_jj/F:abs_dEta/F:centrality/F:dPhi_jj/F:dPhi_ggjj/F:minDR/F:"
                                                "leadPhoton_eta/F:subleadPhoton_eta/F:cos_dPhi_gg/F:leadPhotonID/F:subleadPhotonID/F:"
                                                "sigma_rv/F:sigma_wv/F:prob_vtx/F:diphoton_score/F:"
                                                "leadJetPt/F:leadJetEta/F:leadJetPhi/F:"
                                                "subleadJetPt/F:subleadJetEta/F:subleadJetPhi/F:"
                                                "leadJetRMS/F:subleadJetRMS/F:"
                                                "leadJetPUJID/F:subleadJetPUJID/F"
                                                );
};

//Dummy maker functions
JetStruct make_dummy_jet_struct(){

    JetStruct jetInfo;
    std::vector<float> dummy_vals(1,-999.);
    jetInfo.constituents = dummy_vals;

    return jetInfo;
};

EventStruct make_dummy_event_struct(){

    EventStruct eventInfo;

    eventInfo.weight = -999.;

    eventInfo.lead_pt_m = -999.;
    eventInfo.sublead_pt_m = -999.;
    eventInfo.total_pt_m = -999.;
    eventInfo.mass_gg = -999.;
    eventInfo.mass_jj = -999.;
    eventInfo.abs_dEta = -999.;
    eventInfo.centrality = -999.;
    eventInfo.dPhi_jj = -999.;
    eventInfo.dPhi_ggjj = -999.;
    eventInfo.minDR = -999.;

    eventInfo.leadPhoton_eta = -999.;
    eventInfo.subleadPhoton_eta = -999.;
    eventInfo.cos_dPhi_gg = -999.;
    eventInfo.leadPhotonID = -999.;
    eventInfo.subleadPhotonID = -999.;
    eventInfo.sigma_rv = -999.;
    eventInfo.sigma_wv = -999.;
    eventInfo.prob_vtx = -999.;

    eventInfo.diphoton_score = -999.;

    eventInfo.leadJetRMS = -999.;
    eventInfo.subleadJetRMS = -999.;

    eventInfo.leadJetPUJID = -999.;
    eventInfo.subleadJetPUJID = -999.;


    return eventInfo;
};

vector<int> partonMatchPdgIds(edm::Ptr<flashgg::Jet> jet, Handle<View<reco::GenParticle>> genParticles){

    vector<int> pdgIds;
    for (unsigned i=0;i<genParticles->size();i++){
        edm::Ptr<reco::GenParticle> particle = genParticles->ptrAt(i);
        if (particle->isHardProcess() &&
            (particle->pdgId() == 21 || abs(particle->pdgId()<7)) &&
            particle->pt() > 0.0){

            float dr = deltaR(jet->eta(),jet->phi(),particle->eta(),particle->phi());
            if (dr <= 0.4){
                pdgIds.push_back(particle->pdgId());
            }

        }
    }

    return pdgIds;
};
// **********************************************************************

namespace flashgg {

    class DJINNTreeMaker : public edm::EDAnalyzer
    {
    public:
        explicit DJINNTreeMaker( const edm::ParameterSet & );
        ~DJINNTreeMaker();

        static void fillDescriptions( edm::ConfigurationDescriptions &descriptions );


    private:

        edm::Service<TFileService> fs_;

        TTree *tree_;

        virtual void beginJob() override;
        virtual void analyze( const edm::Event &, const edm::EventSetup & ) override;
        virtual void endJob() override;

        edm::EDGetTokenT<edm::View<flashgg::DiPhotonTagBase> > TagSorterToken_;

        EDGetTokenT<View<DiPhotonCandidate>> diphotonToken_;
        EDGetTokenT<View<DiPhotonMVAResult>> mvaResultToken_;
        std::vector<edm::InputTag> inputTagJets_;
        std::vector<edm::EDGetTokenT<View<flashgg::Jet>>> tokenJets_;
        EDGetTokenT<GenEventInfoProduct> genInfoToken_;
        edm::EDGetTokenT<std::vector<PileupSummaryInfo>> puInfoToken_;
        EDGetTokenT<View<reco::GenParticle>> genPartToken_;

        double lumiWeight_;
        double xs_;

        bool expectMultiples_;

        typedef std::vector<edm::Handle<edm::View<flashgg::Jet>>> JetCollectionVector;

        //Jet rejection parameters
        double _rmsforwardCut;
        string _JetIDLevel;
        std::vector<double> _pujid_wp_pt_bin_1;
        std::vector<double> _pujid_wp_pt_bin_2;
        std::vector<double> _pujid_wp_pt_bin_3;

        bool _useJetID;

        //Pileup weight parameters
        std::vector<double> _dataPu;
        std::vector<double> _mcPu;
        std::vector<double> _puBins;
        edm::InputTag _puInfo;
        bool _puReWeight;
        edm::InputTag _rho;
        bool _useTruePu;
        edm::InputTag _vertexes;

        bool _isData;
        bool _getPu;

        std::vector<double> _puWeights;

        //Stuff for outuput
        EventStruct eventInfo_;

        JetStruct leadJetInfo_;
        JetStruct subleadJetInfo_;

        vector<int> leadPdgIds_;
        vector<int> subleadPdgIds_;

    };


// ******************************************************************************************
// ******************************************************************************************

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
    DJINNTreeMaker::DJINNTreeMaker( const edm::ParameterSet &iConfig ):
        TagSorterToken_( consumes<edm::View<flashgg::DiPhotonTagBase> >( iConfig.getParameter<InputTag> ( "TagSorter" ) ) ),
        diphotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        inputTagJets_ ( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        genInfoToken_( consumes<GenEventInfoProduct>( iConfig.getParameter<InputTag>( "genInfoTag" ) ) ),
        puInfoToken_(consumes<std::vector<PileupSummaryInfo> >( iConfig.getParameter<InputTag> ( "pileupInfo" ) ) ),
        genPartToken_( consumes<View<reco::GenParticle>> ( iConfig.getParameter<InputTag>("GenParticleTag"))),
        lumiWeight_( iConfig.getParameter<double>( "lumiWeight" ) ),
        xs_( iConfig.getParameter<double>( "xs" ) ),
        expectMultiples_( iConfig.getUntrackedParameter<bool>( "ExpectMultiples", false) ),
        _rmsforwardCut( iConfig.getParameter<double> ( "rmsforwardCut") ),
        _JetIDLevel   ( iConfig.getParameter<string> ( "JetIDLevel"   ) ),
        _pujid_wp_pt_bin_1  ( iConfig.getParameter<std::vector<double> > ( "pujidWpPtBin1" ) ),
        _pujid_wp_pt_bin_2  ( iConfig.getParameter<std::vector<double> > ( "pujidWpPtBin2" ) ),
        _pujid_wp_pt_bin_3  ( iConfig.getParameter<std::vector<double> > ( "pujidWpPtBin3" ) ),
        _useJetID( iConfig.getParameter<bool>( "useJetID" ) ),
        _dataPu  ( iConfig.getParameter<std::vector<double> > ( "dataPu" ) ),
        _mcPu    ( iConfig.getParameter<std::vector<double> > ( "mcPu" ) ),
        _puBins  ( iConfig.getParameter<std::vector<double> > ( "puBins" ) ),
        _puInfo  ( iConfig.getParameter<edm::InputTag > ( "puInfo" ) ),
        _puReWeight( iConfig.getParameter<bool>( "puReWeight" ) ),
        _rho     ( iConfig.getParameter<edm::InputTag > ( "rho" ) ),
        _useTruePu( iConfig.getParameter<bool>( "useTruePu" ) ),
        _vertexes( iConfig.getParameter<edm::InputTag > ( "vertexes" ) ),
        _isData( iConfig.getParameter<bool>( "isData" ) ),
        _getPu( iConfig.exists("puInfo") )
    {
        //Prep jet collection
        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }

        //Prep tree
        tree_ = fs_->make<TTree>("JetData","Jet images and jet variables");

        tree_->Branch("eventVars",&eventInfo_.weight,eventInfo_.eventVariableString);
        tree_->Branch("leadConstituents",&leadJetInfo_.constituents);
        tree_->Branch("subleadConstituents",&subleadJetInfo_.constituents);
        tree_->Branch("leadPdgIds",&leadPdgIds_);
        tree_->Branch("subleadPdgIds",&subleadPdgIds_);

        //Pileup weights
        _puWeights = _dataPu;

        auto scl  = std::accumulate(_mcPu.begin(),_mcPu.end(),0.) / std::accumulate(_puWeights.begin(),_puWeights.end(),0.); // rescale input distribs to unit ara
        for( size_t ib = 0; ib<_puWeights.size(); ++ib ) { _puWeights[ib] *= scl / _mcPu[ib]; }

    }

    DJINNTreeMaker::~DJINNTreeMaker()
    {

    }

    void
    DJINNTreeMaker::analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup )
    {

        // ********************************************************************************
        // access edm objects

        std::cout << "In analyze" << std::endl;
        
        Handle<edm::View<flashgg::DiPhotonTagBase> > TagSorter;
        iEvent.getByToken( TagSorterToken_, TagSorter );

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        iEvent.getByToken( mvaResultToken_, mvaResults );

        Handle<View<flashgg::DiPhotonCandidate> > diphotons;
        iEvent.getByToken( diphotonToken_, diphotons );

        Handle<View<reco::GenParticle>> genParticles;
        iEvent.getByToken(genPartToken_,genParticles);

        JetCollectionVector jetCollection( inputTagJets_.size() );
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
            iEvent.getByToken( tokenJets_[j], jetCollection[j] );
        }

        edm::Handle<GenEventInfoProduct> genInfo;
        iEvent.getByToken(genInfoToken_, genInfo);

        edm::Handle<std::vector<PileupSummaryInfo> > puInfo;
        iEvent.getByToken(puInfoToken_, puInfo);

        std::cout << "After tokens etc" << std::endl;

        //Scale (XS weight * lumi weight)
        float scale = 1.0;
        if (!_isData){
            scale = xs_*lumiWeight_;
        }

        //Generator weight
        float genWeight = 1.0;
        if (!_isData){
            genWeight = genInfo->weight();
        }

        //Pileup weight
        float puWeight = 1.0;
        if (!_isData && _getPu){
            
            double truePu = 0.0, obsPu = 0.0;
            for (auto & frame : *puInfo){
                if (frame.getBunchCrossing() == 0){
                    truePu = frame.getTrueNumInteractions();
                    obsPu = frame.getPU_NumInteractions();
                    break;
                }
            }

            float npu = (_useTruePu ? truePu : obsPu);
            if (_puReWeight){
                if (npu <= _puBins.front() || npu >= _puBins.back()){
                    puWeight = 0.0;
                }else{
                    int ibin = (std::lower_bound(_puBins.begin(),
                                                 _puBins.end(),
                                                 npu) - _puBins.begin()) - 1;
                    puWeight = _puWeights[ibin];
                }
            }
            
        }

        std::cout << "after pu" << std::endl;

        //Systematics weights
        float systWeight = 1.0;
        /*
            stuff...
        */
        
        float event_weight = scale*genWeight*puWeight*systWeight;
        


        edm::Ptr<flashgg::DiPhotonCandidate> diphoton;
        edm::Handle<edm::View<flashgg::Jet>> jets;
        flashgg::DiPhotonMVAResult mvares;

        unsigned count = 0;
        for ( auto tag = TagSorter.product()->begin(); tag != TagSorter.product()->end(); tag++) {

            count++;

            std::cout << " Tag " << count << std::endl;

            // ********************************************************************************
            // get the objects
            diphoton = tag->diPhoton();
            jets = jetCollection[diphoton->jetCollectionIndex()];
            mvares = tag->diPhotonMVA();
            
            std::cout << "Got objects" << std::endl;


            //----Select jets
            std::pair<int,int> dijet_indices(-999,-999);
            std::pair<float,float> dijet_pts(0.0,0.0);

            for (unsigned i=0;i<jets->size();i++){

                //The Jet
                edm::Ptr<flashgg::Jet> jet = jets->ptrAt(i);

                

                //Photon removal
                float dr_leadPhoton = deltaR(jet->eta(),jet->phi(),
                                             diphoton->leadingPhoton()->eta(),
                                             diphoton->leadingPhoton()->phi());
                float dr_subleadPhoton = deltaR(jet->eta(),jet->phi(),
                                                diphoton->subLeadingPhoton()->eta(),
                                                diphoton->subLeadingPhoton()->phi());
                if (dr_leadPhoton <= 0.4 || dr_subleadPhoton <= 0.4){
                    continue;
                }

                //----Jet ID/PUJID/RMS Cuts
                //PUJID
                std::vector<std::pair<double,double>> eta_cuts_(4);
                eta_cuts_[0] = std::make_pair (0    ,2.50 );
                eta_cuts_[1] = std::make_pair (2.50 ,2.75 );
                eta_cuts_[2] = std::make_pair (2.75 ,3.00 );
                eta_cuts_[3] = std::make_pair (3.00 ,10);

                if ( (!_pujid_wp_pt_bin_1.empty())  &&
                     (!_pujid_wp_pt_bin_2.empty())  &&
                     (!_pujid_wp_pt_bin_3.empty())  ){
                    bool pass=false;
                    for (UInt_t eta_bin=0; eta_bin < _pujid_wp_pt_bin_1.size(); eta_bin++ ){
                        if ( fabs( jet->eta() ) >  eta_cuts_[eta_bin].first &&
                             fabs( jet->eta() ) <= eta_cuts_[eta_bin].second){
                            if ( jet->pt() >  20 &&
                                 jet->pt() <= 30 && jet->puJetIdMVA() > _pujid_wp_pt_bin_1[eta_bin] )
                                pass=true;
                            if ( jet->pt() >  30 &&
                                 jet->pt() <= 50 && jet->puJetIdMVA() > _pujid_wp_pt_bin_2[eta_bin] )
                                pass=true;
                            if ( jet->pt() >  50 &&
                                 jet->pt() <= 100&& jet->puJetIdMVA() > _pujid_wp_pt_bin_3[eta_bin] )
                                pass=true;
                            if (jet->pt() > 100) pass = true;
                        }
                    }
                    if (!pass) continue;
                }

                //RMS
                if( fabs( jet->eta() ) > 2.5 && jet->rms() > _rmsforwardCut ){ 
                    continue; 
                }

                //JetID
                if( _useJetID ){
                    if( _JetIDLevel == "Loose" && !jet->passesJetID( flashgg::Loose ) ) continue;
                    if( _JetIDLevel == "Tight" && !jet->passesJetID( flashgg::Tight ) ) continue;
                }
               
                // Abs eta cut at 4.7
                if( fabs( jet->eta() ) > 4.7 ) { continue; }

                //----Selection
                //Conditionals for dijet selection
                if (jets->ptrAt(i)->pt() > dijet_pts.first){
                    dijet_indices.second = dijet_indices.first;
                    dijet_pts.second = dijet_pts.first;
                    dijet_indices.first = i;
                    dijet_pts.first = jets->ptrAt(i)->pt();
                }else if (jets->ptrAt(i)->pt() > dijet_pts.second){
                    dijet_indices.second = i;
                    dijet_pts.second = jets->ptrAt(i)->pt();
                }

            }

            // ********************************************************************************
            // do stuff with selected objects

            //If there's no valid dijet we're not interested...
            if (dijet_indices.first >= 0 && dijet_indices.second >= 0){

                edm::Ptr<flashgg::Jet> leadJet = jets->ptrAt(dijet_indices.first);
                edm::Ptr<flashgg::Jet> subleadJet = jets->ptrAt(dijet_indices.second);

                reco::Candidate::LorentzVector p1 = diphoton->leadingPhoton()->p4();
                reco::Candidate::LorentzVector p2 = diphoton->subLeadingPhoton()->p4();
                reco::Candidate::LorentzVector j1 = leadJet->p4();
                reco::Candidate::LorentzVector j2 = subleadJet->p4();

                eventInfo_.weight = event_weight;

                eventInfo_.lead_pt_m = mvares.leadptom;
                eventInfo_.sublead_pt_m = mvares.subleadptom;
                eventInfo_.total_pt_m = diphoton->pt()/diphoton->mass();
                eventInfo_.mass_gg = diphoton->mass();
                eventInfo_.mass_jj = (j1+j2).mass();
                eventInfo_.abs_dEta = fabs(j1.eta()-j2.eta());
                eventInfo_.centrality = exp((-4.0/pow(j1.eta()-j2.eta(),2))*pow((p1+p2).eta() - 0.5*(j1.eta()+j2.eta()),2));
                eventInfo_.dPhi_jj = fabs(deltaPhi(j1.phi(),j2.phi()));
                eventInfo_.dPhi_ggjj = fabs(deltaPhi( (p1+p2).phi(), (j1+j2).phi() ));
                
                float dr_p1_j1 = deltaR(p1.eta(),p1.phi(),j1.eta(),j1.phi());
                float dr_p2_j1 = deltaR(p2.eta(),p2.phi(),j1.eta(),j1.phi());
                float dr_p1_j2 = deltaR(p1.eta(),p1.phi(),j2.eta(),j2.phi());
                float dr_p2_j2 = deltaR(p2.eta(),p2.phi(),j2.eta(),j2.phi());
                eventInfo_.minDR = min(min(dr_p1_j1,dr_p2_j1),min(dr_p1_j2,dr_p2_j2));

                eventInfo_.leadPhoton_eta = mvares.leadeta;
                eventInfo_.subleadPhoton_eta = mvares.subleadeta;
                eventInfo_.cos_dPhi_gg = mvares.CosPhi;
                eventInfo_.leadPhotonID = mvares.leadmva;
                eventInfo_.subleadPhotonID = mvares.subleadmva;
                eventInfo_.sigma_rv = mvares.sigmarv;
                eventInfo_.sigma_wv = mvares.sigmawv;
                eventInfo_.prob_vtx = mvares.vtxprob;
                eventInfo_.diphoton_score = mvares.result;

                eventInfo_.leadJetPt = j1.pt();
                eventInfo_.leadJetEta = j1.eta();
                eventInfo_.leadJetPhi = j1.phi();
                eventInfo_.subleadJetPt = j2.pt();
                eventInfo_.subleadJetEta = j2.eta();
                eventInfo_.subleadJetPhi = j2.phi();

                eventInfo_.leadJetRMS = leadJet->rms();
                eventInfo_.subleadJetRMS = subleadJet->rms();
                eventInfo_.leadJetPUJID = leadJet->puJetIdMVA();
                eventInfo_.subleadJetPUJID = subleadJet->puJetIdMVA();

                //Jet structured Info
                leadJetInfo_.constituents = leadJet->getConstituentInfo();
                subleadJetInfo_.constituents = subleadJet->getConstituentInfo();

                leadPdgIds_ = partonMatchPdgIds(leadJet,genParticles); 
                subleadPdgIds_ = partonMatchPdgIds(subleadJet,genParticles); 

                tree_->Fill();

            }else{
            }
        }

             
    } // analyze

    void
    DJINNTreeMaker::beginJob()
    {
    }

    void
    DJINNTreeMaker::endJob()
    {
    }

    void
    DJINNTreeMaker::fillDescriptions( edm::ConfigurationDescriptions &descriptions )
    {
        //The following says we do not know what parameters are allowed so do no validation
        // Please change this to state exactly what you do use, even if it is no parameters
        edm::ParameterSetDescription desc;
        desc.setUnknown();
        descriptions.addDefault( desc );
    }

} // namespace flashgg

typedef flashgg::DJINNTreeMaker FlashggDJINNTreeMaker;
DEFINE_FWK_MODULE( FlashggDJINNTreeMaker );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

