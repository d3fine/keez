#pragma once

#include <set>
#include <map>

#include "XModMap.h"
#include "KeyListener.h"

class KeySender
{
	public:
		virtual void sendKey(int code, bool down)=0;
};

class Keez
{
	Keez(const Keez&){}
	void operator=(const Keez&){}
	public:
		Keez();
		~Keez();

		void start(const std::set<int>& block_keys,
				void(*)(int,bool));
		void stop();

	private:
		bool mRunning;
		XModMap::modmap mOrigMap;
		std::map<int, int> mKeysMap;
		KeyListener mKL;

		void(*mHandler)(int,bool);
};
