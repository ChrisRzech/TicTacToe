#include "MainMenuScene.hpp"
#include "Resources.hpp"
#include "SceneManager.hpp"

MainMenuScene::MainMenuScene(sf::RenderWindow& window)
    : Scene{window}, m_titleLabel{Resources::getResources().font, "Tic-Tac-Toe"}, m_hostButton{"Host"}, m_joinButton{"Join"}
{
    sf::Vector2u windowSize = window.getSize();
    
    /* Title */
    m_titleLabel.setPositionCenter(windowSize.x * 0.5, windowSize.y * 0.4);
    m_titleLabel.setTextColor(sf::Color::White);
    m_titleLabel.setBackgroundColor(sf::Color::Transparent);
    
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
    m_window.draw(m_titleLabel);
    m_window.draw(m_hostButton);
    m_window.draw(m_joinButton);
    m_window.display();
}