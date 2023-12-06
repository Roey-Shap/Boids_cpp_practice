
#include "Boid.h"
#include "Vector2.h"
#include <iostream>
#include <SFML/Graphics.hpp>

#include "quadTree.h"

using namespace std;

int Boid::boidGUIDCounter = 0;

Boid::Boid(Vector2 spawnPosition)
{
    id = Boid::boidGUIDCounter++;
    position = spawnPosition;
    velocity = Vector2(0, 0);
    acceleration = Vector2(0, 0);
    perceptionRadius = 100.0;
    velocityMax = 3.0;
    maxAddableForce = 0.075;
    sf::CircleShape _drawShape(2.f);
    drawShape = _drawShape;
    drawShape.setFillColor(sf::Color::Green);
}

Boid::Boid()
{
    id = Boid::boidGUIDCounter++;
    position = Vector2(0, 0);
    velocity = Vector2(0, 0);
    acceleration = Vector2(0, 0);
    perceptionRadius = 100.0;
    velocityMax = 3.0;
    maxAddableForce = 0.075;
    sf::CircleShape _drawShape(2.f);
    drawShape = _drawShape;
    drawShape.setFillColor(sf::Color::Green);
}

void Boid::setPosition(Vector2 position)
{
    this->position = position;
}

Vector2 Boid::getPosition()
{
    return Vector2(position.x, position.y);
}

int Boid::getId()
{
    return id;
}

void Boid::setVelocity(Vector2 velocity)
{
    free(&(this->velocity));
    this->velocity = velocity;
}

void Boid::setPositionQueryStructure(void* quadTree)
{
    this->quadTree = quadTree;
}

void Boid::detectEdges(Vector2 boundsMin, Vector2 boundsMax)
{
    position = boundsMin + (position % boundsMax);   
}


Vector2 Boid::alignWithNeighbors(vector<Boid*>* neighbors, int numNeighbors)
{
    Vector2 steerForce = Vector2();
    int neighborCount = 0;
    Boid* neighbor;

    for (int i = 0; i < numNeighbors; i++) {
        neighbor = neighbors->at(i);
        float d = (neighbor->position - position).length();
        if (neighbor != this && d < perceptionRadius) {
            steerForce += neighbor->velocity;
            neighborCount++;
        }
    }
    if (neighborCount > 0) {
        steerForce /= neighborCount;
        steerForce = (steerForce.getNormalized() * velocityMax) - velocity;
        steerForce.clampLength(maxAddableForce);
    }

    return steerForce;
}

Vector2 Boid::separateFromNeighbors(vector<Boid*>* neighbors, int numNeighbors)
{
    Vector2 steerForce = Vector2();
    int neighborCount = 0;
    Boid* neighbor;
    Vector2 deltaPosition;

    for (int i = 0; i < numNeighbors; i++) {
        neighbor = neighbors->at(i);
        deltaPosition = neighbor->position - position;
        float d = deltaPosition.length();
        if (neighbor != this && d > 0 && d < perceptionRadius) {
            steerForce += deltaPosition / (d * d);
            neighborCount++;
        }
    }
    if (neighborCount > 0) {
        steerForce /= neighborCount;
        steerForce = (steerForce.getNormalized() * velocityMax) - velocity;
        steerForce.clampLength(maxAddableForce);
    }

    return steerForce;
}

Vector2 Boid::steerTowardsNeighborAverage(vector<Boid*>* neighbors, int numNeighbors)
{
    Vector2 steerForce = Vector2();
    int neighborCount = 0;
    Boid* neighbor;
    Vector2 deltaPosition;

    for (int i = 0; i < numNeighbors; i++) {
        neighbor = neighbors->at(i);
        float d = (neighbor->position - position).length();
        if (neighbor != this && d < perceptionRadius) {
            steerForce += neighbor->position;
            neighborCount++;
        }
    }
    if (neighborCount > 0) {
        steerForce /= neighborCount;
        steerForce -= position;
        steerForce = (steerForce.getNormalized() * velocityMax) - velocity;
        steerForce.clampLength(maxAddableForce);
    }
    
    return steerForce;
}

void Boid::flock()
{
    vector<Boid*>* quadTreeNeighbors = new vector<Boid*>;
    Vector2 sightOffset = Vector2(perceptionRadius/2, perceptionRadius/2);
    ((QuadTree*)quadTree)->queryRegionForElements(quadTreeNeighbors, position - sightOffset, position + sightOffset);

    int numNeighbors = quadTreeNeighbors->size();
    Vector2 alignment = alignWithNeighbors(quadTreeNeighbors, numNeighbors);
    Vector2 cohesion = steerTowardsNeighborAverage(quadTreeNeighbors, numNeighbors);
    Vector2 separation = separateFromNeighbors(quadTreeNeighbors, numNeighbors);
    //cout << alignment.x << ": " << alignment.y << "\n";
    //cout << cohesion.x << ": " << cohesion.y << "\n";
    //cout << separation.x << ": " << separation.y << "\n";
    //alignment.mult(alignSlider.value());
    //cohesion.mult(cohesionSlider.value());
    //separation.mult(separationSlider.value());

    acceleration += alignment * 0.3;
    acceleration += cohesion * 0.8;
    acceleration -= separation * 0.8;

    delete quadTreeNeighbors;
}

void Boid::step()
{
    //position = (position + Vector2(1, 1)) % Vector2(100, 100);
    position += velocity;
    velocity += acceleration;
    velocity.clampLength(velocityMax);
    acceleration *= 0;

    float buffer = 0;
    detectEdges(Vector2(buffer, buffer), Vector2(800 - buffer, 800 - buffer));
}

void Boid::draw(sf::RenderWindow* window, const Vector2& offset)
{
    //cout << position.x + offset.x << ", " << position.y + offset.y << "\n";
    drawShape.setPosition(sf::Vector2f(position.x + offset.x, position.y + offset.y));

    window->draw(drawShape);
}