#pragma once

#include <afxwin.h>
#include <string>

class Renderer
{
private:
	std::string vertexShader;
	std::string fragmentShader;

	void readFile(const char * fileName, std::string & dest);
	int compileShader(int type, std::string & error);

public:
	Renderer();
	void setupGPUProgram();
	void setUpGPUData();
	void render(HDC hdc);
};