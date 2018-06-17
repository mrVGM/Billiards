#include "stdafx.h"

#include "RenderWindow.h"
#include "GL/glew.h"
#include "Physics.h"
#include <gtc/constants.hpp>

const int RenderWindow::waitingTime = 25;

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

	Physics::getEngine().balls.push_back(Ball());
	Physics::getEngine().balls.push_back(Ball());
	Physics::getEngine().balls.push_back(Ball());

	Physics::getEngine().balls[0].position = glm::vec3(0, 0, 0);
	Physics::getEngine().balls[1].position = glm::vec3(0, 30, 0);

	float angle = glm::pi<float>() / 6.0f;
	glm::vec3 p1(sin(angle), cos(angle), 0);

	Physics::getEngine().balls[2].position = Physics::getEngine().balls[1].position + Ball::diameter * p1;

	Physics::getEngine().time = 0.0;

	App::getApp().mainWindow->cameraControlsBar.updateCamPosition();

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

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	renderer.setupGPUProgram();
	renderer.setUpGPUData();

	while (rendering)
	{
		renderer.render(hdc);
		Sleep(RenderWindow::waitingTime);
	}
}