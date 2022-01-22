#include "JoinMenuScene.hpp"
#include "Resources.hpp"
#include "SceneManager.hpp"
#include <sfml/Network/TcpSocket.hpp>

extern sf::TcpSocket socket;
extern bool isHost;

JoinMenuScene::JoinMenuScene(sf::RenderWindow& window)
    : Scene{window}, m_connectButton{"Connect"}, m_backButton{"Back"}
{
    sf::Vector2u windowSize = window.getSize();
    
    /* Buttons */
    m_connectButton.setPositionCenter(windowSize.x * 0.5, windowSize.y * 0.6);
    m_backButton.setPositionCenter(windowSize.x * 0.5, windowSize.y * 0.7);
}

void JoinMenuScene::update(const Input& input)
{
    if(m_backButton.isPressed(input))
    {
        SceneManager::changeScene("MainMenu");
    }
}

void JoinMenuScene::draw() const
{
    m_window.clear();
    m_window.draw(m_connectButton);
    m_window.draw(m_backButton);
    m_window.display();
}

void JoinMenuScene::joinServer()
{
    
}