#version 330 core

out vec4 color;

varying float lightCoef;

void main()
{
	color = vec4(0.0, lightCoef, lightCoef, 1.0);
}