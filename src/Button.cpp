#include "Button.hpp"

Button::Button(const sf::Font& font, const std::string& text)
    : TextLabel{font, text}
{
    
}

bool Button::isPressed(const Input& input) const
{
    sf::FloatRect globalBounds(getPosition(), getBackgroundSize());
    
    return input.isPressed(Input::Key::LeftClick) &&
           globalBounds.contains(static_cast<sf::Vector2f>(input.mousePosition()));
}