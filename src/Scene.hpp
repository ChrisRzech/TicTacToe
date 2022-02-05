#pragma once
#include "Input.hpp"
#include <sfml/Graphics/Drawable.hpp>

class Scene : public sf::Drawable
{
public:
    Scene(const sf::Vector2u& size);
    virtual ~Scene() = default;
    
    virtual void enter() = 0;
    virtual void update(const Input&) = 0;
    virtual void exit() = 0;
    
protected:
    sf::Vector2u m_size;
};