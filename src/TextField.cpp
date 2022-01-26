#include "TextField.hpp"
#include "Resources.hpp"

static const int INITIAL_TEXT_PADDING_X = 5;
static const int CURSOR_PADDING_Y = 3;

TextField::TextField()
    : TextField{sf::Vector2f{0, 0}, sf::Vector2f{100, 30}, 30}
{
    
}

TextField::TextField(const sf::Vector2f& position, const sf::Vector2f& size, int maxCharacters)
    : m_cursor{sf::Lines, 2}
{
    m_text.setFont(Resources::getResources().font);
    
    m_box.setFillColor(sf::Color::Transparent);
    m_box.setOutlineColor(sf::Color::White);
    m_box.setOutlineThickness(1);
    
    setSize(size);
    setPosition(position);
    setMaxCharacters(maxCharacters);
    setCursorBlinkSpeed(sf::seconds(0.5));
    m_cursorPosition = 0;
    
    m_isFocused = false;
}

void TextField::setPosition(float x, float y)
{
    m_box.setPosition(x, y);
    m_text.setPosition(x + INITIAL_TEXT_PADDING_X, y);
    setCursorPosition(0);
}

void TextField::setPosition(const sf::Vector2f& position)
{
    setPosition(position.x, position.y);
}

void TextField::setPositionCenter(float x, float y)
{
    setPosition(x - m_box.getSize().x / 2, y - m_box.getSize().y / 2);
}

void TextField::setPositionCenter(const sf::Vector2f& position)
{
    setPositionCenter(position.x, position.y);
}

sf::Vector2f TextField::getPosition() const
{
    return m_box.getPosition();
}

void TextField::setSize(float x, float y)
{
    m_box.setSize(sf::Vector2f{x, y});
}

void TextField::setSize(const sf::Vector2f& size)
{
    m_box.setSize(size);
}

sf::Vector2f TextField::getSize() const
{
    return m_box.getSize();
}

std::string TextField::getText() const
{
    return m_text.getString();;
}

void TextField::setCursorBlinkSpeed(const sf::Time& time)
{
    m_cursorBlinkSpeed = time;
}

sf::Time TextField::getCursorBlinkSpeed() const
{
    return m_cursorBlinkSpeed;
}

void TextField::setMaxCharacters(unsigned int newMax)
{
    m_maxCharacters = newMax;
}

unsigned int TextField::getMaxCharacters() const
{
    return m_maxCharacters;
}

void TextField::setFocus(bool focus)
{
    m_isFocused = focus;
}

bool TextField::isFocused() const
{
    return m_isFocused;
}

void TextField::clear()
{
    m_text.setString("");
    m_cursorPosition = 0;
    m_cursorClock.restart();
}

void TextField::update(const Input& input)
{
    if(input.isPressed(Input::Key::LeftClick))
    {
        m_isFocused = m_box.getGlobalBounds().contains(static_cast<sf::Vector2f>(input.mousePosition()));
        
        /* Show cursor if it is focused just now */
        if(m_isFocused)
            enableCursor(true);
    }
    
    if(m_isFocused)
    {
        if(m_text.getString().getSize() < m_maxCharacters)
        {
            std::string keyPressed;
            
            if(input.isPressed(Input::Key::Num0))
                keyPressed = "0";
            else if(input.isPressed(Input::Key::Num1))
                keyPressed = "1";
            else if(input.isPressed(Input::Key::Num2))
                keyPressed = "2";
            else if(input.isPressed(Input::Key::Num3))
                keyPressed = "3";
            else if(input.isPressed(Input::Key::Num4))
                keyPressed = "4";
            else if(input.isPressed(Input::Key::Num5))
                keyPressed = "5";
            else if(input.isPressed(Input::Key::Num6))
                keyPressed = "6";
            else if(input.isPressed(Input::Key::Num7))
                keyPressed = "7";
            else if(input.isPressed(Input::Key::Num8))
                keyPressed = "8";
            else if(input.isPressed(Input::Key::Num9))
                keyPressed = "9";
            else if(input.isPressed(Input::Key::Period))
                keyPressed = ".";
            else
            {
                if(input.isPressed(Input::Key::Backspace) && m_cursorPosition != 0)
                {
                    sf::String string = m_text.getString();
                    string.erase(m_cursorPosition - 1);
                    m_text.setString(string);
                    
                    setCursorPosition(--m_cursorPosition);
                }
            }
            
            if(!keyPressed.empty())
            {
                m_text.setString(m_text.getString() + keyPressed);
                setCursorPosition(++m_cursorPosition);
            }
        }
        
        /* Moving cursor with arrow keys */
        if(input.isPressed(Input::Key::Left))
        {
            if(m_cursorPosition > 0)
                setCursorPosition(--m_cursorPosition);
            
            enableCursor(true);
        }
        else if(input.isPressed(Input::Key::Right))
        {
            if(m_cursorPosition < m_text.getString().getSize())
                setCursorPosition(++m_cursorPosition);
            
            enableCursor(true);
        }
        
        /* Blink cursor */
        if(m_cursorClock.getElapsedTime() >= m_cursorBlinkSpeed)
        {
            m_cursorClock.restart();
            m_showCursor = !m_showCursor;
        }
    }
    else
    {
        enableCursor(false);
    }
}

void TextField::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_text, states);
    target.draw(m_box, states);
    if(m_isFocused && m_showCursor)
        target.draw(m_cursor, states);
}

void TextField::setCursorPosition(int index)
{
    sf::Vector2f textPosition = m_text.findCharacterPos(index);
    float extraSpaceLeft = m_text.getLocalBounds().left;
    extraSpaceLeft = 0;
    
    m_cursor[0] = sf::Vector2f{textPosition.x + extraSpaceLeft, m_box.getPosition().y + CURSOR_PADDING_Y};
    m_cursor[1] = sf::Vector2f{textPosition.x + extraSpaceLeft, m_box.getPosition().y + m_box.getSize().y - CURSOR_PADDING_Y};
}

void TextField::enableCursor(bool enable)
{
    m_showCursor = enable;
    m_cursorClock.restart();
}