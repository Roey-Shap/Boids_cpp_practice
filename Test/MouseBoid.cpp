
#include "MouseBoid.h"

MouseBoid::MouseBoid() : Boid()
{
	drawRadius = 15.f;
    sf::CircleShape _drawShape(drawRadius);
    drawShape = _drawShape;
}

void MouseBoid::step(Vector2 position)
{
	this->position = position;
}

void MouseBoid::setAttractor(bool attract)
{
	if (attract)
	{
		boidType = BoidType::AllTeams;
	}
	else
	{
		boidType = BoidType::NoTeam;
	}
}

bool MouseBoid::getAttractor()
{
	return boidType == BoidType::AllTeams;
}

void MouseBoid::draw(sf::RenderWindow* window, const Vector2& offset)
{
	drawShape.setPosition(sf::Vector2f(position.x, position.y));
	drawShape.setOutlineThickness(1.5);
	drawShape.setOrigin(sf::Vector2f(drawRadius, drawRadius));
	bool amAttractor = boidType == BoidType::AllTeams;
	sf::Color _fillColor = amAttractor ? sf::Color::Transparent : sf::Color(240, 3, 252, (int)(0.3 * 255));
	sf::Color _outlineColor = amAttractor ? sf::Color::White : sf::Color::Magenta;
	drawShape.setFillColor(_fillColor);
	drawShape.setOutlineColor(_outlineColor);
    window->draw(drawShape);
}