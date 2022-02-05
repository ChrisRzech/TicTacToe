#pragma once
#include "Scene.hpp"
#include "Button.hpp"
#include "TextLabel.hpp"

class MainMenuScene : public Scene
{
public:
    MainMenuScene(const sf::Vector2u& size);
    
    void enter() override;
    void update(const Input&) override;
    void exit() override;
    
    void draw(sf::RenderTarget&, sf::RenderStates) const override;
    
private:
    TextLabel m_titleLabel;
    Button m_hostButton;
    Button m_joinButton;
};