#include "stdafx.h"
#include "Physics.h"
#include "Utils.h"

#include "glm.hpp"

static Physics pe;

float Physics::acceleration = -1;

const Utils::Segment Physics::s1(glm::vec3(-88.9 + 5.25 / 2.0, -178.45 + 5.25 / 2.0, 0), glm::vec3(88.9 - 5.25 / 2.0, -178.45 + 5.25 / 2.0, 0));
const Utils::Segment Physics::s2(glm::vec3(88.9 - 5.25 / 2.0, -178.45 + 5.25 / 2.0, 0), glm::vec3(88.9 - 5.25 / 2.0, 178.45 - 5.25 / 2.0, 0));
const Utils::Segment Physics::s3(glm::vec3(88.9 + 5.25 / 2.0, 178.45 - 5.25 / 2.0, 0), glm::vec3(-88.9 - 5.25 / 2.0, 178.45 - 5.25 / 2.0, 0));
const Utils::Segment Physics::s4(glm::vec3(-88.9 + 5.25 / 2.0, 178.45 + 5.25 / 2.0, 0), glm::vec3(-88.9 + 5.25 / 2.0, -178.45 - 5.25 / 2.0, 0));

Physics & Physics::getEngine()
{
	return pe;
}

void Physics::updateState()
{
	EventManager & em = EventManager::getEventManager();
	
	Event * e;
	bool finished = false;
	do
	{
		e = em.nextEvent();
		finished = dynamic_cast<EventInstances::EndFrame *>(e);
		e->handle();
	} while (!finished);
	
}

void Physics::handle(EventInstances::BallStopped * e)
{
	e->ball->stopped = true;
	Physics::time = e->time;
}
void Physics::handle(EventInstances::BoardBounce * e)
{
	double t = e->time - Physics::time;
	move(t);
	e->ball->direction = Utils::reflect(e->ball->direction, e->segment->p2 - e->segment->p1);
	Physics::time = e->time;
}
void Physics::handle(EventInstances::EndFrame * e)
{
	double t = RenderWindow::waitingTime / 1000.0 - Physics::time;
	move(t);
	Physics::time = 0.0;
}

void Physics::move(float t)
{
	for (int i = 0; i < Physics::getEngine().balls.size(); ++i)
	{
		if (Physics::getEngine().balls[i].stopped)
			continue;
		float dist = Utils::route(t, Physics::getEngine().balls[i].speed, Physics::acceleration);
		float sp = Utils::speed(t, Physics::getEngine().balls[i].speed, Physics::acceleration);

		Physics::getEngine().balls[i].speed = sp;
		Physics::getEngine().balls[i].position += dist * Physics::getEngine().balls[i].direction;
	}
}

void Physics::handle(EventInstances::BallCollision * e)
{
	double t = RenderWindow::waitingTime / 1000.0 - Physics::time;
	move(t);
	
	Ball * primary = e->ball1;
	Ball * secondary = e->ball2;
	if (e->ball2->speed > primary->speed)
	{
		primary = e->ball2;
		secondary = e->ball1;
	}

	glm::vec3 line = glm::normalize(secondary->position - primary->position);
	
	float coef = glm::dot(primary->direction, line);

	glm::vec3 velocitySec = secondary->speed * secondary->direction;
	velocitySec += (1 - coef) * primary->speed * line;

	glm::vec3 velocityPrim = coef * primary->speed * primary->direction;

	velocityPrim = (primary->speed * primary->direction + secondary->speed * secondary->direction) - velocityPrim - velocitySec;

	primary->speed = Utils::length(velocityPrim);
	primary->direction = glm::normalize(velocityPrim);

	secondary->speed = Utils::length(velocitySec);
	secondary->direction = glm::normalize(velocitySec);
}