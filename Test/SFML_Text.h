#pragma once

#include <SFML/Graphics.hpp>
#include "Vector2.h"

class SFML_Text
{
public:
    static void drawText(sf::RenderWindow& window, sf::Font font, sf::String text, int pixelSize, sf::Color color, Vector2 position);
};
