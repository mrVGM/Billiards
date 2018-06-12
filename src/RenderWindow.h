#pragma once

#include <afxwin.h>
#include <thread>
#include "Renderer.h"

class RenderWindow : public CWnd
{
	bool rendering;
	std::thread * renderThread;
	friend void worker();
	void render();
public:
	static const int waitingTime;
	HDC hdc;
	Renderer renderer;
	RenderWindow();
	void startRendering();
	void stopRendering();
	~RenderWindow();
};