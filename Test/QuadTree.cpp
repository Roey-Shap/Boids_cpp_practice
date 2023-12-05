
#include "QuadTree.h"
#include "Boid.h"
#include "Vector2.h"

#include <iostream>
#include <vector>

using namespace std;


QuadTree::QuadTree(Vector2* topLeft, Vector2* bottomRight)
{
	boundaryTopLeft = topLeft->copy();
	boundaryBottomRight = bottomRight->copy();

	elements = new Boid * [QuadTree::cellSplitThreshold];
	numElements = 0;

	haveDivided = false;
	QuadTree* northEast = NULL;
	QuadTree* southEast = NULL;
	QuadTree* southWest = NULL;
	QuadTree* northWest = NULL;
}

QuadTree::QuadTree()
{
	QuadTree(new Vector2(0, 0), new Vector2(0, 0));
}

Vector2 QuadTree::getTopLeft()
{
	return *(boundaryTopLeft->copy());
}

Vector2 QuadTree::getBottomRight()
{
	return *(boundaryBottomRight->copy());
}

Vector2 QuadTree::getDimensions()
{
	return (*boundaryBottomRight - *boundaryTopLeft);
}

int QuadTree::getNumElements()
{
	return numElements;
}

Boid** QuadTree::getElements()
{
	return elements;
}

int QuadTree::getCellSplitThreshold()
{
	return cellSplitThreshold;
}

bool QuadTree::getHaveDivided()
{
	return haveDivided;
}


void QuadTree::insertElement(Boid* element)
{
	//if (numElements > elementsArrayMaxSize)
	//{
	//	elementsArrayMaxSize *= 2;
	//	Boid* newElements = new Boid[elementsArrayMaxSize];
	//	for (int i = 0; i < elementsArrayMaxSize; i++)
	//	{
	//		newElements[i] = elements[i];
	//	}

	//	elements = newElements;
	//}

	if (!Vector2::pointInRect((element->getPosition()), boundaryTopLeft, boundaryBottomRight))
	{
		return;
	}

	if (numElements <= cellSplitThreshold)
	{
		numElements += 1;
		elements[numElements - 1] = element;
		element->setPositionQueryStructure(this);
	}
	else
	{
		subdivide();

		northEast->insertElement(element);
		southEast->insertElement(element);
		southWest->insertElement(element);
		northWest->insertElement(element);
	}
}

void QuadTree::subdivide()
{
	if (haveDivided)
	{
		return;
	}

	haveDivided = true;

	Vector2 dims = getDimensions();
	Vector2 middle = (*boundaryTopLeft + *boundaryBottomRight) / 2;
	Vector2 yOffset = Vector2(0, dims.y / 2);
	northEast = new QuadTree((middle - yOffset).copy(), (*boundaryBottomRight - yOffset).copy());
	southEast = new QuadTree(middle.copy(), boundaryBottomRight->copy());
	southWest = new QuadTree((*boundaryTopLeft + yOffset).copy(), (middle + yOffset).copy());
	northWest = new QuadTree(boundaryTopLeft->copy(), middle.copy());
}

void QuadTree::queryRegionForElements(vector<Boid*>* foundElements, Vector2* topLeft, Vector2* bottomRight)
{
	if (!Vector2::RectanglesIntersect(boundaryTopLeft, boundaryBottomRight, topLeft, bottomRight))
	{
		return;
	}

	// add whoever you can see
	for (int i = 0; i < numElements; i++)
	{
		Boid* element = elements[i];
		if (Vector2::pointInRect(element->getPosition()->copy(), topLeft, bottomRight)) {
			foundElements->push_back(element);
		}
	}

	// let your children add whoever they can see
	if (haveDivided)
	{
		northEast->queryRegionForElements(foundElements, topLeft, bottomRight);
		southEast->queryRegionForElements(foundElements, topLeft, bottomRight);
		southWest->queryRegionForElements(foundElements, topLeft, bottomRight);
		northWest->queryRegionForElements(foundElements, topLeft, bottomRight);
	}
}

void QuadTree::draw(sf::RenderWindow* window, const Vector2& offset)
{
	Vector2 dims = getDimensions();
	sf::Color fillColor(0, 0, 0, 0);
	sf::RectangleShape drawRect(sf::Vector2f(dims.x - 2, dims.y - 2));
	drawRect.setPosition(boundaryTopLeft->x + offset.x + 1, boundaryTopLeft->y + offset.y + 1);
	drawRect.setFillColor(fillColor);
	drawRect.setOutlineColor(sf::Color(0, 0, 255, 255 * (numElements / 10.0)));
	drawRect.setOutlineThickness(1);

	window->draw(drawRect);

	if (haveDivided)
	{
		northEast->draw(window, offset);
		southEast->draw(window, offset);
		southWest->draw(window, offset);
		northWest->draw(window, offset);
	}
}

