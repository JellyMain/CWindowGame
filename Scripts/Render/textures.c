#include "Headers/textures.h"
#include <SDL_image.h>
#include <SDL_surface.h>
#include "glad/glad.h"
#include "Headers/draw.h"
#include "Headers/openGL.h"


Texture *CreateRect(Vector2Float size, SDL_Color color)
{
	Texture *texture = calloc(1, sizeof(Texture));

	unsigned char pixel[4] = {color.r, color.g, color.b, color.a};

	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	texture->textureId = textureId;
	texture->height = size.y;
	texture->width = size.x;

	return texture;
}


Texture *CreateTextureFromSurface(SDL_Surface *surface)
{
	Texture *texture = calloc(1, sizeof(Texture));

	SDL_Surface *convertedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);

	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
	             convertedSurface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	texture->textureId = textureId;
	texture->height = convertedSurface->h;
	texture->width = convertedSurface->w;

	SDL_FreeSurface(convertedSurface);

	return texture;
}


Material *CreateMaterial(char *fragShaderName, char *vertShaderName)
{
	Material *material = calloc(1, sizeof(Material));

	material->shaderProgram = CreateShaderProgram(vertShaderName, fragShaderName);
	glUseProgram(material->shaderProgram);
	material->materialUniforms = DictionaryCreate(HashString, StringEquals);
	AddUniformToMaterial(material, "projection", UNIFORM_MAT4F, NULL);
	return material;
}


void AddUniformToMaterial(Material *material, char *uniformName, UniformType uniformType, void *value)
{
	UniformTypeValuePair *uniformTypeValuePair = calloc(1, sizeof(UniformTypeValuePair));
	uniformTypeValuePair->uniformType = uniformType;
	uniformTypeValuePair->uniformValue = value;
	DictionaryAdd(material->materialUniforms, uniformName, uniformTypeValuePair);
}


Texture *LoadTexture(const char *fileName)
{
	Texture *texture = calloc(1, sizeof(Texture));

	char buffer[150];
	snprintf(buffer, sizeof(buffer), "%s%s", "Assets/", fileName);

	SDL_Surface *surface = IMG_Load(buffer);
	if (!surface)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
		               "Failed to load image: %s, Error: %s", buffer, IMG_GetError());
		return NULL;
	}

	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	texture->textureId = textureId;
	texture->height = surface->h;
	texture->width = surface->w;

	SDL_FreeSurface(surface);

	return texture;
}
