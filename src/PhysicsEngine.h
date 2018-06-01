#pragma once

#include <vector>
#include "Utils.h"

struct Ball
{
	glm::vec3 position;
	glm::vec3 velocity;
};

class PhysicsEngine
{
public:
	long time;
	std::vector<Ball> balls;
	static Segment s1, s2, s3, s4;

	void updateState();
	static PhysicsEngine & getEngine();

};

