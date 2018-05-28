#include "MainWindow.h"
#include "CameraControlsBar.h"

#include "resource.h"

#include <GL/glew.h>

App app;

BOOL App::InitInstance()
{
	mainWindow = new MainWindow();
	m_pMainWnd = mainWindow;
	mainWindow->Create(NULL, "Billiards");
	mainWindow->ShowWindow(SW_SHOW);

	mainWindow->initControls();

	return TRUE;
}

void MainWindow::initControls()
{
	cameraControlsBar.Create(IDD_DIALOGBAR, this);
	cameraControlsBar.ShowWindow(SW_SHOW);

	CRect rectControlsBar;
	cameraControlsBar.GetWindowRect(&rectControlsBar);

	CRect rectWindow;
	GetWindowRect(rectWindow);

	renderWindow.Create(0, "", WS_CHILD | WS_VISIBLE, CRect(0,rectControlsBar.Height(),rectWindow.Width(),rectWindow.Height()), this, 0);

	renderWindow.renderer.setDimensions(rectWindow.Width(), rectWindow.Height() - rectControlsBar.Height());

	renderWindow.startRendering();
}

App & App::getApp()
{
	return app;
}