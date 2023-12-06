#pragma once

#include <SFML/Graphics.hpp>

class TriangleShape : public sf::Shape
{
public:

    TriangleShape(float radius) : m_radius(radius)
    {
        update();
    }

    void setRadius(float radius)
    {
        m_radius = radius;
        update();
    }

    float getRadius() const
    {
        return m_radius;
    }

    size_t getPointCount() const
    {
        return 30; // fixed, but could be an attribute of the class if needed
    }

    sf::Vector2f getPoint(unsigned int index) const
    {
        static const float pi = 3.141592654f;

        float angle = index * 2 * pi / getPointCount() - pi / 2;
        float x = std::cos(angle) * m_radius;
        float y = std::sin(angle) * m_radius;

        return sf::Vector2f(x, y);
    }

private:

    float m_radius;
};