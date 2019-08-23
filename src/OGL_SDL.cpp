#include <SDL.h>
#include <GL/glew.h>
#include <iostream>

int main(int argc, char* args[])
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		return 0;
	}

	SDL_Window* window = SDL_CreateWindow(
		"My Game",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		512,
		512,
		SDL_WINDOW_OPENGL
	);

	SDL_GL_SetAttribute
	(
		SDL_GL_CONTEXT_PROFILE_MASK,
		SDL_GL_CONTEXT_PROFILE_CORE
	);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	glewInit();

}