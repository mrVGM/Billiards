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

static EventInstances::BallCollision * testForCollisions(double t)
{
	Physics & p = Physics::getEngine();

	for (int index = 0; index < p.balls.size(); ++index)
	{
		Ball & curBall = p.balls[index];

		int steps = ceil(t * 1000);
		for (int i = 0; i <= steps; ++i)
		{
			double t1 = i * t / steps;

			for (int j = index + 1; j < p.balls.size(); ++j)
			{
				Ball & otherBall = p.balls[j];
				if (otherBall.id == curBall.id)
					continue;

				float curRoute = Utils::route(t1, curBall.speed, Physics::acceleration);
				float otherRoute = Utils::route(t1, otherBall.speed, Physics::acceleration);

				glm::vec3 curPos = curBall.position + (curRoute * curBall.direction);
				glm::vec3 otherPos = otherBall.position + (otherRoute * otherBall.direction);

				if (Utils::length(curPos - otherPos) <= Ball::diameter)
				{
					glm::vec3 tmp = glm::normalize(otherPos - curPos);

					if (glm::dot(tmp, curBall.speed * curBall.direction) > FLT_EPSILON || glm::dot(-tmp, otherBall.speed * otherBall.direction) > FLT_EPSILON)
					{
						if (p.ball1Id == curBall.id && p.ball2Id == otherBall.id && p.lastCollisionTime == t1 + p.time)
						{
							p.ball1Id = -1;
							p.ball2Id = -1;
							return NULL;
						}

						EventInstances::BallCollision * bc = new EventInstances::BallCollision;
						bc->ball1 = &curBall;
						bc->ball2 = &otherBall;
						bc->time = t1 + p.time;

						return bc;
					}
				}
			}
		}
	}
	return NULL;
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
		Event * cur = nextEvent(i);
		if (cur)
		{
			events.push_back(cur);
			if (cur->time < earliest)
				earliest = cur->time;
		}
	}

	double t = events.empty() ? RenderWindow::waitingTime / 1000.0 - p.time : earliest - p.time;
	EventInstances::BallCollision * bc = testForCollisions(t);
	if (bc)
	{
		events.push_back(bc);
		earliest = bc->time;
	}

	if (events.empty())
	{
		EventInstances::EndFrame * ef = new EventInstances::EndFrame;
		ef->time = RenderWindow::waitingTime / 1000.0;
		return ef;
	}

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

Event * EventManager::nextEvent(int index)
{
	Physics & p = Physics::getEngine();
	Ball & curBall = p.balls[index];

	double t = RenderWindow::waitingTime / 1000.0 - p.time;

	if (curBall.stopped)
		return NULL;

	if (curBall.speed == 0.0f)
	{
		EventInstances::BallStopped * ballStopped = new EventInstances::BallStopped;
		ballStopped->time = p.time;
		ballStopped->ball = &curBall;
		return ballStopped;
	}

	if (t < FLT_EPSILON)
		return NULL;

	glm::vec3 offset = Utils::route(t, curBall.speed, Physics::acceleration) * curBall.direction;

	const Utils::Segment const * segment;
	glm::vec3 intersection;

	if (boardBounce(curBall.position, offset, segment, intersection))
	{
		EventInstances::BoardBounce * bb = new EventInstances::BoardBounce;
		float distToBounce = Utils::length(intersection - curBall.position);
		//0.5 * a * t^2 + v0 * t = distToBounce
		bb->time = Utils::time(distToBounce, curBall.speed, Physics::acceleration) + Physics::getEngine().time;
		bb->ball = &curBall;
		bb->segment = segment;
		return bb;
	}

	return NULL;
}