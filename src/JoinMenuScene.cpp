#include "JoinMenuScene.hpp"
#include "Resources.hpp"
#include "SceneManager.hpp"
#include <sfml/Network/TcpSocket.hpp>

extern unsigned short PORT;
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
        socket.setBlocking(true);
        sf::Socket::Status status = socket.connect(m_address, PORT, sf::milliseconds(250));
        socket.setBlocking(false);
        
        if(status == sf::Socket::Status::Done)
        {
            isHost = false;
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
    m_window.clear();
    m_window.draw(m_addressField);
    m_window.draw(m_connectButton);
    m_window.draw(m_backButton);
    m_window.display();
}