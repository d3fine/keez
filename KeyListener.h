#pragma once

#include <pthread.h>

class KeyListener
{
	KeyListener(const KeyListener&){}
	void operator=(const KeyListener&){}
	public:
		KeyListener();
		~KeyListener();

		void start(void(*)(int, bool));
		void stop();

	private:
		pthread_t mTid;
		void(*pHandler)(int,bool);
		bool mRunning;

		static void* threadRoutine(void*);
};
