#pragma once
#ifndef but
#define but
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

class button : public sf::Drawable
{
public:
    button() = delete;
    button(float xB, float yB, float w, float h, sf::String u1, sf::String u2, bool i = false);
    void setColor(sf::Color c);
    void select();
    void unselect();
    void addLetter(sf::String x);
    void addLetter(char x);
    void clearInnerText();
    void deleteLetter();
    sf::String getInnerText();
    sf::Color getColor();
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    float x;
    float y;
    float width;
    float height;
    bool isInput = false;
    sf::Color bgColor = sf::Color::Transparent;
    sf::String url;
    sf::String urlSelect;
    sf::String innerText = "";
    bool selected = false;
};
#endif
