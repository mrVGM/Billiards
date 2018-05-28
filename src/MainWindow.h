#pragma once

#include <afxwin.h>
#include <thread>

#include "CameraControlsBar.h"
#include "Renderer.h"
#include "RenderWindow.h"

class MainWindow : public CFrameWnd
{
private:
	CameraControlsBar cameraControlsBar;
	Renderer renderer;
	bool rendering;
	std::thread * renderThread;
public:
	RenderWindow renderWindow;
	void initControls();
};

class App : public CWinApp
{
public:
	MainWindow * mainWindow;
	BOOL InitInstance() override;
	static App & getApp();
};
