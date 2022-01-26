#pragma once
#include "Scene.hpp"
#include "Button.hpp"
#include "TextField.hpp"

class JoinMenuScene : public Scene
{
public:
    JoinMenuScene(sf::RenderWindow&);
    
    void enter() override;
    void update(const Input&) override;
    void exit() override;
    
    void draw() const override;
    
private:
    void joinServer();
    
    TextField m_addressField;
    Button m_connectButton;
    Button m_backButton;
};