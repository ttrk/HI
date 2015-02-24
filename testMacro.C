#include "../HiForestAnalysis/hiForest.h"
#include "TFile.h"
#include <iostream>
#include "TH1D.h"
#include "TCanvas.h"


void testMacro()
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
