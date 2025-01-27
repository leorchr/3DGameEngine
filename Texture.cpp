#include "Texture.h"
#include "Log.h"
#include <SDL_image.h>
#include <sstream>

int Texture::nextID = 0;

Texture::Texture() : width(0), height(0), textureTarget(GL_TEXTURE_2D), textureObj(0), id(nextID++) {}

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

bool Texture::loadOGL(RendererOGL& renderer, std::string fileName)
{
	// Load from file
	SDL_Surface* surf = IMG_Load(fileName.c_str());
	if (!surf)
	{
		Log::error(LogCategory::Application, "Failed to load texture file " + fileName);
		return false;
	}
	auto filtering = GL_LINEAR;
	
	if (surf->format->format != SDL_PIXELFORMAT_RGB24 && surf->format->format != SDL_PIXELFORMAT_RGBA32)
	{
		surf = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA32, 0);
		filtering = GL_NEAREST;
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);

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