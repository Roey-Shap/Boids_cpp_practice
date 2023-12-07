
#include "BoidRepeller.h"

const float BoidRepeller::entityInfluences[BoidType::NUM_TEAMS][BoidType::NUM_TEAMS] = {
		{1    , 0.25 , 0.25 , 0.25 },	// White
		{0    , 1    , 0.5  , 0    },	// Blue
		{0    , 0.25 , 1    , -1   },	// Red
		{-1   , -1   , -1   , 1    }	// Green
};


const sf::Color BoidRepeller::boidColors[] = {
		sf::Color::White,
		sf::Color::Blue,
		sf::Color::Red,
		sf::Color::Green
};


float BoidRepeller::getInfluenceFromNeighbor(BoidRepeller* other)
{
	return BoidRepeller::entityInfluences[(int)boidType][(int)(other->getBoidType())];
}

Vector2 BoidRepeller::getPosition()
{
	return position;
}

BoidRepeller::BoidRepeller()
{
	boidType = BoidType::NoTeam;
	quadTree = NULL;
}

BoidRepeller::BoidRepeller(Vector2 position)
{
	BoidRepeller();
	this->position = position;
}

BoidType BoidRepeller::getBoidType()
{
	return boidType;
}

void BoidRepeller::setPositionQueryStructure(void* quadTree)
{
	this->quadTree = quadTree;
}
