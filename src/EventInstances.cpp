#include "stdafx.h"

#include "EventInstances.h"
#include "Physics.h"

#define IMPLEMENT_EVENT_HANDLE(name) void EventInstances:: name ::handle() \
{ \
	Physics::getEngine().handle(this); \
}

IMPLEMENT_EVENT_HANDLE(BallStopped)
IMPLEMENT_EVENT_HANDLE(EndFrame)
IMPLEMENT_EVENT_HANDLE(BoardBounce)