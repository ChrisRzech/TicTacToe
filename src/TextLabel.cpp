#include "TextLabel.hpp"

TextLabel::TextLabel(const sf::Font& font, const std::string& text, unsigned int characterSize)
    : m_text{text, font, characterSize}
{
    
}

void TextLabel::setFont(const sf::Font& font)
{
    m_text.setFont(font);
}

const sf::Font* TextLabel::getFont() const
{
    return m_text.getFont();
}

void TextLabel::setText(const std::string& text)
{
    m_text.setString(text);
}

std::string TextLabel::getText() const
{
    return m_text.getString();
}

void TextLabel::setCharacterSize(unsigned int size)
{
    m_text.setCharacterSize(size);
}

unsigned int TextLabel::getCharacterSize() const
{
    return m_text.getCharacterSize();
}

void TextLabel::setPosition(float x, float y)
{
    sf::Vector2f halfTextSize(m_text.getLocalBounds().width / 2, m_text.getLocalBounds().height / 2);
    sf::Vector2f halfBoxSize = m_box.getSize() / 2.f;
    m_text.setPosition(x + halfBoxSize.x - halfTextSize.x - m_text.getLocalBounds().left, y + halfBoxSize.y - halfTextSize.y - m_text.getLocalBounds().top);
    m_box.setPosition(x, y);
}

void TextLabel::setPosition(const sf::Vector2f& position)
{
    setPosition(position.x, position.y);
}

void TextLabel::setPositionCenter(float x, float y)
{
    sf::Vector2f halfBoxSize = m_box.getSize() / 2.f;
    setPosition(x - halfBoxSize.x, y - halfBoxSize.y);
}

void TextLabel::setPositionCenter(const sf::Vector2f& position)
{
    setPositionCenter(position.x, position.y);
}

sf::Vector2f TextLabel::getPosition() const
{
    return m_box.getPosition();
}

void TextLabel::setSize(float x, float y)
{
    setSize(sf::Vector2f{x, y});
}

void TextLabel::setSize(const sf::Vector2f& size)
{
    m_box.setSize(size);
}

sf::Vector2f TextLabel::getSize() const
{
    return m_box.getSize();
}

void TextLabel::setBorderThickness(unsigned int thickness)
{
    m_box.setOutlineThickness(thickness);
}

unsigned int TextLabel::getBorderThickness() const
{
    return m_box.getOutlineThickness();
}

void TextLabel::setTextColor(const sf::Color& color)
{
    m_text.setFillColor(color);
}

sf::Color TextLabel::getTextColor() const
{
    return m_text.getFillColor();
}

void TextLabel::setBackgroundColor(const sf::Color& color)
{
    m_box.setFillColor(color);
}

sf::Color TextLabel::getBackgroundColor() const
{
    return m_box.getFillColor();
}

void TextLabel::setBorderColor(const sf::Color& color)
{
    m_box.setOutlineColor(color);
}

sf::Color TextLabel::getBorderColor() const
{
    return m_box.getOutlineColor();
}

void TextLabel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_box, states);
    target.draw(m_text, states);
}