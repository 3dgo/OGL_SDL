#include "core/common_include.h"

#include "systems/log.h"

#include <SDL.h>
#include <GL/glew.h>

int main(int argc, char *argv[])
{
	blimp::Log::instance()->init();

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		BLIMP_ERROR("Failed to initialize SDL, Error:{}", SDL_GetError());
	}

	SDL_Window *const window{SDL_CreateWindow("Main Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_OPENGL)};
	if (!window)
	{
		BLIMP_ERROR("Failed to create SDL window! Error:{}", SDL_GetError());
	}

	SDL_GLContext context{SDL_GL_CreateContext(window)};
	if (context == NULL)
	{
		BLIMP_ERROR("Failed to create GL context! Error:{}", SDL_GetError());
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetSwapInterval(1);

	glewExperimental = GL_TRUE;
	const GLenum err{glewInit()};
	if (err != GLEW_OK)
	{
		BLIMP_ERROR("Failed to initialize glew! Error:{}", glewGetErrorString(err));
	}

	bool loop = true;

	while (loop)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				loop = false;
			}

			if (event.type == SDL_KEYDOWN)
			{
			}
		}

		glClearColor(0.4, 0.2, 0.4, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}