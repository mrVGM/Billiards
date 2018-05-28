#version 330 core

layout(location = 0) in vec2 position;

uniform vec3 CAM;
uniform mat3 TR;
uniform vec2 AXES;

void main()
{
	vec3 pos = vec3(position.x, position.y, 0);
	pos = pos - CAM;
	pos = TR * pos;
	float depth = (pos.y - 1) / 1000.0;
	pos /= pos.y;

	gl_Position = vec4(pos.x / AXES.x, pos.z / AXES.y, depth, 1.0);
}