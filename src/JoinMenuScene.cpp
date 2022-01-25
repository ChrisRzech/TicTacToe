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
    
    m_addressField.setSize(300, 35);
    m_addressField.setPositionCenter(windowSize.x * 0.5, windowSize.y * 0.5);
    m_connectButton.setPositionCenter(windowSize.x * 0.5, windowSize.y * 0.6);
    m_backButton.setPositionCenter(windowSize.x * 0.5, windowSize.y * 0.7);
}

void JoinMenuScene::update(const Input& input)
{
    if(m_connectButton.isPressed(input))
    {
        
    }
    else if(m_backButton.isPressed(input))
    {
        SceneManager::changeScene("MainMenu");
    }
    else
    {
        m_addressField.update(input);
    }
}

void JoinMenuScene::draw() const
{
    m_window.clear();
    m_window.draw(m_addressField);
    m_window.draw(m_connectButton);
    m_window.draw(m_backButton);
    m_window.display();
}

void JoinMenuScene::joinServer()
{
    
}