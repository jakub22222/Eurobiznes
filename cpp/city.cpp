#include "city.h"

void city::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (showFlag)
		target.draw(flag, states);
	target.draw(whiteRectangle, states);
	if (buildings >= 0)
		target.draw(houses[buildings]);
	target.draw(rectangle, states);
	target.draw(label, states);
	target.draw(priceText, states);
}

bool city::showButton(std::vector<std::shared_ptr<player>> gg, int i, sf::Text& text)
{
	if (owner == -1 && gg[i]->money >= price)
	{
		sf::String msg = L"Chcesz kupiæ miasto " + name + " ?\n";
		msg += "Koszt wynosi " + std::to_string(price) + " $\n";
		msg += "Na koncie pozostanie " + std::to_string(gg[i]->money - price) + " $\n";
		text.setString(msg);
		return true;
	}
	else if (owner != i)
	{
		int price2 = (price + buildings * costOfNewBuilding) * 2;
		if ((gg[i]->money - rent[buildings]) < price2)return false;
		else
		{
			sf::String msg = L"Chcesz okupiæ miasto " + name + " od gracza " + gg[owner]->nick + " ?\n";
			msg += "Koszt wynosi " + std::to_string(price2) + " $\n";
			msg += "Na koncie pozostanie " + std::to_string((gg[i]->money - rent[buildings]) - price2) + " $\n";
			text.setString(msg);
			return true;
		}
	}
	else if (owner == i && buildings < 5 && gg[i]->money >= costOfNewBuilding)
	{
		sf::String msg = L"Chcesz zwiêkszyæ poziom zabudowañ ?\n";
		msg += "Koszt wynosi " + std::to_string(costOfNewBuilding) + " $\n";
		msg += "Na koncie pozostanie " + std::to_string(gg[i]->money - costOfNewBuilding) + " $\n";
		text.setString(msg);
		return true;
	}
	else
	{
		return false;
	}
}

void city::mainEventServer(std::vector<std::shared_ptr<player>> gg, int i, bool dec, sf::Text& text)
{
	sf::String temp = "";
	if (i != owner && owner != -1 && gg[i]->money >= rent[buildings])
	{
		sounds[1].play();
		gg[i]->money -= rent[buildings];
		gg[owner]->money += rent[buildings];
		temp += "Gracz " + gg[i]->nick + sf::String(L" p³aci ") + std::to_string(rent[buildings]) + " $ czynszu dla " + gg[owner]->nick + "\n";
	}
	else if (i != owner && owner != -1 && (gg[i]->money + gg[i]->wealth) >= rent[buildings])
	{
		gg[i]->money -= rent[buildings];
		gg[owner]->money += rent[buildings];
		temp += "Gracz " + gg[i]->nick + sf::String(L" p³aci ") + std::to_string(rent[buildings]) + " $ czynszu dla " + gg[owner]->nick + "\n";
		temp += sf::String(L"Jego maj¹tek zostanie zwindykowany\n");
		gg[i]->vindication = true;
	}
	else if (i != owner && owner != -1 && (gg[i]->money + gg[i]->wealth) < rent[buildings])
	{
		gg[owner]->money += gg[i]->money;
		gg[owner]->money += gg[i]->wealth;
		temp += "Gracz " + gg[i]->nick + sf::String(L" zbankrutowa³ i oddaje swój ca³y maj¹tek ") + std::to_string(gg[i]->money + gg[i]->wealth) + " $  do " + gg[owner]->nick + "\n";
		gg[i]->bankrupt = true;
	}
	sf::Text t;
	if (showButton(gg, i, t) && dec)
	{
		sounds[0].play();
		if (i == owner)
		{
			buildings++;
			gg[i]->money -= costOfNewBuilding;
			gg[i]->wealth += costOfNewBuilding;
			temp += "Gracz " + gg[i]->nick + sf::String(L" podniós³ poziom zabudowañ");
		}
		else if (owner == -1)
		{
			showFlag = true;
			flagTexture.loadFromFile(flags[gg[i]->color]);
			flag.setTexture(flagTexture);
			owner = i;
			gg[i]->money -= price;
			gg[i]->wealth += price;
			buildings = 0;
			temp += "Gracz " + gg[i]->nick + sf::String(L" zakupi³ miasto ") + name;
		}
		else
		{
			flagTexture.loadFromFile(flags[gg[i]->color]);
			flag.setTexture(flagTexture);
			int newPrice = (price + buildings * costOfNewBuilding) * 2;
			gg[owner]->money += newPrice;
			owner = i;
			gg[i]->money -= newPrice;
			gg[i]->wealth += price;
			temp += "Gracz " + gg[i]->nick + sf::String(L" odkupi³ miasto ") + name + " za " + std::to_string(newPrice) + " $ od " + gg[owner]->nick;
		}
	}
	text.setString(temp);
}

int city::getOwner() 
{ 
	return owner; 
};
int city::getValue()
{
	if (owner == -1)return 0;
	else return (price + buildings * costOfNewBuilding);
};
void city::sellField()
{
	owner = -1;
	buildings = -1;
	showFlag = false;
};

void city::setTextures(std::string houseUrl[5], int r)
{
	for (int i = 0; i < 5; ++i)
	{
		houseTextures[i].loadFromFile(houseUrl[i]);
		houses[i].setTexture(houseTextures[i]);
		houses[i].setScale(1, 1);
		houses[i].setTextureRect(sf::IntRect(1, 1, 64, 64));
		houses[i].setOrigin(32, 32);
		switch (r)
		{
		case 90:
		{
			houses[i].setPosition(x - 42, y);
			houses[i].setRotation(r);
			break;
		}
		case -90:
		{
			houses[i].setPosition(x + 42, y);
			houses[i].setRotation(r);
			break;
		}
		case 180:
		{
			houses[i].setPosition(x, y - 42);
			break;
		}
		default:
		{
			houses[i].setPosition(x, y + 42);
			break;
		}
		}
	}
}

city::city(sf::String cit, sf::String coun, int prince, std::vector<int> rents, int cost, sf::Color c, float xx, float yy, int r) : country(coun), price(prince), costOfNewBuilding(cost), color(c)
{
	name = cit;
	x = xx;
	y = yy;
	for (int i = 0; i < 6; ++i)
	{
		rent[i] = rents[i];
	}
	font.loadFromFile("fonts/arial.ttf");
	label.setCharacterSize(15);
	label.setFillColor(sf::Color::Black);
	label.setString(name);
	label.setFont(font);
	center(label);
	priceText.setCharacterSize(15);
	priceText.setFillColor(sf::Color::Black);
	priceText.setStyle(sf::Text::Bold);
	std::string s = std::to_string(price) + " $";
	priceText.setString(s);
	priceText.setFont(font);
	center(priceText);
	rectangle.setSize(sf::Vector2f(80, 20));
	rectangle.setFillColor(color);
	rectangle.setOrigin(40, 10);
	flag.setScale(1, 1);
	flag.setTextureRect(sf::IntRect(1, 1, 32, 32));
	flag.setOrigin(16, 16);
	for (int i = 0; i < 6; i++)
	{
		flags[i] = "img/flag" + std::to_string(i) + ".png";
	}
	rotation = r;
	switch (rotation)
	{
	case 90:
	{
		flag.setPosition(x + 80, y - 15);
		flag.setRotation((float)rotation);
		label.setPosition(x + 68, y);
		label.setRotation((float)rotation);
		priceText.setPosition(x + 48, y);
		priceText.setRotation((float)rotation);
		rectangle.setPosition(x + 65, y);
		rectangle.setRotation((float)rotation);
		break;
	}
	case -90:
	{
		flag.setPosition(x - 80, y - 15);
		flag.setRotation((float)rotation);
		label.setPosition(x - 68, y);
		label.setRotation((float)rotation);
		priceText.setPosition(x - 48, y);
		priceText.setRotation((float)rotation);
		rectangle.setPosition(x - 65, y);
		rectangle.setRotation((float)rotation);
		break;
	}
	case 180:
	{
		flag.setPosition(x + 15, y + 80);
		flag.setRotation((float)rotation);
		label.setPosition(x, y + 68);
		label.move(0, -5);
		priceText.setPosition(x, y + 48);
		priceText.move(0, -5);
		rectangle.setPosition(x, y + 65);
		break;
	}
	default:
	{
		flag.setPosition(x - 15, y - 80);
		label.setPosition(x, y - 68);
		priceText.setPosition(x, y - 48);
		rectangle.setPosition(x, y - 65);
		break;
	}
	}
	this->setWhiteRec();
	std::string url[6]{ "img/house0.png", "img/house1.png" ,"img/house2.png" ,"img/house3.png" ,"img/house4.png", "img/hotel.png" };
	setTextures(url, r);
}