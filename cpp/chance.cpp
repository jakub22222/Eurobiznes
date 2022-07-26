#include "chance.h"
void chance::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(whiteRectangle, states);
	target.draw(image, states);
}

chance::chance(float xx, float yy, sf::Color c, int r)
{
	x = xx;
	y = yy;
	color = c;
	name = "Take a chance on me";
	if (color == sf::Color::Red)
		texture.loadFromFile("img/redChance.png");
	else texture.loadFromFile("img/blueChance.png");
	image.setTexture(texture);
	image.setScale(1, 1);
	image.setOrigin(40, 64);
	image.setTextureRect(sf::IntRect(1, 1, 80, 128));
	image.setPosition(x, y);
	rotation = r;
	image.setRotation((float)rotation);
	this->setWhiteRec();
}
void chance::mainEventServer(std::vector<std::shared_ptr<player>> gg, int i, bool dec, sf::Text& text)
{
	sounds[2].play();
	sf::String temp = L"Treœæ szansy poci¹gniêtej przez gracza ";
	temp += gg[i]->nick + " :\n";

	if (color == sf::Color::Red)
	{
		if (redNumbers.size() == 0)
			fillVector(redNumbers, sizeRed);
		int caseNumber = *(redNumbers.end() - 1);
		redNumbers.pop_back();
		switch (caseNumber)
		{
		case 0:
		{
			temp += sf::String(L"NIC");
			break;
		}
		case 1:
		{
			temp += sf::String(L"P³acisz na budowê szpitala 400 $");
			gg[i]->money -= 400;
			if (gg[i]->money < 0)
			{
				if (gg[i]->wealth >= -gg[i]->money)
					gg[i]->vindication = true;
				else
				{
					gg[i]->bankrupt = true;
				}
			}
			break;
		}
		case 2:
		{
			temp += sf::String(L"Otrzymujesz zwrot nadp³aconego podatku dochdowego: 40 $");
			gg[i]->money += 40;
			break;
		}
		default:
		{
			temp += "case inny";
			break;
		}
		}

	}
	else
	{
		if (blueNumbers.size() == 0)
			fillVector(blueNumbers, sizeBlue, 2);
		int caseNumber = *(blueNumbers.end() - 1);
		blueNumbers.pop_back();
		switch (caseNumber)
		{
		case 0:
		{
			temp += sf::String(L"Idziesz na START, nie otrzymujesz bonusu");
			gg[i]->pawn.setPosition(960.f, 956.f);
			gg[i]->position = 0;
			break;
		}
		case 1:
		{
			temp += sf::String(L"Bank wyp³aca wyp³aca Ci nale¿ne odsetki w wyskoœci 300 $");
			gg[i]->money += 300;
			break;
		}
		case 2:
		{
			temp += sf::String(L"Otrzymujesz karte, która chroni przed kar¹ wiêzienia");
			gg[i]->card = true;
			break;
		}
		default:
		{
			temp += "case inny";
			break;
		}
		}
	}
	text.setString(temp);
}

void chance::fillVector(std::vector<int>& numbers, int size, int multiplier)
{
	for (int i = 0; i < size; ++i)
		numbers.push_back(i);

	std::random_device rd;
	std::mt19937 g(seed * multiplier);

	std::shuffle(numbers.begin(), numbers.end(), g);
}

bool chance::showButton(std::vector<std::shared_ptr<player>> gg, int i, sf::Text& text) { return false; }
int chance::getOwner() { return -1; };
int chance::getValue() { return 0; };
void chance::sellField() { return; };