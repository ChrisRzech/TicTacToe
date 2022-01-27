#include "Input.hpp"
#include "GameScene.hpp"
#include "MainMenuScene.hpp"
#include "HostMenuScene.hpp"
#include "JoinMenuScene.hpp"
#include "SceneManager.hpp"
#include <sfml/Network/TcpSocket.hpp>
#include <sfml/Network/TcpListener.hpp>
#include <sfml/Window/Event.hpp>
#include <iostream>

static unsigned short PORT = 50'000;
sf::TcpSocket socket;
bool isHost = false;

void hostServer(sf::TcpSocket& socket)
{
    std::cout << "Waiting for connection..." << std::endl;
    
    sf::TcpListener listener;
    listener.listen(PORT);
    listener.accept(socket);
    
    std::cout << "Connected" << std::endl;
}

void joinServer(sf::TcpSocket& socket)
{
    sf::Socket::Status status;
    do
    {
        std::cout << "Enter address: ";
        std::string address;
        std::getline(std::cin, address);
        
        if(address == "localhost")
            address = sf::IpAddress::getLocalAddress().toString();
        
        std::cout << "Attempting to connect to \"" << address << ":" << PORT << "\"..." << std::endl;
        status = socket.connect(address, PORT, sf::seconds(5));
        
        if(status != sf::Socket::Status::Done)
            std::cout << "Failure to connect" << std::endl;
    }
    while(status != sf::Socket::Status::Done);
    
    std::cout << "Connected" << std::endl;
}

bool setupSocket(sf::TcpSocket& socket)
{
    std::string answer;
    do
    {
        std::cout << "Host server? (y/n): ";
        std::getline(std::cin, answer);
        
        if(answer == "y")
            hostServer(socket);
        else if(answer == "n")
            joinServer(socket);
    }
    while(answer != "y" && answer != "n");
    
    return answer == "y";
}

int main()
{
    socket.setBlocking(false);

    sf::RenderWindow window(sf::VideoMode{600, 600}, "TicTacToe", sf::Style::Close);
    window.setFramerateLimit(60);
    
    const std::vector<Input::Key> polledInputs =
    {
        Input::Key::LeftClick,
        
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