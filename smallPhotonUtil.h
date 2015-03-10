/*
 * makePlotsUtil.h
 *
 *  Created on: Feb 20, 2015
 *      Author: kaya
 */

#ifndef SMALLPHOTONUTIL_H_
#define SMALLPHOTONUTIL_H_

#include <string>

using  std::string;
using  std::cout;
using  std::endl;

void mergeCuts(TCut cut, TCut* cuts);
void mergeCuts(TCut cut, TCut* cuts, int len);
TList* getListOfALLKeys(TDirectoryFile* dir);
TList* getListOfALLKeys(TDirectoryFile* dir, const char* type);
//void saveAllHistogramsToPicture(TDirectoryFile* dir, const char* fileType="gif");		// this line gives error if I use default value option (here const char* fileType="gif") in the function definition.

/*
 * merge a set of cuts into a single one. the resulting cut is the cut that satisfies all individual cuts
 *
 * need to initialize "cut" before this function.
 */
void mergeCuts(TCut cut, TCut* cuts)
{
	// http://stackoverflow.com/questions/4108313/how-do-i-find-the-length-of-an-array
	int len = (sizeof (cuts) / sizeof (*cuts));
	mergeCuts(cut,cuts,len);
}

/*
 * merge a set of cuts into a single one. the resulting cut is the cut that satisfies all individual cuts
 *
 * need to initialize "cut" before this function.
 */
void mergeCuts(TCut cut, TCut* cuts, int len)
{
	if (len>0)
	{
		cut=cuts[0];
		for (int i=1; i<len; i++)
		{
			cut = cut && cuts[i];
		}
	}
	else
	{
		cout << "there are not cuts to merge" <<endl;
	}
}

TList* getListOfALLKeys(TDirectoryFile* dir)
{
	const char *className_Dir = "TDirectoryFile";

	TList* keysInDir = dir->GetListOfKeys();
	TIter* iter = new TIter(keysInDir);

	TDirectoryFile *subdir;
	TKey*  key;
	TList* keys=new TList();
	TList *newKeys=new TList();

	while ((key=(TKey*)iter())) {

		keys->Add(key);

		// traverse directories in a DFS manner (recursively)
		if(key->IsFolder())
		{
			subdir=(TDirectoryFile*)key->ReadObj();
			newKeys=getListOfALLKeys(subdir);
			keys->AddAll(newKeys);
		}
	}
	return keys;
}

/*
 * get list of all keys under a directory "dir" for objects of a given "type"
 */
TList* getListOfALLKeys(TDirectoryFile* dir, const char* type)
{
	TList* keysOfType=new TList();
	TList* keysALL = getListOfALLKeys(dir);

	TIter* iter = new TIter(keysALL);
	TKey*  key;
	while ((key=(TKey*)iter()))
	{
//		http://www.cplusplus.com/reference/cstring/strcmp/
		if(strcmp(key->GetClassName(), type) == 0)
		{
			keysOfType->Add(key);
		}
	}

	return keysOfType;
}

/*
 * get list of all histograms under a directory "dir" for objects of a given "type"
 */
TList* getListOfALLHistograms(TDirectoryFile* dir)
{
	TList* histos=new TList();
	TList* keysHisto = getListOfALLKeys(dir, "TH1D");

	TIter* iter = new TIter(keysHisto);
	TKey*  key;
	while ((key=(TKey*)iter()))
	{
		histos->Add((TH1D*)key->ReadObj());
	}

	return histos;
}

/*
 * save recursively all the histograms inside a TDirectoryFile "dir" to images
 */
void saveAllHistogramsToPicture(TDirectoryFile* dir, const char* fileType="gif", const char* directoryToBeSavedIn="", int styleIndex=0, int rebin=1)
{
	TList* keysHisto = getListOfALLKeys(dir, "TH1D");

	TH1D*  h;
	TKey*  key;
	TIter* iter = new TIter(keysHisto);
	TCanvas* c1=new TCanvas();
	while ((key=(TKey*)iter()))
	{
		h = (TH1D*)key->ReadObj();

		if(rebin!=1)
		{
			h->Rebin(rebin);
		}

		if(styleIndex==1)
		{
			h->Draw("E");
		}
		else
		{
			h->Draw();
		}

		if(strcmp(directoryToBeSavedIn, "") == 0)	// save in the current directory if no directory is specified
		{
			c1->SaveAs(Form("%s.%s" ,h->GetName(), fileType));	// name of the file is the name of the histogram
		}
		else
		{
			c1->SaveAs(Form("%s/%s.%s", directoryToBeSavedIn ,h->GetName(), fileType));
		}
	}
	c1->Close();
}

/*
 * 	function to save histogram pictures into directories that are mostly likely to be preferred.
 *
 *	absolute path of the macro calling this method = /path/to/macro/myMacro.C
 *	absolute path of the TDirectoryFile = /path/to/file/myFile.root
 *
 *	dirType = 0 (default option) --> save files under   /path/to/macro/
 *	dirType = 1 				 --> save files under   /path/to/macro/myFile
 *	dirType = 2 				 --> save files under   /path/to/file/
 *	dirType = 3 				 --> save files under   /path/to/file/myFile
 *
 * */
void saveAllHistogramsToPicture(TDirectoryFile* dir, const char* fileType="gif", int dirType=0, int styleIndex=0, int rebin=1)
{
	const char* directoryToBeSavedIn="";

	if(dirType==0)
	{
		directoryToBeSavedIn="";
	}
	else if(dirType==1)
	{
		  const char* baseName=gSystem->BaseName(dir->GetName()); 	//    const char* baseName=basename(f->GetName());	does NOT work
		  TString dirName(baseName);
		  dirName.ReplaceAll(".root","");
		  gSystem->mkdir(dirName);
		  directoryToBeSavedIn=dirName;
	}
	else if(dirType==2)
	{
		directoryToBeSavedIn=gSystem->DirName(dir->GetName());
	}
	else if(dirType==3)
	{
		  TString dirName(dir->GetName());
		  dirName.ReplaceAll(".root","");
		  gSystem->mkdir(dirName);
		  directoryToBeSavedIn=dirName;
	}

	saveAllHistogramsToPicture(dir,fileType,directoryToBeSavedIn, styleIndex, rebin);
}

/*
 * MODIFY THIS
 */
void saveAllCanvasesToPicture(TList* canvases, const char* fileType="gif", const char* directoryToBeSavedIn="")
{
	TCanvas* c;
	TIter* iter = new TIter(canvases);
	while ((c=(TCanvas*)iter()))
	{
		if(strcmp(directoryToBeSavedIn, "") == 0)	// save in the current directory if no directory is specified
		{
			c->SaveAs(Form("%s.%s" ,c->GetName(), fileType));	// name of the file is the name of the histogram
		}
		else
		{
			c->SaveAs(Form("%s/%s.%s", directoryToBeSavedIn ,c->GetName(), fileType));
		}
	}
//	c->Close();
}

/*
 *  divide histograms element wise
 */
TList* divideHistogramList(TList* histoList1, TList* histoList2)
{
	TH1D::SetDefaultSumw2();
	TList* histos_Division=new TList();

	TH1D*  h1;
	TH1D*  h2;
	TH1D*  h_division;
	for(int i=0; i<histoList1->GetEntries(); i++)
	{
		h1=(TH1D*)histoList1->At(i);
		h2=(TH1D*)histoList2->At(i);

//		cout<<h1->GetNbinsX()<<endl;
//		cout<<h2->GetNbinsX()<<endl;

//		h1->Rebin(5);
//		h2->Rebin(5);

		h1->Scale(1/(h1->GetEntries()));
		h2->Scale(1/(h2->GetEntries()));

//		cout<<h1->GetNbinsX()<<endl;
//		cout<<h2->GetNbinsX()<<endl;

		h_division=new TH1D(h1->GetName(),h1->GetTitle(),h1->GetNbinsX(),h1->GetXaxis()->GetXmin(),h1->GetXaxis()->GetXmax());
//		h_division=new TH1D();
		h_division->Divide(h1,h2);
		h_division->SetName(Form("%s_ratio",h1->GetName()));
		h_division->SetTitle(Form("ratio of %s",h1->GetTitle()));

		histos_Division->Add(h_division);
	}

	return histos_Division;
}

/*
 *  divide histograms from 2 directories element wise
 */
TList* divideHistogramList(TDirectoryFile* dir1, TDirectoryFile* dir2)
{
	TList* histoList1=getListOfALLHistograms(dir1);
	TList* histoList2=getListOfALLHistograms(dir2);

	return divideHistogramList(histoList1, histoList2);
}

void saveAllHistogramsToFile(const char* fileName, TList* histos)
{
	TFile* f=new TFile(fileName, "RECREATE");

	TH1D* h;
	TIter* iter = new TIter(histos);
	while ((h=(TH1D*)iter()))
	{
		h->Write();
	}

	f->Close();
}

/*
 * get a list files and folders in the given directory
 * https://root.cern.ch/phpBB3/viewtopic.php?f=3&t=12793
 *
 * default option : all files in the current directory will be returned
 *
 *
 *
 * how to access the result :
 *
    TObjString* tmpObjStr;
    TString     tmpStr;
	for(int i=0; i<list->GetEntries(); i++)
	{
		tmpObjStr=(TObjString*)list->At(i);
		tmpStr   =tmpObjStr->GetString();
		cout     << tmpStr <<endl;
	}
 */
TList* getFileNames(const char *dirname=".", const char *ext="")
{
   // ignore errors of Eclipse about TSystem objects
   TSystemDirectory dir(dirname, dirname);
   TList* files = dir.GetListOfFiles();
   TList* outFileNames=new TList();
   if (files) {
      TSystemFile *file;
      // TString cannot be stored in a TCollection... use TObjString instead.
      // https://root.cern.ch/root/htmldoc/TString.html
      TObjString* fname;
      TIter next(files);
      while ((file=(TSystemFile*)next())) {
         fname = new TObjString(file->GetName());
         if (fname->GetString().EndsWith(ext)) {
//            cout << fname->GetString().Data() << endl;
            	outFileNames->Add(fname);
         }
      }
   }
   else
   {
	   cout << "no files with given extension found in : " << dirname << endl;
   }

   return outFileNames;
}

/*
 * copied from https://github.com/CmsHI/HiForestAnalysis/blob/master/commonUtility.h
 */
Double_t getDR( Double_t eta1, Double_t phi1, Double_t eta2, Double_t phi2){
  Double_t theDphi = getDPHI( phi1, phi2);
  Double_t theDeta = eta1 - eta2;
  return TMath::Sqrt ( theDphi*theDphi + theDeta*theDeta);
}

/*
 * copied from https://github.com/CmsHI/HiForestAnalysis/blob/master/commonUtility.h
 */
Double_t getDPHI( Double_t phi1, Double_t phi2) {
  Double_t dphi = phi1 - phi2;

  if ( dphi > 3.141592653589 )
    dphi = dphi - 2. * 3.141592653589;
  if ( dphi <= -3.141592653589 )
    dphi = dphi + 2. * 3.141592653589;

  if ( TMath::Abs(dphi) > 3.141592653589 ) {
    cout << " commonUtility::getDPHI error!!! dphi is bigger than 3.141592653589 " << endl;
  }

  return dphi;
}

Double_t getDETA(Double_t eta1, Double_t eta2){
	return eta1 - eta2;
}

#endif /* SMALLPHOTONUTIL_H_ */
