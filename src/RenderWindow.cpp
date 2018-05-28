#include "RenderWindow.h"
#include "MainWindow.h"
#include "GL/glew.h"

RenderWindow::RenderWindow() : renderThread(NULL)
{
}

static void worker()
{
	App::getApp().mainWindow->renderWindow.render();
}

void RenderWindow::startRendering()
{
	if (renderThread)
		return;

	rendering = true;
	renderThread = new std::thread(worker);
}

void RenderWindow::stopRendering()
{
	if (!renderThread)
		return;
	rendering = false;
	renderThread->join();
	delete renderThread;
	renderThread = NULL;
}

RenderWindow::~RenderWindow()
{
	stopRendering();
}

void RenderWindow::render()
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

	hdc = (HDC)*dc;

	int pf = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, pf, &pfd);

	HGLRC context = wglCreateContext(HDC(*dc));
	wglMakeCurrent(hdc, context);

	GLenum res = glewInit();

	renderer.setupGPUProgram();
	renderer.setUpGPUData();

	while (rendering)
	{
		renderer.render(hdc);
		Sleep(25);
	}
}