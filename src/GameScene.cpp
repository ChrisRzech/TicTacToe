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
    m_turn = g_isHost;
    
    m_selector.add(g_socket);
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
                sendMove(TicTacToeMove{cell.first, cell.second});
                
                m_turn = false;
                m_ttt.setMark(cell.first, cell.second);
                handleWinner(m_ttt.checkWin());
            }
        }
    }
    else
    {
        std::optional<TicTacToeMove> move = receiveMove();
        
        if(move.has_value())
        {
            m_turn = true;
            m_ttt.setMark(move->row, move->col);
            handleWinner(m_ttt.checkWin());
        }
    }
}

void GameScene::exit()
{
    
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

std::optional<TicTacToeMove> GameScene::receiveMove()
{
    std::optional<TicTacToeMove> move;
    
    if(m_selector.wait(sf::milliseconds(10)))
    {
        sf::Packet packet;
        sf::Socket::Status status = g_socket.receive(packet);
        if(status == sf::Socket::Status::Done)
        {
            TicTacToeMove receivedMove;
            packet >> receivedMove;
            move = receivedMove;
        }
    }
    
    return move;
}

void GameScene::handleWinner(const std::optional<bool>& winner)
{
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