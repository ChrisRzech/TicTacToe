#pragma once
#include "Scene.hpp"
#include "Button.hpp"
#include "TextLabel.hpp"
#include <sfml/Network/SocketSelector.hpp>
#include <sfml/Network/TcpListener.hpp>

class HostMenuScene : public Scene
{
public:
    HostMenuScene(const sf::Vector2u& size);
    
    void enter() override;
    void update(const Input&) override;
    void exit() override;
    
    void draw(sf::RenderTarget&, sf::RenderStates) const override;
    
private:
    TextLabel m_waitLabel;
    Button m_backButton;
    
    sf::SocketSelector m_selector;
    sf::TcpListener m_listener;
};