#pragma once
#ifndef industryField
#define industryField
#include "field.h"
class industry :
    public field
{
public:
	int getOwner();
	int getValue();
	void sellField();
	void mainEventServer(std::vector<std::shared_ptr<player>> gg, int i, bool dec, sf::Text& text);
	bool showButton(std::vector<std::shared_ptr<player>> gg, int i, sf::Text& text);
	industry(sf::String n, float xx, float yy, char type = 'R', int p = 400, int r = 0);
private:
	int price;
	int owner=-1;
	sf::Texture texture;
	sf::Sprite image;
	sf::Text priceText;
	sf::String flags[6];
	sf::Texture flagTexture;
	sf::Sprite flag;
	bool showFlag = false;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if(showFlag)
		target.draw(flag, states);
		target.draw(whiteRectangle, states);
		target.draw(image,states);
		target.draw(priceText,states);
		target.draw(label,states);
	}
};
#endif
