#include "Texture.h"
#include "Log.h"
#include <SDL_image.h>
#include <sstream>

Texture::Texture(GLenum textureTarget, const std::string& fileName) : textureObj(0), fileName(""), width(0), height(0), SDLTexture(nullptr)
{
	this->textureTarget = textureTarget;
	this->fileName      = fileName;
}

Texture::Texture() : width(0), height(0), fileName(""), textureTarget(GL_TEXTURE_2D), textureObj(0) {}

Texture::~Texture()
{
}

void Texture::unload()
{
	if (SDLTexture)
	{
		SDL_DestroyTexture(SDLTexture);
	}
	else
	{
		glDeleteTextures(1, &textureObj);
	}
}

/*
bool Texture::loadSDL(RendererSDL& renderer, const string& filenameP)
{
	filename = filenameP;
	// Load from file
	SDL_Surface* surf = IMG_Load(filename.c_str());
	if (!surf)
	{
		Log::error(LogCategory::Application, "Failed to load texture file " + filename);
		return false;
	}
	width = surf->w;
	height = surf->h;

	// Create texture from surface
	SDLTexture = SDL_CreateTextureFromSurface(renderer.toSDLRenderer(), surf);
	SDL_FreeSurface(surf);
	if (!SDLTexture)
	{
		Log::error(LogCategory::Render, "Failed to convert surface to texture for "+ filename);
		return false;
	}
	Log::info("Loaded texture " + filename);
	return true;
}
*/

bool Texture::loadOGL(RendererOGL& renderer)
{
	// Load from file
	SDL_Surface* surf = IMG_Load(fileName.c_str());
	if (!surf)
	{
		Log::error(LogCategory::Application, "Failed to load texture file " + fileName);
		return false;
	}
	width = surf->w;
	height = surf->h;
	int format = 0;
	if (surf->format->format == SDL_PIXELFORMAT_RGB24)
	{
		format = GL_RGB;
	}
	else if (surf->format->format == SDL_PIXELFORMAT_RGBA32)
	{
		format = GL_RGBA;
	}
	glGenTextures(1, &textureObj);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, surf->pixels);
	SDL_FreeSurface(surf);


	Log::info("Loaded texture " + fileName);
	// Enable bilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture::updateInfo(int& widthOut, int& heightOut)
{
	widthOut = width;
	heightOut = height;
}

void Texture::createFromSurface(SDL_Surface* surface)
{
	width = surface->w;
	height = surface->h;

	glGenTextures(1, &textureObj);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::bind(GLenum TextureUnit) const
{
	glActiveTexture(TextureUnit);
	glBindTexture(textureTarget, textureObj);
}