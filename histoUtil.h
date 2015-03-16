/*
 * makePlotsUtil.h
 *
 *  Created on: Feb 20, 2015
 *      Author: kaya
 */

#ifndef SMALLPHOTONUTIL_H_
#define SMALLPHOTONUTIL_H_

//#include <TCut.h>               // compiling macros give error if this is not included.

int      getNumBins(double xmin, double xmax, int numBinsPerUnitX);
bool     compareHistograms(TH1* h1, TH1* h2);

using  std::string;
using  std::cout;
using  std::endl;

/*
 * get total number of bins corresponding to range [xmin, xmax] and number of bins for 1 unit of x
 *
 */
int getNumBins(double xmin, double xmax, int numBinsPerUnitX)
{
	double width=xmax-xmin;
	int n = (int)ceil(width*numBinsPerUnitX);
	return n;
}

/*
 * compare two histograms bin by bin.
 * */
bool compareHistograms(TH1* h1, TH1* h2)
{
	int numBins=h1->GetNbinsX();
	if(numBins != h2->GetNbinsX())
		return false;

	for(int i=0; i<numBins; i++)
	{
		if(h1->GetBinContent(i)!=h2->GetBinContent(i))
			return false;
	}
	return true;
}
