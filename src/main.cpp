#include "Input.hpp"
#include "GameScene.hpp"
#include "MainMenuScene.hpp"
#include "HostMenuScene.hpp"
#include "JoinMenuScene.hpp"
#include "SceneManager.hpp"
#include <sfml/Network/TcpSocket.hpp>
#include <sfml/Window/Event.hpp>

static unsigned short PORT = 50'000;
sf::TcpSocket socket;
bool isHost = false;

int main()
{
    socket.setBlocking(false);

    sf::RenderWindow window(sf::VideoMode{600, 600}, "TicTacToe", sf::Style::Close);
    window.setFramerateLimit(60);
    
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
        /* Window event */
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
        SceneManager::getScene()->draw();
    }
}