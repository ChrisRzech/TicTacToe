#pragma once
#include "Scene.hpp"
#include "Button.hpp"

class JoinMenuScene : public Scene
{
public:
    JoinMenuScene(sf::RenderWindow&);
    
    void update(const Input&) override;
    void draw() const override;
    
private:
    void joinServer();
    
    std::string m_addressString;
    sf::Text m_addressText;
    sf::RectangleShape m_addressBox;
    Button m_connectButton;
    Button m_backButton;
};