#include "TicTacToeDrawer.hpp"
#include "Input.hpp"
#include "Packets.hpp"
#include <thread>
#include <iostream>

static unsigned short PORT = 50'000;
static bool PACKET_RECEIVED = false;
static TicTacToeMove PACKET_DATA;
static bool WAITING_FOR_PACKET = false;

void hostServer(sf::TcpSocket& socket)
{
    std::cout << "Waiting for connection on " << sf::IpAddress::getLocalAddress() << ":" << PORT << "..." << std::endl;
    
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

bool getSocket(sf::TcpSocket& socket)
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

void sendMove(sf::TcpSocket& socket, const TicTacToeMove& move)
{
    sf::Packet packet;
    packet << move;
    socket.send(packet);
}

void receiveMove(sf::TcpSocket& socket, TicTacToeMove& move)
{
    auto f =
    [&socket]
    ()
    {
        sf::Packet packet;
        socket.receive(packet);
        packet >> PACKET_DATA;
        PACKET_RECEIVED = true;
        WAITING_FOR_PACKET = false;
    };
    
    
    /* Spawn detached thread to receive packet */
    std::thread thread(f);
    thread.detach();
}

int main()
{
    sf::TcpSocket socket;
    bool isHost = getSocket(socket);
    
    sf::VideoMode windowVideoMode(600, 600);
    sf::RenderWindow window(windowVideoMode, "TicTacToe", sf::Style::Close);
    window.setFramerateLimit(60);
    
    const std::vector<Input::Key> polledInputs =
    {
        Input::Key::LeftClick
    };
    Input input(window, 10, polledInputs);

    TicTacToe ttt;
    TicTacToeDrawer tttDrawer(ttt, sf::Vector2f{0, 0}, sf::Vector2u{600, 600});
    bool myTurn = isHost;
    
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
        
        /* Input */
        if(window.hasFocus())
            input.poll();
        
        /* Logic */
        if(myTurn)
        {
            if(input.isPressed(Input::Key::LeftClick))
            {
                std::pair<int, int> cell = tttDrawer.convertPointToCell(static_cast<sf::Vector2f>(input.mousePosition()));
                
                if(!ttt.getMark(cell.first, cell.second).has_value())
                {
                    ttt.setMark(cell.first, cell.second);
                    
                    sendMove(socket, TicTacToeMove{cell.first, cell.second});
                    
                    myTurn = false;
                    
                    std::optional<bool> winner = ttt.checkWin();
                    
                    if(winner.has_value())
                    {
                        if(winner.value())
                        {
                            std::cout << "X" << std::endl;
                        }
                        else
                        {
                            std::cout << "O" << std::endl;
                        }
                    }
                    else
                    {
                        std::cout << "No winner" << std::endl;
                    }
                }
            }
        }
        else
        {
            if(PACKET_RECEIVED)
            {
                PACKET_RECEIVED = false;
                myTurn = true;
                ttt.setMark(PACKET_DATA.row, PACKET_DATA.col);
            }
            else if(!WAITING_FOR_PACKET)
            {
                WAITING_FOR_PACKET = true;
                receiveMove(socket, PACKET_DATA);
            }
        }
        
        /* Render */
        window.clear();
        window.draw(tttDrawer);
        window.display();
    }
}