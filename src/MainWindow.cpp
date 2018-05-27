#include "MainWindow.h"
#include "CameraControlsBar.h"

#include "resource.h"

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

MainWindow::MainWindow()
{
	cameraControlsBar = new CameraControlsBar();
}

MainWindow::~MainWindow()
{
	delete cameraControlsBar;
}

void MainWindow::initControls()
{
	cameraControlsBar->Create(IDD_DIALOGBAR, this);
	cameraControlsBar->ShowWindow(SW_SHOW);
}