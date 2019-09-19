#include "core/common_include.h"

#include <fstream>
#include <sstream>

#include "systems/log.h"

#include <SDL.h>
#include <GL/glew.h>

std::array<std::array<GLfloat, 3>, 3> vertices{
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0f
};

std::array<std::array<GLfloat, 4>, 3> colors{
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
};

int main(int argc, char *argv[])
{
	blimp::Log::instance()->init();

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		BLIMP_ERROR("Failed to initialize SDL, Error:{}", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Window *const window{SDL_CreateWindow("Main Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_OPENGL)};
	if (!window)
	{
		BLIMP_ERROR("Failed to create SDL window! Error:{}", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_GLContext context{SDL_GL_CreateContext(window)};
	if (context == NULL)
	{
		BLIMP_ERROR("Failed to create GL context! Error:{}", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetSwapInterval(1);

	glewExperimental = GL_TRUE;
	const GLenum err{glewInit()};
	if (err != GLEW_OK)
	{
		BLIMP_ERROR("Failed to initialize glew! Error:{}", glewGetErrorString(err));
		return EXIT_FAILURE;
	}

	std::array<GLuint, 2> vbo;
	std::array<GLuint, 1> vao;
	const GLuint POS_BUFFER_INDEX = 0;
	const GLuint COLOR_BUFFER_INDEX = 1;

	// Buffers
	glGenBuffers(2, vbo.data());
	glGenVertexArrays(1, vao.data());

	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(POS_BUFFER_INDEX, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(POS_BUFFER_INDEX);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(COLOR_BUFFER_INDEX, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(COLOR_BUFFER_INDEX);
	
	// Shaders
	GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	
	std::ifstream t_vert("src/shaders/vert.glsl");
	std::stringstream buffer_vert;
	buffer_vert << t_vert.rdbuf();
	std::string vert_shader_code = buffer_vert.str();

	std::ifstream t_frag("src/shaders/frag.glsl");
	std::stringstream buffer_frag;
	buffer_frag << t_frag.rdbuf();
	std::string frag_shader_code = buffer_frag.str();
	
	std::array<const char*, 1> vert_shader_code_array{vert_shader_code.c_str()};
	std::array<GLint, 1> vert_shader_code_size{(GLint)vert_shader_code.length()};

	std::array<const char*, 1> frag_shader_code_array{frag_shader_code.c_str()};
	std::array<GLint, 1> frag_shader_code_size{(GLint)frag_shader_code.length()};

	glShaderSource(vert_shader, 1, vert_shader_code_array.data(), vert_shader_code_size.data());
	glCompileShader(vert_shader);

	glShaderSource(frag_shader, 1, frag_shader_code_array.data(), frag_shader_code_size.data());
	glCompileShader(frag_shader);

	GLuint program = glCreateProgram();
	
	glAttachShader(program, vert_shader);
	glAttachShader(program, frag_shader);

	glBindAttribLocation(program, 0, "in_Pos");
	glBindAttribLocation(program, 1, "in_Color");

	glLinkProgram(program);

	glUseProgram(program);

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
				switch(event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					loop = false;
					break;
				}
			}
		}

		glClearColor(0.4f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glDrawArrays(GL_TRIANGLES, 0, 3);

		SDL_GL_SwapWindow(window);
	}

	glUseProgram(0);
	glDetachShader(program, vert_shader);
	glDetachShader(program, frag_shader);

	glDeleteProgram(program);

	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}