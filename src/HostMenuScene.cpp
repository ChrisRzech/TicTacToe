#include "HostMenuScene.hpp"
#include "Resources.hpp"
#include "SceneManager.hpp"
#include <sfml/Network/TcpSocket.hpp>

extern unsigned short g_port;
extern sf::TcpSocket g_socket;
extern bool g_isHost;

HostMenuScene::HostMenuScene(sf::RenderWindow& window)
    : Scene{window},
      m_waitLabel{Resources::get().font, "Waiting for opponent..."},
      m_backButton{Resources::get().font, "Back"}
{
    sf::Vector2u windowSize = window.getSize();

    m_waitLabel.setPositionCenter(windowSize.x * 0.5, windowSize.y * 0.45);
    
    m_backButton.setBackgroundSize(80, 40);
    m_backButton.setPositionCenter(windowSize.x * 0.5, windowSize.y * 0.55);
    m_backButton.setBorderThickness(1);
    m_backButton.setTextColor(sf::Color::White);
    m_backButton.setBackgroundColor(sf::Color::Transparent);
    m_backButton.setBorderColor(sf::Color::White);
}

void HostMenuScene::enter()
{
    m_listener.listen(g_port);
    m_selector.add(m_listener);
}

void HostMenuScene::update(const Input& input)
{
    if(m_backButton.isPressed(input))
    {
        SceneManager::changeScene("MainMenu");
    }
    
    if(m_selector.wait(sf::milliseconds(10)))
    {
        sf::Socket::Status status = m_listener.accept(g_socket);
        if(status == sf::Socket::Status::Done)
        {
            g_isHost = true;
            SceneManager::changeScene("Game");
        }
    }
}

void HostMenuScene::exit()
{
    m_listener.close();
    m_selector.clear();
}

void HostMenuScene::draw() const
{
    m_window.draw(m_waitLabel);
    m_window.draw(m_backButton);
}