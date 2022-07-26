#include "industry.h"
industry::industry(sf::String n, float xx, float yy, char type, int p, int r)
{
	name = n;
	x = xx;
	y = yy;
	price = p;
	font.loadFromFile("fonts/arial.ttf");
	label.setCharacterSize(15);

	label.setFillColor(sf::Color::Blue);
	label.setString(name);
	label.setFont(font);
	center(label);

	priceText.setCharacterSize(15);
	priceText.setFillColor(sf::Color::Blue);
	priceText.setStyle(sf::Text::Bold);
	std::string s = std::to_string(price) + " $";
	priceText.setString(s);
	priceText.setFont(font);
	center(priceText);

	switch (type)
	{
	case 'P':
		texture.loadFromFile("img/power.png");
		break;
	case 'W':
		texture.loadFromFile("img/water.png");
		break;
	default:
		texture.loadFromFile("img/rail.png");
		break;
	}
	image.setTexture(texture);
	image.setScale(1, 1);
	image.setOrigin(32, 32);
	for (int i = 0; i < 6; i++)
	{
		flags[i] = "img/flag" + std::to_string(i) + ".png";
	}
	image.setTextureRect(sf::IntRect(1, 1, 64, 64));
	flag.setScale(1, 1);
	flag.setTextureRect(sf::IntRect(1, 1, 32, 32));
	flag.setOrigin(16, 16);
	rotation = r;
	switch (rotation)
	{
	case 90:
	{
		flag.setPosition(x + 80, y - 15);
		flag.setRotation((float)rotation);
		image.setPosition(x - 42, y);
		image.setRotation((float)rotation);
		label.setPosition(x + 68, y);
		label.setRotation((float)rotation);
		priceText.setPosition(x + 48, y);
		priceText.setRotation((float)rotation);
		break;
	}
	case -90:
	{
		flag.setPosition(x - 80, y - 15);
		flag.setRotation((float)rotation);
		image.setPosition(x + 42, y);
		image.setRotation((float)rotation);
		label.setPosition(x - 68, y);
		label.setRotation((float)rotation);
		priceText.setPosition(x - 48, y);
		priceText.setRotation((float)rotation);
		break;
	}
	case 180:
	{
		flag.setPosition(x + 15, y + 80);
		flag.setRotation((float)rotation);
		image.setPosition(x, y - 42);
		label.setPosition(x, y + 68);
		label.move(0, -5);
		priceText.setPosition(x, y + 48);
		priceText.move(0, -5);
		break;
	}
	default:
	{
		flag.setPosition(x - 15, y - 80);
		image.setPosition(x, y + 42);
		label.setPosition(x, y - 68);
		priceText.setPosition(x, y - 48);
		break;
	}
	}
	this->setWhiteRec();
}

bool industry::showButton(std::vector<std::shared_ptr<player>> gg, int i, sf::Text& text)
{
	if (owner == -1 && gg[i]->money >= price)
	{
		sf::String msg = L"Chcesz kupiæ " + name + " ?\n";
		msg += "Koszt wynosi " + std::to_string(price) + " $\n";
		msg += "Na koncie pozostanie " + std::to_string(gg[i]->money - price) + " $\n";
		text.setString(msg);
		return true;
	}
	return false;
}

void industry::mainEventServer(std::vector<std::shared_ptr<player>> gg, int i, bool dec, sf::Text& text)
{
	sf::String temp = "";
	if (owner == -1 && gg[i]->money >= price && dec)
	{
		sounds[0].play();
		showFlag = true;
		flagTexture.loadFromFile(flags[gg[i]->color]);
		flag.setTexture(flagTexture);
		owner = i;
		gg[i]->money -= price;
		gg[i]->wealth += price;
		temp += "Gracz " + gg[i]->nick + sf::String(L" zakupi³ obiekt przemys³owy ") + name;
		if (price == 400)gg[i]->trains++;
	}
	else if (i != owner && owner != -1)
	{
		int tempRent;
		if (price == 400)
		{
			if (gg[owner]->trains == 1)tempRent = 50;
			else if (gg[owner]->trains == 2)tempRent = 100;
			else if (gg[owner]->trains == 3)tempRent = 200;
			else tempRent = 400;
		}
		else tempRent = 10 * gg[i]->lastMove;

		if (gg[i]->money >= tempRent)
		{
			sounds[1].play();
			gg[i]->money -= tempRent;
			gg[owner]->money += tempRent;
			temp += "Gracz " + gg[i]->nick + sf::String(L" p³aci ") + std::to_string(tempRent) + " $ czynszu dla " + gg[owner]->nick + "\n";
		}
		else if ((gg[i]->money + gg[i]->wealth) >= tempRent)
		{
			gg[i]->money -= tempRent;
			gg[owner]->money += tempRent;
			temp += "Gracz " + gg[i]->nick + sf::String(L" p³aci ") + std::to_string(tempRent) + " $ czynszu dla " + gg[owner]->nick + "\n";
			temp += sf::String(L"Jego maj¹tek zostanie zwindykowany\n");
			gg[i]->vindication = true;
		}
		else if ((gg[i]->money + gg[i]->wealth) < tempRent)
		{
			gg[owner]->money += gg[i]->money;
			gg[owner]->money += gg[i]->wealth;
			temp += "Gracz " + gg[i]->nick + sf::String(L" zbankrutowa³ i oddaje swój ca³y maj¹tek ") + std::to_string(gg[i]->money + gg[i]->wealth) + " $  do " + gg[owner]->nick + "\n";
			gg[i]->bankrupt = true;
		}
	}
	text.setString(temp);
}

int industry::getOwner() 
{ 
	return owner; 
};
int industry::getValue()
{
	if (owner == -1)return 0;
	else return price;
};
void industry::sellField()
{
	owner = -1;
	showFlag = false;
};