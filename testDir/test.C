#include <string>

#include <TLegend.h>

#include "../smallPhotonUtil.h"
#include "../systemUtils.h"

using  std::string;
using  std::cout;
using  std::endl;

const char *type1 = "TDirectoryFile";

void test()
{
  TList* histos_pt40=getListOfALLHistograms(new TFile("~/Desktop/histos/photonCuts_out_all_pt40_run0.root","READ"));
  TList* histos_pt60=getListOfALLHistograms(new TFile("~/Desktop/histos/photonCuts_out_all_pt60_run0.root","READ"));
  TList* histos_pt80=getListOfALLHistograms(new TFile("~/Desktop/histos/photonCuts_out_all_pt80_run0.root","READ"));

  TList* canvases=new TList();

  TH1D* h1;
  TH1D* h2;
  TH1D* h3;
  TCanvas* c1=new TCanvas("c1");	// must be here

  double xcor1;
  double ycor1;
  double xcoroff;
  double ycoroff;
  for(int i=0; i<histos_pt40->GetEntries(); i++)
  {
		  h1=(TH1D*)histos_pt40->At(i);
		  h2=(TH1D*)histos_pt60->At(i);
		  h3=(TH1D*)histos_pt80->At(i);

/*		  cout<<h1->GetNbinsX()<<endl;
		  cout<<h1->GetXaxis()->GetXmin()<<endl;
		  cout<<h1->GetXaxis()->GetXmax()<<endl;*/

		  h1->Rebin(1);
		  h2->Rebin(1);
		  h3->Rebin(1);

//		  h1->SetAxisRange(20,200);
//		  h1->SetBins(20,h1->GetXaxis()->GetXmin(),h1->GetXaxis()->GetXmax());
//		  h2->SetBins(20,h2->GetXaxis()->GetXmin(),h2->GetXaxis()->GetXmax());
//		  h3->SetBins(20,h3->GetXaxis()->GetXmin(),h3->GetXaxis()->GetXmax());

/*		  cout<<"-----------"<<endl;
		  cout<<h1->GetNbinsX()<<endl;
		  cout<<h1->GetXaxis()->GetXmin()<<endl;
		  cout<<h1->GetXaxis()->GetXmax()<<endl;*/

		  h1->Draw();
		  h2->Draw("same");
		  h3->Draw("same");

		  h1->SetLineWidth(2);
		  h2->SetLineWidth(2);
		  h3->SetLineWidth(2);

		  h2->SetLineColor(kRed);
		  h3->SetLineColor(kGreen);

		  xcor1=0.8;
		  ycor1=0.55;
		  xcoroff=0.2;
		  ycoroff=0.3;

		  TLegend* leg = new TLegend(xcor1,ycor1,xcor1+xcoroff,ycor1+xcoroff);
		  leg->AddEntry(h1, "p_{T}^{#gamma}>40");
		  leg->AddEntry(h2, "p_{T}^{#gamma}>60");
		  leg->AddEntry(h3, "p_{T}^{#gamma}>80");
		  leg->SetHeader("different p_{T}^{#gamma} cuts");
		  leg->Draw();

//		  c1->SetLogy();		// some curves may not appear in linear scale
		  c1->SetName(h1->GetName());

		  c1->SaveAs(Form("~/Desktop/histos/drawsameAll1/%s.gif",c1->GetName()));

//		  canvases->Add();
  }

  c1->Close();

//  saveAllCanvasesToPicture(canvases, "gif","~/Desktop/histos/cnvs");

  //  TFile *f = new TFile("~/Desktop/photonCuts_out_full.root","READ");
  /*TSystem t;
  const char* baseName=gSystem->BaseName(f->GetName());
//    const char* baseName=basename(f->GetName());	// NOT working
  TString dirName(baseName);
  dirName.ReplaceAll(".root","");
  gSystem->mkdir(dirName);*/


//  saveAllHistogramsToPicture(f,"gif", dirName);
//  saveAllHistogramsToPicture(f,"gif",1);

/*  TList* histos=getListOfALLHistograms(f);

  TH1D*  h;
  TIter* iter = new TIter(histos);
  while ((h=(TH1D*)iter->Next()))
  	{
  		cout << h->GetName() << endl;
  	}*/

/*	TH1D*  h;
	TH1D*  h2;
	TKey*  key;
    TList* keysHisto=getListOfALLKeys(f, "TH1D");
	TIter* iter = new TIter(keysHisto);
	TCanvas* c1=new TCanvas();
	while ((key=(TKey*)iter->Next()))
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
	}*/



//  saveAllHistogramsToPicture(f,"pdf");

//  TString a(Form("asdasd.root"));
//  a.ReplaceAll(".root","ASDAS");
//  cout<<a<<endl;
}

void test2()
{
	int ptCut=40;
	TFile* f1=new TFile(Form("~/Desktop/histos/photonCuts_out_all_pt%d_run1.root", ptCut),"READ");
	TFile* f2=new TFile(Form("~/Desktop/histos/photonCuts_out_all_pt%d_run2.root", ptCut),"READ");

	TList* histos_Division=divideHistogramList(f1,f2);
	saveAllHistogramsToFile(Form("~/Desktop/histos/photonCuts_out_all_pt%d_ratio.root", ptCut),histos_Division);

	TFile* f_ratio=new TFile(Form("~/Desktop/histos/photonCuts_out_all_pt%d_ratio.root", ptCut),"READ");
	saveAllHistogramsToPicture(f_ratio, "gif", 3, 1);
}

void test_getListOfSOMEKeys()
{
	  const char* dirName="/home/kaya/Documents/cgate/output/out_HI/20032015";
	  TFile* file1 = new TFile(Form("%s/photonCuts_20032015_pt40_run0.root",dirName),"READ");

	  TList* histos_pt40 = getListOfHistograms(file1, "spike");
	  for (int i=0; i<histos_pt40->GetEntries() ; i++)
	  {
		  cout << histos_pt40->At(i)->GetName() <<endl;
	  }
}

void testCompare()
{
	int ptCut=40;
	TFile* f1=new TFile(Form("~/Desktop/histos/12032015/photonCuts_out_all_pt%d_run1.root", ptCut),"READ");
	TFile* f2=new TFile(Form("~/Desktop/histos/12032015/photonCuts_out_all_pt%d_run2.root", ptCut),"READ");
	TFile* f0=new TFile(Form("~/Desktop/histos/12032015/photonCuts_out_all_pt%d_run0.root", ptCut),"READ");

	TList* histos1=getListOfALLHistograms(f1);
	TList* histos2=getListOfALLHistograms(f2);
	TList* histos0=getListOfALLHistograms(f0);
	int len=histos1->GetEntries();

	TList* histosSum=new TList();
	TH1D* h_tmp;
	TH1D* h_tmp1
	TH1D* h_tmp2;
	for(int i=0; i<len; i++)
	{
		h_tmp1=(TH1D*)histos1->At(i);
		h_tmp2=(TH1D*)histos2->At(i);
		h_tmp=new TH1D("h_tmp","addition",h_tmp1->GetNbinsX(),h_tmp1->GetXaxis()->GetXmin(),h_tmp1->GetXaxis()->GetXmax());
		h_tmp->Add(h_tmp1, h_tmp2, 1, 1);
		histosSum->Add(h_tmp);

		if(i==0)
		{
			TCanvas* c1=new TCanvas();
			h_tmp->Draw();

			TCanvas* c2=new TCanvas();
			(TH1D*)histos0->At(i)->Draw();

			TH1D* h_diff=new TH1D("h_diff","subtraction",h_tmp1->GetNbinsX(),h_tmp1->GetXaxis()->GetXmin(),h_tmp1->GetXaxis()->GetXmax());
			h_diff->Add(h_tmp, (TH1D*)histos0->At(i), 1, -1);
			TCanvas* c3=new TCanvas();
			h_diff->Draw();
		}

		cout<< compareHistograms(h_tmp, (TH1D*)histos0->At(i)) <<endl;
	}
}

void test_getFileNames()
{
	std::vector<string> fileNames = getFileNames("/home/kaya/Desktop","pdf");

	for(int i=0; i<fileNames.size(); i++)
	{
		cout << fileNames.at(i) << endl;
	}
}
