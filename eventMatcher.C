#include "EventMatchingCMS.h"
#include "../HiForestAnalysis/hiForest.h"

// necessary for GCC C++ Compiler to work
#include <string>
using  std::string;
using  std::cout;
using  std::endl;
//

void eventMatcher(const TString fileName1, const TString fileName2)
{

	  HiForest *c1 = new HiForest(fileName1, "forest", cPPb, false);
	  HiForest *c2 = new HiForest(fileName2, "forest", cPPb, false);
	  cout << "fileName1 = " << fileName1 <<endl;
	  cout << "fileName2 = " << fileName2 <<endl;

	  c1->LoadNoTrees();
	  c1->hasEvtTree    = true;
	  c1->hasPhotonTree = true;
	  c1->hasAkPu3JetTree = true;

	  c2->LoadNoTrees();
	  c2->hasEvtTree    = true;
	  c2->hasPhotonTree = true;
	  c2->hasAkPu3JetTree = true;

	  c1->InitTree();
	  c2->InitTree();

	  Long64_t entries1 = c1->evtTree->GetEntries();
	  Long64_t entries2 = c2->evtTree->GetEntries();

	  EventMatchingCMS* eventMatcher1=new EventMatchingCMS();
	  EventMatchingCMS* eventMatcher2=new EventMatchingCMS();

	  bool firstDuplicate=false;

	  cout << "file1" << endl;
	  // check for duplicate events in the old file
	  bool     eventAdded1;
	  Long64_t duplicateEvents1 = 0;
	  for( Long64_t i = 0; i < entries1; ++i)
	  {
		  c1->GetEntry(i);

		  eventAdded1 = eventMatcher1->addEvent(c1->evt.evt, c1->evt.lumi, c1->evt.run, i);
		  if(!eventAdded1)
		  {
//			  cout << "Event in file1 could not be added : " << " evt = "   << c1->evt.evt
//					  	  	  	  	  	  	  	  	  	  	 << " lumi = "  << c1->evt.lumi
//															 << " run = "   << c1->evt.run
//															 << " entry = " << i
//															 << endl;
			  duplicateEvents1++;

			  if(!firstDuplicate)
			  {
				  firstDuplicate=true;

				  long long dup_entry1=eventMatcher1->retrieveEventNoErase(c1->evt.evt, c1->evt.lumi, c1->evt.run);
				  long long dup_entry2=i;

				  cout << "dup_entry1 = " << dup_entry1 << endl;
				  cout << "dup_entry2 = " << dup_entry2 << endl;

				  c1->GetEntry(dup_entry1);
				  int dup_nPhotons1 = c1->photon.nPhotons;
				  int dup_nref1     = c1->akPu3PF.nref;
				  float dup_pt1[126];
				  for (int j=0; j<126; j++)
				  {
					  dup_pt1[j]=c1->photon.pt[j];
				  }
				  float dup_jtpt1[504];
				  for (int j=0; j<504; j++)
				  {
					  dup_jtpt1[j]=c1->akPu3PF.jtpt[j];
				  }

				  c1->GetEntry(dup_entry2);
				  int dup_nPhotons2 = c1->photon.nPhotons;
				  int dup_nref2     = c1->akPu3PF.nref;
				  float dup_pt2[126];
				  for (int j=0; j<126; j++)
				  {
					  dup_pt2[j]=c1->photon.pt[j];
				  }
				  float dup_jtpt2[504];
				  for (int j=0; j<504; j++)
				  {
					  dup_jtpt2[j]=c1->akPu3PF.jtpt[j];
				  }


				  cout << "dup_entry1 nPhotons = " << dup_nPhotons1 << endl;
				  cout << "dup_entry2 nPhotons = " << dup_nPhotons2 << endl;
				  cout << "dup_entry1 nref = " << dup_nref1 << endl;
				  cout << "dup_entry2 nref = " << dup_nref2 << endl;

				  bool same = (dup_nPhotons1 == dup_nPhotons2);
				  if (same)
				  {
					  for (int j=0; j<dup_nPhotons1; j++)
					  {
						  cout << dup_pt1[j] << " | " << dup_pt2[j] << endl;
						  if (dup_pt1[j] != dup_pt2[j])
						  {
							  same=false;
						  }
					  }
				  }
				  if(!same)
				  {
					  cout << "dup_entry1 = " << dup_entry1 << " and " << "dup_entry2 = " << dup_entry2 << "do not have same content" << endl;
				  }

				  same = (dup_nref1 == dup_nref2);
				  if (same)
				  {
					  for (int j=0; j<dup_nref1; j++)
					  {
						  cout << dup_jtpt1[j] << " | " << dup_jtpt2[j] << endl;
						  if (dup_jtpt1[j] != dup_jtpt2[j])
						  {
							  same=false;
						  }
					  }
				  }
				  if(!same)
				  {
					  cout << "dup_entry1 = " << dup_entry1 << " and " << "dup_entry2 = " << dup_entry2 << "do not have same content" << endl;
				  }

			  }
		  }
	  }	  
	  firstDuplicate=false;

	  cout << "file2" << endl;
	  // check for duplicate events in the new file
	  bool     eventAdded2;
	  Long64_t duplicateEvents2 = 0;
	  for( Long64_t i = 0; i < entries2; ++i)
	  {
		  c2->GetEntry(i);

		  eventAdded2 = eventMatcher2->addEvent(c2->evt.evt, c2->evt.lumi, c2->evt.run, i);
		  if(!eventAdded2)
		  {
//			  cout << "Event in file2 could not be added : "  << " evt = "   << c2->evt.evt
//					  	  	  	  	  	  	  	  	  	  	  << " lumi = "  << c2->evt.lumi
//															  << " run = "   << c2->evt.run
//															  << " entry = " << i
//															  << endl;
			  duplicateEvents2++;

			  if(!firstDuplicate)
			  {
				  firstDuplicate=true;

				  long long dup_entry1=eventMatcher2->retrieveEventNoErase(c2->evt.evt, c2->evt.lumi, c2->evt.run);
				  long long dup_entry2=i;

				  cout << "dup_entry1 = " << dup_entry1 << endl;
				  cout << "dup_entry2 = " << dup_entry2 << endl;

				  c2->GetEntry(dup_entry1);
				  int dup_nPhotons1 = c2->photon.nPhotons;
				  int dup_nref1     = c2->akPu3PF.nref;
				  float dup_pt1[126];
				  for (int j=0; j<126; j++)
				  {
					  dup_pt1[j]=c2->photon.pt[j];
				  }
				  float dup_jtpt1[504];
				  for (int j=0; j<504; j++)
				  {
					  dup_jtpt1[j]=c2->akPu3PF.jtpt[j];
				  }

				  c2->GetEntry(dup_entry2);
				  int dup_nPhotons2 = c2->photon.nPhotons;
				  int dup_nref2     = c2->akPu3PF.nref;
				  float dup_pt2[126];
				  for (int j=0; j<126; j++)
				  {
					  dup_pt2[j]=c2->photon.pt[j];
				  }
				  float dup_jtpt2[504];
				  for (int j=0; j<504; j++)
				  {
					  dup_jtpt2[j]=c2->akPu3PF.jtpt[j];
				  }


				  cout << "dup_entry1 nPhotons = " << dup_nPhotons1 << endl;
				  cout << "dup_entry2 nPhotons = " << dup_nPhotons2 << endl;
				  cout << "dup_entry1 nref = " << dup_nref1 << endl;
				  cout << "dup_entry2 nref = " << dup_nref2 << endl;

				  bool same = (dup_nPhotons1 == dup_nPhotons2);
				  if (same)
				  {
					  for (int j=0; j<dup_nPhotons1; j++)
					  {
						  cout << dup_pt1[j] << " | " << dup_pt2[j] << endl;
						  if (dup_pt1[j] != dup_pt2[j])
						  {
							  same=false;
						  }
					  }
				  }
				  if(!same)
				  {
					  cout << "dup_entry1 = " << dup_entry1 << " and " << "dup_entry2 = " << dup_entry2 << "do not have same content" << endl;
				  }

				  same = (dup_nref1 == dup_nref2);
				  if (same)
				  {
					  for (int j=0; j<dup_nref1; j++)
					  {
						  cout << dup_jtpt1[j] << " | " << dup_jtpt2[j] << endl;
						  if (dup_jtpt1[j] != dup_jtpt2[j])
						  {
							  same=false;
						  }
					  }
				  }
				  if(!same)
				  {
					  cout << "dup_entry1 = " << dup_entry1 << " and " << "dup_entry2 = " << dup_entry2 << "do not have same content" << endl;
				  }

			  }
		  }
	  }

	  // try to find the events from the old file in the new file
	  long long    eventRetrieved1;
	  Long64_t notRetrievedEvents1 = 0;
	  for( Long64_t i = 0; i < entries1; ++i)
	  {
		  c1->GetEntry(i);

//		  eventRetrieved1 = eventMatcher2->retrieveEvent(c1->evt.evt, c1->evt.lumi, c1->evt.run);
		  eventRetrieved1 = eventMatcher2->retrieveEventNoErase(c1->evt.evt, c1->evt.lumi, c1->evt.run);
		  if(eventRetrieved1 < 0)
		  {
//			  cout << "Event in file1 could not be found in file2 : "  << " evt = "   << c1->evt.evt
//					  	  	  	  	  	  	  	  	  	  	           << " lumi = "  << c1->evt.lumi
//															           << " run = "   << c1->evt.run
//															           //<< " entry = " << i
//																	   << endl;

			  notRetrievedEvents1++;
		  }
	  }

	  // try to find the events from the new file in the old file
	  long long     eventRetrieved2;
	  Long64_t notRetrievedEvents2 = 0;
	  for( Long64_t i = 0; i < entries2; ++i)
	  {
		  c2->GetEntry(i);

//		  eventRetrieved2 = eventMatcher1->retrieveEvent(c2->evt.evt, c2->evt.lumi, c2->evt.run);
		  eventRetrieved2 = eventMatcher1->retrieveEventNoErase(c2->evt.evt, c2->evt.lumi, c2->evt.run);
		  if(eventRetrieved2 < 0)
		  {
//			  cout << "Event in file2 could not be found in file1 : "  << " evt = "   << c2->evt.evt
//					  	  	  	  	  	  	  	  	  	  	           << " lumi = "  << c2->evt.lumi
//															           << " run = "   << c2->evt.run
//															           //<< " entry = " << i
//																	   << endl;

			  notRetrievedEvents2++;
		  }
	  }


	  cout << "Duplicate events in file1 = " << duplicateEvents1 << endl;
	  cout << "Duplicate events in file2 = " << duplicateEvents2 << endl;

	  cout << "Events in file1 that are not in file2 = " << notRetrievedEvents1 << endl;
	  cout << "Events in file2 that are not in file1 = " << notRetrievedEvents2 << endl;

	  c1->inf->Close();
	  c2->inf->Close();
}

int main()
{
	const TString fileName1="/mnt/hadoop/cms/store/user/luck/2014-photon-forests/pPb_DATA_photon30trig_localJEC_v1.root";
	const TString fileName2="/mnt/hadoop/cms/store/user/luck/2014-photon-forests/pPb_DATA_photon30trig_localJEC_v2.root";

	eventMatcher(fileName1, fileName2);
}

