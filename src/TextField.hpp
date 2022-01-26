#pragma once
#include "Input.hpp"
#include <sfml/Graphics/Text.hpp>
#include <sfml/Graphics/RectangleShape.hpp>
#include <sfml/Graphics/RenderTarget.hpp>
#include <sfml/Graphics/VertexArray.hpp>
#include <sfml/System/Clock.hpp>

class TextField : public sf::Drawable
{
public:
    TextField();
    TextField(const sf::Vector2f& position, const sf::Vector2f& size, int maxCharacters);
    
    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f&);
    void setPositionCenter(float x, float y);
    void setPositionCenter(const sf::Vector2f&);
    sf::Vector2f getPosition() const;
    
    void setSize(float x, float y);
    void setSize(const sf::Vector2f&);
    sf::Vector2f getSize() const;
    
    std::string getText() const;
    
    void setCursorBlinkSpeed(const sf::Time&);
    sf::Time getCursorBlinkSpeed() const;
    
    void setMaxCharacters(unsigned int);
    unsigned int getMaxCharacters() const;
    
    void setFocus(bool);
    bool isFocused() const;
    
    void update(const Input&);
    void draw(sf::RenderTarget&, sf::RenderStates states) const override;
    
private:
    void setCursorPosition(int index);
    void enableCursor(bool enable);
    
    sf::Text m_text;
    sf::RectangleShape m_box;
    sf::VertexArray m_cursor;
    sf::Clock m_cursorClock;
    sf::Time m_cursorBlinkSpeed;
    bool m_showCursor;
    unsigned int m_cursorPosition;
    unsigned int m_maxCharacters;
    bool m_isFocused;
};