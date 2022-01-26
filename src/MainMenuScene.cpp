#include "MainMenuScene.hpp"
#include "Resources.hpp"
#include "SceneManager.hpp"
#include <sfml/Network/TcpSocket.hpp>
#include <iostream>

extern sf::TcpSocket socket;
extern bool isHost;

MainMenuScene::MainMenuScene(sf::RenderWindow& window)
    : Scene{window}, m_hostButton{"Host"}, m_joinButton{"Join"}
{
    sf::Vector2u windowSize = window.getSize();
    
    /* Title */
    m_titleText.setFont(Resources::getResources().font);
    m_titleText.setString("Tic-Tac-Toe");
    sf::FloatRect titleTextRect = m_titleText.getLocalBounds();
    sf::Vector2f titleTextPositionCentered(windowSize.x * 0.5 - titleTextRect.width * 0.5, windowSize.y * 0.4 - titleTextRect.height * 0.5);
    m_titleText.setPosition(titleTextPositionCentered - sf::Vector2f{titleTextRect.left, titleTextRect.top});
    
    /* Buttons */
    m_hostButton.setPositionCenter(windowSize.x * 0.4, windowSize.y * 0.6);
    m_joinButton.setPositionCenter(windowSize.x * 0.6, windowSize.y * 0.6);
}

void MainMenuScene::enter()
{
    
}

void MainMenuScene::update(const Input& input)
{
    if(m_hostButton.isPressed(input))
    {
        SceneManager::changeScene("HostMenu");
    }
    else if(m_joinButton.isPressed(input))
    {
        SceneManager::changeScene("JoinMenu");
    }
}

void MainMenuScene::exit()
{
    
}

void MainMenuScene::draw() const
{
    m_window.clear();
    m_window.draw(m_titleText);
    m_window.draw(m_hostButton);
    m_window.draw(m_joinButton);
    m_window.display();
}