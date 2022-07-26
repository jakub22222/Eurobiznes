#pragma once
#ifndef gameFile
#define gameFile
#define DRAW window->pollEvent(event);\
		window->clear(sf::Color::White);\
		window->draw(*this);\
		window->display();
#include <string>
#include <iostream>
#include <chrono>
#include <random>
#include <functional>
#include <vector>
#include <thread>
#include <utility>
#include <sstream>
#include <ranges>
#include <filesystem>
#include <fstream>
#include "field.h"
#include "city.h"
#include "special.h"
#include "fastTravell.h"
#include "industry.h"
#include "chance.h"
#include "jail.h"
#include "player.h"
#include "button.h"
class game : public sf::Drawable
{
public:
	game() = delete;
	game(std::string nick, int color, int numOfPlayers, sf::RenderWindow* w);
	game(std::string n, int color, sf::IpAddress serwerIP, sf::RenderWindow* w);
	void serverGameplay();
	sf::RenderWindow* window;
	int lastThrow;
	static sf::Color getColor(int i);
	bool connectionError = false;
private:
	void readSettings();
	void startTimer();
	void sellProperties();
	void loadBoard();
	void movePlayer(int id);
	void setLabels();
	void loadTexts();
	void loadTextures();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void connectWithPlayer(std::string serverNick, int serverColor);
	void connectWithSerwer(std::string nick, int c, sf::IpAddress serwerIP, int port = 53000);
	void sendPlayers();
	void getPlayers();
	void setEndTextAndSound();
	void loadSound();
	void removePlayer();
	void hideInfo(int interwal);
	void recive();
	void send();
	sf::Font font;
	std::vector<sf::Text> playerNames;
	std::vector<sf::Text> playerMoney;
	sf::Text decision;
	sf::Text information;
	sf::Text vindicationed;
	std::vector<sf::RectangleShape> playerRectangle;
	std::vector<sf::Text> endTexts;
	sf::Texture bgTexture;
	sf::Texture logoTexture;
	sf::Texture logoTexture2;
	sf::Texture redCardTexture;
	sf::Texture blueCardTexture;
	sf::Sprite bg;
	sf::Sprite logo;
	sf::Sprite logo2;
	sf::Sprite redCard;
	sf::Sprite blueCard;
	sf::SoundBuffer buffer;
	sf::Sound sound;
	sf::SoundBuffer buffer2;
	sf::Sound cube;
	sf::SoundBuffer buffer3;
	sf::Sound vin;
	sf::SoundBuffer buffer4;
	sf::Sound banc;
	bool amISerwer;
	bool showThrow = false;
	bool showYesNo = false;
	bool showEnd = true;
	int minutes = 20;
	int seconds = 0;
	int defaultMoney = 3000;
	float speed = 5.f;
	sf::Text timer;
	std::vector<std::unique_ptr<field>> fields;
	std::vector<std::shared_ptr<player>> players;
	std::unique_ptr<button> yes;
	std::unique_ptr<button> no;
	std::unique_ptr<button> go;
	int numberOfPlayers;
	int seed;
	int bancrupts=0;
	int myId;
	std::atomic<bool> doneRecive = false;
	std::atomic<bool> doneSend = false;
	std::atomic<bool> playerDecison = false;
	std::atomic<bool> listen = false;
	std::atomic<bool> kill = false;
	std::atomic<bool> sending = false;
	std::atomic<bool> showInfo = false;
	std::atomic<int> actualPlayer = -1;
};
#endif