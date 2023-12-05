#pragma once


#include <iostream>
#include "Vector2.h"
#include <SFML/Graphics.hpp>

#include <vector>

using namespace std;

class Boid
{
private:
    static int boidGUIDCounter;

    int id;
    Vector2 position;
    float direction;
    Vector2 velocity;
    Vector2 acceleration;
    float perceptionRadius;
    float velocityMax;
    float maxAddableForce;
    sf::CircleShape drawShape;

    void* quadTree;
public:
    void setPosition(Vector2 position);
    Vector2* getPosition();
    void setDirection(float direction);
    void setVelocity(Vector2 velocity);
    void setPositionQueryStructure(void* quadTree);
    int getId();

    void detectEdges(Vector2 boundsMin, Vector2 boundsMax);
    Vector2 alignWithNeighbors(vector<Boid*>* neighbors, int numNeighbors);
    Vector2 separateFromNeighbors(vector<Boid*>* neighbors, int numNeighbors);
    Vector2 steerTowardsNeighborAverage(vector<Boid*>* neighbors, int numNeighbors);
    void flock();
    void step();

    void draw(sf::RenderWindow* window, const Vector2& offset);

    Boid(Vector2 spawnPosition);
    Boid();
};