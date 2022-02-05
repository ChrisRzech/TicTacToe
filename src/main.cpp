#include "Input.hpp"
#include "GameScene.hpp"
#include "MainMenuScene.hpp"
#include "HostMenuScene.hpp"
#include "JoinMenuScene.hpp"
#include "SceneManager.hpp"
#include "Resources.hpp"
#include <sfml/Network/TcpSocket.hpp>
#include <sfml/Window/Event.hpp>
#include <sfml/Graphics/RenderWindow.hpp>

unsigned short g_port = 25'565;
sf::TcpSocket g_socket;
bool g_isHost = false;

int main()
{
    sf::RenderWindow window(sf::VideoMode{600, 600}, "Tic-Tac-Toe", sf::Style::Close);
    window.setFramerateLimit(60);
    const sf::Image& windowIcon = Resources::get().windowIcon;
    window.setIcon(windowIcon.getSize().x, windowIcon.getSize().y, windowIcon.getPixelsPtr());
    
    Input input(window, 10);
    
    SceneManager::addScene("MainMenu", new MainMenuScene(window.getSize()));
    SceneManager::addScene("HostMenu", new HostMenuScene(window.getSize()));
    SceneManager::addScene("JoinMenu", new JoinMenuScene(window.getSize()));
    SceneManager::addScene("Game", new GameScene(window.getSize()));
    
    SceneManager::changeScene("MainMenu");
    
    while(window.isOpen())
    {
        sf::Event windowEvent;
        while(window.pollEvent(windowEvent))
        {
            switch(windowEvent.type)
            {
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }
                
                default:
                {
                    break;
                }
            }
        }
        
        if(window.hasFocus())
            input.poll();
        
        SceneManager::getScene().update(input);
        
        window.clear();
        window.draw(SceneManager::getScene());
        window.display();
    }
}