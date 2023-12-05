#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include "Boid.h"
#include "QuadTree.h"
#include "Vector2.h"
#include "CustomMath.h"
#include <random>

using namespace std;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define NUM_BOIDS 1000

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Boids", sf::Style::Titlebar | sf::Style::Close);
    //sf::Window::SetFramerateLimit(60
    window.setFramerateLimit(60);

    QuadTree* quadTree = NULL;

    srand((unsigned)time(NULL));

    Boid* boids[NUM_BOIDS];
    const Vector2& spawnBoundsMin = Vector2(0, 0);
    const Vector2& spawnBoundsMax = Vector2(WINDOW_WIDTH, WINDOW_HEIGHT);

    for (int i = 0; i < NUM_BOIDS; i++)
    {
        boids[i] = new Boid(Vector2::Vector2Random(spawnBoundsMin, spawnBoundsMax));
        //cout << i << ": " << boids[i]->getPosition().x << ", " << boids[i]->getPosition().y << "\n";
    }

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
                window.close();
        }

        window.clear();

        if (quadTree != NULL)
        {
            delete quadTree;
        }

        quadTree = new QuadTree(new Vector2(0, 0), new Vector2(WINDOW_WIDTH, WINDOW_HEIGHT));

        for (int i = 0; i < NUM_BOIDS; i++)
        {
            quadTree->insertElement(boids[i]);
        }
        


        // update loop
        for (int i = 0; i < NUM_BOIDS; i++)
        {
            boids[i]->flock();
            boids[i]->step();
        }


        quadTree->draw(&window, offset);

        // draw loop
        for (int i = 0; i < NUM_BOIDS; i++)
        {
            boids[i]->draw(&window, offset);
        }

        /*
        sf::Color fillColor(0, 0, 255, 255);
        sf::RectangleShape drawRect(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        drawRect.setPosition(0, 0);
        drawRect.setFillColor(fillColor);
        drawRect.setOutlineColor(sf::Color::White);
        drawRect.setOutlineThickness(1);
        window.draw(drawRect);
        */

        window.display();

        currentTime = clock.getElapsedTime();
        fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds()); // the asSeconds returns a float
        std::cout << "fps =" << floor(fps) << std::endl; // flooring it will make the frame rate a rounded number
        previousTime = currentTime;
    }

    // call each boid's destructor
    for (int i = 0; i < NUM_BOIDS; i++)
    {
        delete boids[i];
    }

    delete quadTree;

    return 0;
}

/*
int main()
{
    sf::Window window(sf::VideoMode(640, 480), "Boids", sf::Style::Titlebar | sf::Style::Close);
    sf::Event event;


    printf("Test!");
    return 0;
}
*/