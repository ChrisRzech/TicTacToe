#include "Input.hpp"
#include "GameScene.hpp"
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
    isHost = setupSocket(socket);
    
    sf::VideoMode windowVideoMode(600, 600);
    sf::RenderWindow window(windowVideoMode, "TicTacToe", sf::Style::Close);
    window.setFramerateLimit(60);
    
    const std::vector<Input::Key> polledInputs =
    {
        Input::Key::LeftClick
    };
    Input input(window, 10, polledInputs);
    
    Scene* scene = new GameScene(window);
    
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
        
        scene->update(input);
        scene->draw();
    }
}