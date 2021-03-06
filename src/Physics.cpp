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

Physics::Physics() : ball1Id(-1), ball2Id(-1)
{
}

Physics & Physics::getEngine()
{
	return pe;
}

void Physics::updateState()
{
	EventManager & em = EventManager::getEventManager();
	
	Physics::getEngine().finishedUpdatingState = false;
	Physics::getEngine().time = 0.0;

	Event * e;
	while (!finishedUpdatingState)
	{
		e = em.nextEvent();
		double t = e->time - Physics::time;
		move(t);
		e->handle();
		Physics::time = e->time;
		delete e;
	}
}

void Physics::handle(EventInstances::BallStopped * e)
{
	e->ball->stopped = true;
}
void Physics::handle(EventInstances::BoardBounce * e)
{
	e->ball->direction = Utils::reflect(e->ball->direction, e->segment->p2 - e->segment->p1);
}
void Physics::handle(EventInstances::EndFrame * e)
{
	Physics::getEngine().finishedUpdatingState = true;
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
	glm::vec3 normal = e->ball2->position - e->ball1->position;
	normal = glm::normalize(normal);
	glm::vec3 tangent = glm::cross(glm::vec3(0, 0, 1), normal);
	tangent = glm::normalize(tangent);

	glm::mat3 mat(tangent, normal, glm::vec3(0, 0, 1));
	glm::mat3 matInv = glm::inverse(mat);

	glm::vec3 vel1 = e->ball1->speed * e->ball1->direction;
	glm::vec3 vel2 = e->ball2->speed * e->ball2->direction;

	vel1 = matInv * vel1;
	vel2 = matInv * vel2;

	float tmp = vel1.y;
	vel1.y = vel2.y;
	vel2.y = tmp;

	vel1 = mat * vel1;
	vel2 = mat * vel2;

	e->ball1->speed = Utils::length(vel1);
	e->ball2->speed = Utils::length(vel2);

	if (e->ball1->speed > 0)
	{
		e->ball1->direction = glm::normalize(vel1);
		e->ball1->stopped = false;
	}
	if (e->ball2->speed > 0)
	{
		e->ball2->direction = glm::normalize(vel2);
		e->ball2->stopped = false;
	}

	lastCollisionTime = e->time;
	ball1Id = e->ball1->id;
	ball2Id = e->ball2->id;
}