#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in float modelId;

uniform vec3 CAM;
uniform mat3 TR;
uniform vec2 AXES;
uniform vec3 BALL;

varying vec3 col;

void main()
{
	vec3 pos = position;

	if (modelId == 1)
	{
		pos += BALL;
		pos += vec3(0, 0, 5 + 5.25 / 2.0);
	}
	pos = pos - CAM;

	float lightCoef = max(0, dot(-normalize(pos), normal));

	if (modelId == 0)
	{
		col = vec3(0, 1, 0.3) * lightCoef;
	}
	else if (modelId == 1)
	{
		col = vec3(1, 0, 0) * lightCoef;
	}

	pos = TR * pos;

	float depth = (pos.y - 1) / 1000.0;
	pos /= pos.y;

	gl_Position = vec4(pos.x / AXES.x, pos.z / AXES.y, depth, 1.0);
}