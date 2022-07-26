#include "player.h"

void player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(pawn, states);
}

std::string player::getNick()
{
	return nick;
}
sf::Uint32 player::getColorUint32()
{
	sf::Uint32 temp = color;
	return color;
}
void player::setColor(int c)
{
	pawnTexture.loadFromFile(urlTab[c]);
	pawnTexture2.loadFromFile(urlTab2[c]);
	pawn.setTexture(pawnTexture);
	pawn.setOrigin(37,45);
	color = c;
}
void player::setNick(std::string n)
{
	nick = n;
}
void player::setXY(float xx, float yy)
{
	pawn.setPosition(xx, yy);
}

void player::update(sf::Vector2f newPosition)
{
	float dx;
	float dy;
	sf::Vector2f currentPosition = pawn.getPosition();
	dx = newPosition.x - currentPosition.x;
	dy = newPosition.y - currentPosition.y;

if (dx > dy)
{
	velocity.x = -3;
	velocity.y = -(dy / dx) * 3;
}
else
{
	velocity.x = -(dx / dy) * 3;
	velocity.y = -3;
}

while (1)
{
	if (pawn.getPosition().x <= newPosition.x)break;
	if (pawn.getPosition().y <= newPosition.x)velocity.y = 0;
	if (velocity == sf::Vector2f{ 0,0 })break;
	pawn.move(velocity);
}

}

