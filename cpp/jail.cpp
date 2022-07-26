#include "jail.h"
void jail::mainEventServer(std::vector<std::shared_ptr<player>> gg, int i, bool dec, sf::Text& text)
{
	text.setString(L"Pozdrowienia do wiêzienia");
}

int jail::getOwner() { return -1; };
int jail::getValue() { return 0; };
void jail::sellField() { return; };

bool jail::showButton(std::vector<std::shared_ptr<player>> gg, int i, sf::Text& text)
{
	return false;
}

jail::jail(float xx, float yy, int r)
{
	width = 150;
	x = xx;
	y = yy;
	name = "jail";
	texture.loadFromFile("img/jail.png");
	image.setTexture(texture);
	image.setScale(1, 1);
	image.setTextureRect(sf::IntRect(1, 1, 150, 150));
	image.setOrigin(75, 75);
	image.setPosition(x, y);
	image.setRotation(r);
}

void jail::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(image, states);
}