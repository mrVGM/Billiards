#include "stdafx.h"

#include "Utils.h"

typedef glm::vec3 Point;
typedef glm::vec3 Vector;

bool Utils::intersects(const glm::vec3 & point, const glm::vec3 & dir, const Utils::Segment & segment, glm::vec3 & intersection, bool &lies)
{
	Vector v1 = segment.p1 - point;
	Vector v2 = segment.p2 - point;

	if (glm::cross(v1, v2) == Vector(0, 0, 0))
	{
		intersection = point;
		lies = true;
		return true;
	}

	lies = false;

	Vector tmp1 = glm::cross(v1, dir);
	Vector tmp2 = glm::cross(dir, v2);

	if (tmp1.z < 0 || tmp2.z < 0)
		return false;

	Vector x = glm::normalize(dir);
	Vector y = glm::cross(Vector(0, 0, 1), x);
	Vector z(0, 0, 1);

	glm::mat3 tr(x, y, z);
	tr = glm::inverse(tr);

	v1 = tr * v1;
	v2 = tr * v2;

	float c1 = abs(v1.y);
	float c2 = abs(v2.y);

	intersection = segment.p1 * c2 / (c1 + c2) + segment.p2 * c1 / (c1 + c2);

	if (Utils::length(intersection - point) > Utils::length(dir))
		return false;

	return true;
}

glm::vec3 Utils::reflect(const glm::vec3 & ray, const glm::vec3 & surf)
{
	glm::vec3 x = glm::normalize(surf);
	glm::vec3 z(0,0,1);
	glm::vec3 y = glm::cross(z, x);
	
	glm::mat3 tr(x, y, z);
	glm::mat3 trInv = glm::inverse(tr);

	glm::vec3 r = trInv * ray;
	r.y = -r.y;

	return tr * r;
}

float Utils::length(const glm::vec3 & v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}