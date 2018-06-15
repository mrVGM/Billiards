#pragma once

#include <glm.hpp>

struct Ball
{
private:
	static int count;
public:
	Ball();

	static const float diameter;
	
	int id;
	bool stopped;
	glm::vec3 position;
	float speed;
	glm::vec3 direction;
};
