#pragma once
#ifndef jailField
#define jailField
#include "field.h"
class jail : public field
{
public:
	void mainEventServer(std::vector<std::shared_ptr<player>> gg, int i, bool dec, sf::Text& text);
	int getOwner();
	int getValue();
	void sellField();
	bool showButton(std::vector<std::shared_ptr<player>> gg, int i, sf::Text& text);
	jail(float xx, float yy, int r = 0);
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
#endif
