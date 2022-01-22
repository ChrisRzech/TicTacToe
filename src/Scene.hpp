#pragma once
#include <sfml/Graphics/RenderWindow.hpp>
#include "Input.hpp"

class Scene
{
public:
    Scene(sf::RenderWindow&);
    virtual ~Scene() = default;
    
    virtual void update(const Input&) = 0;
    virtual void draw() const = 0;

protected:
    sf::RenderWindow& m_window;
};