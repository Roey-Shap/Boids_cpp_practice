#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>

#include <random>

#include "Boid.h"
#include "QuadTree.h"
#include "Vector2.h"
#include "CustomMath.h"
#include "Fonts.h"
#include "SFML_Text.h"

#include "Simulation.h"

using namespace std;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define NUM_BOIDS 2000


int main()
{
    #ifdef _DEBUG
        Simulation::showDebugInfo = true;
    #endif

    sf::Font font_OpenSans;
    if (!font_OpenSans.loadFromFile(FONT_OPEN_SANS_PATH))
    {
        cout << "Couldn't find font :(";
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Boids", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    float deltaTimeMouseIncrement = 0.1;
    float deltaTimeMin = 0;

    QuadTree* quadTree = NULL;

    srand((unsigned)time(NULL));

    Boid* boids[NUM_BOIDS];
    const Vector2& spawnBoundsMin = Vector2(0, 0);
    const Vector2& spawnBoundsMax = Vector2(WINDOW_WIDTH, WINDOW_HEIGHT);

    int numExistingBoids = 0;

    Boid* nearestBoid;
    float bestDistanceToBoid = WINDOW_WIDTH * WINDOW_WIDTH;
    const Vector2& offset = Vector2(0, 0);
    float fps;
    static boidColor selectedBoidColor = (boidColor)0;
    sf::Clock clock = sf::Clock::Clock();
    sf::Time previousTime = clock.getElapsedTime();
    sf::Time currentTime;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                int randNumBoids = randomRange(5, 10);
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                cout << "Mouse pos: " << mousePos.x << ": " << mousePos.y << std::endl;
                Vector2* mousePosVec = new Vector2(mousePos.x, mousePos.y);
                Boid* spawnedBoid;

                for (int i = 0; i < randNumBoids; i++)
                {
                    Vector2 ranPos = *mousePosVec + Vector2::Vector2Random(Vector2(-25), Vector2(25));
                    spawnedBoid = new Boid(ranPos, (BoidType) selectedBoidColor);
                    int boidColorIndex = (int)selectedBoidColor;
                    if (selectedBoidColor == boidColor::NUM_ELEMENTS)
                    {
                        boidColorIndex = randomRange(0, boidColor::NUM_ELEMENTS);
                    }

                    spawnedBoid->color = (boidColor)boidColorIndex;
                    boids[numExistingBoids + i] = spawnedBoid;
                    spawnedBoid->detectEdges(Vector2(0, 0), Vector2(WINDOW_WIDTH, WINDOW_HEIGHT));
                }

                numExistingBoids += randNumBoids;
            }
            else if (event.type == sf::Event::MouseWheelMoved)
            {
                Simulation::deltaTime = roundTo(Simulation::deltaTime + event.mouseWheel.delta * deltaTimeMouseIncrement, deltaTimeMouseIncrement);
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                int keyCode = event.key.scancode;
                if (keyCode == sf::Keyboard::Scan::K)
                {
                    Simulation::showDebugInfo = !Simulation::showDebugInfo;
                }
                else if (keyCode <= sf::Keyboard::Scan::Num5 && keyCode >= sf::Keyboard::Scan::Num1)
                {
                    selectedBoidColor = (boidColor)(keyCode - sf::Keyboard::Scan::Num1);
                }
            }
        }

        window.clear();

        if (quadTree != NULL)
        {
            delete quadTree;
        }

        quadTree = new QuadTree(Vector2(0, 0), Vector2(WINDOW_WIDTH, WINDOW_HEIGHT));

        for (int i = 0; i < numExistingBoids; i++)
        {
            quadTree->insertElement((BoidRepeller*)boids[i]);
        }
        
        bestDistanceToBoid = WINDOW_WIDTH * WINDOW_WIDTH;
        nearestBoid = NULL;


        // update loop
        for (int i = 0; i < numExistingBoids; i++)
        {
            Boid* boid = boids[i];
            boid->flock();
            boid->step();
            float d = (Vector2(sf::Mouse::getPosition(window)) - boid->getPosition()).length();
            if (d <= bestDistanceToBoid)
            {
                bestDistanceToBoid = d;
                nearestBoid = boid;
            }
        }


        quadTree->draw(&window, offset);

        // draw loop
        for (int i = 0; i < numExistingBoids; i++)
        {
            boids[i]->draw(&window, offset);
        }


        // Debug information
        if (Simulation::showDebugInfo)
        {
            currentTime = clock.getElapsedTime();
            fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds()); // the asSeconds returns a float
            std::cout << "fps = " << floor(fps) << std::endl; // flooring it will make the frame rate a rounded number
            previousTime = currentTime;

            sf::String debugText = "FPS: " + to_string(int(fps)) + "\n" +
                "# Boids: " + to_string(numExistingBoids) + "\n" +
                "Timescale: " + to_string(Simulation::deltaTime) + "\n";

            SFML_Text::drawText(window, font_OpenSans, debugText,
                18, sf::Color::Magenta, Vector2(0, 0));
        }

        /*if (nearestBoid != NULL)
        {
            sf::String boidDebugText = "ID: " + to_string(nearestBoid->getId()) + "\n" +
                                       "Position: " + nearestBoid->getPosition().toString() + "\n" + 
                                       "Velocity: " + nearestBoid->getVelocity().toString() + "\n" +
                                       "Direction: " + to_string(nearestBoid->getVelocity().angleDegrees() * 180.f / 3.1415926535);
            SFML_Text::drawText(window, font_OpenSans, boidDebugText,
                                24, sf::Color::White, Vector2(100, 0));
        }*/

        sf::String selectedBoidColorText = "Selected Boid Color (1-4, 5=random): " + to_string((int)selectedBoidColor + 1);
        
        sf::Color selectedColorTextDisplayColor = Boid::boidColors[(int)selectedBoidColor];
        if (selectedBoidColor == boidColor::NUM_ELEMENTS)
        { 
            selectedColorTextDisplayColor = Boid::boidColors[(int)(clock.getElapsedTime().asSeconds() * 2.0) % (int)boidColor::NUM_ELEMENTS];
        }
        SFML_Text::drawText(window, font_OpenSans, selectedBoidColorText,
            24, selectedColorTextDisplayColor, Vector2(100, 0));

        window.display();
    }

    // call each boid's destructor
    for (int i = 0; i < numExistingBoids; i++)
    {
        delete boids[i];
    }

    delete quadTree;

    return 0;
}
