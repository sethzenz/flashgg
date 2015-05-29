// Benoit Courbon
// https://raw.githubusercontent.com/bcourbon/flashgg/132cd5163b3564da81b79b91829e9aec47126b78/TagAlgos/src/WorkspaceCombiner.cc

#include "../interface/WorkspaceCombiner.h"

using namespace std;
using namespace RooFit;

// ----------------------------------------------------------------------------------------------------

WorkspaceCombiner::WorkspaceCombiner()
{
}

// ----------------------------------------------------------------------------------------------------

WorkspaceCombiner::~WorkspaceCombiner()
{
}

// ----------------------------------------------------------------------------------------------------

void WorkspaceCombiner::Init(string outputFileName_,vector<string> inputfiles_){

	inputFileNames.clear();
	workspaceNames.clear();
	workspacePaths.clear();
	data.clear();
	trees.clear();
	histos.clear();
	treePaths.clear();
	histoPaths.clear();

	for(unsigned int f=0;f<inputfiles_.size();f++) inputFileNames.push_back(inputfiles_[f]);

	outputFileName=outputFileName_;

}

// ----------------------------------------------------------------------------------------------------

TDirectoryFile* WorkspaceCombiner::GetFirstFile(){

	TDirectoryFile *file0=TFile::Open(inputFileNames[0].c_str());
	return file0;

}

// ----------------------------------------------------------------------------------------------------
void WorkspaceCombiner::GetWorkspaces(TDirectoryFile *file){

	TList *listofkeys=file->GetListOfKeys();
	for(int k=0;k<listofkeys->GetSize();k++){
		TKey *key=(TKey*)listofkeys->At(k);
  		if(strcmp(key->GetClassName(),"RooWorkspace")==0){
  			RooWorkspace *work=(RooWorkspace*)file->Get(key->GetName());
			//work->Print();
  			std::list<RooAbsData*> allData = work->allData();
  			vector<RooDataSet*> allDataClone;
  			for(std::list<RooAbsData *>::iterator it=allData.begin(); it!=allData.end(); ++it) {
		        	RooDataSet *dataset = dynamic_cast<RooDataSet *>(*it);
				allDataClone.push_back((RooDataSet*)dataset->Clone());
  			}
			data.push_back(allDataClone);
			workspaceNames.push_back(work->GetName());
			string workpath="";
			while(file->InheritsFrom("TFile")==false){
				workpath.insert(0,Form("/%s",file->GetName()));
				file=(TDirectoryFile*)file->GetMotherDir();
			}
			workpath.erase(0,1);
			workspacePaths.push_back(workpath);
			delete work;
		}
		if(strcmp(key->GetClassName(),"TDirectoryFile")==0){
		TDirectoryFile *newdirectory=(TDirectoryFile*)file->Get(key->GetName());
		GetWorkspaces(newdirectory);
		}
  	}
  }	

// ----------------------------------------------------------------------------------------------------

void WorkspaceCombiner::MergeWorkspaces(){

	for(unsigned int f=1;f<inputFileNames.size();f++){	  
    
  		TFile *file=TFile::Open(inputFileNames[f].c_str());
	  
		cout <<endl<< "Combining Current File " << f << " / " << inputFileNames.size() << " - " << inputFileNames[f] << endl<<endl;
	 
		file->cd();

		for(unsigned int w=0;w<workspaceNames.size();w++){
	  		RooWorkspace *work;
			if(workspacePaths[w]=="") work = (RooWorkspace*) file->Get(workspaceNames[w].c_str()); //look into root folder
			else{										   //look into directory
				TDirectory *dir=(TDirectory*) file->Get(workspacePaths[w].c_str());
				work=(RooWorkspace*) dir->Get(workspaceNames[w].c_str());
			}
			//work->Print();
			std::list<RooAbsData*> allData = work->allData();
			int d=0;
  			for(std::list<RooAbsData *>::iterator it=allData.begin(); it!=allData.end(); ++it) {
		        	RooDataSet *dataset = dynamic_cast<RooDataSet *>(*it);
  				data[w][d]->append(*dataset);
  				//data[w][d]->Print();			
				d++;
  			}
		}

	file->Close();
 
        cout <<endl<< "Finished Combining File - " << inputFileNames[f] << endl<<endl;
	  
	}	 
}

// ----------------------------------------------------------------------------------------------------
TDirectoryFile* WorkspaceCombiner::MergeTreesAndHistograms(){

	std::string inputchain=inputFileNames[0];
	for(unsigned int i=1;i<inputFileNames.size();i++){
		inputchain.append(" ");
		inputchain.append(inputFileNames[i]);
	}
 	
	outputAux="outputTreesAndHistos.root";

	gSystem->Exec(Form("hadd -f %s %s",outputAux.c_str(),inputchain.c_str()));

	TDirectoryFile *outputAuxFile=TFile::Open(outputAux.c_str());

	return outputAuxFile;

}

// ----------------------------------------------------------------------------------------------------
void WorkspaceCombiner::GetTreesAndHistograms(TDirectoryFile *file){

	TList *listofkeys=file->GetListOfKeys();
	for(int k=0;k<listofkeys->GetSize();k++){
		TKey *key=(TKey*)listofkeys->At(k);
  		if(strcmp(key->GetClassName(),"TTree")==0){
			TTree *tree=new TTree;
  			tree=(TTree*)file->Get(key->GetName());
			trees.push_back(tree);
			string treepath="";
			TDirectoryFile *fileclone=file;
			while(fileclone->InheritsFrom("TFile")==false){
				treepath.insert(0,Form("/%s",fileclone->GetName()));
				fileclone=(TDirectoryFile*)fileclone->GetMotherDir();
			}
			treepath.erase(0,1);
			treePaths.push_back(treepath);
		}
		if(strcmp(key->GetClassName(),"TH1F")==0){
  			TH1F *histo1=(TH1F*)file->Get(key->GetName());
			histos.push_back(histo1);
			string histopath="";
			TDirectoryFile *fileclone=file;
			while(fileclone->InheritsFrom("TFile")==false){
				histopath.insert(0,Form("/%s",fileclone->GetName()));
				fileclone=(TDirectoryFile*)fileclone->GetMotherDir();
			}
			histopath.erase(0,1);
			histoPaths.push_back(histopath);
		}
		if(strcmp(key->GetClassName(),"TH2F")==0){
  			TH2F *histo2=(TH2F*)file->Get(key->GetName());
			histos.push_back(histo2);
			string histopath="";
			TDirectoryFile *fileclone=file;
			while(fileclone->InheritsFrom("TFile")==false){
				histopath.insert(0,Form("/%s",fileclone->GetName()));
				fileclone=(TDirectoryFile*)fileclone->GetMotherDir();
			}
			histopath.erase(0,1);
			histoPaths.push_back(histopath);
		}
		if(strcmp(key->GetClassName(),"TDirectoryFile")==0){
		TDirectoryFile *newdirectory=(TDirectoryFile*)file->Get(key->GetName());
		GetTreesAndHistograms(newdirectory);
		}
  	}

}
// ----------------------------------------------------------------------------------------------------

void WorkspaceCombiner::Save(){

  TFile *outfile = TFile::Open(outputFileName.c_str(), "RECREATE", "ROOT file with merge workspace");


	for(unsigned int w=0;w<workspaceNames.size();w++){ 
        	RooWorkspace *outputws = new RooWorkspace(); 
        	outputws->SetName(workspaceNames[w].c_str());
  		for(unsigned int d=0;d<data[w].size();d++) {
			outputws->import(*data[w][d]);
			//data[w][d]->Print();
		}
		if(outfile->GetDirectory(workspacePaths[w].c_str())==false) outfile->mkdir(workspacePaths[w].c_str());
		outfile->cd(workspacePaths[w].c_str());
  		outputws->Write();
		//outputws->Print();
		delete outputws;
	}

	for(unsigned int t=0;t<trees.size();t++){ 
		if(outfile->GetDirectory(treePaths[t].c_str())==false) outfile->mkdir(treePaths[t].c_str());
		outfile->cd(treePaths[t].c_str());
  		trees[t]->CloneTree()->Write();
		//trees[t]->CloneTree()->Print();
	}

	for(unsigned int h=0;h<histos.size();h++){ 
		if(outfile->GetDirectory(histoPaths[h].c_str())==false) outfile->mkdir(histoPaths[h].c_str());
		outfile->cd(histoPaths[h].c_str());
  		histos[h]->Write();
		//histos[h]->Print();
	}

	outfile->Close();
	gSystem->Exec(Form("rm %s",outputAux.c_str()));
}

// ----------------------------------------------------------------------------------------------------

