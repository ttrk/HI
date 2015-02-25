#include "../HiForestAnalysis/hiForest.h"
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
 * 02/23/2015
Hey guys,

Here's a summary of the cuts required at each step, and attached is the short C macro I wrote while you watched using the HiForestAnalysis class. You can find the HiForestAnalysis class here: https://github.com/CmsHI/HiForestAnalysis

    1. abs(eta) < 1.44
    2. leading photon in the event
    3. spike rejection cuts
        	sigmaIphiIphi > 0.002
        	sigmaIetaIeta > 0.002
        	swissCross < 0.9
        	abs(seedTime) < 3
    4. Isolation Cuts
        	ecalIso < 4.2
        	hcalIso < 2.2
        	trackIso < 2.0
        	hadronicOverEm < 0.1
    5. purity enhancement cut
        	sigmaIetaIeta < 0.01

At each step, please count the photons, and make distributions for the pt, eta, and phi. Also make distributions of the jets with pt > 30 and abs(eta) < 3.0 in each event for number of jets per event, pt, eta, and phi.

Alex
*/

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

const float cut_eta = 1.44;
// spike rejection
const float cut_sigmaIphiIphi = 0.002;
const float cut_sigmaIetaIeta_gt = 0.002;
const float cut_swissCross = 0.9;
const float cut_seedTime = 3;
// Isolation
const float cut_ecalIso = 4.2;
const float cut_hcalIso = 2.2;
const float cut_trackIso = 2.0;
const float cut_hadronicOverEm = 0.1;
// purity enhancement
const float cut_sigmaIetaIeta_lt = 0.01;



void photonCuts()
{
  HiForest *c = new HiForest("/mnt/hadoop/cms/store/user/luck/2014-photon-forests/pPb_DATA_photon30trig_localJEC_v1.root", "forest", cPPb, false);

  c->LoadNoTrees();
  c->hasPhotonTree = true;
  c->hasAkPu3JetTree = true;
  c->hasEvtTree = true;
  c->hasSkimTree = true;

  c->InitTree();

  bool passed_eta;
  bool passed_spike_reject;
  bool passed_iso;
  bool passed_purity;

  int count_after_eta=0;			// number of events whose leading photon passes eta cut
  int count_after_spike_reject=0;	// number of events whose leading photon passes spike rejection cut and previous cuts
  int count_after_iso=0;			// number of events whose leading photon passes isolation cut and previous cuts
  int count_after_purity=0;			// number of events whose leading photon passes purity enhancement cut and previous cuts

  std::cout << "number of entries: " << c->GetEntries() << std::endl;

  TCanvas *c1 = new TCanvas();
  c->photonTree->Draw("pt");


  TH1D *photon_pt_after_eta = new TH1D("photon_pt_after_eta",";p_{T}",100, 0, 200);
  TH1D *photon_pt_after_spike_reject = new TH1D("photon_pt_after_spike_reject",";p_{T}",100, 0, 200);
  TH1D *photon_pt_after_iso = new TH1D("photon_pt_after_iso",";p_{T}",100, 0, 200);
  TH1D *photon_pt_after_purity = new TH1D("photon_pt_after_purity",";p_{T}",100, 0, 200);

//  Long64_t entries = c->photonTree->GetEntries();
    Long64_t entries = 10000;	// work with a smaller set to get faster results
  for( Long64_t i = 0; i < entries; ++i)
  {
	  c->GetEntry(i);

	  for( int j = 0; j < c->photon.nPhotons; ++j)
	  {
		  passed_eta=false;
		  passed_spike_reject=false;
		  passed_iso=false;
		  passed_purity=false;

		  passed_eta = TMath::Abs(c->photon.eta[j]) < cut_eta;

		  // eta cut
		  if(passed_eta)
		  {
			  count_after_eta++;
			  photon_pt_after_eta->Fill(c->photon.pt[j]);

			  // spike rejection
			  passed_spike_reject = (c->photon.sigmaIphiIphi[j] 			> cut_sigmaIphiIphi &&
					  	  	  	  	 c->photon.sigmaIetaIeta[j] 			> cut_sigmaIetaIeta_gt &&
									 c->photon.swissCrx[j]      			< cut_swissCross    &&
									 TMath::Abs(c->photon.seedTime[j])		< cut_seedTime);
			  if (passed_spike_reject)
			  {
				  count_after_spike_reject++;
				  photon_pt_after_spike_reject->Fill(c->photon.pt[j]);

				  // isolation
				  passed_iso = (c->photon.ecalRecHitSumEtConeDR04[j] < cut_ecalIso	&&
					  	  	    c->photon.hcalTowerSumEtConeDR04[j]  < cut_hcalIso	&&
								c->photon.trkSumPtHollowConeDR04[j]  < cut_trackIso	&&
								c->photon.hadronicOverEm[j]   	     < cut_hadronicOverEm);
				  if (passed_iso)
				  {
					  count_after_iso++;
					  photon_pt_after_iso->Fill(c->photon.pt[j]);

					  // purity enhancement
					  passed_purity = c->photon.sigmaIetaIeta[j] < cut_sigmaIetaIeta_lt ;
					  if (passed_purity)
					  {
						  count_after_purity++;
						  photon_pt_after_purity->Fill(c->photon.pt[j]);
					  }
				  }
			  }
			  break;	// only leading photon is selected.
		  }
	  }
  }

  TCanvas *c2 = new TCanvas();
  photon_pt_after_eta->Draw();
  TCanvas *c3 = new TCanvas();
  photon_pt_after_spike_reject->Draw();
  TCanvas *c4 = new TCanvas();
  photon_pt_after_iso->Draw();
  TCanvas *c5 = new TCanvas();
  photon_pt_after_purity->Draw();

  cout << " count after eta cut : "<< count_after_eta << endl;
  cout << " count after spike rejection cut : "<< count_after_spike_reject << endl;
  cout << " count after isolation cut : "<< count_after_iso << endl;
  cout << " count after purity enhancement cut : "<< count_after_purity << endl;
}
