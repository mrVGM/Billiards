#pragma once

#include <afxwin.h>
#include <thread>

#include "CameraControlsBar.h"
#include "Renderer.h"

class MainWindow : public CFrameWnd
{
private:
	CameraControlsBar cameraControlsBar;
	Renderer renderer;
	bool rendering;
	std::thread * renderThread;
	friend void worker();
	void render();
public:
	HDC hdc;
	MainWindow();
	void initControls();
	void startRendering();
	void stopRendering();
	~MainWindow();
};

class App : public CWinApp
{
public:
	MainWindow * mainWindow;
	BOOL InitInstance() override;
};
