#pragma once
#ifndef virtualField
#define virtualField

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include "player.h"
class field : public sf::Drawable
{
public:
	virtual void mainEventServer(std::vector<std::shared_ptr<player>> gg, int i, bool dec, sf::Text& text) = 0;
	virtual bool showButton(std::vector<std::shared_ptr<player>> gg, int i, sf::Text &text) = 0;
	virtual int getOwner() = 0;
	virtual int getValue() = 0;
	virtual void sellField() = 0;
	static std::vector<sf::Sound> sounds;
	static std::vector<sf::SoundBuffer> buffers;
	
	virtual void center(sf::Text& text)
	{
		float w = text.getLocalBounds().width;
		float h = text.getLocalBounds().height;
		text.setOrigin(w / 2, h / 2);
	}
	virtual sf::Vector2f getPlace(int id)
	{
		if (places[0]==-1)
		{
			places[0] = id;
			return sf::Vector2f{ x - 10.f,y - 25.f };
		}
		else if (places[1] == -1)
		{
			places[1] = id;
			return sf::Vector2f{ x + 10.f,y - 25.f };
		}
		else if (places[2] == -1)
		{
			places[2] = id;
			return sf::Vector2f{ x - 10.f,y};
		}
		else
		{
			places[3] = id;
			return sf::Vector2f{ x + 10.f,y };
		}
	}
	virtual void leavePlace(int id)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (places[i] == id)
			{
				places[i] = -1;
				break;
			}
		}
	}
	virtual void setWhiteRec()
	{
		whiteRectangle.setSize(sf::Vector2f(width, height));
		whiteRectangle.setFillColor(sf::Color(255,255,255,128));
		whiteRectangle.setOrigin(width/2, height/2);
		whiteRectangle.setPosition(x, y);
		whiteRectangle.setRotation((float)rotation);
	}
	sf::String name;
	sf::Text label;
	sf::Font font;
	float width=80;
	float height=150;
	float x;
	float y;
	int places[4]{-1,-1,-1,-1};
	int rotation;
	sf::Texture texture;
	sf::Sprite image;
	sf::RectangleShape whiteRectangle;
};
#endif
