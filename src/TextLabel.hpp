#pragma once
#include <sfml/Graphics/Text.hpp>
#include <sfml/Graphics/RectangleShape.hpp>
#include <sfml/Graphics/RenderTarget.hpp>

class TextLabel : public sf::Drawable
{
public:
    TextLabel() = default;
    TextLabel(const sf::Font&, const std::string& text, unsigned int characterSize = 30);
    
    void setFont(const sf::Font&);
    const sf::Font* getFont() const;
    
    void setText(const std::string&);
    std::string getText() const;
    
    void setCharacterSize(unsigned int);
    unsigned int getCharacterSize() const;
    
    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f&);
    void setPositionCenter(float x, float y);
    void setPositionCenter(const sf::Vector2f&);
    sf::Vector2f getPosition() const;
    
    void setBackgroundSize(float x, float y);
    void setBackgroundSize(const sf::Vector2f&);
    sf::Vector2f getBackgroundSize() const;
    
    void setBorderThickness(float);
    float getBorderThickness() const;
    
    void setTextColor(const sf::Color&);
    sf::Color getTextColor() const;
    
    void setBackgroundColor(const sf::Color&);
    sf::Color getBackgroundColor() const;
    
    void setBorderColor(const sf::Color&);
    sf::Color getBorderColor() const;
    
    void draw(sf::RenderTarget&, sf::RenderStates) const override;
    
private:
    sf::Text m_text;
    sf::RectangleShape m_box;
};