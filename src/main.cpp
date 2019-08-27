#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <GL/glew.h>
#include <SDL.h>

int main(int argc, char* argv[])
{
	std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("console");
	logger->set_level(spdlog::level::trace);
	spdlog::set_default_logger(logger);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		logger->error("Failed to init SDL!");
		return EXIT_FAILURE;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Window* window = SDL_CreateWindow("OGL_SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_OPENGL);
	if (!window)
	{
		logger->error("Failed to create window!");
		return EXIT_FAILURE;
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);
	

	GLenum result = glewInit();
	if(result != GLEW_OK)
	{
		logger->error("Failed to init glew! Error:{}", glewGetErrorString(result));
		return EXIT_FAILURE;
	}
	glewExperimental = GL_TRUE;

	glViewport(0, 0, 512, 512);
	

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
		}
		glClearColor(0.4f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		

		SDL_GL_SwapWindow(window);
	}

	// Clean up
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return EXIT_SUCCESS;
}