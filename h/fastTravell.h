#pragma once
#ifndef ftField
#define ftField
#include "field.h"
class fastTravell : public field
{
public: 
	void mainEventServer(std::vector<std::shared_ptr<player>> gg, int i, bool dec, sf::Text& text);
	void mainEventClient(std::vector<std::shared_ptr<player>> gg, int i, int me);
	int getOwner();
	int getValue();
	void sellField();
	bool showButton(std::vector<std::shared_ptr<player>> gg, int i, sf::Text& text);
	fastTravell() = delete;
	fastTravell(float xx, float yy);
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
};
#endif

