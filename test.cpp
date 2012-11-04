#include <iostream>
#include <stdlib.h>
#include "Keez.h"
#include "XModMap.h"
#include <assert.h>
#include <stdio.h>

using namespace std;

int main(int argc, char** argv)
{
//	Keez k;

//	k.blockKey(100);
//	cout << "OK" << endl;
//	k.blockKey(100);

	int c1, c2;

	if(argc!=3
			|| sscanf(argv[1], "%d", &c1) != 1
			|| sscanf(argv[2], "%d", &c2) != 1)
	{
		cout << "Usage: " << argv[0] << " code1 code2" << endl;
		return -1;
	}

	XModMap::modmap m = XModMap::getMap();

	assert(c1>=0 && c1<=255 && c2>=0 && c2<=255);

	cout << "disable keys: " << c1 << " (" << m[c1] << ")  <--->  " << c2 << " ("
		<< m[c2] << ")" << endl;

	Keez k;

	set<int> block_keys;
	block_keys.insert(c1);
	block_keys.insert(c2);

	k.start(block_keys);

	sleep(10);

	k.stop();
}
