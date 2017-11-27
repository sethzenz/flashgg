// system include files
#include <memory>
#include <cmath>
#include <sstream>

#include "TString.h"


//Define the structs for filling the trees
struct JetStruct {
    float pt;
    float eta;
    float phi;
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

};

//Maker functions
JetStruct make_jet_struct(edm::Ptr<flashgg::Jet> jet){

    JetStruct jetInfo;
    jetInfo.pt = jet->pt();
    jetInfo.eta = jet->eta();
    jetInfo.phi = jet->phi();
    jetInfo.constituents = jet->getConstituentInfo();

    return jetInfo;
};

EventStruct make_event_struct(edm::Ptr<flashgg::DiPhotonCandidate diphoton,
                               edm::Ptr<flashgg::Jet> leadJet,
                               edm::Ptr<flashgg::Jet> subleadJet){

    reco::Candidate::LorentzVector p1 = diphoton->leadingPhoton()->p4();
    reco::Candidate::LorentzVector p2 = diphoton->subleadingPhoton()->p4();
    reco::Candidate::LorentzVector j1 = leadJet->p4();
    reco::Candidate::LorentzVector j2 = subleadJet->p4();

    EventStruct eventInfo;

    eventInfo.lead_pt_m = p1.pt()/(p1+p2).mass();
    eventInfo.sublead_pt_m = p2.pt()/(p1+p2).mass();  
    eventInfo.total_pt_m = (p1.pt()+p2.pt())/(p1+p2).mass(); 
    eventInfo.mass_gg = (p1+p2).mass();
    eventInfo.mass_jj = (j1+j2).mass();
    eventInfo.abs_dEta = fabs(j1.eta()-j2.eta());
    eventInfo.centrality = exp((-4.0/pow(j1.eta()-j2.eta(),2))*pow((p1+p2).eta() - 0.5*(j1.eta()+j2.eta()),2));
    eventInfo.dPhi_jj = deltaPhi(j1.phi(),j2.phi());
    eventInfo.dPhi_ggjj = deltaPhi( (p1+p2).phi(), (j1+j2).phi() );

    float dr_p1_j1 = deltaR(p1.eta(),p1.phi(),j1.eta(),j1.phi());
    float dr_p2_j1 = deltaR(p2.eta(),p2.phi(),j1.eta(),j1.phi());
    float dr_p1_j2 = deltaR(p1.eta(),p1.phi(),j2.eta(),j2.phi());
    float dr_p2_j2 = deltaR(p2.eta(),p2.phi(),j2.eta(),j2.phi());

    eventInfo.minDR = min(min(dr_p1_j1,dr_p2_j1),min(dr_p1_j2,dr_p2_j2));

    eventInfo.leadPhoton_eta = p1.eta();
    eventInfo.subleadPhoton_eta = p2.eta();
    eventInfo.cos_dPhi_gg = cos( deltaPhi(p1.phi(),p2.phi()) );
    eventInfo.leadPhotonID = diphoton->leadPhotonId();
    eventInfo.subleadPhotonID = diphoton->subleadPhotonId();
    eventInfo.sigma_rv = 0;
    eventInfo.sigma_wv = 0;
    eventInfo.prob_vtx = diphoton->vtxProbMVA();

    eventInfo.diphoton_score = 0;

    return eventInfo;
};

//Dummy maker functions
JetStruct make_dummy_jet_struct(){

    JetStruct jetInfo;
    jetInfo.pt = -999.;
    jetInfo.eta = -999.; 
    jetInfo.phi =  -999.;
    jetInfo.constituents = -999.;

    return jetInfo;
};

EventStruct make_dummy_event_struct(){

    EventStruct eventInfo;
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


    return eventInfo;
};




