#include "HostMenuScene.hpp"
#include "Resources.hpp"
#include "SceneManager.hpp"
#include <sfml/Network.hpp>

extern sf::TcpSocket socket;
extern bool isHost;

HostMenuScene::HostMenuScene(sf::RenderWindow& window)
    : Scene{window}, m_backButton{"Back"}
{
    sf::Vector2u windowSize = window.getSize();

    /* Wait text */
    m_waitText.setFont(Resources::getResources().font);
    m_waitText.setString("Waiting for opponent...");
    sf::FloatRect waitTextRect = m_waitText.getLocalBounds();
    sf::Vector2f waitTextPositionCentered(windowSize.x * 0.5 - waitTextRect.width * 0.5, windowSize.y * 0.5 - waitTextRect.height * 0.5);
    m_waitText.setPosition(waitTextPositionCentered - sf::Vector2f{waitTextRect.left, waitTextRect.top});
    
    /* Buttons */
    m_backButton.setPositionCenter(windowSize.x * 0.5, windowSize.y * 0.6);
}

void HostMenuScene::update(const Input& input)
{
    //TODO listen for connection
    //hostServer();
    
    if(m_backButton.isPressed(input))
    {
        SceneManager::changeScene("MainMenu");
    }
}

void HostMenuScene::draw() const
{
    m_window.clear();
    m_window.draw(m_waitText);
    m_window.draw(m_backButton);
    m_window.display();
}

void HostMenuScene::hostServer()
{
    
}