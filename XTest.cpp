#include "XTest.h"

#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/extensions/record.h>
#include <X11/extensions/XTest.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <iostream>

using namespace std;

struct CallbackClosure {
  Display *ctrlDisplay;
  Display *dataDisplay;
  int curX;
  int curY;
  void *initialObject;
};


static XRecordContext recContext;
static Display* display;
static struct CallbackClosure userData;

bool XTest::mRunning = false;
void (*mCallback)(int, bool) = NULL;

typedef union {
  unsigned char type;
  xEvent event;
  xResourceReq req;
  xGenericReply reply;
  xError error;
  xConnSetupPrefix setup;
} XRecordDatum;

static void eventCallback(XPointer priv, XRecordInterceptData* hook)
{
	XRecordDatum *data = (XRecordDatum*)hook->data;
	unsigned char t;
	int c;
	char keys[32];
	int modifiers;

	if(hook->category != XRecordFromServer)
	{
		XRecordFreeData(hook);
		return;
	}
	t = data->event.u.u.type;
	c = data->event.u.u.detail;

	if(t==KeyPress)
	{
		if(mCallback!=NULL)
			mCallback(c, true);
	}
	else if(t==KeyRelease)
	{
		if(mCallback!=NULL)
			mCallback(c, false);
	}
	
	XRecordFreeData(hook);
}

void XTest::start(void(*cb)(int,bool))
{
	if(XTest::mRunning)
	{
		cerr << "ERROR: XTest::start: already running" << endl;
		abort();
	}

	XTest::mRunning = true;

	mCallback = cb;

	int ev, er, ma, mi;
	XRecordRange *recRange;
	XRecordClientSpec recClientSpec;

	display=XOpenDisplay(0); 

	if(display==NULL)
	{
		fprintf(stderr, "ERROR: bl_start() not in initialized state\n");
		abort();
	}

	if(!XTestQueryExtension(display, &ev, &er, &ma, &mi))
	{
		fprintf(stderr, "ERROR: XTest extension is not loaded to X server\n");
		abort();
	}

	XSynchronize(display, False);

	if(!XRecordQueryVersion(display, &ma, &mi))
	{
		fprintf(stderr, "ERROR: XRecord extension is not loaded to X server\n");
		abort();
	}

	recRange = XRecordAllocRange();
	if(!recRange)
	{
		fprintf(stderr, "ERROR: Couldn't allocate record range\n");
		abort();
	}

	recRange->device_events.first = KeyPress;
	recRange->device_events.last = KeyRelease; // TODO review this line

	recClientSpec = XRecordAllClients;

	recContext = XRecordCreateContext(display, 0, &recClientSpec, 1, &recRange, 1);
	if(!recContext)
	{
		fprintf(stderr, "ERROR: failed to create XRecord context\n");
		abort();
	}

	// TODO: FakeEvent255?
    XTestFakeKeyEvent(userData.ctrlDisplay, 255, True, CurrentTime);
    XTestFakeKeyEvent(userData.ctrlDisplay, 255, False, CurrentTime);

	if(!XRecordEnableContext(display, recContext, eventCallback, (XPointer) &userData))
	{
		fprintf(stderr, "ERROR: XRecord enable context failed\n");
		abort();
	}
}

void XTest::stop()
{
	if(XTest::mRunning==false)
	{
		cerr << "ERROR: XTest::stop: not running" << endl;
		abort();
	}

	XTest::mRunning = false;

	if(!XRecordDisableContext(display, recContext))
	{
		fprintf(stderr, "ERROR: XRecord disable context failed\n");
		abort();
	}
	/*
	if(!XCloseDisplay(display))
	{
		fprintf(stderr, "ERROR: XCloseDisplay failed\n");
		abort();
	}
	*/
}
