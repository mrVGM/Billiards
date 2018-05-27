#pragma once

#include <afxwin.h>

class App : public CWinApp
{
private:
	CFrameWnd * mainWindow;
public:
	BOOL InitInstance() override;
};

class MainWindow : public CFrameWnd
{
};
