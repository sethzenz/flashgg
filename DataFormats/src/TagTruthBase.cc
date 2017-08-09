#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "flashgg/DataFormats/interface/UncertaintyScheme.h"

using namespace flashgg;

TagTruthBase::TagTruthBase( const TagTruthBase &b )
{
    copyBaseInfo( b );
}

TagTruthBase *TagTruthBase::clone() const
{
    TagTruthBase *result = new TagTruthBase( *this );
    return result;
}

void TagTruthBase::setHTXSInfo( int stage0cat, int stage1cat, int njets, float pTH, float pTV ) {
    stage0cat_ = stage0cat;
    stage1cat_ = stage1cat;
    njets_ = njets;
    pTH_ = pTH;
    pTV_ = pTV;
    setggHweights( njets, pTH, stage1cat );
}

void TagTruthBase::copyBaseInfo( const TagTruthBase &b ) {
    setGenPV( b.genPV() );
    setHTXSInfo( b.HTXSstage0cat(), b.HTXSstage1cat(), b.HTXSnjets(), b.HTXSpTH(), b.HTXSpTV() );
}

//in order to set the ggH weights, use suggested code (in header file)
// order: THU_ggH_Mu, THU_ggH_Res, THU_ggH_Mig01, THU_ggH_Mig12, THU_ggH_VBF2j, THU_ggH_VBF3j, THU_ggH_PT60, THU_ggH_PT120, THU_ggH_qmtop
void TagTruthBase::setggHweights( int njets, float pTH, int stage1cat ) 
{
  std::vector<float> theWeightsUp  = qcd_ggF_uncertSF_2017( njets, pTH, stage1cat, 1. );
  ggHweights_["THU_ggH_MuUp01sigma"]    = theWeightsUp[0];
  ggHweights_["THU_ggH_ResUp01sigma"]   = theWeightsUp[1];
  ggHweights_["THU_ggH_Mig01Up01sigma"] = theWeightsUp[2];
  ggHweights_["THU_ggH_Mig12Up01sigma"] = theWeightsUp[3];
  ggHweights_["THU_ggH_VBF2jUp01sigma"] = theWeightsUp[4];
  ggHweights_["THU_ggH_VBF3jUp01sigma"] = theWeightsUp[5];
  ggHweights_["THU_ggH_PT60Up01sigma"]  = theWeightsUp[6];
  ggHweights_["THU_ggH_PT120Up01sigma"] = theWeightsUp[7];
  ggHweights_["THU_ggH_qmtopUp01sigma"] = theWeightsUp[8];

  std::vector<float> theWeightsDown = qcd_ggF_uncertSF_2017( njets, pTH, stage1cat, -1. );
  ggHweights_["THU_ggH_MuDown01sigma"]    = theWeightsDown[0];
  ggHweights_["THU_ggH_ResDown01sigma"]   = theWeightsDown[1];
  ggHweights_["THU_ggH_Mig01Down01sigma"] = theWeightsDown[2];
  ggHweights_["THU_ggH_Mig12Down01sigma"] = theWeightsDown[3];
  ggHweights_["THU_ggH_VBF2jDown01sigma"] = theWeightsDown[4];
  ggHweights_["THU_ggH_VBF3jDown01sigma"] = theWeightsDown[5];
  ggHweights_["THU_ggH_PT60Down01sigma"]  = theWeightsDown[6];
  ggHweights_["THU_ggH_PT120Down01sigma"] = theWeightsDown[7];
  ggHweights_["THU_ggH_qmtopDown01sigma"] = theWeightsDown[8];

  //for( uint i=0; i<ggHweights_.size(); i++ ) {
  //  std::cout << "Weight " << i << " = " << ggHweights_[i] << std::endl;
  //}
}


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
