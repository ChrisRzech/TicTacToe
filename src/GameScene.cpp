#include "GameScene.hpp"
#include <sfml/Network/TcpSocket.hpp>
#include <thread>
#include <iostream>

extern sf::TcpSocket g_socket;
extern bool g_isHost;

GameScene::GameScene(sf::RenderWindow& window)
    : Scene{window}, m_tttDrawer{m_ttt, sf::Vector2f{0, 0}, window.getSize()}
{
    
}

void GameScene::enter()
{
    g_socket.setBlocking(true);
    m_turn = g_isHost;
    m_packetReceived = false;
    m_waitingForPacket = false;
}

void GameScene::update(const Input& input)
{
    if(m_turn)
    {
        if(input.isPressed(Input::Key::LeftClick))
        {
            std::pair<int, int> cell = m_tttDrawer.convertPointToCell(static_cast<sf::Vector2f>(input.mousePosition()));
            
            if(!m_ttt.getMark(cell.first, cell.second).has_value())
            {
                m_ttt.setMark(cell.first, cell.second);
                
                sendMove(TicTacToeMove{cell.first, cell.second});
                
                m_turn = false;
                
                std::optional<bool> winner = m_ttt.checkWin();
                
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
        if(!m_waitingForPacket)
        {
            m_waitingForPacket = true;
            receiveMove(m_packetData);
        }
        else if(m_packetReceived)
        {
            m_packetReceived = false;
            m_turn = true;
            
            m_ttt.setMark(m_packetData.row, m_packetData.col);
            
            std::optional<bool> winner = m_ttt.checkWin();
            
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

void GameScene::exit()
{
    g_socket.setBlocking(false);
}

void GameScene::draw() const
{
    m_window.clear();
    m_window.draw(m_tttDrawer);
    m_window.display();
}

void GameScene::sendMove(const TicTacToeMove& move)
{
    sf::Packet packet;
    packet << move;
    g_socket.send(packet);
}

void GameScene::receiveMove(TicTacToeMove& move)
{
    auto f =
    [this]
    ()
    {
        sf::Packet packet;
        sf::Socket::Status status = g_socket.receive(packet);
        
        if(status == sf::Socket::Status::Done)
        {
            packet >> m_packetData;
            m_packetReceived = true;
            m_waitingForPacket = false;
        }
    };
    
    /* Spawn a detached thread since the other player's move can take a long time */
    std::thread thread(f);
    thread.detach();
}