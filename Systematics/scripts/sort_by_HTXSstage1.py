from ROOT import *

#import gc
#gc.set_debug(gc.DEBUG_LEAK)
#gc.disable()

stage1catname2num = {"UNKNOWN":0, 
                     # Gluon fusion
                     "GG2H_FWDH" : 100, "GG2H_VBFTOPO_JET3VETO" : 101, "GG2H_VBFTOPO_JET3" : 102, "GG2H_0J" : 103, 
                     "GG2H_1J_PTH_0_60" : 104, "GG2H_1J_PTH_60_120" : 105, "GG2H_1J_PTH_120_200" : 106, "GG2H_1J_PTH_GT200" : 107,
                     "GG2H_GE2J_PTH_0_60" : 108, "GG2H_GE2J_PTH_60_120" : 109, "GG2H_GE2J_PTH_120_200" : 110, "GG2H_GE2J_PTH_GT200" : 111, 
                     # "VBF"
                     "VBF_FWDH" : 200, "VBF_VBFTOPO_JET3VETO" : 201, "VBF_VBFTOPO_JET3" : 202, "VBF_VH2JET" : 203, "VBF_REST" : 204, "VBF_PTJET1_GT200" : 205
                     # qq -> WH
                     "QQ2HLNU_FWDH" = 300, "QQ2HLNU_PTV_0_150" = 301, "QQ2HLNU_PTV_150_250_0J" = 302, "QQ2HLNU_PTV_150_250_GE1J" = 303, "QQ2HLNU_PTV_GT250" = 304,
                     # qq -> ZH
                     "QQ2HLL_FWDH" = 400, "QQ2HLL_PTV_0_150" = 401, "QQ2HLL_PTV_150_250_0J" = 402, "QQ2HLL_PTV_150_250_GE1J" = 403, "QQ2HLL_PTV_GT250" = 404,
                     # gg -> ZH
                     "GG2HLL_FWDH" = 500, "GG2HLL_PTV_0_150" = 501, "GG2HLL_PTV_GT150_0J" = 502, "GG2HLL_PTV_GT150_GE1J" = 503,
                     # ttH
                     "TTH_FWDH" = 600, "TTH" = 601,
                     # bbH
                     "BBH_FWDH" = 700, "BBH" = 701,
                     # tH
                     "TH_FWDH" = 800, "TH" = 801}
stage1catnum2name = {v: k for k, v in stage1catname2num.iteritems()}
relevantstage1cats = [cat for cat in stage1catname2num.values()] #if cat > 0 and (cat <= 49 or cat == 60 or cat == 61)]
#relevantstage1cats = [40] # for debugging
#print relevantstage1cats

from sys import argv
fn = "output.root"
if len(argv) > 1:
   fn = argv[1]
assert(fn.count(".root"))

_file = TFile(fn)
_ws = _file.Get("tagsDumper/cms_hgg_13TeV")
_data = _ws.allData()
#print _data

files = {}
wss = {}

for cat in relevantstage1cats:
   wss[cat] = RooWorkspace("cms_hgg_13TeV","cms_hgg_13TeV")
   getattr(wss[cat],'import')(_ws.var("IntLumi"))

for ds in _data:
   initw = ds.sumEntries()
   sumfinw = 0.
   print "STARTING DATASET:",ds.GetName(),"weight:",ds.sumEntries()
   remainingArgList = RooArgSet(ds.get())
   anarg = ds.get().find("stage1cat")
   if not anarg:
      print "CANNOT SPLIT",ds.GetName(),"BECAUSE IT HAS NO stage1cat"
      continue
   remainingArgList.remove(anarg)
   for cat in relevantstage1cats:
      newds = ds.reduce(remainingArgList,"stage1cat==%i" % cat)
      getattr(wss[cat],'import')(newds)
      print "  ENDING DATASET for category %i (%s):"%(cat,stage1catnum2name[cat]),newds.GetName(),"weight:",newds.sumEntries()
      sumfinw += newds.sumEntries()
   if (initw == 0. and abs(sumfinw) > 0.0001) or (initw > 0. and abs((initw - sumfinw)/initw) > 0.001):
      if len(relevantstage1cats)==1:
         # debugging/testing
         print " DISAGREEMENT IN DATASET SUMWEIGHTS BEFORE AND AFTER:",initw,sumfinw
      else:
         raise Exception," DISAGREEMENT IN DATASET SUMWEIGHTS BEFORE AND AFTER: %.4f %.4f"%(initw,sumfinw)

for cat in relevantstage1cats:
#   print cat
#   wss[cat].Print()
   newfn = fn.replace(".root","_%s.root" % stage1catnum2name[cat])
   newf = TFile(newfn,"RECREATE")
   newf.mkdir("tagsDumper")
   newf.cd("tagsDumper")
   wss[cat].Write()
   newf.Close()
#   print "end of",cat
   del(wss[cat]) # must happen before python does automatic garbage collection of datasets
#   print "deleted workspace"

#print "end of script"
