#include "GameScene.hpp"
#include "Resources.hpp"
#include <sfml/Network/TcpSocket.hpp>

extern sf::TcpSocket g_socket;
extern bool g_isHost;

GameScene::GameScene(sf::RenderWindow& window)
    : Scene{window},
      m_tttDrawer{m_ttt, sf::Vector2f{0, 0}, window.getSize()},
      m_winnerLabel{Resources::get().font, ""},
      m_restartButton{Resources::get().font, "Restart"},
      m_waitRestartLabel{Resources::get().font, "Waiting for opponent..."},
      m_turnSelectLabel{Resources::get().font, ""},
      m_selectFirstButton{Resources::get().font, "First"},
      m_selectSecondButton{Resources::get().font, "Second"}
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
    
    m_turnSelectLabel.setBackgroundSize(300, 175);
    m_turnSelectLabel.setPositionCenter(windowSize.x * 0.5, windowSize.y * 0.5);
    m_turnSelectLabel.setBorderThickness(3);
    m_turnSelectLabel.setTextColor(sf::Color::White);
    m_turnSelectLabel.setBackgroundColor(sf::Color::Black);
    m_turnSelectLabel.setBorderColor(sf::Color::White);
    
    m_selectFirstButton.setBackgroundSize(110, 40);
    m_selectFirstButton.setPositionCenter(windowSize.x * 0.38, windowSize.y * 0.6);
    m_selectFirstButton.setBorderThickness(1);
    m_selectFirstButton.setTextColor(sf::Color::White);
    m_selectFirstButton.setBackgroundColor(sf::Color::Black);
    m_selectFirstButton.setBorderColor(sf::Color::White);
    
    m_selectSecondButton.setBackgroundSize(110, 40);
    m_selectSecondButton.setPositionCenter(windowSize.x * 0.62, windowSize.y * 0.6);
    m_selectSecondButton.setBorderThickness(1);
    m_selectSecondButton.setTextColor(sf::Color::White);
    m_selectSecondButton.setBackgroundColor(sf::Color::Black);
    m_selectSecondButton.setBorderColor(sf::Color::White);
}

void GameScene::enter()
{
    m_selector.add(g_socket);
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
    else if(m_selectingTurn)
    {
        if(g_isHost)
        {
            if(m_selectFirstButton.isPressed(input))
            {
                m_myMark = TicTacToe::Mark::X;
                sendStartTurn(TicTacToe::Mark::O);
                m_selectingTurn = false;
            }
            else if(m_selectSecondButton.isPressed(input))
            {
                m_myMark = TicTacToe::Mark::O;
                sendStartTurn(TicTacToe::Mark::X);
                m_selectingTurn = false;
            }
        }
        else
        {
            std::optional<TicTacToe::Mark> receivedMark = receiveStartTurn();
            if(receivedMark.has_value())
            {
                m_myMark = receivedMark.value();
                m_selectingTurn = false;
            }
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
    m_window.draw(m_tttDrawer);
    
    if(m_gameover)
    {
        m_window.draw(m_winnerLabel);
        
        if(m_wantRestart)
            m_window.draw(m_waitRestartLabel);
        else
            m_window.draw(m_restartButton);
    }
    else if(m_selectingTurn)
    {
        m_window.draw(m_turnSelectLabel);
        
        if(g_isHost)
        {
            m_window.draw(m_selectFirstButton);
            m_window.draw(m_selectSecondButton);
        }
    }
}

void GameScene::restart()
{
    m_ttt.clear();
    
    m_ttt.setTurn(TicTacToe::Mark::X);
    m_gameover = false;
    m_wantRestart = false;
    m_selectingTurn = true;
    
    m_turnSelectLabel.setText(g_isHost ? "Go first or second?" : "Host selecting turn...");
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

void GameScene::sendStartTurn(TicTacToe::Mark mark)
{
    sf::Int8 x = static_cast<sf::Int8>(mark);
    sf::Packet packet;
    packet << x;
    g_socket.send(packet);
}

std::optional<TicTacToe::Mark> GameScene::receiveStartTurn()
{
    std::optional<TicTacToe::Mark> mark;
    
    if(m_selector.wait(sf::milliseconds(10)))
    {
        sf::Packet packet;
        sf::Socket::Status status = g_socket.receive(packet);
        if(status == sf::Socket::Status::Done)
        {
            sf::Int8 receivedMark;
            packet >> receivedMark;
            mark = static_cast<TicTacToe::Mark>(receivedMark);
        }
    }
    
    return mark;
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