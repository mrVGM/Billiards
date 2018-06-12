#pragma once

#include <glm.hpp>

struct Ball
{
	bool stopped;
	glm::vec3 position;
	glm::vec3 velocity;
};