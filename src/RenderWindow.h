#pragma once

#include <afxwin.h>
#include <thread>
#include "Renderer.h"

class RenderWindow : public CWnd
{
	Renderer renderer;
	bool rendering;
	std::thread * renderThread;
	friend void worker();
	void render();
public:
	HDC hdc;
	RenderWindow();
	void startRendering();
	void stopRendering();
	~RenderWindow();
};