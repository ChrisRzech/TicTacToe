#pragma once
#include "Input.hpp"
#include <sfml/Graphics/RenderWindow.hpp>

class Scene
{
public:
    Scene(sf::RenderWindow&);
    virtual ~Scene() = default;
    
    virtual void enter() = 0;
    virtual void update(const Input&) = 0;
    virtual void exit() = 0;
    
    virtual void draw() const = 0;

protected:
    sf::RenderWindow& m_window;
};