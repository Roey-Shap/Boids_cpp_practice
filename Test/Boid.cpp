
#include "Boid.h"

using namespace std;

int Boid::boidGUIDCounter = 0;


Boid::Boid(Vector2 spawnPosition, BoidType boidType) : BoidRepeller(spawnPosition)
{
    id = Boid::boidGUIDCounter++;
    position = spawnPosition;
    prevPosition = spawnPosition;
    velocity = Vector2(0, 0);
    acceleration = Vector2(0, 0);
    perceptionRadius = 200.0;
    velocityMax = 3.0;
    maxAddableForce = 0.075;
    renderDebugText = false;

    quadTree = NULL;

    this->boidType = boidType;
}

Boid::Boid() : BoidRepeller()
{
    Boid(Vector2(), BoidType::Team1);
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

Vector2 Boid::getVelocity()
{
    return Vector2(velocity.x, velocity.y);
}

void Boid::detectEdges(Vector2 boundsMin, Vector2 boundsMax)
{
    position = boundsMin + (position % boundsMax);   
}


Vector2 Boid::alignWithNeighbors(vector<BoidRepeller*>* neighbors, int numNeighbors)
{
    Vector2 steerForce = Vector2();
    float totalNeighborWeight = 0;
    float neighborWeight = 0;
    BoidRepeller* neighbor;

    for (int i = 0; i < numNeighbors; i++) {
        neighbor = neighbors->at(i);
        /*float d = (neighbor->position - position).length();*/
        if (neighbor != this/*&& d < perceptionRadius*/) {
            neighborWeight = getInfluenceFromNeighbor(neighbor);
            steerForce += (neighbor->velocity) * neighborWeight;
            totalNeighborWeight += neighborWeight;
        }
    }
    if (totalNeighborWeight > 0) {
        steerForce /= totalNeighborWeight;
        steerForce = (steerForce.getNormalized() * velocityMax) - velocity;
        steerForce.clampLength(maxAddableForce);
    }

    return steerForce;
}

Vector2 Boid::separateFromNeighbors(vector<BoidRepeller*>* neighbors, int numNeighbors)
{
    Vector2 steerForce = Vector2();
    float totalNeighborWeight = 0;
    float neighborWeight = 0;
    BoidRepeller* neighbor;
    Vector2 deltaPosition;

    for (int i = 0; i < numNeighbors; i++) {
        neighbor = neighbors->at(i);
        deltaPosition = neighbor->position - position;
        float d = deltaPosition.length();
        if (neighbor != this && d > 0 /*&& d < perceptionRadius*/ /*&& neighbor->getBoidType() == boidType*/) {
            neighborWeight = abs(getInfluenceFromNeighbor(neighbor));
            steerForce += deltaPosition * neighborWeight / (d * d);
            totalNeighborWeight += neighborWeight;
        }
    }
    if (totalNeighborWeight > 0) {
        steerForce /= totalNeighborWeight;
        steerForce = (steerForce.getNormalized() * velocityMax) - velocity;
        steerForce.clampLength(maxAddableForce);
    }

    return steerForce;
}

Vector2 Boid::steerTowardsNeighborAverage(vector<BoidRepeller*>* neighbors, int numNeighbors)
{
    Vector2 steerForce = Vector2();
    int totalNeighborWeight = 0;  //totalNeighborWeight
    float neighborWeight = 0;
    BoidRepeller* neighbor;

    for (int i = 0; i < numNeighbors; i++) {
        neighbor = neighbors->at(i);
        //float d = (neighbor->position - position).length();
        if (neighbor != this/*&& d < perceptionRadius*/) {
            neighborWeight = getInfluenceFromNeighbor(neighbor);
            steerForce += (neighbor->position) * neighborWeight;
            totalNeighborWeight += neighborWeight;
        }
    }
    if (totalNeighborWeight > 0) {
        steerForce /= totalNeighborWeight;
        steerForce -= position;
        steerForce = (steerForce.getNormalized() * velocityMax) - velocity;
        steerForce.clampLength(maxAddableForce);
    }
    
    return steerForce;
}

void Boid::flock()
{
    vector<BoidRepeller*>* quadTreeNeighbors = new vector<BoidRepeller*>;
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

    float factor = 2.0;
    acceleration += alignment * 0.3 * factor;
    acceleration += cohesion * 0.8 * factor;
    acceleration -= separation * 0.85 * factor;

    delete quadTreeNeighbors;
}

void Boid::step()
{
    prevPosition = *(position.copy());
    position += velocity * Simulation::deltaTime;
    velocity += acceleration * Simulation::deltaTime * Simulation::deltaTime;
    velocity.clampLength(velocityMax);
    acceleration *= 0;

    float buffer = 0;
    detectEdges(Vector2(buffer, buffer), Vector2(800 - buffer, 800 - buffer));

    /*bool mouseIsClose = (Vector2(sf::Mouse::getPosition()) - position).length() <= 10;
    setRenderDebugText(mouseIsClose);*/
}

void Boid::setRenderDebugText(bool active)
{
    renderDebugText = active;
}

void Boid::draw(sf::RenderWindow* window, const Vector2& offset)
{
    //cout << position.x + offset.x << ", " << position.y + offset.y << "\n";
    //drawShape.setPosition(sf::Vector2f(position.x + offset.x, position.y + offset.y));

    // define a 100x100 square, red, with a 10x10 texture mapped on it

    float headingAngle = (position - prevPosition).angleDegrees();

    for (int i = 0; i < 3; i++)
    {
        float rad = 3 * (i == 0 ? 5 : 3);
        int colorIndex = (int)color;
        sf::Vertex vertex = getRegularPolygonVertex(position + offset, rad, headingAngle, i, 3, Boid::boidColors[colorIndex]);
        vertices[i] = vertex;
    }

    window->draw(vertices, 3, sf::Triangles);
}