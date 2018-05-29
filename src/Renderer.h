#pragma once

#include <afxwin.h>
#include <string>

#include <glm.hpp>

#include <vector>

struct Vertex
{
	float position[3];
	float normal[3];
	float id;
};

struct Model
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indeces;
	void read(const char * fileName);
};

class Renderer
{
private:
	std::string vertexShader;
	std::string fragmentShader;

	void readFile(const char * fileName, std::string & dest);
	int compileShader(int type, std::string & error);

	float xScale;
	float yScale;

	bool changed;
	glm::vec3 camPosition;
	float transform[9];

	int glProgram;

	void setUniforms();

	int renderElements;
	
public:
	Renderer();
	void setupGPUProgram();
	void setUpGPUData();
	void render(HDC hdc);
	void setDimensions(int width, int height);
	void setCamPosition(const glm::vec3 & position);
};