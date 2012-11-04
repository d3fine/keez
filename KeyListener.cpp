#include "KeyListener.h"

#include <iostream>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

KeyListener::KeyListener()
	: mRunning(false)
{
}

KeyListener::~KeyListener()
{
	if(mRunning)
		stop();
}

void KeyListener::start(void (*handler)(int,bool))
{
	if(mRunning)
	{
		cerr << "ERROR: KeyListener::start: already running" << endl;
		abort();
	}

	pHandler = handler;

	mRunning = true;

	if(pthread_create(&mTid, NULL, threadRoutine, (void*)this) != 0)
	{
		cerr << "ERROR: Failed to create thread" << endl;
		abort();
	}
}

void KeyListener::stop()
{
	if(!mRunning)
	{
		cerr << "ERROR: KeyListener::stop: not running" << endl;
		abort();
	}

	pthread_cancel(mTid);
	pthread_join(mTid, NULL);

	cout << "thread canceled" << endl;

	mRunning = false;
}

void* KeyListener::threadRoutine(void* arg)
{
	KeyListener &k = *(KeyListener*)arg;

	cout << "thread started" << endl;

	while(true)
	{
		usleep(1000);
	}
}
