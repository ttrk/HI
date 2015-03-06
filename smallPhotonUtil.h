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
 * save recursively all histograms under a directory "dir".
 */
void saveAllHistogramsToPicture(TDirectoryFile* dir, const char* fileType="gif")
{
	TList* keysHisto = getListOfALLKeys(dir, "TH1D");
	TString dirName ( Form(dir->GetName()));
	dirName.ReplaceAll(".root","");		// remove filetype ".root" extension if there is any


	TH1D*  h;
	TKey*  key;
	TIter* iter = new TIter(keysHisto);
	TCanvas* c1=new TCanvas();
	while ((key=(TKey*)iter()))
	{
		h = (TH1D*)key->ReadObj();
		h->Draw();
		c1->SaveAs(Form("%s.%s" ,h->GetName(), fileType));	// name of the file is the name of the histogram
	}
	c1->Close();
}

#endif /* SMALLPHOTONUTIL_H_ */
