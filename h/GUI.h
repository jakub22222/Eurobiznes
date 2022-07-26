#pragma once
#ifndef userInterface
#define userInterface
#include <SFML/Graphics.hpp>
#include "button.h"
#include "field.h"
#include "city.h"
#include "windows.h"
#include "game.h"
#include <math.h> 
#include <regex> 
class GUI
{
public:
    sf::Vector2f resolution;
	GUI() = delete;
	GUI(float resx, float resy);
	void start();
private:
	bool validIp(std::string ip);
	void subMenu0(sf::RenderWindow& window, sf::Sprite& bg, sf::Sound& s);
	void subMenu1(sf::RenderWindow& window, sf::Sprite& bg, sf::Sound& s);
	void subMenu2(sf::RenderWindow& window, sf::Sprite& bg, sf::Sound& s);
	void subMenu3(sf::RenderWindow& window, sf::Sprite& bg, sf::Sound& s);
	void subMenu4(sf::RenderWindow& window, sf::Sprite& bg, sf::Sound& s);
	
};
#endif
