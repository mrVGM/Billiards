#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 col[];
out vec3 finalColor;

void main()
{
	gl_Position = gl_in[0].gl_Position;
	finalColor = col[0];
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	finalColor = col[1];
	EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	finalColor = col[2];
	EmitVertex();

	EndPrimitive();
}