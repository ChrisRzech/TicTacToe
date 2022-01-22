#include "Button.hpp"
#include "Resources.hpp"

static const unsigned int padding = 10;

Button::Button(const std::string& text)
    : Button{sf::Vector2f{0, 0}, text}
{
    
}

Button::Button(sf::Vector2f position, const std::string& text)
{
    /* Text */
    m_text.setFont(Resources::getResources().font);
    m_text.setString(text);
    
    /* Box */
    m_box.setFillColor(sf::Color::Transparent);
    m_box.setOutlineColor(sf::Color::White);
    m_box.setOutlineThickness(1);
    m_box.setSize(sf::Vector2f{m_text.getLocalBounds().width + padding * 2, m_text.getLocalBounds().height + padding * 2});
    
    setPosition(position);
}

void Button::setPosition(float x, float y)
{
    /* NOTE:
       sf::Text has "extra" space at the top of normal characters. This is for
       special charaters like capital letters with accents. In order to align up
       the positions, subtract the top and left values of the bounding box from
       the position. */
    
    m_box.setPosition(x, y);
    m_text.setPosition(x - m_text.getLocalBounds().left + padding, y - m_text.getLocalBounds().top + padding);
}

void Button::setPosition(const sf::Vector2f& position)
{
    setPosition(position.x, position.y);
}

void Button::setPositionCenter(float x, float y)
{
    setPosition(x - m_box.getLocalBounds().width / 2, y - m_box.getLocalBounds().height / 2);
}

void Button::setPositionCenter(const sf::Vector2f& position)
{
    setPositionCenter(position.x, position.y);
}

sf::Vector2f Button::getPosition() const
{
    return m_box.getPosition();
}

bool Button::isPressed(const Input& input) const
{
    return input.isPressed(Input::Key::LeftClick) &&
           m_box.getGlobalBounds().contains(static_cast<sf::Vector2f>(input.mousePosition()));
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_box, states);
    target.draw(m_text, states);
}