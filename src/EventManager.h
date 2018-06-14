#pragma once

#include <queue>
#include "Utils.h"
#include "Primitives.h"

class Event
{
public:
	float time;
	virtual void handle() = 0;
	~Event()
	{
		bool r = true;
	}
};

class EventManager
{
private:
	Event * nextEvent(Ball & ball);
public:
	std::queue<Event *> eventQueue;
	Event * nextEvent();
	static EventManager & getEventManager();
};
