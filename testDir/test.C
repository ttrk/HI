#include <string>

#include "../smallPhotonUtil.h"
using  std::string;
using  std::cout;
using  std::endl;

const char *type1 = "TDirectoryFile";

void test()
{

  TFile *f = new TFile("~/Desktop/photonCuts_out_pt40_run1.root","READ");
//  TSystem t;
//  const char* baseName=gSystem->BaseName(f->GetName());
////    const char* baseName=basename(f->GetName());	// NOT working
//  TString dirName(baseName);
//  dirName.ReplaceAll(".root","");
//  gSystem->mkdir(dirName);


//  saveAllHistogramsToPicture(f,"gif", dirName);
//    saveAllHistogramsToPicture(f,"gif",1);

	TH1D*  h;
	TH1D*  h2;
	TKey*  key;
    TList* keysHisto=getListOfALLKeys(f, "TH1D");
	TIter* iter = new TIter(keysHisto);
	TCanvas* c1=new TCanvas();
	while ((key=(TKey*)iter()))
	{
		h = (TH1D*)key->ReadObj();
		h2= (TH1D*)key->ReadObj();
		if(strcmp(h->GetName(), "photon_pt_after_eta") == 0)	// save in the current directory if no directory is specified
		{
			h->Draw();
			h2->Scale(1/h2->GetEntries());
		    TCanvas* c2=new TCanvas();
			h2->Draw();
		}
	}

//  saveAllHistogramsToPicture(f,"pdf");

//  TString a(Form("asdasd.root"));
//  a.ReplaceAll(".root","ASDAS");
//  cout<<a<<endl;
}
