#include <SDL_image.h>
#include <stdio.h>
#include "Headers/openGL.h"
#include "glad/glad.h"
#include "../General/Headers/structs.h"
#include "Headers/textures.h"

#define OpenGLCall(x) GLClearAllErrors(); x; GLCheckErrorStatus(__LINE__);


void GLAPIENTRY OpenGLDebugCallback(GLenum source, GLenum type, GLuint id,
                                    GLenum severity, GLsizei length,
                                    const GLchar *message, const void *userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
		return;

	SDL_Log("---------------");
	SDL_Log("Debug message (%d): %s", id, message);

	switch (source)
	{
		case GL_DEBUG_SOURCE_API:
			SDL_Log("Source: API");
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			SDL_Log("Source: Window System");
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			SDL_Log("Source: Shader Compiler");
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			SDL_Log("Source: Third Party");
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			SDL_Log("Source: Application");
			break;
		case GL_DEBUG_SOURCE_OTHER:
			SDL_Log("Source: Other");
			break;
	}

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:
			SDL_Log("Type: Error");
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			SDL_Log("Type: Deprecated Behaviour");
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			SDL_Log("Type: Undefined Behaviour");
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			SDL_Log("Type: Portability");
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			SDL_Log("Type: Performance");
			break;
		case GL_DEBUG_TYPE_MARKER:
			SDL_Log("Type: Marker");
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			SDL_Log("Type: Push Group");
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			SDL_Log("Type: Pop Group");
			break;
		case GL_DEBUG_TYPE_OTHER:
			SDL_Log("Type: Other");
			break;
	}

	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:
			SDL_Log("Severity: high");
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			SDL_Log("Severity: medium");
			break;
		case GL_DEBUG_SEVERITY_LOW:
			SDL_Log("Severity: low");
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			SDL_Log("Severity: notification");
			break;
	}
	SDL_Log("---------------");
}


void GLClearAllErrors()
{
	while (glGetError() != GL_NO_ERROR) {}
}


bool GLCheckErrorStatus(const int line)
{
	while (glGetError() != GL_NO_ERROR)
	{
		SDL_Log("OpenGL error %d on line %d", glGetError(), line);
		return true;
	}

	return false;
}


void InitOpenGL(App *app)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	app->hiddenWindow = SDL_CreateWindow(
		"Init",
		0, 0, 1, 1,
		SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN
	);

	app->glContext = SDL_GL_CreateContext(app->hiddenWindow);

	InitRenderPipeline(app);
}


char *LoadShaderSource(const char *shaderPath)
{
	char filePathBuffer[150];
	snprintf(filePathBuffer, sizeof(filePathBuffer), "%s%s", "Shaders/", shaderPath);

	SDL_Log("Loading shader: %s", filePathBuffer);

	FILE *file = fopen(filePathBuffer, "rb");
	if (!file)
	{
		SDL_Log("Failed to open shader file: %s", shaderPath);
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *buffer = malloc(fileSize + 1);
	if (!buffer)
	{
		SDL_Log("Failed to allocate memory for shader: %s", shaderPath);
		fclose(file);
		return NULL;
	}

	size_t bytesRead = fread(buffer, 1, fileSize, file);
	buffer[bytesRead] = '\0';

	fclose(file);
	return buffer;
}


void CalculateProjectionMatrix(float projection[16], int windowWidth, int windowHeight)
{
	float left = 0.0f;
	float right = (float) windowWidth;
	float bottom = (float) windowHeight;
	float top = 0.0f;
	float near = -1.0f;
	float far = 1.0f;

	projection[0] = 2.0f / (right - left);
	projection[1] = 0.0f;
	projection[2] = 0.0f;
	projection[3] = 0.0f;

	projection[4] = 0.0f;
	projection[5] = 2.0f / (top - bottom);
	projection[6] = 0.0f;
	projection[7] = 0.0f;

	projection[8] = 0.0f;
	projection[9] = 0.0f;
	projection[10] = -2.0f / (far - near);
	projection[11] = 0.0f;

	projection[12] = -(right + left) / (right - left);
	projection[13] = -(top + bottom) / (top - bottom);
	projection[14] = -(far + near) / (far - near);
	projection[15] = 1.0f;
}


// void SetProjectionMatrix(Renderer *renderer, int windowWidth, int windowHeight)
// {
// 	float left = 0.0f;
// 	float right = (float) windowWidth;
// 	float bottom = (float) windowHeight;
// 	float top = 0.0f;
// 	float near = -1.0f;
// 	float far = 1.0f;
//
// 	float projection[16] = {
// 		2.0f / (right - left), 0.0f, 0.0f, 0.0f,
// 		0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
// 		0.0f, 0.0f, -2.0f / (far - near), 0.0f,
// 		-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1.0f
// 	};
//
// 	glUseProgram(renderer->shaderProgram);
// 	glUniformMatrix4fv(renderer->projectionLocation, 1, GL_FALSE, projection);
// }


void InitRenderPipeline(App *app)
{
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		SDL_Log("Failed to initialize GLAD");
	}

	PrintOpenGLInfo();


	//Debug

	// glEnable(GL_DEBUG_OUTPUT);
	// glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	// glDebugMessageCallback(OpenGLDebugCallback, NULL);
	// glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

	//

	app->renderer = CreateRenderer();
}


GLuint CreateShaderProgram(char *vertexShaderName, char *fragmentShaderName)
{
	const char *vertexShaderSource = NULL;
	const char *fragmentShaderSource = NULL;

	if (vertexShaderName == NULL)
	{
		vertexShaderSource = LoadShaderSource("default.vert");
	}
	else
	{
		vertexShaderSource = LoadShaderSource(vertexShaderName);
	}

	if (fragmentShaderName == NULL)
	{
		fragmentShaderSource = LoadShaderSource("default.frag");
	}
	else
	{
		fragmentShaderSource = LoadShaderSource(fragmentShaderName);
	}


	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		SDL_Log("Vertex shader compilation failed: %s", infoLog);
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		SDL_Log("Fragment shader compilation failed: %s", infoLog);
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		SDL_Log("Shader program linking failed: %s", infoLog);
	}

	glValidateProgram(shaderProgram);
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	free((void *) vertexShaderSource);
	free((void *) fragmentShaderSource);

	return shaderProgram;
}


Renderer *CreateRenderer()
{
	Renderer *renderer = calloc(1, sizeof(Renderer));
	renderer->defaultMaterial = CreateMaterial(NULL, NULL);
	renderer->defaultGizmoMaterial = CreateMaterial("Gizmos/gizmos.frag", "Gizmos/gizmos.vert");

	glGenVertexArrays(1, &renderer->entitiesVAO);
	glGenVertexArrays(1, &renderer->gizmosVAO);
	glGenBuffers(1, &renderer->VBO);
	glGenBuffers(1, &renderer->EBO);

	glBindVertexArray(renderer->entitiesVAO);

	glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	unsigned int indices[] = {0, 1, 2, 2, 3, 0};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));

	glBindVertexArray(renderer->gizmosVAO);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->EBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return renderer;
}


void PrintOpenGLInfo()
{
	printf("Vendor %s \n", glGetString(GL_VENDOR));
	printf("Renderer %s \n", glGetString(GL_RENDERER));
	printf("Version %s \n", glGetString(GL_VERSION));
	printf("GLSL %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}
