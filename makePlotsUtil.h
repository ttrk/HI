/*
 * makePlotsUtil.h
 *
 *  Created on: Feb 20, 2015
 *      Author: kaya
 */

#ifndef MAKEPLOTSUTIL_H_
#define MAKEPLOTSUTIL_H_

#include <string>

using  std::string;
using  std::cout;
using  std::endl;

void mergeCuts(TCut cut, TCut* cuts);
void mergeCuts(TCut cut, TCut* cuts, int len);

/*
 * merge a set of cuts into a single one. the resulting cut is the cut that satisfies all individual cuts
 *
 * need to initialize "cut" before this function.
 */
void mergeCuts(TCut cut, TCut* cuts)
{
	// http://stackoverflow.com/questions/4108313/how-do-i-find-the-length-of-an-array
	int len = (sizeof (cuts) / sizeof (*cuts));
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

#endif /* MAKEPLOTSUTIL_H_ */
