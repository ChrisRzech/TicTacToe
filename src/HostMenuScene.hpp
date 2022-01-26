#pragma once
#include "Scene.hpp"
#include "Button.hpp"
#include <sfml/Network/Socket.hpp>
#include <sfml/Network/TcpListener.hpp>

class HostMenuScene : public Scene
{
public:
    HostMenuScene(sf::RenderWindow&);
    
    void enter() override;
    void update(const Input&) override;
    void exit() override;
    
    void draw() const override;
    
private:
    void hostServer();
    
    sf::Text m_waitText;
    Button m_backButton;
    
    sf::TcpListener m_listener;
    sf::Socket::Status m_socketStatus;
};