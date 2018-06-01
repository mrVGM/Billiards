#include "stdafx.h"
#include "PhysicsEngine.h"
#include "Utils.h"

#include "glm.hpp"

static PhysicsEngine pe;

Segment PhysicsEngine::s1(glm::vec3(-88.9 + 5.25 / 2.0, -178.45 + 5.25 / 2.0, 0), glm::vec3(88.9 - 5.25 / 2.0, -178.45 + 5.25 / 2.0, 0));
Segment PhysicsEngine::s2(glm::vec3(88.9 - 5.25 / 2.0, -178.45 + 5.25 / 2.0, 0), glm::vec3(88.9 - 5.25 / 2.0, 178.45 - 5.25 / 2.0, 0));
Segment PhysicsEngine::s3(glm::vec3(88.9 + 5.25 / 2.0, 178.45 - 5.25 / 2.0, 0), glm::vec3(-88.9 - 5.25 / 2.0, 178.45 - 5.25 / 2.0, 0));
Segment PhysicsEngine::s4(glm::vec3(-88.9 + 5.25 / 2.0, 178.45 + 5.25 / 2.0, 0), glm::vec3(-88.9 + 5.25 / 2.0, -178.45 - 5.25 / 2.0, 0));

PhysicsEngine & PhysicsEngine::getEngine()
{
	return pe;
}

void PhysicsEngine::updateState()
{
	long newTime = GetTickCount();

	float diff = (newTime - time) / 1000.0f;

	for (int i = 0; i < balls.size(); ++i)
	{
		Ball & curBall = balls[i];
		glm::vec3 offset = diff * curBall.velocity;

		while (length(offset) > 0)
		{
			glm::vec3 intersection;

			Segment * segment = 0;
			bool lies;

			if (intersects(curBall.position, offset, s1, intersection, lies))
			{
				segment = &s1;
			}
			else if (intersects(curBall.position, offset, s2, intersection, lies))
			{
				segment = &s2;
			}
			else if (intersects(curBall.position, offset, s3, intersection, lies))
			{
				segment = &s3;
			}
			else if (intersects(curBall.position, offset, s4, intersection, lies))
			{
				segment = &s4;
			}
			
			if (lies && glm::cross(segment->p2 - segment->p1, curBall.velocity).z >=0)
				segment = 0;

			if (!segment)
			{
				curBall.position += offset;
				break;
			}
			
			float reminder = length(offset) - length(intersection - curBall.position);

			glm::vec3 unitDir = glm::normalize(reflect(offset, segment->p1 - segment->p2));
			offset = reminder * unitDir;
			curBall.velocity = length(curBall.velocity) * unitDir;
			curBall.position = intersection;
		}
	}

	time = newTime;
}