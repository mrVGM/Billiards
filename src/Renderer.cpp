#include "Renderer.h"
#include "Config.h"
#include "MainWindow.h"

#include <fstream>
#include <thread>

#include <afxwin.h>

#include <GL\glew.h>

Renderer::Renderer()
{
	std::string inputFilesDir(INPUT_FILES_DIR);
	std::string vertexShaderPath = inputFilesDir + "vertex.shader";
	std::string fragmentShaderPath = inputFilesDir + "fragment.shader";

	readFile(vertexShaderPath.c_str(), vertexShader);
	readFile(fragmentShaderPath.c_str(), fragmentShader);
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