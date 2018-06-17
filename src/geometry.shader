#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 15) out;

uniform vec3 CAM;
uniform mat3 TR;
uniform vec2 AXES;
uniform vec3 BALL[];

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

void table(vec3 p0, vec3 p1, vec3 p2, vec3 n0, vec3 n1, vec3 n2)
{
	vec3 color = vec3(0, 1, 0.3);

	gl_Position = project(p0);
	finalColor = brightness(p0, n0) * color;
	EmitVertex();

	gl_Position = project(p1);
	finalColor = brightness(p1, n1) * color;
	EmitVertex();

	gl_Position = project(p2);
	finalColor = brightness(p2, n2) * color;
	EmitVertex();

	EndPrimitive();
}

void ball(vec3 p0, vec3 p1, vec3 p2, vec3 n0, vec3 n1, vec3 n2, vec3 offset)
{
	vec3 color = vec3(1, 0, 0);

	p0 += vec3(0, 0, 5 + 5.25 / 2.0);
	p1 += vec3(0, 0, 5 + 5.25 / 2.0);
	p2 += vec3(0, 0, 5 + 5.25 / 2.0);

	p0 += offset;
	p1 += offset;
	p2 += offset;

	gl_Position = project(p0);
	finalColor = brightness(p0, n0) * color;
	EmitVertex();

	gl_Position = project(p1);
	finalColor = brightness(p1, n1) * color;
	EmitVertex();

	gl_Position = project(p2);
	finalColor = brightness(p2, n2) * color;
	EmitVertex();

	EndPrimitive();
}

void main()
{
	vec3 p0 = gl_in[0].gl_Position.xyz;
	vec3 p1 = gl_in[1].gl_Position.xyz;
	vec3 p2 = gl_in[2].gl_Position.xyz;

	if (model[0] == 0)
	{
		table(p0, p1, p2, n[0], n[1], n[2]);
	}
	else if (model[0] == 1)
	{
		ball(p0, p1, p2, n[0], n[1], n[2], BALL[0]);
		ball(p0, p1, p2, n[0], n[1], n[2], BALL[1]);
		ball(p0, p1, p2, n[0], n[1], n[2], BALL[2]);
		ball(p0, p1, p2, n[0], n[1], n[2], BALL[3]);
		ball(p0, p1, p2, n[0], n[1], n[2], BALL[4]);
	}

}
