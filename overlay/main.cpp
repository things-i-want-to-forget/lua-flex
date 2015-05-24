#include <Windows.h>
#include <string>
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_syswm.h"
#include <dwmapi.h>
#include "msockapi.h"
#include "msockbuff.h"

#define err() printf("%i\n", GetLastError())

#undef main

HWND mainwind, gamewind;

SDL_GLContext context;
SDL_Window *wind;
MSockAPI *sock;
MSockBuffer packet;

void display()
{
	if (!packet.Initialize(sock))
		return;
	switch (packet.id)
	{
	case MPACKET_LINE:
	{
		glBegin(GL_LINES);
		{
			unsigned char r, g, b, a;
			packet.read(&r);
			packet.read(&g);
			packet.read(&b);
			packet.read(&a);
			glColor4ub(r, g, b, a);

			int x1, x2, y1, y2;
			packet.read(&x1);
			packet.read(&x2);
			packet.read(&y1);
			packet.read(&y2);

			glVertex2i(x1, y1);
			glVertex2i(x2, y2);

		}
		glEnd();
	}
	break;
	case MPACKET_FILLED_RECT:
	{

		glBegin(GL_QUADS);
		{
			unsigned char r, g, b, a;
			packet.read(&r);
			packet.read(&g);
			packet.read(&b);
			packet.read(&a);
			glColor4ub(r, g, b, a);

			int x1, x2, y1, y2;
			packet.read(&x1);
			packet.read(&x2);
			packet.read(&y1);
			packet.read(&y2);

			glVertex2i(x1, y1);
			glVertex2i(x1, y2);
			glVertex2i(x2, y2);
			glVertex2i(x2, y1);

		}
		glEnd();

	}
	break;
	case MPACKET_OUTLINED_RECT:
	{
		glBegin(GL_LINES);
		{
			unsigned char r, g, b, a;
			packet.read(&r);
			packet.read(&g);
			packet.read(&b);
			packet.read(&a);
			glColor4ub(r, g, b, a);

			int x1, x2, y1, y2;
			packet.read(&x1);
			packet.read(&x2);
			packet.read(&y1);
			packet.read(&y2);

			glVertex2i(x1, y1);
			glVertex2i(x1, y2);

			glVertex2i(x1, y2);
			glVertex2i(x2, y2);

			glVertex2i(x2, y2);
			glVertex2i(x2, y1);

			glVertex2i(x2, y1);
			glVertex2i(x1, y1);

		}
		glEnd();
	}
	break;
	case MPACKET_FLUSH:
	{

		glFlush();
		SDL_GL_SwapWindow(wind);
	}
	break;
	case MPACKET_CLEAR:
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	break;
	}

	packet.Reset();
}

DWORD WINAPI PollEvents(LPVOID)
{
	SDL_Event event;
	while (1)
	{
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
			exit(0);
		printf("%i\n", event.type);
		SDL_Delay(100);
	}
}

static void setup_opengl(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, h, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(char *argv[], int argc)
{
	while (!(gamewind = FindWindowA(NULL, "Counter-Strike: Global Offensive")));


	printf("Found window! \n");

	sock = new MSockAPI();
	sock->Connect("\\\\.\\pipe\\lf_esp");
	printf("Connected: %i\n", sock->connected());
	printf("Got pipe!\n");
	err();

	while (!ConnectNamedPipe(sock->pipe, NULL))
	{
		if (GetLastError() == ERROR_PIPE_CONNECTED)
			break;
		if (!IsWindow(gamewind))
			exit(0);
		SDL_Delay(100);
	}
	printf("Client connected!\n");

	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	RECT rekt;
	GetWindowRect(gamewind, &rekt);

	wind = SDL_CreateWindow("test", rekt.left, rekt.top, rekt.right - rekt.left, rekt.bottom - rekt.top, SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);

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

	err();
	CreateThread(0, 0, &PollEvents, 0, 0, 0);



	while (IsWindow(gamewind))
	{
		display();
		SDL_Delay(1000 / 240);
	}


	return 0;
}