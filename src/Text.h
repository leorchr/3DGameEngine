#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Color.h"
#include <string>

class Text
{
public:

	Text() = delete;
	Text(std::string content, Vector2 position);
	~Text();
	Vector2 getPosition();
	void setPosition(Vector2 position);
	std::string getContent();
	void setContent(std::string content, const Vector3& color = Color::white, int pointSize = 40);
	class Texture* getTexture();

private:
	std::string text;
	Vector2 position;
	class Texture* texture;
	class Font& font;
};

