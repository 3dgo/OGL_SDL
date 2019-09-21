#include "core/common_include.h"

#include <fstream>
#include <sstream>

#include "systems/log.h"

#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>

std::array<std::array<GLfloat, 9>, 4> vertices{
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f
};

std::array<GLint, 6> indices = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

std::optional<std::string> get_shader_compile_error(GLuint shader)
{
	GLint success;
	
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	
	if(success)
		return {};
	
	GLint log_length;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
	char* shader_log = new char[log_length];
	glGetShaderInfoLog(shader, log_length, &log_length, shader_log);

	std::string shader_log_str(shader_log);
	delete[] shader_log;
	return shader_log_str;
}

std::optional<std::string> get_shader_link_error(GLuint program)
{
	GLint success;

	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if(success)
	{
		return {};
	}
	
	GLint log_length;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
	char* shader_log = new char[log_length];
	glGetProgramInfoLog(program, log_length, &log_length, shader_log);

	std::string shader_log_str(shader_log);
	delete[] shader_log;
	return shader_log_str;
}

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

	std::array<GLuint, 1> vbo;
	std::array<GLuint, 1> ebo;
	std::array<GLuint, 1> vao;
	const GLuint POS_BUFFER_INDEX = 0;
	const GLuint COLOR_BUFFER_INDEX = 1;
	const GLuint TEXCOORD_BUFFER_INDEX = 2;

	// Buffers
	glGenBuffers(1, vbo.data());
	glGenBuffers(1, ebo.data());
	glGenVertexArrays(1, vao.data());

	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(POS_BUFFER_INDEX, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(POS_BUFFER_INDEX);

	glVertexAttribPointer(COLOR_BUFFER_INDEX, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(COLOR_BUFFER_INDEX);

	glVertexAttribPointer(TEXCOORD_BUFFER_INDEX, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(7 * sizeof(GLfloat)));
	glEnableVertexAttribArray(TEXCOORD_BUFFER_INDEX);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Shaders
	GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	
	std::ifstream t_vert;
	t_vert.open("src/shaders/vert.glsl");
	std::stringstream buffer_vert;
	buffer_vert << t_vert.rdbuf();
	std::string vert_shader_code = buffer_vert.str();
	t_vert.close();

	std::ifstream t_frag;
	t_frag.open("src/shaders/frag.glsl");
	std::stringstream buffer_frag;
	buffer_frag << t_frag.rdbuf();
	std::string frag_shader_code = buffer_frag.str();
	t_frag.close();
	
	std::array<const char*, 1> vert_shader_code_array{vert_shader_code.c_str()};
	std::array<GLint, 1> vert_shader_code_size{(GLint)vert_shader_code.length()};

	std::array<const char*, 1> frag_shader_code_array{frag_shader_code.c_str()};
	std::array<GLint, 1> frag_shader_code_size{(GLint)frag_shader_code.length()};

	glShaderSource(vert_shader, 1, vert_shader_code_array.data(), vert_shader_code_size.data());
	glCompileShader(vert_shader);
	auto vert_error = get_shader_compile_error(vert_shader);
	if(vert_error)
	{
		BLIMP_ERROR("Vertex shader failed to compile! Error:{}", vert_error.value());
		return EXIT_FAILURE;
	}

	glShaderSource(frag_shader, 1, frag_shader_code_array.data(), frag_shader_code_size.data());
	glCompileShader(frag_shader);
	auto frag_error = get_shader_compile_error(frag_shader);
	if(frag_error)
	{
		BLIMP_ERROR("Fragment shader failed to compile! Error:{}", frag_error.value());
		return EXIT_FAILURE;
	}

	GLuint program = glCreateProgram();

	glAttachShader(program, vert_shader);
	glAttachShader(program, frag_shader);

	glBindAttribLocation(program, 0, "in_Pos");
	glBindAttribLocation(program, 1, "in_Color");
	glBindAttribLocation(program, 2, "in_TexCoord");

	glLinkProgram(program);
	auto link_error = get_shader_link_error(program);
	if(link_error)
	{
		BLIMP_ERROR("Shader failed to link! Error:{}", link_error.value());
		return EXIT_FAILURE;
	}


	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);	

	glUseProgram(program);

	// Textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_Surface* tex_suf = IMG_Load("resources/Stonewall15_512x512.bmp");

	std::array<GLuint, 1> textures;
	glGenTextures(1, textures.data());
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_suf->w, tex_suf->h, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_suf->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	SDL_FreeSurface(tex_suf);

	//main loop
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

		glUseProgram(program);
		GLfloat time_sec = SDL_GetTicks() / 1000.0;
		GLint tint_color_location = glGetUniformLocation(program, "tintColor");
		if(tint_color_location == -1)
		{
			BLIMP_ERROR("Could not find uniform attribute: {}!", "tintColor");
			return EXIT_FAILURE;
		}
		GLfloat b = sin(time_sec) / 2.0f + 0.5f;
		glUniform4f(tint_color_location, b, b, b, 1.0);

		glBindVertexArray(vao[0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glBindVertexArray(0);

		SDL_GL_SwapWindow(window);
	}

	glUseProgram(0);
	glDetachShader(program, vert_shader);
	glDetachShader(program, frag_shader);

	glDeleteProgram(program);

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}