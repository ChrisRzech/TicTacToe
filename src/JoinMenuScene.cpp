#include "JoinMenuScene.hpp"
#include "Resources.hpp"
#include "SceneManager.hpp"
#include <sfml/Network/TcpSocket.hpp>

extern unsigned short g_port;
extern sf::TcpSocket g_socket;
extern bool g_isHost;

JoinMenuScene::JoinMenuScene(sf::RenderWindow& window)
    : Scene{window},
      m_label{Resources::get().font, "Enter address"},
      m_connectButton{Resources::get().font, "Connect"},
      m_backButton{Resources::get().font, "Back"}
{
    sf::Vector2u windowSize = window.getSize();
    
    m_label.setPositionCenter(windowSize.x * 0.5, windowSize.y * 0.35);
    
    m_addressField.setSize(300, 35);
    m_addressField.setPositionCenter(windowSize.x * 0.5, windowSize.y * 0.45);
    
    m_connectButton.setBackgroundSize(120, 40);
    m_connectButton.setPositionCenter(windowSize.x * 0.5, windowSize.y * 0.55);
    m_connectButton.setBorderThickness(1);
    m_connectButton.setTextColor(sf::Color::White);
    m_connectButton.setBackgroundColor(sf::Color::Transparent);
    m_connectButton.setBorderColor(sf::Color::White);
    
    m_backButton.setBackgroundSize(80, 40);
    m_backButton.setPositionCenter(windowSize.x * 0.5, windowSize.y * 0.65);
    m_backButton.setBorderThickness(1);
    m_backButton.setTextColor(sf::Color::White);
    m_backButton.setBackgroundColor(sf::Color::Transparent);
    m_backButton.setBorderColor(sf::Color::White);
}

void JoinMenuScene::enter()
{
    m_addressField.setFocus(true);
    m_addressField.clear();
    
    m_tryConnecting = false;
}

void JoinMenuScene::update(const Input& input)
{
    if(m_connectButton.isPressed(input) || input.isPressed(Input::Key::Enter))
    {
        std::string string = m_addressField.getText();
        m_address = (string == "localhost" ? sf::IpAddress::getLocalAddress() : string);
        //NOTE: sf::IpAddress can block for a little bit
        
        //sf::IpAddress validates the address a bit
        m_tryConnecting = m_address != sf::IpAddress::None;
    }
    else if(m_backButton.isPressed(input))
    {
        SceneManager::changeScene("MainMenu");
    }
    else
    {
        m_addressField.update(input);
    }
    
    if(m_tryConnecting)
    {
        //NOTE: sf::TcpSocket::connect() doesn't work as expected when non-blocking
        sf::Socket::Status status = g_socket.connect(m_address, g_port, sf::milliseconds(250));
        
        if(status == sf::Socket::Status::Done)
        {
            g_isHost = false;
            SceneManager::changeScene("Game");
        }
        else
        {
            m_tryConnecting = false;
        }
    }
}

void JoinMenuScene::exit()
{
    
}

void JoinMenuScene::draw() const
{
    m_window.draw(m_label);
    m_window.draw(m_addressField);
    m_window.draw(m_connectButton);
    m_window.draw(m_backButton);
}