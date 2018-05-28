#pragma once

#include <afxwin.h>

#include "CameraControlsBar.h"
#include "RenderWindow.h"

class MainWindow : public CFrameWnd
{
private:
	CameraControlsBar cameraControlsBar;
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
