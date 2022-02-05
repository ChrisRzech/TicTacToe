#include "MainMenuScene.hpp"
#include "Resources.hpp"
#include "SceneManager.hpp"

MainMenuScene::MainMenuScene(const sf::Vector2u& size)
    : Scene{size},
      m_titleLabel{Resources::get().font, "Tic-Tac-Toe"},
      m_hostButton{Resources::get().font, "Host"},
      m_joinButton{Resources::get().font, "Join"}
{
    m_titleLabel.setPositionCenter(size.x * 0.5, size.y * 0.45);
    m_titleLabel.setTextColor(sf::Color::White);
    m_titleLabel.setBackgroundColor(sf::Color::Transparent);
    
    m_hostButton.setBackgroundSize(80, 40);
    m_hostButton.setPositionCenter(size.x * 0.4, size.y * 0.55);
    m_hostButton.setBorderThickness(1);
    m_hostButton.setTextColor(sf::Color::White);
    m_hostButton.setBackgroundColor(sf::Color::Transparent);
    m_hostButton.setBorderColor(sf::Color::White);
    
    m_joinButton.setBackgroundSize(80, 40);
    m_joinButton.setPositionCenter(size.x * 0.6, size.y * 0.55);
    m_joinButton.setBorderThickness(1);
    m_joinButton.setTextColor(sf::Color::White);
    m_joinButton.setBackgroundColor(sf::Color::Transparent);
    m_joinButton.setBorderColor(sf::Color::White);
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

void MainMenuScene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_titleLabel, states);
    target.draw(m_hostButton, states);
    target.draw(m_joinButton, states);
}