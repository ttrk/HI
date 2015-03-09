#include <string>

#include "../smallPhotonUtil.h"
using  std::string;
using  std::cout;
using  std::endl;

const char *type1 = "TDirectoryFile";

void test()
{

  TFile *f = new TFile("~/Desktop/photonCuts_out_pt40_run2.root","READ");
//  TSystem t;
//  const char* baseName=gSystem->BaseName(f->GetName());
////    const char* baseName=basename(f->GetName());	// NOT working
//  TString dirName(baseName);
//  dirName.ReplaceAll(".root","");
//  gSystem->mkdir(dirName);


//  saveAllHistogramsToPicture(f,"gif", dirName);
    saveAllHistogramsToPicture(f,"gif",1);

//  saveAllHistogramsToPicture(f,"pdf");

//  TString a(Form("asdasd.root"));
//  a.ReplaceAll(".root","ASDAS");
//  cout<<a<<endl;
}
