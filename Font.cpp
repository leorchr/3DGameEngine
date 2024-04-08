#include "Font.h"
#include "Log.h"
#include "Texture.h"
#include <sstream>

Font::Font()
{
}

Font::~Font()
{
}

bool Font::initialize()
{
	if (TTF_Init() != 0)
	{
		Log::error(LogCategory::System, "Failed to initialize SDL_ttf");
		return false;
	}
	return true;
}

void Font::close()
{
	TTF_Quit();
}

void Font::unload()
{
	for (auto& font : fontData)
	{
		TTF_CloseFont(font.second);
	}
}

Texture* Font::renderText(const string& text, const Vector3& color, int pointSize) 
{
	Texture* texture = nullptr;
	SDL_Color sdlColor;
	sdlColor.r = static_cast(color.x * 255);
	sdlColor.r = static_cast(color.y * 255);
	sdlColor.r = static_cast(color.z * 255);
	sdlColor.a = 255;

	auto iter = fontData.find(pointSize);
	if (iter != fontData.end()) {
		TTF_Font* font = iter->second;
		SDL_Surface* surf = TTF_RenderUTF8_Blended(font, text.c_str(), sdlColor);
		if(surf != nullptr) {
			texture = new Texture();
			texture->createFromSurface(surf);
			SDL_FreeSurface(surf);
		}
	}
	else {
		std::ostringstream loadError;
		loadError << "Point size " << pointSize << " is unsupported";
		Log::error(LogCategory::Application, loadError.str());
	}

	return texture;
}

void Font::addFontData(int size, TTF_Font* fontSize) {
	fontData.emplace(size, fontSize);
}