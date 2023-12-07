#pragma once


#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Vector2.h"
#include "Simulation.h"

using namespace std;    // apparently it's bad to do namespace std in a header file - how to avoid it though? The class predefinition thing?

extern enum boidColor
{
    WHITE,
    BLUE,
    RED,
    GREEN,

    NUM_ELEMENTS
};


class Boid
{
private:
    static int boidGUIDCounter;

    int id;
    Vector2 position;
    Vector2 prevPosition;
    Vector2 velocity;
    Vector2 acceleration;
    float perceptionRadius;
    float velocityMax;
    float maxAddableForce;
    sf::Vertex vertices[3];

    bool renderDebugText;
    //TriangleShape drawShape;

    void* quadTree;
public:
    boidColor color = boidColor::WHITE;

    void setPosition(Vector2 position);
    Vector2 getPosition();
    void setVelocity(Vector2 velocity);
    Vector2 getVelocity();
    void setPositionQueryStructure(void* quadTree);
    int getId();
    void setRenderDebugText(bool active);

    void detectEdges(Vector2 boundsMin, Vector2 boundsMax);
    Vector2 alignWithNeighbors(vector<Boid*>* neighbors, int numNeighbors);
    Vector2 separateFromNeighbors(vector<Boid*>* neighbors, int numNeighbors);
    Vector2 steerTowardsNeighborAverage(vector<Boid*>* neighbors, int numNeighbors);
    void flock();
    void step();

    void draw(sf::RenderWindow* window, const Vector2& offset);

    Boid(Vector2 spawnPosition);
    Boid();

    static const sf::Color boidColors[(int)boidColor::NUM_ELEMENTS];
};