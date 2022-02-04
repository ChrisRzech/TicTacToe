#include "Input.hpp"
#include "GameScene.hpp"
#include "MainMenuScene.hpp"
#include "HostMenuScene.hpp"
#include "JoinMenuScene.hpp"
#include "SceneManager.hpp"
#include "Resources.hpp"
#include <sfml/Network/TcpSocket.hpp>
#include <sfml/Window/Event.hpp>

unsigned short g_port = 25'565;
sf::TcpSocket g_socket;
bool g_isHost = false;

int main()
{
    sf::RenderWindow window(sf::VideoMode{600, 600}, "Tic-Tac-Toe", sf::Style::Close);
    window.setFramerateLimit(60);
    const sf::Image& windowIcon = Resources::get().windowIcon;
    window.setIcon(windowIcon.getSize().x, windowIcon.getSize().y, windowIcon.getPixelsPtr());
    
    const std::vector<Input::Key> polledInputs =
    {
        Input::Key::LeftClick,
        Input::Key::Enter,
        
        Input::Key::Num0,
        Input::Key::Num1,
        Input::Key::Num2,
        Input::Key::Num3,
        Input::Key::Num4,
        Input::Key::Num5,
        Input::Key::Num6,
        Input::Key::Num7,
        Input::Key::Num8,
        Input::Key::Num9,
        Input::Key::Period,
        Input::Key::Backspace,
        Input::Key::Left,
        Input::Key::Right,
        
        Input::Key::L,
        Input::Key::O,
        Input::Key::C,
        Input::Key::A,
        Input::Key::H,
        Input::Key::S,
        Input::Key::T
    };
    Input input(window, 10, polledInputs);
    
    SceneManager::addScene("MainMenu", new MainMenuScene(window));
    SceneManager::addScene("HostMenu", new HostMenuScene(window));
    SceneManager::addScene("JoinMenu", new JoinMenuScene(window));
    SceneManager::addScene("Game", new GameScene(window));
    
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
        
        SceneManager::getScene()->update(input);
        
        window.clear();
        SceneManager::getScene()->draw();
        window.display();
    }
}