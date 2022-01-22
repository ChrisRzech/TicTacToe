#pragma once
#include "Input.hpp"
#include <sfml/Graphics/Text.hpp>
#include <sfml/Graphics/RectangleShape.hpp>
#include <sfml/Graphics/RenderTarget.hpp>

class Button : public sf::Drawable
{
public:
    Button(const std::string& text);
    Button(sf::Vector2f position, const std::string& text);
    
    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f&);
    void setPositionCenter(float x, float y);
    void setPositionCenter(const sf::Vector2f&);
    sf::Vector2f getPosition() const;
    
    bool isPressed(const Input&) const;
    
    void draw(sf::RenderTarget&, sf::RenderStates states) const override;

private:
    sf::Text m_text;
    sf::RectangleShape m_box;
};