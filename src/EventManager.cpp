#include "stdafx.h"

#include <glm.hpp>

#include "EventManager.h"
#include "RenderWindow.h"
#include "Utils.h"
#include "Physics.h"

#include "EventInstances.h"

static EventManager em;

static bool boardBounce(const glm::vec3 & position, const glm::vec3 & offset, const Utils::Segment * & segment, glm::vec3 & intersection)
{
	bool lies;

	if (Utils::intersects(position, offset, Physics::s1, intersection, lies))
	{
		segment = &Physics::s1;
	}
	else if (Utils::intersects(position, offset, Physics::s2, intersection, lies))
	{
		segment = &Physics::s2;
	}
	else if (Utils::intersects(position, offset, Physics::s3, intersection, lies))
	{
		segment = &Physics::s3;
	}
	else if (Utils::intersects(position, offset, Physics::s4, intersection, lies))
	{
		segment = &Physics::s4;
	}
	else {
		return false;
	}

	if (lies && glm::cross(segment->p2 - segment->p1, offset).z >= 0)
		return false;

	return true;
}

Event * EventManager::nextEvent()
{
	if (!eventQueue.empty())
	{
		Event * res = eventQueue.front();
		eventQueue.pop();
		return res;
	}

	Physics & p = Physics::getEngine();

	std::vector<Event *> events;

	float earliest = RenderWindow::waitingTime / 1000.0f + 1.0f;
	for (int i = 0; i < p.balls.size(); ++i)
	{
		Event * cur = nextEvent(p.balls[i]);
		if (cur)
		{
			events.push_back(cur);
			if (cur->time < earliest)
				earliest = cur->time;
		}
	}

	if (events.empty())
		return new EventInstances::EndFrame();

	for (int i = 0; i < events.size(); ++i)
	{
		if (events[i]->time == earliest)
			eventQueue.push(events[i]);
		else
			delete events[i];
	}
	Event * res = eventQueue.front();
	eventQueue.pop();
	return res;
}

EventManager & EventManager::getEventManager()
{
	return em;
}

Event * EventManager::nextEvent(Ball & ball)
{
	Physics & p = Physics::getEngine();
	Ball & curBall = ball;

	if (curBall.stopped)
		return NULL;

	if (Utils::length(curBall.velocity) == 0.0f)
	{
		EventInstances::BallStopped * ballStopped = new EventInstances::BallStopped;
		ballStopped->time = p.time;
		ballStopped->ball = &curBall;
		return ballStopped;
	}

	double t = RenderWindow::waitingTime / 1000.0 - p.time;

	double speed = Utils::length(curBall.velocity) + Physics::acceleration * t;
	if (speed <= 0)
	{
		//0 = length(curBall.velocity) + Physics::acceleration * t
		t = -Utils::length(curBall.velocity) / Physics::acceleration;
	}

	glm::vec3 offset = Utils::route(t, Utils::length(curBall.velocity), Physics::acceleration) * glm::normalize(curBall.velocity);

	const Utils::Segment const * segment;
	glm::vec3 intersection;

	if (boardBounce(curBall.position, offset, segment, intersection))
	{
		EventInstances::BoardBounce * bb = new EventInstances::BoardBounce;
		float distToBounce = Utils::length(intersection - curBall.position);
		//0.5 * a * t^2 + v0 * t = distToBounce
		bb->time = Utils::time(distToBounce, Utils::length(curBall.velocity), Physics::acceleration);
		bb->ball = &curBall;
		bb->segment = segment;
		return bb;
	}

	return NULL;
}