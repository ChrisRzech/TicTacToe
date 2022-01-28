#pragma once
#include "Scene.hpp"
#include "Button.hpp"
#include "TextLabel.hpp"
#include "TextField.hpp"
#include <sfml/Network/IpAddress.hpp>

class JoinMenuScene : public Scene
{
public:
    JoinMenuScene(sf::RenderWindow&);
    
    void enter() override;
    void update(const Input&) override;
    void exit() override;
    
    void draw() const override;
    
private:
    TextLabel m_label;
    TextField m_addressField;
    Button m_connectButton;
    Button m_backButton;
    
    bool m_tryConnecting;
    sf::IpAddress m_address;
};