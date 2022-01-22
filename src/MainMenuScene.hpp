#pragma once
#include "Scene.hpp"
#include "Button.hpp"

class MainMenuScene : public Scene
{
public:
    MainMenuScene(sf::RenderWindow&);
    
    void update(const Input&) override;
    void draw() const override;
    
private:
    sf::Text m_titleText;
    Button m_hostButton;
    Button m_joinButton;
};