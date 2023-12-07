#pragma once


#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Vector2.h"
#include "Simulation.h"
#include "BoidRepeller.h"
#include "QuadTree.h"
#include "CustomMath.h"

#include "Simulation.h"


class Boid : public BoidRepeller
{
private:
    static int boidGUIDCounter;

    int id;
    Vector2 prevPosition;
    Vector2 acceleration;
    float perceptionRadius;
    float velocityMax;
    float maxAddableForce;
    sf::Vertex vertices[3];

    bool renderDebugText;

public:
    boidColor color = boidColor::WHITE;

    void setPosition(Vector2 position);
    Vector2 getPosition();
    void setVelocity(Vector2 velocity);
    Vector2 getVelocity();
    int getId();
    void setRenderDebugText(bool active);

    void detectEdges(Vector2 boundsMin, Vector2 boundsMax);
    Vector2 alignWithNeighbors(std::vector<BoidRepeller*>* neighbors, int numNeighbors);
    Vector2 separateFromNeighbors(std::vector<BoidRepeller*>* neighbors, int numNeighbors);
    Vector2 steerTowardsNeighborAverage(std::vector<BoidRepeller*>* neighbors, int numNeighbors);
    void flock();
    void step();

    void draw(sf::RenderWindow* window, const Vector2& offset);

    Boid(Vector2 spawnPosition, BoidType boidType);
    Boid();
};