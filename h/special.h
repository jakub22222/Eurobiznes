#pragma once
#ifndef specialField
#define specialField
#include "field.h"
class special : public field
{
public:
	void mainEventServer(std::vector<std::shared_ptr<player>> gg, int i, bool dec, sf::Text& text);
	int getOwner();
	int getValue();
	void sellField();
	bool showButton(std::vector<std::shared_ptr<player>> gg, int i, sf::Text& text);
	special(float w, float xx, float yy, int b, sf::String n, int r = 0);
private:
	int bonus;
	sf::Text bonusText;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
#endif
