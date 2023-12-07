#pragma once

#include "Vector2.h"

extern enum BoidType
{
	Team1,
	Team2,
	Team3,
	Team4,
	NUM_TEAMS,
	NoTeam,

	NUM_VALUES
};

extern enum boidColor
{
	WHITE,
	BLUE,
	RED,
	GREEN,

	NUM_ELEMENTS
};

class BoidRepeller
{
protected:
	BoidType boidType;
	void* quadTree;

public:
	Vector2 velocity = Vector2();
	Vector2 position = Vector2();

	Vector2 getPosition();
	BoidType getBoidType();
	float getInfluenceFromNeighbor(BoidRepeller* other);
	void setPositionQueryStructure(void* quadTree);
	BoidRepeller();
	BoidRepeller(Vector2 position);

	static const sf::Color boidColors[(int)boidColor::NUM_ELEMENTS];
	static const float entityInfluences[BoidType::NUM_TEAMS][BoidType::NUM_TEAMS];
};