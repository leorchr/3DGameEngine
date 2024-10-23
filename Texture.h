#pragma once
#include <string>
#include "RendererOGL.h"
using std::string;

class Texture
{
public:
	Texture();
	~Texture();

	void unload();
	//bool loadSDL(RendererSDL& rendererP, const string& filenameP);
	bool loadOGL(RendererOGL& rendererP, std::string fileName);
	

	inline SDL_Texture* toSDLTexture() const { return SDLTexture; }
	void updateInfo(int& widthOut, int& heightOut);
	void createFromSurface(struct SDL_Surface* surface);
	void bind(GLenum TextureUnit) const;

	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }

private:
	int width;
	int height;
	SDL_Texture* SDLTexture = nullptr;
	GLenum textureTarget;
	GLuint textureObj;
};

