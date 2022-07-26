#include "special.h"
void special::mainEventServer(std::vector<std::shared_ptr<player>> gg, int i, bool dec, sf::Text& text)
{
	sf::String temp;
	gg[i]->money += bonus;
	if (bonus == -200)
	{
		if (gg[i]->money < 0)
		{
			if (gg[i]->wealth >= -gg[i]->money)
				gg[i]->vindication = true;
			else
			{
				gg[i]->bankrupt = true;
			}
		}
		temp = "Gracz " + gg[i]->nick + sf::String(L" p³aci podatek od wzbogacenia w wyskoœci 200 $");
	}
	else if (bonus == 0)temp = "Gracz " + gg[i]->nick + sf::String(L" korzysta z bezp³atnego parkingu");
	else temp = "Gracz " + gg[i]->nick + sf::String(L" otrzymuje 400 $");
	text.setString(temp);
}

int special::getOwner() { return -1; };
int special::getValue() { return 0; };
void special::sellField() { return; };
bool special::showButton(std::vector<std::shared_ptr<player>> gg, int i, sf::Text& text){ return false; }

special::special(float w, float xx, float yy, int b, sf::String n, int r)
{
	width = w;
	x = xx;
	y = yy;
	bonus = b;
	name = n;
	font.loadFromFile("fonts/arial.ttf");
	label.setCharacterSize(15);
	label.setFillColor(sf::Color::Blue);
	label.setString(name);
	label.setFont(font);
	center(label);
	if (b != 0)
	{
		bonusText.setCharacterSize(20);
		bonusText.setFillColor(sf::Color::Blue);
		std::string s;
		if (bonus > 0)s += "+";
		s += std::to_string(bonus) + " $";
		bonusText.setString(s);
		bonusText.setFont(font);
		center(bonusText);
	}
	rotation = r;
	switch (rotation)
	{
	case 90:
	{
		label.setPosition(x + 50, y);
		label.setRotation((float)rotation);
		bonusText.setPosition(x + 25, y);
		bonusText.setRotation((float)rotation);
		break;
	}
	case -90:
	{
		label.setPosition(x - 50, y);
		label.setRotation((float)rotation);
		bonusText.setPosition(x - 25, y);
		bonusText.setRotation((float)rotation);
		break;
	}
	case 180:
	{
		label.setPosition(x, y + 50);
		label.move(0, -5);
		bonusText.setPosition(x, y + 25);
		bonusText.move(0, -5);
		break;
	}
	default:
	{
		label.setPosition(x, y - 50);
		bonusText.setPosition(x, y - 25);
		break;
	}
	}
	this->setWhiteRec();
}
void special::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(whiteRectangle, states);
	target.draw(label, states);
	target.draw(bonusText, states);
}