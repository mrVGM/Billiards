#include "stdafx.h"

#include "Renderer.h"

#include <fstream>
#include <thread>
#include <sstream>

#include <GL\glew.h>

#include <gtc\constants.hpp>

#include "Physics.h"

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

	file.close();
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
	Model table(0);
	std::string tableOBJ(INPUT_FILES_DIR);
	tableOBJ += "table.obj";

	table.read(tableOBJ.c_str());

	Model ball(1);
	std::string ballOBJ(INPUT_FILES_DIR);
	ballOBJ += "ball.obj";

	ball.read(ballOBJ.c_str());

	table.add(ball);

	unsigned int bufferId[2];
	glGenBuffers(2, bufferId);

	glBindBuffer(GL_ARRAY_BUFFER, bufferId[0]);
	glBufferData(GL_ARRAY_BUFFER, table.vertices.size() * sizeof(Vertex), &table.vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)(6 * sizeof(float)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, table.indeces.size() * sizeof(unsigned int), &table.indeces[0], GL_STATIC_DRAW);

	renderElements = table.indeces.size();
}

void Renderer::render(HDC hdc)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	Physics::getEngine().updateState();
	setUniforms();

	glDrawElements(GL_TRIANGLES, renderElements, GL_UNSIGNED_INT, 0);
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
	int BALL = glGetUniformLocation(glProgram, "BALL");

	glUniform3f(BALL, Physics::getEngine().balls[0].position.x, Physics::getEngine().balls[0].position.y, Physics::getEngine().balls[0].position.z);

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

void Model::read(const char * fileName)
{
	using namespace std;
	ifstream file(fileName);

	int v = 0, n = 0;

	string line;
	stringstream ss;
	while (getline(file, line))
	{
		ss.str(line);
		ss.clear();
		
		string type;
		ss >> type;

		if (type == "v")
		{
			if (v == vertices.size())
				vertices.push_back(Vertex(modelId));

			ss >> vertices[v].position[0] >> vertices[v].position[1] >> vertices[v].position[2];
			++v;
		}
		else if (type == "vn")
		{
			if (n == vertices.size())
				vertices.push_back(Vertex(modelId));

			ss >> vertices[n].normal[0] >> vertices[n].normal[1] >> vertices[n].normal[2];
			++n;
		}
		else if (type == "f")
		{
			string ind1, ind2, ind3, ind4;
			unsigned int i1, i2, i3, i4;
			ss >> ind1 >> ind2 >> ind3 >> ind4;

			ss.str(ind1);
			ss.clear();

			ss >> i1;
			indeces.push_back(i1-1);

			ss.str(ind2);
			ss.clear();

			ss >> i2;
			indeces.push_back(i2-1);

			ss.str(ind3);
			ss.clear();

			ss >> i3;
			indeces.push_back(i3-1);

			if (ind4 != "")
			{
				ss.str(ind4);
				ss.clear();

				ss >> i4;
				indeces.push_back(i1 - 1);
				indeces.push_back(i3 - 1);
				indeces.push_back(i4 - 1);
			}
		}
	}

	file.close();
}

void Model::add(const Model & other)
{
	const int n = vertices.size();
	vertices.insert(vertices.end(), other.vertices.begin(), other.vertices.end());
	for (int i = 0; i < other.indeces.size(); ++i)
		indeces.push_back(n + other.indeces[i]);
}