#pragma once
#include "Input.hpp"
#include "TextLabel.hpp"

class Button : public TextLabel
{
public:
    Button(const sf::Font&, const std::string& text);
    
    bool isPressed(const Input&) const;
    
    void draw(sf::RenderTarget&, sf::RenderStates states) const override;
};