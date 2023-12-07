#pragma once

#include "Vector2.h"
#include "Boid.h"

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

using namespace std;

// TODO Destructor, free array

class QuadTree
{
private:
	Vector2 boundaryTopLeft;
	Vector2 boundaryBottomRight;

	BoidRepeller** elements;
	int numElements;
	bool haveDivided;
	QuadTree* northEast;
	QuadTree* southEast;
	QuadTree* southWest;
	QuadTree* northWest;

public:
	static const int cellSplitThreshold = 4;

	Vector2 getTopLeft();
	Vector2 getBottomRight();
	Vector2 getDimensions();
	int getCellSplitThreshold();
	int getNumElements();
	bool getHaveDivided();

	BoidRepeller** getElements();

	void queryRegionForElements(vector<BoidRepeller*>* elementsFound, Vector2 topLeft, Vector2 bottomRight);

	void insertElement(BoidRepeller* element);
	void subdivide();

	void draw(sf::RenderWindow* window, const Vector2& offset);

	const QuadTree(Vector2 topLeft, Vector2 bottomRight);
	QuadTree();
	~QuadTree();
};