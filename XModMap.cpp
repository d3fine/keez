#include "XModMap.h"

#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

void err_xmm_format(const string& line)
{
	cerr << "xmodmap file format error, line='" << line << "'" << endl;
	abort();
}

static void extract_keymap(string line, int& key, string& val)
{
	char dummy[100];

	if(line.empty()==false && line[line.length()-1]=='\n')
		line = line.substr(0, line.length()-1);

	if(sscanf(line.c_str(), "%s %d", dummy, &key)!=2)
	{
		err_xmm_format(line);
	}

	size_t eq_pos = line.find(" =");
	if(eq_pos==string::npos)
	{
		err_xmm_format(line);
	}

	val = line.substr(eq_pos+2);
}

XModMap::modmap XModMap::getMap()
{
	modmap result;

	FILE* fl = popen("xmodmap -pke", "r");
	if(fl==0)
	{
		throw "Failed to create pipe";
	}

	while(!feof(fl))
	{
		char buf[256];
		fgets(buf, 256, fl);

		int key;
		string val;
		extract_keymap(buf, key, val);
		result[key]=val;
	}

	fclose(fl);

	return result;
}

void XModMap::setMap(const XModMap::modmap& m)
{
	FILE* fl = popen("xmodmap -", "w");

	for(int i=8; i<256; i++)
	{
		if(m.find(i)==m.end())
		{
			cerr << "XModMap::setMap failed: map doesn't contain key " << i << endl;
			abort();
		}

		fprintf(fl, "keycode %d =%s\n", i, m.at(i).c_str());
	}

	fclose(fl);
}
