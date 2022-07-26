#include "fastTravell.h"

void fastTravell::mainEventServer(std::vector<std::shared_ptr<player>> gg, int i, bool dec, sf::Text& text)
{
	sf::String temp;
	if (!gg[i]->card)
	{
		sounds[3].play();
		gg[i]->roundsInJail = 2;
		gg[i]->pawn.setPosition(60.f + 10.f * (float)i, 1020.f);
		gg[i]->position = 10;
		gg[i]->inJail = true;
		temp = "Gracz " + gg[i]->nick + sf::String(L" idzie do wiêzienia na 2 rundy");
	}
	else
	{
		gg[i]->card = false;
		temp = "Gracz " + gg[i]->nick + sf::String(L" u¿ywa karty i nie idzie do wiêzienia");
	}
	text.setString(temp);
}
void fastTravell::mainEventClient(std::vector<std::shared_ptr<player>> gg, int i, int me)
{

}
fastTravell::fastTravell(float xx, float yy)
{
	name = L"Kara wiêzienia";
	width = 150;
	x = xx;
	y = yy;
	font.loadFromFile("fonts/arial.ttf");
	label.setCharacterSize(20);
	label.setFillColor(sf::Color::White);
	label.setString(name);
	label.setFont(font);
	center(label);
	label.setPosition(x, y-55);
	texture.loadFromFile("img/travell.jpg");
	image.setTexture(texture);
	image.setScale(1, 1);
	image.setOrigin(75,75);
	image.setTextureRect(sf::IntRect(1, 1, 150, 150));
	image.setPosition(x, y);
	this->setWhiteRec();
}
void fastTravell::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
	target.draw(whiteRectangle, states);
	target.draw(image,states);
	target.draw(label, states);
}

int fastTravell::getOwner() { return -1; };
int fastTravell::getValue() { return 0; };
void fastTravell::sellField() { return; };
bool fastTravell::showButton(std::vector<std::shared_ptr<player>> gg, int i, sf::Text& text){ return false;}
