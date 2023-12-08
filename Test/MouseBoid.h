
#pragma once

#include "Boid.h"

class MouseBoid : public Boid
{
private:
	sf::CircleShape drawShape;
	float drawRadius;

public:
	MouseBoid();
	void step(Vector2 position);
	void setAttractor(bool value);
	bool getAttractor();
	void draw(sf::RenderWindow* window, const Vector2& offset) override;
};