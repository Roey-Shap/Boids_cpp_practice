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

using namespace std;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define NUM_BOIDS 2000

int main()
{
    sf::Font font_OpenSans;
    if (!font_OpenSans.loadFromFile(FONT_OPEN_SANS_PATH))
    {
        cout << "Couldn't find font :(";
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Boids", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    QuadTree* quadTree = NULL;

    srand((unsigned)time(NULL));

    Boid* boids[NUM_BOIDS];
    const Vector2& spawnBoundsMin = Vector2(0, 0);
    const Vector2& spawnBoundsMax = Vector2(WINDOW_WIDTH, WINDOW_HEIGHT);

    int numExistingBoids = 0;

    const Vector2& offset = Vector2(0, 0);
    float fps;
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

            if (event.type == sf::Event::MouseButtonReleased)
            {
                int randNumBoids = randomRange(10, 20);
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                cout << mousePos.x << ":" << mousePos.y;
                Vector2* mousePosVec = new Vector2(mousePos.x, mousePos.y);
                Boid* spawnedBoid;

                for (int i = 0; i < randNumBoids; i++)
                {
                    Vector2 ranPos = *mousePosVec + Vector2::Vector2Random(Vector2(-25), Vector2(25));
                    spawnedBoid = new Boid(ranPos);
                    boids[numExistingBoids + i] = spawnedBoid;
                    spawnedBoid->detectEdges(Vector2(0, 0), Vector2(WINDOW_WIDTH, WINDOW_HEIGHT));
                }

                numExistingBoids += randNumBoids;
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
            quadTree->insertElement(boids[i]);
        }
        


        // update loop
        for (int i = 0; i < numExistingBoids; i++)
        {
            boids[i]->flock();
            boids[i]->step();
        }


        quadTree->draw(&window, offset);

        // draw loop
        for (int i = 0; i < numExistingBoids; i++)
        {
            boids[i]->draw(&window, offset);
        }


        // Debug information
        currentTime = clock.getElapsedTime();
        fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds()); // the asSeconds returns a float
        std::cout << "fps = " << floor(fps) << std::endl; // flooring it will make the frame rate a rounded number
        previousTime = currentTime;

        sf::Text text;
        text.setFont(font_OpenSans);
        text.setString("FPS: " + to_string(int(fps)) + "\n" + 
                        "# Boids: " + to_string(numExistingBoids) + "\n");
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setPosition(0, 0);
        window.draw(text);

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
