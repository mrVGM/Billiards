#include "stdafx.h"
#include "Physics.h"
#include "Utils.h"
#include <fstream>

#include "glm.hpp"

static Physics pe;

float Physics::acceleration = -1;

Utils::Segment Physics::s1(glm::vec3(-88.9 + 5.25 / 2.0, -178.45 + 5.25 / 2.0, 0), glm::vec3(88.9 - 5.25 / 2.0, -178.45 + 5.25 / 2.0, 0));
Utils::Segment Physics::s2(glm::vec3(88.9 - 5.25 / 2.0, -178.45 + 5.25 / 2.0, 0), glm::vec3(88.9 - 5.25 / 2.0, 178.45 - 5.25 / 2.0, 0));
Utils::Segment Physics::s3(glm::vec3(88.9 + 5.25 / 2.0, 178.45 - 5.25 / 2.0, 0), glm::vec3(-88.9 - 5.25 / 2.0, 178.45 - 5.25 / 2.0, 0));
Utils::Segment Physics::s4(glm::vec3(-88.9 + 5.25 / 2.0, 178.45 + 5.25 / 2.0, 0), glm::vec3(-88.9 + 5.25 / 2.0, -178.45 - 5.25 / 2.0, 0));

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

		if (Utils::length(curBall.velocity) == 0.0f)
		{
			break;
		}

		float currentVelocity = Utils::length(curBall.velocity);
		float newVelocity = acceleration * diff + currentVelocity;

		if (newVelocity < 0)
		{
			curBall.velocity *= 0.0f;
			
			diff = -Utils::length(curBall.velocity) / acceleration;
		}

		glm::vec3 offset = (0.5f * acceleration * diff * diff + Utils::length(curBall.velocity) * diff) *  glm::normalize(curBall.velocity);

		while (Utils::length(offset) > 0)
		{
			glm::vec3 intersection;

			Utils::Segment * segment = 0;
			bool lies;

			if (Utils::intersects(curBall.position, offset, s1, intersection, lies))
			{
				segment = &s1;
			}
			else if (Utils::intersects(curBall.position, offset, s2, intersection, lies))
			{
				segment = &s2;
			}
			else if (Utils::intersects(curBall.position, offset, s3, intersection, lies))
			{
				segment = &s3;
			}
			else if (Utils::intersects(curBall.position, offset, s4, intersection, lies))
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
			
			float reminder = Utils::length(offset) - Utils::length(intersection - curBall.position);

			glm::vec3 unitDir = glm::normalize(Utils::reflect(offset, segment->p1 - segment->p2));
			offset = reminder * unitDir;
			curBall.velocity = newVelocity * unitDir;
			curBall.position = intersection;
		}
	}

	time = newTime;
}