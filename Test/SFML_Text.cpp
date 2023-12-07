
#include "SFML_Text.h"

#include <SFML/Graphics.hpp>
#include "Vector2.h"

void SFML_Text::drawText(sf::RenderWindow& window, sf::Font font, sf::String text, int pixelSize, sf::Color color, Vector2 position)
{
    sf::Text textObject;
    textObject.setFont(font);
    textObject.setString(text);
    textObject.setCharacterSize(pixelSize);
    textObject.setFillColor(color);
    textObject.setPosition(position.x, position.y);
    window.draw(textObject);
}