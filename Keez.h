#pragma once

#include <set>
#include <map>

#include "XModMap.h"

class Keez
{
	public:
		Keez();
		~Keez();

		void start(const std::set<int>& block_keys);
		void stop();

	private:
		bool mRunning;
		XModMap::modmap mOrigMap;
};
