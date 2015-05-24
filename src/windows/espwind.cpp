#include <Windows.h>
#include <string>
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_syswm.h"
#include <dwmapi.h>


HWND mainwind, gamewind;

SDL_GLContext context;
SDL_Window *wind;
HGLRC rendercontext;
HDC renderdc;

static void setup_opengl(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, h, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void StartPaint(void)
{
	wglMakeCurrent(renderdc, rendercontext);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void EndPaint(void)
{
	glFlush();
	SDL_GL_SwapWindow(wind);
	wglMakeCurrent(NULL, NULL);
}

void CreateESPWind(void)
{
	gamewind = GetActiveWindow();
	RECT rekt = { 0, 0, 100, 100 };
	GetWindowRect(gamewind, &rekt);
	/*
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);


	wind = SDL_CreateWindow("", rekt.left, rekt.top, rekt.right - rekt.left, rekt.bottom - rekt.top, SDL_WINDOW_BORDERLESS);
	
	context = SDL_GL_CreateContext(wind);

	setup_opengl(rekt.right - rekt.left, rekt.bottom - rekt.top);


	SDL_SysWMinfo wmInfo;

	SDL_VERSION(&wmInfo.version);

	SDL_GetWindowWMInfo(wind, &wmInfo);
	mainwind = wmInfo.info.win.window;

	SetWindowLongA(mainwind, GWL_EXSTYLE, GetWindowLongA(mainwind, GWL_EXSTYLE) | WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT);
	SetWindowLongA(mainwind, GWL_STYLE, GetWindowLongA(mainwind, GWL_STYLE) | WS_POPUP);

	DWM_BLURBEHIND bb = { DWM_BB_ENABLE | DWM_BB_BLURREGION, true, CreateRectRgn(0, 0, -1, -1), true };
	DwmEnableBlurBehindWindow(mainwind, &bb);

	SetLayeredWindowAttributes(mainwind, RGB(0, 0, 0), 255, LWA_COLORKEY | LWA_ALPHA);

	SetParent(mainwind, gamewind);

	*/
}