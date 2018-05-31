#include "stdafx.h"
#include "PhysicsEngine.h"

#include "glm.hpp"

static PhysicsEngine pe;

Segment PhysicsEngine::s1(glm::vec3(-88.9 + 5.25 / 2.0, -178.45 + 5.25 / 2.0, 0), glm::vec3(88.9 - 5.25 / 2.0, -178.45 + 5.25 / 2.0, 0));
Segment PhysicsEngine::s2(glm::vec3(88.9 - 5.25 / 2.0, -178.45 + 5.25 / 2.0, 0), glm::vec3(88.9 - 5.25 / 2.0, 178.45 - 5.25 / 2.0, 0));
Segment PhysicsEngine::s3(glm::vec3(-88.9 + 5.25 / 2.0, 178.45 - 5.25 / 2.0, 0), glm::vec3(88.9 - 5.25 / 2.0, 178.45 - 5.25 / 2.0, 0));
Segment PhysicsEngine::s4(glm::vec3(-88.9 + 5.25 / 2.0, -178.45 + 5.25 / 2.0, 0), glm::vec3(-88.9 + 5.25 / 2.0, 178.45 - 5.25 / 2.0, 0));

PhysicsEngine & PhysicsEngine::getEngine()
{
	return pe;
}

void PhysicsEngine::updateState()
{
	long newTime = GetTickCount();

	float diff = (newTime - time) / 1000;

	for (int i = 0; i < balls.size(); ++i)
	{
		Ball & curBall = balls[i];
		glm::vec3 offset = diff * curBall.velocity;

		while (offset.length() > 0)
		{
			glm::vec3 intersection;

			Segment * segment = 0;

			if (intersects(curBall.position, offset, s1, intersection))
			{
				segment = &s1;
			}
			else if (intersects(curBall.position, offset, s2, intersection))
			{
				segment = &s2;
			}
			else if (intersects(curBall.position, offset, s3, intersection))
			{
				segment = &s3;
			}
			else if (intersects(curBall.position, offset, s4, intersection))
			{
				segment = &s4;
			}
			
			if (!segment)
				break;

			float reminder = offset.length() - (intersection - curBall.position).length();

			glm::vec3 offset = reminder * glm::normalize(reflect(offset, segment->p1 - segment->p2));
			curBall.position = intersection;
		}
	}

	time = newTime;
}