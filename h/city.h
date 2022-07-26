#pragma once
#ifndef cityField
#define cityField
#include "field.h"
class city : public field
{
public:
	city() = delete;
	city(sf::String cit, sf::String coun, int prince, std::vector<int> rents, int cost, sf::Color c, float xx, float yy, int r = 0);
	sf::String country;
	sf::String flags[6];
	sf::Texture houseTextures[6];
	sf::Texture flagTexture;
	sf::Sprite houses[5];
	sf::Sprite flag;
	sf::Color color;
	sf::Text priceText;
	sf::Text rentText;
	sf::RectangleShape rectangle;
	int price;
	int rent[6];
	int costOfNewBuilding;
	int buildings = -1;
	int owner = -1;
	bool monopoly = false;
	bool showFlag = false;
	void setTextures(std::string houseUrl[5], int r);
	int getOwner();
	int getValue();
	void sellField();
	void mainEventServer(std::vector<std::shared_ptr<player>> gg, int i, bool dec, sf::Text& text);
	bool showButton(std::vector<std::shared_ptr<player>> gg, int i, sf::Text& text);
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
#endif
