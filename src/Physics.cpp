#include "stdafx.h"
#include "Physics.h"
#include "Utils.h"
#include <fstream>

#include "glm.hpp"

static Physics pe;

float Physics::acceleration = -1;

Segment Physics::s1(glm::vec3(-88.9 + 5.25 / 2.0, -178.45 + 5.25 / 2.0, 0), glm::vec3(88.9 - 5.25 / 2.0, -178.45 + 5.25 / 2.0, 0));
Segment Physics::s2(glm::vec3(88.9 - 5.25 / 2.0, -178.45 + 5.25 / 2.0, 0), glm::vec3(88.9 - 5.25 / 2.0, 178.45 - 5.25 / 2.0, 0));
Segment Physics::s3(glm::vec3(88.9 + 5.25 / 2.0, 178.45 - 5.25 / 2.0, 0), glm::vec3(-88.9 - 5.25 / 2.0, 178.45 - 5.25 / 2.0, 0));
Segment Physics::s4(glm::vec3(-88.9 + 5.25 / 2.0, 178.45 + 5.25 / 2.0, 0), glm::vec3(-88.9 + 5.25 / 2.0, -178.45 - 5.25 / 2.0, 0));

Physics & Physics::getEngine()
{
	return pe;
}

void Physics::updateState()
{
	long newTime = GetTickCount();

	float diff = (newTime - time) / 1000.0f;

	for (int i = 0; i < balls.size(); ++i)
	{
		Ball & curBall = balls[i];

		if (length(curBall.velocity) == 0.0f)
		{
			break;
		}

		float currentVelocity = length(curBall.velocity);
		float newVelocity = acceleration * diff + currentVelocity;

		if (newVelocity < 0)
		{
			curBall.velocity *= 0.0f;
			
			diff = -length(curBall.velocity) / acceleration;
		}

		glm::vec3 offset = (0.5f * acceleration * diff * diff + length(curBall.velocity) * diff) *  glm::normalize(curBall.velocity);

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
				curBall.velocity *= newVelocity / currentVelocity;
				break;
			}
			
			float reminder = length(offset) - length(intersection - curBall.position);

			glm::vec3 unitDir = glm::normalize(reflect(offset, segment->p1 - segment->p2));
			offset = reminder * unitDir;
			curBall.velocity = newVelocity * unitDir;
			curBall.position = intersection;
		}
	}

	time = newTime;
}