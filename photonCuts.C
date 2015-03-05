#include "../HiForestAnalysis/hiForest.h"
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TString.h>
#include <TCut.h>
#include <TKey.h>

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
 * 27.02.2015
	1) Event Selection
	- hbheNoiseFilter
	- pACollisionEventSelectionPA
	- |vz|<15

	2) jet selection
	- only jets with deltaR > 0.4 from photon

	3) additional jet selection
	 	 deltaPhi > 7/8 * pi

	4) pretty histograms
		- x-axis labels
		- box with cuts
		- possibly put multiple distributions in 1 plot, use colors
*/

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
const float cut_vz = 15;

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

// cuts for jets
const float cut_jet_pt = 30;
const float cut_jet_eta = 3;
const float cut_jet_photon_deltaR = 0.4;
const float cut_jet_photon_deltaPhi = (7 * TMath::Pi()) / 8 ;	// 7/8 * TMath::Pi() --> evaluates to 0;

void photonCuts()
{
  const TString inFile_str="/mnt/hadoop/cms/store/user/luck/2014-photon-forests/pPb_DATA_photon30trig_localJEC_v1.root";
  HiForest *c = new HiForest(inFile_str, "forest", cPPb, false);

  c->LoadNoTrees();
  c->hasPhotonTree = true;
  c->hasAkPu3JetTree = true;
  c->hasEvtTree = true;
  c->hasSkimTree = true;

  c->InitTree();

  bool passed_event_selection;

  bool passed_eta;
  bool passed_spike_reject;
  bool passed_iso;
  bool passed_purity;

  bool passed_jet_pt;
  bool passed_jet_eta;
  bool passed_jet_photon_deltaR;
  bool passed_jet_photon_deltaPhi;	//  this is not a cut, but an additional selection

  const TString outFile_str="photonCuts_out.root";
  TFile *outFile = new TFile(outFile_str,"RECREATE");

  int count_after_eta=0;			// number of events which pass the eta cut, ie. events where there is a photon that passes eta cut
  int count_after_spike_reject=0;	// number of events whose leading photon after the eta cut passes spike rejection cut
  int count_after_iso=0;			// number of events whose leading photon after the eta cut passes isolation cut and previous cuts
  int count_after_purity=0;			// number of events whose leading photon after the eta cut passes purity enhancement cut and previous cuts

  	  	  	  	  	  	  	  	  	// selected jet : a jet that passes jet_pt and jet_eta cut
  int numJets_after_eta=0;			// number of selected jets in an event which passes the eta cut
  int numJets_after_spike_reject=0;	// number of selected jets in an event which passes the spike rejection cut and all previous cuts
  int numJets_after_iso=0;			// number of selected jets in an event which passes the isolation cut and all previous cuts
  int numJets_after_purity=0;		// number of selected jets in an event which passes the purity enhancement cut and all previous cuts

  	  	  	  	  	  	  	  	  	// the above definition for selected jet &&
              	  	  	  	  	  	// the jet has a delta_Phi > (7/8 * pi) with
              	  	  	  	  	  	// the leading photon
  int numJets_after_eta_dPhiCut=0;
  int numJets_after_spike_reject_dPhiCut=0;
  int numJets_after_iso_dPhiCut=0;
  int numJets_after_purity_dPhiCut=0;

  std::cout << "number of entries: " << c->GetEntries() << std::endl;

  TCanvas *c1 = new TCanvas();
  c->photonTree->Draw("pt");

  // PHOTON histograms
  TH1D *photon_pt_after_eta = new TH1D("photon_pt_after_eta","p_{T}^{#gamma} after |#eta^{#gamma}|<1.44;p_{T}",100, 0, 200);
  TH1D *photon_eta_after_eta = new TH1D("photon_eta_after_eta","#eta^{#gamma} after |#eta^{#gamma}|<1.44;\eta",100, -2, 2);
  TH1D *photon_phi_after_eta = new TH1D("photon_phi_after_eta","#phi^{#gamma} after |#eta^{#gamma}|<1.44;\phi",100, -TMath::Pi(), TMath::Pi());

  TH1D *photon_pt_after_spike_reject = new TH1D("photon_pt_after_spike_reject","p_{T}^{#gamma} after spike rejection cut;p_{T}",100, 0, 200);
  TH1D *photon_eta_after_spike_reject = new TH1D("photon_eta_after_spike_reject","#eta^{#gamma} after spike rejection cut;\eta",100, -2, 2);
  TH1D *photon_phi_after_spike_reject = new TH1D("photon_phi_after_spike_reject","#phi^{#gamma} after spike rejection cut;\phi",100, -TMath::Pi(), TMath::Pi());

  TH1D *photon_pt_after_iso = new TH1D("photon_pt_after_iso","p_{T}^{#gamma} after isolation cut;p_{T}",100, 0, 200);
  TH1D *photon_eta_after_iso = new TH1D("photon_eta_after_iso","#eta^{#gamma} after isolation cut;\eta",100, -2, 2);
  TH1D *photon_phi_after_iso = new TH1D("photon_phi_after_iso","#phi^{#gamma} after isolation cut;\phi",100, -TMath::Pi(), TMath::Pi());

  TH1D *photon_pt_after_purity = new TH1D("photon_pt_after_purity","p_{T}^{#gamma} after purity enhancement cut;p_{T}",100, 0, 200);
  TH1D *photon_eta_after_purity = new TH1D("photon_eta_after_purity","#eta^{#gamma} after purity enhancement cut;\eta",100, -2, 2);
  TH1D *photon_phi_after_purity = new TH1D("photon_phi_after_purity","#phi^{#gamma} after purity enhancement cut;\phi",100, -TMath::Pi(), TMath::Pi());

  // JET histograms
  int jet_count_last_bin=16;
  Double_t jet_photon_DPHI_firstBin=0;
  TH1D *jet_pt_after_eta = new TH1D("jet_pt_after_eta","p_{T}^{Jet} after |#eta^{#gamma}|<1.44;p_{T}",100, 0, 200);
  TH1D *jet_eta_after_eta = new TH1D("jet_eta_after_eta","#eta^{Jet} after |#eta^{#gamma}|<1.44;\eta",100, -2, 2);
  TH1D *jet_phi_after_eta = new TH1D("jet_phi_after_eta","#phi^{Jet} after |#eta^{#gamma}|<1.44;\phi",100, -TMath::Pi(), TMath::Pi());
  TH1D *jet_count_after_eta = new TH1D("jet_count_after_eta","number of selected jets after |#eta^{#gamma}|<1.44",100, 0,jet_count_last_bin);
  TH1D *jet_photon_DPHI_after_eta = new TH1D("jet_photon_DPHI_after_eta","#Delta#phi_{J,#gamma} after |#eta^{#gamma}|<1.44;\phi",100, jet_photon_DPHI_firstBin, TMath::Pi());

  TH1D *jet_pt_after_spike_reject = new TH1D("jet_pt_after_spike_reject","p_{T}^{Jet} after spike rejection cut;p_{T}",100, 0, 200);
  TH1D *jet_eta_after_spike_reject = new TH1D("jet_eta_after_spike_reject","#eta^{Jet} after spike rejection cut;\eta",100, -2, 2);
  TH1D *jet_phi_after_spike_reject = new TH1D("jet_phi_after_spike_reject","#phi^{Jet} after spike rejection cut;\phi",100, -TMath::Pi(), TMath::Pi());
  TH1D *jet_count_after_spike_reject = new TH1D("jet_count_after_spike_reject","number of selected jets after spike rejection cut",100, 0,jet_count_last_bin);
  TH1D *jet_photon_DPHI_after_spike_reject = new TH1D("jet_photon_DPHI_after_spike_reject","#Delta#phi_{J,#gamma} after spike rejection cut;\phi",100, jet_photon_DPHI_firstBin, TMath::Pi());

  TH1D *jet_pt_after_iso = new TH1D("jet_pt_after_iso","p_{T}^{Jet} after isolation cut;p_{T}",100, 0, 200);
  TH1D *jet_eta_after_iso = new TH1D("jet_eta_after_iso","#eta^{Jet} after isolation cut;\eta",100, -2, 2);
  TH1D *jet_phi_after_iso = new TH1D("jet_phi_after_iso","#phi^{Jet} after isolation cut;\phi",100, -TMath::Pi(), TMath::Pi());
  TH1D *jet_count_after_iso = new TH1D("jet_count_after_iso","number of selected jets after isolation cut",100, 0,jet_count_last_bin);
  TH1D *jet_photon_DPHI_after_iso = new TH1D("jet_photon_DPHI_after_iso","#Delta#phi_{J,#gamma} after isolation cut;\phi",100, jet_photon_DPHI_firstBin, TMath::Pi());

  TH1D *jet_pt_after_purity = new TH1D("jet_pt_after_purity","p_{T}^{Jet} after purity enhancement cut;p_{T}",100, 0, 200);
  TH1D *jet_eta_after_purity = new TH1D("jet_eta_after_purity","#eta^{Jet} after purity enhancement cut;\eta",100, -2, 2);
  TH1D *jet_phi_after_purity = new TH1D("jet_phi_after_purity","#phi^{Jet} after purity enhancement cut;\phi",100, -TMath::Pi(), TMath::Pi());
  TH1D *jet_count_after_purity = new TH1D("jet_count_after_purity","number of selected jets after purity enhancement cut",100, 0,jet_count_last_bin);
  TH1D *jet_photon_DPHI_after_purity = new TH1D("jet_photon_DPHI_after_purity","#Delta#phi_{J,#gamma} after purity enhancement cut;\phi",100, jet_photon_DPHI_firstBin, TMath::Pi());

// JET histograms with deltaPhi > 7/8 * pi
  Double_t jet_photon_DPHI_dPhiCut_firstBin=(6 * TMath::Pi()) / 8 ;
  TH1D *jet_pt_after_eta_dPhiCut = new TH1D("jet_pt_after_eta_dPhiCut","p_{T}^{Jet} with #Delta#phi_{J,#gamma}>#frac{7}{8}#pi after |#eta^{#gamma}|<1.44;p_{T}",100, 0, 200);
  TH1D *jet_eta_after_eta_dPhiCut = new TH1D("jet_eta_after_eta_dPhiCut","#eta^{Jet} with #Delta#phi_{J,#gamma}>#frac{7}{8}#pi after |#eta^{#gamma}|<1.44;\eta",100, -2, 2);
  TH1D *jet_phi_after_eta_dPhiCut = new TH1D("jet_phi_after_eta_dPhiCut","#phi^{Jet} with #Delta#phi_{J,#gamma}>#frac{7}{8}#pi after |#eta^{#gamma}|<1.44;\phi",100, -TMath::Pi(), TMath::Pi());
  TH1D *jet_count_after_eta_dPhiCut = new TH1D("jet_count_after_eta_dPhiCut","number of selected jets with #Delta#phi_{J,#gamma}>#frac{7}{8}#pi after |#eta^{#gamma}|<1.44",100, 0,jet_count_last_bin);
  TH1D *jet_photon_DPHI_after_eta_dPhiCut = new TH1D("jet_photon_DPHI_after_eta_dPhiCut","#Delta#phi_{J,#gamma} with #Delta#phi_{J,#gamma}>#frac{7}{8}#pi after |#eta^{#gamma}|<1.44;\phi",100, jet_photon_DPHI_dPhiCut_firstBin, TMath::Pi());

  TH1D *jet_pt_after_spike_reject_dPhiCut = new TH1D("jet_pt_after_spike_reject_dPhiCut","p_{T}^{Jet} with #Delta#phi_{J,#gamma}>#frac{7}{8}#pi after spike rejection cut;p_{T}",100, 0, 200);
  TH1D *jet_eta_after_spike_reject_dPhiCut = new TH1D("jet_eta_after_spike_reject_dPhiCut","#eta^{Jet} with #Delta#phi_{J,#gamma}>#frac{7}{8}#pi after spike rejection cut;\eta",100, -2, 2);
  TH1D *jet_phi_after_spike_reject_dPhiCut = new TH1D("jet_phi_after_spike_reject_dPhiCut","#phi^{Jet} with #Delta#phi_{J,#gamma}>#frac{7}{8}#pi after spike rejection cut;\phi",100, -TMath::Pi(), TMath::Pi());
  TH1D *jet_count_after_spike_reject_dPhiCut = new TH1D("jet_count_after_spike_reject_dPhiCut","number of selected jets with #Delta#phi_{J,#gamma}>#frac{7}{8}#pi after spike rejection cut",100, 0,jet_count_last_bin);
  TH1D *jet_photon_DPHI_after_spike_reject_dPhiCut = new TH1D("jet_photon_DPHI_after_spike_reject_dPhiCut","#Delta#phi_{J,#gamma} with #Delta#phi_{J,#gamma}>#frac{7}{8}#pi after spike rejection cut;\phi",100, jet_photon_DPHI_dPhiCut_firstBin, TMath::Pi());

  TH1D *jet_pt_after_iso_dPhiCut = new TH1D("jet_pt_after_iso_dPhiCut","p_{T}^{Jet} with #Delta#phi_{J,#gamma}>#frac{7}{8}#pi after isolation cut;p_{T}",100, 0, 200);
  TH1D *jet_eta_after_iso_dPhiCut = new TH1D("jet_eta_after_iso_dPhiCut","#eta^{Jet} with #Delta#phi_{J,#gamma}>#frac{7}{8}#pi after isolation cut;\eta",100, -2, 2);
  TH1D *jet_phi_after_iso_dPhiCut = new TH1D("jet_phi_after_iso_dPhiCut","#phi^{Jet} with #Delta#phi_{J,#gamma}>#frac{7}{8}#pi after isolation cut;\phi",100, -TMath::Pi(), TMath::Pi());
  TH1D *jet_count_after_iso_dPhiCut = new TH1D("jet_count_after_iso_dPhiCut","number of selected jets with #Delta#phi_{J,#gamma}>#frac{7}{8}#pi after isolation cut",100, 0,jet_count_last_bin);
  TH1D *jet_photon_DPHI_after_iso_dPhiCut = new TH1D("jet_photon_DPHI_after_iso_dPhiCut","#Delta#phi_{J,#gamma} with #Delta#phi_{J,#gamma}>#frac{7}{8}#pi after isolation cut;\phi",100, jet_photon_DPHI_dPhiCut_firstBin, TMath::Pi());

  TH1D *jet_pt_after_purity_dPhiCut = new TH1D("jet_pt_after_purity_dPhiCut","p_{T}^{Jet} with #Delta#phi_{J,#gamma}>#frac{7}{8}#pi after purity enhancement cut;p_{T}",100, 0, 200);
  TH1D *jet_eta_after_purity_dPhiCut = new TH1D("jet_eta_after_purity_dPhiCut","#eta^{Jet} with #Delta#phi_{J,#gamma}>#frac{7}{8}#pi after purity enhancement cut;\eta",100, -2, 2);
  TH1D *jet_phi_after_purity_dPhiCut = new TH1D("jet_phi_after_purity_dPhiCut","#phi^{Jet} with #Delta#phi_{J,#gamma}>#frac{7}{8}#pi after purity enhancement cut;\phi",100, -TMath::Pi(), TMath::Pi());
  TH1D *jet_count_after_purity_dPhiCut = new TH1D("jet_count_after_purity_dPhiCut","number of selected jets with #Delta#phi_{J,#gamma}>#frac{7}{8}#pi after purity enhancement cut",100, 0,jet_count_last_bin);
  TH1D *jet_photon_DPHI_after_purity_dPhiCut = new TH1D("jet_photon_DPHI_after_purity_dPhiCut","#Delta#phi_{J,#gamma} with #Delta#phi_{J,#gamma}>#frac{7}{8}#pi after purity enhancement cut;\phi",100, jet_photon_DPHI_dPhiCut_firstBin, TMath::Pi());

//  Long64_t entries = c->photonTree->GetEntries();
    Long64_t entries = 10000;	// work with a smaller set to get faster results

    int index_leading_photon;
    float jet_photon_deltaR;
    float jet_photon_deltaPhi;
//  int index_leading_jet;
  for( Long64_t i = 0; i < entries; ++i)
  {
	  c->GetEntry(i);

	  passed_event_selection = c->skim.pHBHENoiseFilter 			   > 0 	&&
			  	  	  	  	   c->skim.pPAcollisionEventSelectionPA    > 0  &&
							   TMath::Abs(c->evt.vz)				   < cut_vz;
//      if(!passed_event_selection) continue;		// this event failed to pass. Skip to the next of iteration. Go to next event.

	  passed_eta=false;
	  passed_spike_reject=false;
	  passed_iso=false;
	  passed_purity=false;

	  index_leading_photon=-1;
	  for( int j = 0; j < c->photon.nPhotons; ++j)
	  {

		  passed_eta = TMath::Abs(c->photon.eta[j]) < cut_eta;
		  passed_spike_reject = (c->photon.sigmaIphiIphi[j] 			> cut_sigmaIphiIphi &&
				  	  	  	  	 c->photon.sigmaIetaIeta[j] 			> cut_sigmaIetaIeta_gt &&
								 c->photon.swissCrx[j]      			< cut_swissCross    &&
								 TMath::Abs(c->photon.seedTime[j])		< cut_seedTime);
		  passed_iso = (c->photon.ecalRecHitSumEtConeDR04[j] < cut_ecalIso	&&
			  	  	    c->photon.hcalTowerSumEtConeDR04[j]  < cut_hcalIso	&&
						c->photon.trkSumPtHollowConeDR04[j]  < cut_trackIso	&&
						c->photon.hadronicOverEm[j]   	     < cut_hadronicOverEm);
		  passed_purity = c->photon.sigmaIetaIeta[j] < cut_sigmaIetaIeta_lt ;

		  // eta cut
		  if(passed_eta)
		  {
			  count_after_eta++;
			  photon_pt_after_eta->Fill(c->photon.pt[j]);
			  photon_eta_after_eta->Fill(c->photon.eta[j]);
			  photon_phi_after_eta->Fill(c->photon.phi[j]);
		  }
		  // spike rejection
		  if (passed_eta && passed_spike_reject)
		  {
			  count_after_spike_reject++;
			  photon_pt_after_spike_reject->Fill(c->photon.pt[j]);
			  photon_eta_after_spike_reject->Fill(c->photon.eta[j]);
			  photon_phi_after_spike_reject->Fill(c->photon.phi[j]);
		  }
		  // isolation
		  if (passed_eta && passed_spike_reject && passed_iso)
		  {
			  count_after_iso++;
			  photon_pt_after_iso->Fill(c->photon.pt[j]);
			  photon_eta_after_iso->Fill(c->photon.eta[j]);
			  photon_phi_after_iso->Fill(c->photon.phi[j]);
		  }
		  // purity enhancement
		  if (passed_eta && passed_spike_reject && passed_iso && passed_purity)
		  {
			  count_after_purity++;
			  photon_pt_after_purity->Fill(c->photon.pt[j]);
			  photon_eta_after_purity->Fill(c->photon.eta[j]);
			  photon_phi_after_purity->Fill(c->photon.phi[j]);
		  }

		  if(passed_eta)
		  {
			  index_leading_photon = j;		// leading photon will be used for delta Phi
			  break;	// only leading photon is selected.
		  }
	  }

	  if (index_leading_photon < 0) continue;

//	  index_leading_jet=-1;
	  numJets_after_eta=0;
	  numJets_after_spike_reject=0;
	  numJets_after_iso=0;
	  numJets_after_purity=0;

	  numJets_after_eta_dPhiCut=0;
	  numJets_after_spike_reject_dPhiCut=0;
	  numJets_after_iso_dPhiCut=0;
	  numJets_after_purity_dPhiCut=0;

	  for( int j = 0; j < c->akPu3PF.nref; ++j)
	  {
		  jet_photon_deltaR   =   getDR(c->akPu3PF.jtphi[j],c->photon.phi[index_leading_photon],
				  	  	  	            c->akPu3PF.jteta[j],c->photon.eta[index_leading_photon]);
		  jet_photon_deltaPhi = getDPHI(c->akPu3PF.jtphi[j],c->photon.phi[index_leading_photon]);

		  passed_jet_photon_deltaPhi = TMath::Abs(jet_photon_deltaPhi) > cut_jet_photon_deltaPhi;

		  passed_jet_photon_deltaR = jet_photon_deltaR	   			  > cut_jet_photon_deltaR;
		  passed_jet_pt			   = c->akPu3PF.jtpt[j]			      > cut_jet_pt;
		  passed_jet_eta 		   = TMath::Abs(c->akPu3PF.jteta[j])  < cut_jet_eta;

		  if(passed_jet_photon_deltaR && passed_jet_pt && passed_jet_eta)
		  {
/*
			  if(index_leading_jet==-1)
				  index_leading_jet = j;	// first jet that passes the jet cut is the leading jet of the event.
			  	  	  	  	  	  	  	  	// forget about leading jet for now.
*/

			  // eta cut
			  if(passed_eta)
			  {
				  numJets_after_eta++;
				  jet_pt_after_eta->Fill(c->akPu3PF.jtpt[j]);
				  jet_eta_after_eta->Fill(c->akPu3PF.jteta[j]);
				  jet_phi_after_eta->Fill(c->akPu3PF.jtphi[j]);
				  jet_photon_DPHI_after_eta->Fill(TMath::Abs(jet_photon_deltaPhi));

				  if(passed_jet_photon_deltaPhi)
				  {
					  numJets_after_eta_dPhiCut++;
					  jet_pt_after_eta_dPhiCut->Fill(c->akPu3PF.jtpt[j]);
					  jet_eta_after_eta_dPhiCut->Fill(c->akPu3PF.jteta[j]);
					  jet_phi_after_eta_dPhiCut->Fill(c->akPu3PF.jtphi[j]);
					  jet_photon_DPHI_after_eta_dPhiCut->Fill(TMath::Abs(jet_photon_deltaPhi));
				  }
			  }
			  // spike rejection
			  if (passed_eta && passed_spike_reject)
			  {
				  numJets_after_spike_reject++;
				  jet_pt_after_spike_reject->Fill(c->akPu3PF.jtpt[j]);
				  jet_eta_after_spike_reject->Fill(c->akPu3PF.jteta[j]);
				  jet_phi_after_spike_reject->Fill(c->akPu3PF.jtphi[j]);
				  jet_photon_DPHI_after_spike_reject->Fill(TMath::Abs(jet_photon_deltaPhi));

				  if(passed_jet_photon_deltaPhi)
				  {
					  numJets_after_spike_reject_dPhiCut++;
					  jet_pt_after_spike_reject_dPhiCut->Fill(c->akPu3PF.jtpt[j]);
					  jet_eta_after_spike_reject_dPhiCut->Fill(c->akPu3PF.jteta[j]);
					  jet_phi_after_spike_reject_dPhiCut->Fill(c->akPu3PF.jtphi[j]);
					  jet_photon_DPHI_after_spike_reject_dPhiCut->Fill(TMath::Abs(jet_photon_deltaPhi));
				  }
			  }
			  // isolation
			  if (passed_eta && passed_spike_reject && passed_iso)
			  {
				  numJets_after_iso++;
				  jet_pt_after_iso->Fill(c->akPu3PF.jtpt[j]);
				  jet_eta_after_iso->Fill(c->akPu3PF.jteta[j]);
				  jet_phi_after_iso->Fill(c->akPu3PF.jtphi[j]);
				  jet_photon_DPHI_after_iso->Fill(TMath::Abs(jet_photon_deltaPhi));

				  if(passed_jet_photon_deltaPhi)
				  {
					  numJets_after_iso_dPhiCut++;
					  jet_pt_after_iso_dPhiCut->Fill(c->akPu3PF.jtpt[j]);
					  jet_eta_after_iso_dPhiCut->Fill(c->akPu3PF.jteta[j]);
					  jet_phi_after_iso_dPhiCut->Fill(c->akPu3PF.jtphi[j]);
					  jet_photon_DPHI_after_iso_dPhiCut->Fill(TMath::Abs(jet_photon_deltaPhi));
				  }
			  }
			  // purity enhancement
			  if (passed_eta && passed_spike_reject && passed_iso && passed_purity)
			  {
				  numJets_after_purity++;
				  jet_pt_after_purity->Fill(c->akPu3PF.jtpt[j]);
				  jet_eta_after_purity->Fill(c->akPu3PF.jteta[j]);
				  jet_phi_after_purity->Fill(c->akPu3PF.jtphi[j]);
				  jet_photon_DPHI_after_purity->Fill(TMath::Abs(jet_photon_deltaPhi));

				  if(passed_jet_photon_deltaPhi)
				  {
					  numJets_after_purity_dPhiCut++;
					  jet_pt_after_purity_dPhiCut->Fill(c->akPu3PF.jtpt[j]);
					  jet_eta_after_purity_dPhiCut->Fill(c->akPu3PF.jteta[j]);
					  jet_phi_after_purity_dPhiCut->Fill(c->akPu3PF.jtphi[j]);
					  jet_photon_DPHI_after_purity_dPhiCut->Fill(TMath::Abs(jet_photon_deltaPhi));
				  }
			  }
		  }
	  }

	  // number of selected jets in an event after the relevant cuts on photons
	  // eta cut
	  if(passed_eta)
	  {
		  jet_count_after_eta->Fill(numJets_after_eta);

		  if (passed_jet_photon_deltaPhi)
		  {
			  jet_count_after_eta_dPhiCut->Fill(numJets_after_eta_dPhiCut);
		  }
	  }
	  // spike rejection
	  if (passed_eta && passed_spike_reject)
	  {
		  jet_count_after_spike_reject->Fill(numJets_after_spike_reject);

		  if (passed_jet_photon_deltaPhi)
		  {
			  jet_count_after_spike_reject_dPhiCut->Fill(numJets_after_spike_reject_dPhiCut);
		  }
	  }
	  // isolation
	  if (passed_eta && passed_spike_reject && passed_iso)
	  {
		  jet_count_after_iso->Fill(numJets_after_iso);

		  if (passed_jet_photon_deltaPhi)
		  {
			  jet_count_after_iso_dPhiCut->Fill(numJets_after_iso_dPhiCut);
		  }
	  }
	  // purity enhancement
	  if (passed_eta && passed_spike_reject && passed_iso && passed_purity)
	  {
		  jet_count_after_purity->Fill(numJets_after_purity);

		  if (passed_jet_photon_deltaPhi)
		  {
			  jet_count_after_purity_dPhiCut->Fill(numJets_after_purity_dPhiCut);
		  }
	  }
  }

  cout << " event count after eta cut : "<< count_after_eta << endl;
  cout << " event count after spike rejection cut : "<< count_after_spike_reject << endl;
  cout << " event count after isolation cut : "<< count_after_iso << endl;
  cout << " event count after purity enhancement cut : "<< count_after_purity << endl;

  // save objects to File
  // PHOTON histograms
  const char *photon_dir="photons";
  outFile->mkdir(photon_dir);
  outFile->cd(photon_dir);
  photon_pt_after_eta->Write();
  photon_eta_after_eta->Write();
  photon_phi_after_eta->Write();
  photon_pt_after_spike_reject->Write();
  photon_eta_after_spike_reject->Write();
  photon_phi_after_spike_reject->Write();
  photon_pt_after_iso->Write();
  photon_eta_after_iso->Write();
  photon_phi_after_iso->Write();
  photon_pt_after_purity->Write();
  photon_eta_after_purity->Write();
  photon_phi_after_purity->Write();

//  photon_pt_after_eta->SaveAs(Form("gifs/%s.gif",photon_pt_after_eta->GetName()));

  // SELECTED JET histograms
  const char *jet_dir="jets";
  outFile->mkdir(jet_dir);
  outFile->cd(jet_dir);
  jet_pt_after_eta->Write();
  jet_eta_after_eta->Write();
  jet_phi_after_eta->Write();
  jet_count_after_eta->Write();
  jet_photon_DPHI_after_eta->Write();

  jet_pt_after_spike_reject->Write();
  jet_eta_after_spike_reject->Write();
  jet_phi_after_spike_reject->Write();
  jet_count_after_spike_reject->Write();
  jet_photon_DPHI_after_spike_reject->Write();

  jet_pt_after_iso->Write();
  jet_eta_after_iso->Write();
  jet_phi_after_iso->Write();
  jet_count_after_iso->Write();
  jet_photon_DPHI_after_iso->Write();

  jet_pt_after_purity->Write();
  jet_eta_after_purity->Write();
  jet_phi_after_purity->Write();
  jet_count_after_purity->Write();
  jet_photon_DPHI_after_purity->Write();

  // SELECTED JET with deltaPhi>7/8*Pi histograms
  const char *jet_dir_dPhiCut="jets_deltaPhi_Cut";
  outFile->mkdir(jet_dir_dPhiCut);
  outFile->cd(jet_dir_dPhiCut);
  jet_pt_after_eta_dPhiCut->Write();
  jet_eta_after_eta_dPhiCut->Write();
  jet_phi_after_eta_dPhiCut->Write();
  jet_count_after_eta_dPhiCut->Write();
  jet_photon_DPHI_after_eta_dPhiCut->Write();

  jet_pt_after_spike_reject_dPhiCut->Write();
  jet_eta_after_spike_reject_dPhiCut->Write();
  jet_phi_after_spike_reject_dPhiCut->Write();
  jet_count_after_spike_reject_dPhiCut->Write();
  jet_photon_DPHI_after_spike_reject_dPhiCut->Write();

  jet_pt_after_iso_dPhiCut->Write();
  jet_eta_after_iso_dPhiCut->Write();
  jet_phi_after_iso_dPhiCut->Write();
  jet_count_after_iso_dPhiCut->Write();
  jet_photon_DPHI_after_iso_dPhiCut->Write();

  jet_pt_after_purity_dPhiCut->Write();
  jet_eta_after_purity_dPhiCut->Write();
  jet_phi_after_purity_dPhiCut->Write();
  jet_count_after_purity_dPhiCut->Write();
  jet_photon_DPHI_after_purity_dPhiCut->Write();

  std::cout << "output written to : " << outFile_str << std::endl;

  c->inf->Close();
  outFile->Close();
}

Double_t getDPHI(Double_t phi1, Double_t phi2) {
    Double_t dphi = phi1 - phi2;
    if (dphi > 3.141592653589)
        dphi = dphi - 2. * 3.141592653589;
    if (dphi <= -3.141592653589)
        dphi = dphi + 2. * 3.141592653589;
    if (TMath::Abs(dphi) > 3.141592653589) {
        cout << " commonUtility::getDPHI error!!! dphi is bigger than 3.141592653589 " << endl;
    }
    return dphi;
}

Double_t getDETA(Double_t eta1, Double_t eta2){
	return eta1 - eta2;
}

Double_t getDR(Double_t phi1, Double_t phi2, Double_t eta1, Double_t eta2){
	Double_t dphi = getDPHI(phi1,phi2);
	Double_t deta = getDETA(eta1,eta2);
	return TMath::Sqrt( deta*deta+dphi*dphi );
}
