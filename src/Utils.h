#pragma once

#include <glm.hpp>

struct Segment
{
	Segment() {}
	Segment(const glm::vec3 & x, const glm::vec3 & y) : p1(x), p2(y) {}
	glm::vec3 p1;
	glm::vec3 p2;
};

bool intersects(const glm::vec3 & point, const glm::vec3 & dir, const Segment & segment, glm::vec3 & intersection);