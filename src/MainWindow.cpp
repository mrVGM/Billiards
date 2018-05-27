#include "MainWindow.h"

App app;

BOOL App::InitInstance()
{
	mainWindow = new MainWindow();
	m_pMainWnd = mainWindow;
	mainWindow->Create(NULL, "Billiards");
	mainWindow->ShowWindow(SW_SHOW);
	return TRUE;
}