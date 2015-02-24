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

    abs(eta) < 1.44
    leading photon in the event
    spike rejection cuts
        sigmaIphiIphi > 0.002
        sigmaIetaIeta > 0.002
        swissCross < 0.9
        abs(seedTime) < 3
    Isolation Cuts
        ecalIso < 4.2
        hcalIso < 2.2
        trackIso < 2.0
        hadronicOverEm < 0.1
    purity enhancement cut
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

const int MAXPHOTONS = 50;
const double maxPt = 1313;

void photonCuts()
{
  HiForest *c = new HiForest("/mnt/hadoop/cms/store/user/luck/2014-photon-forests/pPb_DATA_photon30trig_localJEC_v1.root", "forest", cPPb, false);

  c->LoadNoTrees();
  c->hasPhotonTree = true;
  c->hasAkPu3JetTree = true;
  c->hasEvtTree = true;
  c->hasSkimTree = true;

  c->InitTree();

  std::cout << "number of entries: " << c->GetEntries() << std::endl;

  TCanvas *c1 = new TCanvas();
  c->photonTree->Draw("pt");

  TH1D *photon_pt = new TH1D("photon_pt",";p_{T}",100, 0, 200);

  for( Long64_t i = 0; i < 10000; ++i)
  {
    c->GetEntry(i);

    for( int j = 0; j < c->photon.nPhotons; ++j)
    {
      if(TMath::Abs(c->photon.eta[j]) > 1.44) continue;
      photon_pt->Fill(c->photon.pt[j]);
      break;
    }
  }

  TCanvas *c2 = new TCanvas();
  photon_pt->Draw();
}
