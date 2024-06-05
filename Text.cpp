#include "Text.h"
#include "Texture.h"
#include "Assets.h"

Text::Text(std::string content, Vector2 position) :
	text(content),
	position(position),
	texture(nullptr),
	font(Assets::getFont("Carlito"))
{
	texture = font.renderText(text, Color::white, 40);
}

Text::~Text()
{
	if (texture)
	{
		texture->unload();
		delete texture;
		texture = nullptr;
	}
}

Vector2 Text::getPosition() { return position; }

void Text::setPosition(Vector2 position)
{
	this->position = position;
}

std::string Text::getContent() { return text; }

void Text::setContent(std::string content, const Vector3& color, int pointSize)
{
	texture->unload();
	delete texture;
	texture = nullptr;
	text = content;
	texture = font.renderText(text, color, pointSize);
}

Texture* Text::getTexture() { return texture; }