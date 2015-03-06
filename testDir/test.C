#include <string>

#include "../smallPhotonUtil.h"
using  std::string;
using  std::cout;
using  std::endl;

const char *type1 = "TDirectoryFile";

void test()
{

  TFile *f = new TFile("~/Desktop/photonCuts_out.root","READ");

/*  TList *myKeys=getListOfALLKeys(f);
//    TList *myKeys=getListOfALLKeys(f,"TH1D");
  TIter *next=new TIter(myKeys);
  TKey *key;
  while ((key=(TKey*)next())) {
	  	  cout<< key->GetName() << " - " << key->GetClassName() << endl;
	}*/

  saveAllHistogramsToPicture(f);
//  saveAllHistogramsToPicture(f,"pdf");

//  TString a(Form("asdasd.root"));
//  a.ReplaceAll(".root","ASDAS");
//  cout<<a<<endl;
}
