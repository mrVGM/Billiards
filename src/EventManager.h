#pragma once

#include <queue>
#include "Utils.h"

class Event
{
public:
	float time;
	virtual void handle() = 0;
};

class EventManager
{
public:
	std::queue<Event *> eventQueue;
	Event * nextEvent();
};
