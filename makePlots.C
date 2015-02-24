#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TString.h>
#include <TCut.h>

#include <iostream>
#include <map>

//#include "makePlotsUtil.h"

// necessary for GCC C++ Compiler to work
#include <string>
using  std::string;
using  std::cout;
using  std::endl;
//

/*
 There are several introductory projects that the photon analysis needs to complete in order to update the pPb results.

    basic kinematics plots of objects which pass our selection cuts
        photons
            pt distribution
            corrected pt distribution
            eta distribution
            phi distribution
        jets
            number of selected jets per event
            pt distribution
            raw pt distribution
            eta distribution
            phi distribution
    perform a cross check showing the ratio of all the basic kinematic distributions from the first half of the run ( run < 211313 ) over the second half of the run ( run > 211313 )
    for each of our selection cuts, tell us the number of passing objects or events which pass that cut (for events, photons, and jets)
 */
/*
 * cuts for photons :
 *
 * */

/*
 * structure of the macro
 *
 * 1. initialize variables
 * 		Ex. file names, I/O files,
 *
 *
 *		- variables for the names of the branches : const char*
 *		- variables for the numerical values of the cuts : double/float/int
 *		- histograms to be plotted : TH1D
 *		- selection cuts : TCut
 *
 *		Ex. :
 *			const char *br_photon_eta = "eta";		// name of the eta branch
 *			float lt_eta = 1.44 ;					// eta less than 1.44
 *			TCut  cut_eta = Form("abs(%s)<(%f)", br_photon_eta, lt_eta);
 * 2.
 * */

const int MAXPHOTONS = 50;
const double maxPt = 1313;

void photonCuts()
{
	const TString input_File = "/mnt/hadoop/cms/store/user/luck/2014-photon-forests/pPb_DATA_photon30trig_localJEC_v1.root";
	//const TString jet_Directory="akVs3CaloJetAnalyzer/";
	//const TString jet_Tree="t";
	const TString photon_Directory="multiPhotonAnalyzer/";
	const TString photon_Tree="photon";

	const char *type = "photon";
	const int nBins = 90;

	// PHOTONS
	Float_t max_pt_photons;
	Int_t max_pt_photons_index;

	TFile *fFile = TFile::Open(input_File);
	//TTree *tree_jet = (TTree*)fFile->Get(jet_Directory+jet_Tree);
	TTree *tree_photon = (TTree*)fFile->Get(photon_Directory+photon_Tree);

	//tree_photon->Draw("pt");

	const char *br_photon_nPhotons = "nPhotons";
	const char *br_photon_pt = "pt";
	const char *br_photon_eta = "eta";

	Int_t photon_n;
	Float_t photon_pt[MAXPHOTONS];
	tree_photon->SetBranchAddress(br_photon_nPhotons,	&photon_n);
	tree_photon->SetBranchAddress(br_photon_pt,			 photon_pt);

	TFile *outFile = new TFile(Form("hist_out_%s.root",type),"RECREATE");
	TH1D *h_max_photon_pt		 = new TH1D("max_photon_pt",		"; max photon p_{T} (GeV)"		,nBins,0,maxPt);
	TH1D *h_index_max_photon_pt  = new TH1D("index_max_photon_pt",	"; index of max photon p_{T}"	,nBins,0,maxPt);
	TH1D *h_photon_pt			 = new TH1D("photon_pt",			"; photon p_{T} (GeV)"			,nBins,0,maxPt);

	TH1D *h_photon_pt_cut 		 = new TH1D("photon_pt_cut",		"; photon p_{T} with cut (GeV)"	,nBins,0,maxPt);

	float lt_eta = 1.44 ;
	TCut cut_eta= Form("abs(%s)<(%f)", br_photon_eta, lt_eta);

	outFile->cd();

	Long64_t entries = tree_photon->GetEntries();
	for(Long64_t j = 0; j < entries; ++j)
	{
		tree_photon->GetEntry(j);

		max_pt_photons=-1; // default value for maximum PT = -1.
		max_pt_photons_index=-1; // default value for index of maximum PT object = -1.

		if (photon_n>0)
		{
            for (int i = 0; i < photon_n; ++i) {

                // update the maximum of photon PT s
                if (photon_pt[i]>max_pt_photons)
                {
                    max_pt_photons=photon_pt[i];
                    max_pt_photons_index=i;
                }
            }
        	h_max_photon_pt->Fill(photon_pt[max_pt_photons_index]);
        	h_index_max_photon_pt->Fill(max_pt_photons_index);
		}
	}
	tree_photon->Draw(Form("%s>>%s",br_photon_pt,	h_photon_pt->GetName())			    	);
	tree_photon->Draw(Form("%s>>%s",br_photon_pt,	h_photon_pt_cut->GetName()),	cut_eta );

	h_max_photon_pt			->Write();
	h_index_max_photon_pt	->Write();
	h_photon_pt				->Write();
	h_photon_pt_cut			->Write();
	std::cout << "histograms written" << std::endl;

	outFile->Write();
	std::cout << "output file written" << std::endl;
}

int main()
{
	photonCuts();
	return 0;
}
