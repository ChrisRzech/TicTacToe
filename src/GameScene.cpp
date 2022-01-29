#include "GameScene.hpp"
#include "Resources.hpp"
#include <sfml/Network/TcpSocket.hpp>

extern sf::TcpSocket g_socket;
extern bool g_isHost;

GameScene::GameScene(sf::RenderWindow& window)
    : Scene{window},
      m_tttDrawer{m_ttt, sf::Vector2f{0, 0}, window.getSize()},
      m_winnerLabel{Resources::getResources().font, ""},
      m_restartButton{Resources::getResources().font, "Restart"},
      m_waitRestartLabel{Resources::getResources().font, "Waiting for opponent..."}
{
    sf::Vector2u windowSize = window.getSize();
    
    m_winnerLabel.setBackgroundSize(300, 175);
    m_winnerLabel.setPositionCenter(windowSize.x * 0.5, windowSize.y * 0.5);
    m_winnerLabel.setBorderThickness(3);
    m_winnerLabel.setTextColor(sf::Color::White);
    m_winnerLabel.setBackgroundColor(sf::Color::Black);
    m_winnerLabel.setBorderColor(sf::Color::White);
    
    m_restartButton.setBackgroundSize(120, 40);
    m_restartButton.setPositionCenter(windowSize.x * 0.5, windowSize.y * 0.6);
    m_restartButton.setBorderThickness(1);
    m_restartButton.setTextColor(sf::Color::White);
    m_restartButton.setBackgroundColor(sf::Color::Black);
    m_restartButton.setBorderColor(sf::Color::White);
    
    m_waitRestartLabel.setPositionCenter(windowSize.x * 0.5, windowSize.y * 0.6);
    m_waitRestartLabel.setTextColor(sf::Color::White);
    m_waitRestartLabel.setBackgroundColor(sf::Color::Black);
}

void GameScene::enter()
{
    m_selector.add(g_socket);
    m_myMark = (g_isHost ? TicTacToe::Mark::X : TicTacToe::Mark::O);
    restart();
}

void GameScene::update(const Input& input)
{
    if(m_gameover)
    {
        if(m_wantRestart && receiveRestart())
        {
            restart();
        }
        else if(m_restartButton.isPressed(input))
        {
            m_wantRestart = true;
            sendRestart();
        }
    }
    else
    {
        if(m_ttt.getTurn() == m_myMark)
        {
            if(input.isPressed(Input::Key::LeftClick))
            {
                std::pair<int, int> cell = m_tttDrawer.convertPointToCell(static_cast<sf::Vector2f>(input.mousePosition()));
                
                if(m_ttt.getMark(cell.first, cell.second) == TicTacToe::Mark::EMPTY)
                {
                    sendMove(TicTacToeMove{cell.first, cell.second});
                    
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
                m_ttt.setMark(move->row, move->col);
                handleWinner(m_ttt.checkWin());
            }
        }
    }
}

void GameScene::exit()
{
    m_selector.clear();
}

void GameScene::draw() const
{
    m_window.clear();
    m_window.draw(m_tttDrawer);
    if(m_gameover)
    {
        m_window.draw(m_winnerLabel);
        
        if(m_wantRestart)
            m_window.draw(m_waitRestartLabel);
        else
            m_window.draw(m_restartButton);
    }
    m_window.display();
}

void GameScene::restart()
{
    m_ttt.clear();
    
    m_ttt.setTurn(TicTacToe::Mark::X);
    m_gameover = false;
    m_wantRestart = false;
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

void GameScene::sendRestart()
{
    sf::Int8 x = 0; //Don't care what is sent, just let them know to restart
    sf::Packet packet;
    packet << x;
    g_socket.send(packet);
}

bool GameScene::receiveRestart()
{
    bool restart = false;
    
    if(m_selector.wait(sf::milliseconds(10)))
    {
        sf::Packet packet;
        sf::Socket::Status status = g_socket.receive(packet);
        if(status == sf::Socket::Status::Done)
        {
            //Don't care what is received, just wanted to know if they want to restart
            restart = true;
        }
    }
    
    return restart;
}

void GameScene::handleWinner(TicTacToe::WinCondition winner)
{
    switch(winner)
    {
    case TicTacToe::WinCondition::NOT_DONE:
    {
        return; //Do nothing in this function
        break;
    }
    
    case TicTacToe::WinCondition::X:
    {
        if(m_myMark == TicTacToe::Mark::X)
        {
            m_winnerLabel.setText("You win!");
            m_winnerLabel.setBorderColor(sf::Color::Green);
        }
        else
        {
            m_winnerLabel.setText("You lose!");
            m_winnerLabel.setBorderColor(sf::Color::Red);
        }
        break;
    }
    
    case TicTacToe::WinCondition::O:
    {
        if(m_myMark == TicTacToe::Mark::O)
        {
            m_winnerLabel.setText("You win!");
            m_winnerLabel.setBorderColor(sf::Color::Green);
        }
        else
        {
            m_winnerLabel.setText("You lose!");
            m_winnerLabel.setBorderColor(sf::Color::Red);
        }
        break;
    }
    
    case TicTacToe::WinCondition::DRAW:
    {
        m_winnerLabel.setText("Draw!");
        m_winnerLabel.setBorderColor(sf::Color::White);
        break;
    }
    }
    
    m_gameover = true;
}