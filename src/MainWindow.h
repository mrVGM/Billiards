#pragma once

#include <afxwin.h>

#include "CameraControlsBar.h"
#include "RenderWindow.h"

class MainWindow : public CFrameWnd
{
public:
	CameraControlsBar cameraControlsBar;
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
