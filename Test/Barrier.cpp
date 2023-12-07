
#include "Barrier.h"

Barrier::Barrier() : BoidRepeller()
{
	position = Vector2(0, 0);
	boidType = BoidType::NoTeam;
}

Barrier::Barrier(Vector2 position) : BoidRepeller()
{
	this->position = position;
	boidType = BoidType::NoTeam;
}