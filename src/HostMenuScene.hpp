#pragma once
#include "Scene.hpp"
#include "Button.hpp"

class HostMenuScene : public Scene
{
public:
    HostMenuScene(sf::RenderWindow&);
    
    void update(const Input&) override;
    void draw() const override;
    
private:
    void hostServer();
    
    sf::Text m_waitText;
    Button m_backButton;
};