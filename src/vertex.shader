#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform vec3 CAM;
uniform mat3 TR;
uniform vec2 AXES;

varying float lightCoef;

void main()
{
	vec3 pos = position;
	pos = pos - CAM;

	lightCoef = max(0, dot(-normalize(pos), normal));

	pos = TR * pos;

	float depth = (pos.y - 1) / 1000.0;
	pos /= pos.y;

	gl_Position = vec4(pos.x / AXES.x, pos.z / AXES.y, depth, 1.0);
}