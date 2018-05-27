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

	mainWindow->setUpOpenGLContext();

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

void MainWindow::setUpOpenGLContext()
{
	CDC * dc = GetDC();

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	CRect rect;
	GetWindowRect(&rect);

	HDC hdc = (HDC)*dc;

	int pf = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, pf, &pfd);

	HGLRC context = wglCreateContext(HDC(*dc));
	wglMakeCurrent(hdc, context);

	GLenum res = glewInit();
}