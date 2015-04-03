#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TH1D.h>
#include <TString.h>
#include <TCut.h>
#include <TKey.h>

#include <iostream>
#include <map>

#include "../HiForestAnalysis/hiForest.h"
#include "smallPhotonUtil.h"
#include "histoUtil.h"
#include "systemUtils.h"

// necessary for GCC C++ Compiler to work
#include <string>
using  std::string;
using  std::cout;
using  std::endl;
//

void plotSameRatio()
{
	  const char* dirName="/home/kaya/Documents/cgate/output/out_HI/20032015";

	  TList* histos_pt40=getListOfALLHistograms(new TFile(Form("%s/photonCuts_20032015_pt40_ratio.root",dirName),"READ"));
	  TList* histos_pt60=getListOfALLHistograms(new TFile(Form("%s/photonCuts_20032015_pt60_ratio.root",dirName),"READ"));
	  TList* histos_pt80=getListOfALLHistograms(new TFile(Form("%s/photonCuts_20032015_pt80_ratio.root",dirName),"READ"));

	  TList* canvases=new TList();

	  TH1D* h1;
	  TH1D* h2;
	  TH1D* h3;
	  TCanvas* c1=new TCanvas("c1");	// must be here

	  double xcor1;
	  double ycor1;
	  double xcoroff;
	  double ycoroff;

	  int rebinFactor = 2;
	  for(int i=0; i<histos_pt40->GetEntries(); i++)
	  {
			  h1=(TH1D*)histos_pt40->At(i);
			  h2=(TH1D*)histos_pt60->At(i);
			  h3=(TH1D*)histos_pt80->At(i);

			  if(rebinFactor != 1)
			  {
				  h1->Rebin(rebinFactor);
				  h2->Rebin(rebinFactor);
				  h3->Rebin(rebinFactor);
			  }

	//		  h1->SetAxisRange(20,200);
	//		  h1->SetBins(20,h1->GetXaxis()->GetXmin(),h1->GetXaxis()->GetXmax());
	//		  h2->SetBins(20,h2->GetXaxis()->GetXmin(),h2->GetXaxis()->GetXmax());
	//		  h3->SetBins(20,h3->GetXaxis()->GetXmin(),h3->GetXaxis()->GetXmax());

	/*		  cout<<"-----------"<<endl;
			  cout<<h1->GetNbinsX()<<endl;
			  cout<<h1->GetXaxis()->GetXmin()<<endl;
			  cout<<h1->GetXaxis()->GetXmax()<<endl;*/

			  h1->Draw("e0");
			  h2->Draw("e0same");
			  h3->Draw("e0same");

			  h1->SetLineWidth(2);
			  h2->SetLineWidth(2);
			  h3->SetLineWidth(2);

			  h2->SetLineColor(kRed);
			  h3->SetLineColor(kGreen);

			  xcor1=0.8;
			  ycor1=0.55;
			  xcoroff=0.2;
			  ycoroff=0.3;

//			  h1->GetYaxis()->SetRangeUser(.0001, h3->GetMaximum());

			  TLegend* leg = new TLegend(xcor1,ycor1,xcor1+xcoroff,ycor1+xcoroff);
			  leg->AddEntry(h1, "p_{T}^{#gamma}>40");
			  leg->AddEntry(h2, "p_{T}^{#gamma}>60");
			  leg->AddEntry(h3, "p_{T}^{#gamma}>80");
			  leg->SetHeader("different p_{T}^{#gamma} cuts");
			  leg->Draw();

			  c1->SetLogy();		// some curves may not appear in linear scale
			  c1->SetName(h1->GetName());

			  c1->SaveAs(Form("%s/drawsameAllRatio2/%s.pdf",dirName ,c1->GetName()));
	  }
	  c1->Close();
}

void plotSame()
{
	  const char* dirName="/home/kaya/Documents/cgate/output/out_HI/20032015";

	  TList* histos_pt40=getListOfALLHistograms(new TFile(Form("%s/photonCuts_20032015_pt40_run0.root",dirName),"READ"));
	  TList* histos_pt60=getListOfALLHistograms(new TFile(Form("%s/photonCuts_20032015_pt60_run0.root",dirName),"READ"));
	  TList* histos_pt80=getListOfALLHistograms(new TFile(Form("%s/photonCuts_20032015_pt80_run0.root",dirName),"READ"));

	  TList* canvases=new TList();

	  TH1D* h1;
	  TH1D* h2;
	  TH1D* h3;
	  TCanvas* c1=new TCanvas("c1");	// must be here

	  double xcor1;
	  double ycor1;
	  double xcoroff;
	  double ycoroff;

	  int rebinFactor = 4;
	  for(int i=0; i<histos_pt40->GetEntries(); i++)
	  {
			  h1=(TH1D*)histos_pt40->At(i);
			  h2=(TH1D*)histos_pt60->At(i);
			  h3=(TH1D*)histos_pt80->At(i);

			  if(rebinFactor != 1)
			  {
				  h1->Rebin(rebinFactor);
				  h2->Rebin(rebinFactor);
				  h3->Rebin(rebinFactor);
			  }

	//		  h1->SetAxisRange(20,200);
	//		  h1->SetBins(20,h1->GetXaxis()->GetXmin(),h1->GetXaxis()->GetXmax());
	//		  h2->SetBins(20,h2->GetXaxis()->GetXmin(),h2->GetXaxis()->GetXmax());
	//		  h3->SetBins(20,h3->GetXaxis()->GetXmin(),h3->GetXaxis()->GetXmax());

	/*		  cout<<"-----------"<<endl;
			  cout<<h1->GetNbinsX()<<endl;
			  cout<<h1->GetXaxis()->GetXmin()<<endl;
			  cout<<h1->GetXaxis()->GetXmax()<<endl;*/

			  h1->Draw("hist");
			  h2->Draw("histsame");
			  h3->Draw("histsame");

			  h1->SetLineWidth(2);
			  h2->SetLineWidth(2);
			  h3->SetLineWidth(2);

			  h2->SetLineColor(kRed);
			  h3->SetLineColor(kGreen);

			  xcor1=0.8;
			  ycor1=0.55;
			  xcoroff=0.2;
			  ycoroff=0.3;

//			  h1->GetYaxis()->SetRangeUser(0, c1->GetY2());

			  TLegend* leg = new TLegend(xcor1,ycor1,xcor1+xcoroff,ycor1+xcoroff);
			  leg->AddEntry(h1, "p_{T}^{#gamma}>40");
			  leg->AddEntry(h2, "p_{T}^{#gamma}>60");
			  leg->AddEntry(h3, "p_{T}^{#gamma}>80");
			  leg->SetHeader("different p_{T}^{#gamma} cuts");
			  leg->Draw();

//			  c1->SetLogy();		// some curves may not appear in linear scale
			  c1->SetName(h1->GetName());

			  c1->SaveAs(Form("%s/drawsameAll4/%s.gif",dirName ,c1->GetName()));
	  }
	  c1->Close();
}

void produceRatioHistograms(const char* pt="40")
{
	const char* dirName="/home/kaya/Documents/cgate/output/out_HI/19032015";

	TFile* f1=new TFile(Form("%s/photonCuts_19032015_pt%s_run1.root",dirName,pt), "READ");
	TFile* f2=new TFile(Form("%s/photonCuts_19032015_pt%s_run2.root",dirName,pt), "READ");
	TList* histos_Ratio = divideHistogramList(f1,f2);

	const char* outFile_str = Form("%s/photonCuts_19032015_pt%s_ratio.root", dirName, pt);
	saveAllHistogramsToFile(outFile_str, histos_Ratio);

	saveAllHistogramsToPicture(new TFile(outFile_str,"READ"),"gif",3,1);
}

/*
Here are the minutes for today's meetings. Please feel free to add things which I forgot:

- Kaya - show ratio of green over blue plots from last slide (is there a change in jet eta trend?)
  - change normalization of ratio plots
*/
void produceRatioHistograms80to40()
{
	const char* dirName="/home/kaya/Documents/cgate/output/out_HI/20032015";

	TFile* f1=new TFile(Form("%s/photonCuts_20032015_pt80_ratio.root",dirName), "READ");
	TFile* f2=new TFile(Form("%s/photonCuts_20032015_pt40_ratio.root",dirName), "READ");
	TList* histos_Ratio = divideHistogramList(f1,f2, 1,false);

	const char* outFile_str = Form("%s/photonCuts_20032015_pt80to40_ratio.root", dirName);
	saveAllHistogramsToFile(outFile_str, histos_Ratio);

	saveAllHistogramsToPicture(new TFile(outFile_str,"READ"),"gif",3,1);

	TH1D* h_ratio;
	TCanvas* c1=new TCanvas("c1");	// must be here
	for (int i=0; i<histos_Ratio->GetEntries(); i++)
	{
		h_ratio = (TH1D*)histos_Ratio->At(i);
		if (strcmp(h_ratio->GetName(), "jet_eta_after_purity_ratio_ratio")==0)
		{
			h_ratio->Draw();

			h_ratio->SetLineWidth(2);
			h_ratio->SetTitle("green plot over blue plot from last slide");

//			TLegend* leg = new TLegend(0.3,0.3,0.3,0.3);
			TLegend* leg = new TLegend(0.3, 0.1, 0.7, 0.3);
			leg->AddEntry(h_ratio, "#frac{#eta^{Jet} ratio with p_{T}^{#gamma}>80}{#eta^{Jet} ratio with p_{T}^{#gamma}>40}","le");
			leg->Draw();

			/*
			 TPaveText *pt = new TPaveText(0.3, 0.1, 0.6, 0.25);
			 TText* text1  = pt->AddText("#eta^{Jet} ratio with p_{T}^{#gamma}>80");
			 text1->SetTextColor(kRed);
			 pt->Draw();
			 */


			c1->SetLogy();
			c1->SaveAs(Form("%s/photonCuts_20032015_pt80to40_ratio/%s.pdf",dirName ,h_ratio->GetName()));
		}
	}
}

void saveAllToImage()
{
	const char* dirName="/home/kaya/Documents/cgate/output/out_HI/20032015";

	const char* fileNames[] = {"photonCuts_20032015_pt40_run0.root",
							   "photonCuts_20032015_pt40_run1.root",
							   "photonCuts_20032015_pt40_run2.root"};

	int len_fileNames=(sizeof (fileNames) / sizeof (*fileNames));
	TFile* f;
	for(int i=0; i<len_fileNames; i++)
	{
		f=new TFile(Form("%s/%s",dirName,fileNames[i]),"READ");
		saveAllHistogramsToPicture(f,"gif",3);
	}
}

void saveAllToImage2()
{
	const char* dirName="/home/kaya/Documents/cgate/output/out_HI/03042015";

	vector<string> fileNames = getFileNames2(dirName,"root");

	TFile* f;
	for(int i=0; i < (int)fileNames.size(); i++)
	{
		f=new TFile(Form("%s/%s",dirName,fileNames.at(i).c_str()),"READ");
		saveAllHistogramsToPicture(f,"gif",3);
	}
}
