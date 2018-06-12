#pragma once

#include <glm.hpp>

namespace Utils
{
	struct Segment
	{
		Segment() {}
		Segment(const glm::vec3 & x, const glm::vec3 & y) : p1(x), p2(y) {}
		glm::vec3 p1;
		glm::vec3 p2;
	};

	bool intersects(const glm::vec3 & point, const glm::vec3 & dir, const Segment & segment, glm::vec3 & intersection, bool & lies);
	glm::vec3 reflect(const glm::vec3 & ray, const glm::vec3 & surf);

	float length(const glm::vec3 & v);
}