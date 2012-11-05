#include "Keez.h"

#include <iostream>
#include <stdlib.h>
#include <assert.h>

using namespace std;

static XModMap::modmap unmap(const XModMap::modmap& orig, const set<int>& keys, map<int, int>& kmap)
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
		kmap[*it]=ptr;
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

void Keez::start(const set<int>& block_keys, void(*h)(int,bool))
{
	if(mRunning)
	{
		cerr << "ERROR: Keez::start: Already started" << endl;
		abort();
	}

	// make new and store current xmodmap
	mOrigMap = XModMap::getMap();
	XModMap::modmap cur_map = unmap(mOrigMap, block_keys, mKeysMap);

	XModMap::setMap(cur_map);

	mHandler=h;

	mRunning=true;

	mKL.start(mHandler);
}

void Keez::stop()
{
	if(!mRunning)
	{
		cerr << "ERROR: Keez::stop: Not started" << endl;
		abort();
	}

	XModMap::setMap(mOrigMap);
	
	cout << "xmodmap restored, stopping listener" << endl;
	mKL.stop();
	cout << "Listener stopped" << endl;

	// restore orig map

	mRunning=false;
}

