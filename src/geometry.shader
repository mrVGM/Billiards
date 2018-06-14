#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform vec3 CAM;
uniform mat3 TR;
uniform vec2 AXES;
uniform vec3 BALL;

in float model[];
in vec3 n[];

out vec3 finalColor;

vec4 project(vec3 pos)
{
	pos = pos - CAM;
	pos = TR * pos;
	float depth = (pos.y - 1) / 1000.0;
	pos /= pos.y;

	return vec4(pos.x / AXES.x, pos.z / AXES.y, depth, 1.0);
}

float brightness(vec3 pos, vec3 normal)
{
	return max(dot(normalize(CAM - pos), normal), 0.1);
}

void main()
{
	vec3 color = vec3(0.1, 1, 0);

	vec3 p0 = gl_in[0].gl_Position.xyz;
	vec3 p1 = gl_in[1].gl_Position.xyz;
	vec3 p2 = gl_in[2].gl_Position.xyz;

	if (model[0] == 0)
	{
		color = vec3(0, 1, 0.3);
	}
	else if (model[0] == 1)
	{
		color = vec3(1, 0, 0);

		p0 += vec3(0, 0, 5 + 5.25 / 2.0);
		p1 += vec3(0, 0, 5 + 5.25 / 2.0);
		p2 += vec3(0, 0, 5 + 5.25 / 2.0);
		
		p0 += BALL;
		p1 += BALL;
		p2 += BALL;
	}

	gl_Position = project(p0);
	finalColor = brightness(p0, n[0]) * color;
	EmitVertex();

	gl_Position = project(p1);
	finalColor = brightness(p1, n[1]) * color;
	EmitVertex();

	gl_Position = project(p2);
	finalColor = brightness(p2, n[2]) * color;
	EmitVertex();

	EndPrimitive();
}