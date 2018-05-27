#pragma once

#include <afxwin.h>
#include "CameraControlsBar.h"

class MainWindow : public CFrameWnd
{
private:
	CameraControlsBar * cameraControlsBar;
public:
	MainWindow();
	void initControls();
	void setUpOpenGLContext();
	~MainWindow();
};

class App : public CWinApp
{
private:
	MainWindow * mainWindow;
public:
	BOOL InitInstance() override;
};
