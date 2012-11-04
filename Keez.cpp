#include "Keez.h"

#include <iostream>
#include <stdlib.h>
#include <assert.h>

using namespace std;

static XModMap::modmap unmap(const XModMap::modmap& orig, const set<int>& keys)
{
	XModMap::modmap res = orig;
	int ptr=255;
	
	for(set<int>::const_iterator it = keys.begin(); it != keys.end(); it++)
	{
		assert(*it>=8 && *it<=255);
		
		if(orig.at(*it).empty())
		{
			cerr << "ERROR: Requested block of key " << *it << ", which has no action associated with it. It is probably an error. "
				<< "Current policy prevents blocking of such keys." << endl;
			abort();
		}

		while(res[ptr].empty()==false)
		{
			ptr--;
			if(ptr==8)
			{
				cerr << "ERROR: Not enough \"empty\" keys to map" << endl;
				abort();
			}
		}

		res[ptr] = res[*it];
		res[*it] = "";
	}

	return res;
}

Keez::Keez()
	: mRunning(false)
{
}

Keez::~Keez()
{
	if(mRunning)
		stop();
}

void Keez::start(const set<int>& block_keys)
{
	if(mRunning)
	{
		cerr << "ERROR: Keez::start: Already started" << endl;
		abort();
	}

	// make new and store current xmodmap
	mOrigMap = XModMap::getMap();
	XModMap::modmap cur_map = unmap(mOrigMap, block_keys);

	XModMap::setMap(cur_map);

	mRunning=true;
}

void Keez::stop()
{
	if(!mRunning)
	{
		cerr << "ERROR: Keez::stop: Not started" << endl;
		abort();
	}

	// restore orig map
	XModMap::setMap(mOrigMap);

	mRunning=false;
}
