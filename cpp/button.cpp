#include "button.h"

button::button(float xB, float yB, float w, float h, sf::String u1, sf::String u2, bool i )
{
    x = xB;
    y = yB;
    width = w;
    height = h;
    url = u1;
    urlSelect = u2;
    isInput = i;
}
sf::Color button::getColor()
{
    return bgColor;
}
void button::setColor(sf::Color c)
{
    bgColor = c;
}
void button::select()
{
    this->selected = true;
}
void button::unselect()
{
    this->selected = false;
}
void button::addLetter(sf::String x)
{
    innerText += x;
}
void button::addLetter(char x)
{
    innerText += x;
}
void button::clearInnerText()
{
    innerText.clear();
}
void button::deleteLetter()
{
    innerText = innerText.substring(0, innerText.getSize() - 1);
}
sf::String button::getInnerText()
{
    return innerText;
}
void button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Font font;
    font.loadFromFile("fonts/arial.ttf");
    sf::Text text(innerText, font);
    if (isInput)
    {
        text.setCharacterSize(30);
        text.setPosition(x-width/2.f+5, y-height/2.f+5);
        text.setFillColor(sf::Color::Black);
        text.setStyle(sf::Text::Italic);
    }
    sf::Texture texture;
    if (selected)
    texture.loadFromFile(urlSelect);
    else
    texture.loadFromFile(url);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setOrigin(this->width / 2, this->height / 2);
    sprite.setScale(1, 1);
    sprite.setTextureRect(sf::IntRect(1, 1, this->width, this->height));
    if(bgColor != sf::Color::Transparent)
    sprite.setColor(bgColor);
    sprite.setPosition(this->x, this->y);
    target.draw(sprite, states);
    if(innerText != "")
    target.draw(text, states);
}