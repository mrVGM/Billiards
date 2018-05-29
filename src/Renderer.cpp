#include "stdafx.h"

#include "Renderer.h"

#include <fstream>
#include <thread>

#include <GL\glew.h>

#include <gtc\constants.hpp>

Renderer::Renderer()
{
	std::string inputFilesDir(INPUT_FILES_DIR);
	std::string vertexShaderPath = inputFilesDir + "vertex.shader";
	std::string fragmentShaderPath = inputFilesDir + "fragment.shader";

	readFile(vertexShaderPath.c_str(), vertexShader);
	readFile(fragmentShaderPath.c_str(), fragmentShader);
	
	setCamPosition(glm::vec3(-10, -10, 10));
}

void Renderer::readFile(const char * fileName, std::string & dest)
{
	std::ifstream file(fileName);
	
	std::string tmp;
	while (std::getline(file, tmp))
		dest += tmp + '\n';
}

void Renderer::setupGPUProgram()
{
	int program = glCreateProgram();

	std::string error;
	int vs = compileShader(GL_VERTEX_SHADER, error);
	int fs = compileShader(GL_FRAGMENT_SHADER, error);
	
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	glUseProgram(program);
	glProgram = program;
}

void Renderer::setUpGPUData()
{
	float pts[] =
	{
		-0.5, -0.5,
		+0.5, -0.5,
		+0.0, +0.5
	};

	unsigned int indeces[] =
	{
		0,1,2
	};

	unsigned int bufferId[2];
	glGenBuffers(2, bufferId);

	glBindBuffer(GL_ARRAY_BUFFER, bufferId[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(float), pts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int), indeces, GL_STATIC_DRAW);
}

void Renderer::render(HDC hdc)
{
	glClear(GL_COLOR_BUFFER_BIT);
	setUniforms();
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	BOOL res = SwapBuffers(hdc);
}

int Renderer::compileShader(int type, std::string & error)
{
	error = "No error";
	int shader = glCreateShader(type);

	const char * tmp;
	if (type == GL_VERTEX_SHADER)
	{
		tmp = vertexShader.c_str();
		glShaderSource(shader, 1, &tmp, 0);
	}
	else if (type == GL_FRAGMENT_SHADER)
	{
		tmp = fragmentShader.c_str();
		glShaderSource(shader, 1, &tmp, 0);
	}
	glCompileShader(shader);

	int result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (result != GL_FALSE)
		return shader;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);
	char * message = new char[result];
	glGetShaderInfoLog(shader, result, &result, message);
	error = message;

	delete[] message;

	if (error == "")
	{
		error = "Unknown error";
	}
}

void Renderer::setDimensions(int width, int height)
{
	xScale = tan(glm::pi<float>() / 4.0f);
	yScale = height * xScale / width;
}

void Renderer::setCamPosition(const glm::vec3 & position)
{
	camPosition = position;

	glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0, 0.0, 1.0), position));
	glm::vec3 dir = glm::normalize(-position);
	glm::vec3 up = glm::cross(right, dir);

	glm::mat3 tr;

	tr[0][0] = right.x;
	tr[1][0] = right.y;
	tr[2][0] = right.z;

	tr[0][1] = dir.x;
	tr[1][1] = dir.y;
	tr[2][1] = dir.z;

	tr[0][2] = up.x;
	tr[1][2] = up.y;
	tr[2][2] = up.z;

	tr = glm::inverse(tr);

	transform[0] = tr[0][0];
	transform[1] = tr[0][1];
	transform[2] = tr[0][2];

	transform[3] = tr[1][0];
	transform[4] = tr[1][1];
	transform[5] = tr[1][2];

	transform[6] = tr[2][0];
	transform[7] = tr[2][1];
	transform[8] = tr[2][2];

	changed = true;
}

void Renderer::setUniforms()
{
	if (!changed)
		return;

	int CAM = glGetUniformLocation(glProgram, "CAM");
	int TR = glGetUniformLocation(glProgram, "TR");
	int AXES = glGetUniformLocation(glProgram, "AXES");

	glUniform3f(CAM, camPosition.x, camPosition.y, camPosition.z);
	glUniformMatrix3fv(TR, 1, GL_TRUE, transform);
	glUniform2f(AXES, xScale, yScale);

	changed = false;
}