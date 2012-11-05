#include "KeyListener.h"

#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include "XTest.h"

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

	cout << "stopping XTest" << endl;
	XTest::stop();
	cout << "XTest stopped" << endl;
	mRunning = false;
	cout << "Joining thread, ptr=" << this << endl;
	pthread_join(mTid, NULL);
}

void* KeyListener::threadRoutine(void* arg)
{
	KeyListener &k = *(KeyListener*)arg;

	cout << "thread started" << endl;

	XTest::start(k.pHandler);

	cout << "Thread: kl=" << &k << endl;
	while(k.mRunning)
	{
		usleep(1000);
	}
}
