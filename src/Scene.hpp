#pragma once
#include <sfml/Graphics.hpp>
#include "Input.hpp"

class Scene
{
public:
    Scene(sf::RenderWindow&);
    
    virtual void update(const Input&) = 0;
    virtual void draw() const = 0;

protected:
    sf::RenderWindow& m_window;
};