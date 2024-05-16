#include "Window.h"
#include <exception>
#include <GL/glew.h>

GEngine::Window::Window(const WinInfo& info)
	: winInfo{info}, window{nullptr}
{}

GEngine::Window::~Window()
{}

void GEngine::Window::Create()
{
	Uint32 SDLFlags = SDL_WINDOW_OPENGL;

	if (winInfo.flags & WIND_INVISIBLE)
		SDLFlags |= SDL_WINDOW_HIDDEN;
	if (winInfo.flags & WIND_INVISIBLE)
		SDLFlags |= SDL_WINDOW_HIDDEN;
	if (winInfo.flags & WIND_BORDERLESS)
		SDLFlags |= SDL_WINDOW_BORDERLESS;

	window = SDL_CreateWindow(winInfo.name.c_str(), winInfo.width, winInfo.height, SDLFlags);
	if (window == nullptr) throw std::exception{ "ERROR: Could not create window." };

	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr) throw std::exception{ "ERROR: Could not create GLContext." };

	GLenum gl_err = glewInit();
	if (gl_err != GLEW_OK) throw std::exception{ "ERROR: Could not initalize GLEW." };

	glClearColor(winInfo.clear.r / 255.0, winInfo.clear.g / 255.0, winInfo.clear.b / 255.0, winInfo.clear.a / 255.0);

	SDL_GL_SetSwapInterval(0); // VSYNC

	// alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GEngine::Window::SwapBuffer()
{
	SDL_GL_SwapWindow(window);
}
