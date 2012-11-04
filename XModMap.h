#pragma once

#include <map>
#include <string>

// xmodmap util wrapper class

class XModMap
{
	public:
		typedef std::map<int, std::string> modmap;

		static modmap getMap();
		static void setMap(const modmap&);
};
