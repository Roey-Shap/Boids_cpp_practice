
#include "BoidRepeller.h"

// sort of works, but would probably be far more effective with a matrix for each of cohesion, separation, and alignment
const float BoidRepeller::entityInfluences[BoidType::NUM_TEAMS + 2][BoidType::NUM_TEAMS + 2] = {
		{1    , 0.25 , 0.25 , 0.25   , -10  ,  1},	// White
		{0    , 1    , 0.5  , 0      , -10  ,  1},	// Blue
		{0    , 0.25 , 1    , -10    , -10  ,  1},	// Red
		{-10   , -10   , -10   , 1   , -10  ,  1},	// Green
		{0,    0,     0,     0       ,  0  ,  0},	//Repeller
		{0,    0,     0,     0       ,  0  ,  0}	//Attractor
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
