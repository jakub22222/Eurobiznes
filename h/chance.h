#pragma once
#ifndef chanceField
#define chanceField
#include "field.h"
#include <random>
#include <algorithm>
#include <vector>
class chance : public field
{
public:
	static std::vector<int> redNumbers;
	static std::vector<int> blueNumbers;
	static int sizeRed;
	static int sizeBlue;
	static int seed;
	static void fillVector(std::vector<int>& numbers, int size, int multiplier = 1);
	void mainEventServer(std::vector<std::shared_ptr<player>> gg, int i, bool dec, sf::Text& text);
	bool showButton(std::vector<std::shared_ptr<player>> gg, int i, sf::Text& text);
	int getOwner();
	int getValue();
	void sellField();
	chance(float xx, float yy, sf::Color c, int r = 0);
private:
	sf::Color color;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};


#endif

