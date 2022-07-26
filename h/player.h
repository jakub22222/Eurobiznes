#pragma once
#ifndef playerField
#define playerField
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <windows.h>


class player : public sf::Drawable
{
public:
	player() = delete;
	player(std::string n, int i = 0) : nick(n), id(i) {};
	player(int i) : id(i) {};
	std::string getNick();
	sf::Uint32 getColorUint32();
	void setColor(int c);
	void setNick(std::string n);
	sf::TcpSocket socket;
	void setXY(float xx, float yy);
	void update(sf::Vector2f newPosition);
//private:
	sf::Packet packet;
	float x=1;
	float y=1;
	int id = -1;
	int trains = 0;
	int position = 0;
	int money;
	int travel = -1;
	int lastMove = 0;
	int roundsInJail = 0;
	int wealth = 0;
	bool card = false;
	bool inJail = false;
	bool bankrupt = false;
	bool vindication = false;
	int color;
	std::string nick;
	float defaultSpeed = 1;
	sf::Vector2f velocity{ 0, 0 };
	sf::Sprite pawn;
	sf::Texture pawnTexture;
	sf::Texture pawnTexture2;
	std::string urlTab[6]{"img/shrek0.png","img/shrek1.png" ,"img/shrek2.png" ,"img/shrek3.png" ,"img/shrek4.png" ,"img/shrek5.png" };
	std::string urlTab2[6]{ "img/runningShrek0.png","img/runningShrek1.png" ,"img/runningShrek2.png" ,"img/runningShrek3.png" ,"img/runningShrek4.png" ,"img/runningShrek5.png" };
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
#endif
