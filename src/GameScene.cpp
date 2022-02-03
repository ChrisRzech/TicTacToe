#include "GameScene.hpp"
#include "Resources.hpp"
#include <sfml/Network/TcpSocket.hpp>
#include <sfml/Network/Packet.hpp>

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
    switch(m_state)
    {
    case State::TURN_SELECT:
    {
        if(g_isHost)
        {
            if(m_selectFirstButton.isPressed(input))
            {
                m_myMark = TicTacToe::Mark::X;
                sendStartTurn(TicTacToe::Mark::O);
                m_state = State::PLAY;
            }
            else if(m_selectSecondButton.isPressed(input))
            {
                m_myMark = TicTacToe::Mark::O;
                sendStartTurn(TicTacToe::Mark::X);
                m_state = State::PLAY;
            }
        }
        else
        {
            std::optional<TicTacToe::Mark> myMark = receiveStartTurn();
            if(myMark.has_value())
            {
                m_myMark = myMark.value();
                m_state = State::PLAY;
            }
        }
        
        break;
    }
    
    case State::PLAY:
    {
        if(m_ttt.getTurn() == m_myMark)
        {
            if(input.isPressed(Input::Key::LeftClick))
            {
                std::pair<int, int> move = m_tttDrawer.convertPointToCell(static_cast<sf::Vector2f>(input.mousePosition()));
                
                if(m_ttt.getMark(move.first, move.second) == TicTacToe::Mark::EMPTY)
                {
                    sendMove(move);
                    
                    m_ttt.setMark(move.first, move.second);
                    handleWinner(m_ttt.checkWin());
                }
            }
        }
        else
        {
            std::optional<std::pair<int, int>> move = receiveMove();
            if(move.has_value())
            {
                m_ttt.setMark(move->first, move->second);
                handleWinner(m_ttt.checkWin());
            }
        }
        
        break;
    }
    
    case State::GAMEOVER:
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
        
        break;
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
    
    switch(m_state)
    {
    case State::TURN_SELECT:
    {
        m_window.draw(m_turnSelectLabel);
        
        if(g_isHost)
        {
            m_window.draw(m_selectFirstButton);
            m_window.draw(m_selectSecondButton);
        }
        
        break;
    }
    
    case State::PLAY:
    {
        break;
    }
    
    case State::GAMEOVER:
    {
        m_window.draw(m_winnerLabel);
        
        if(m_wantRestart)
            m_window.draw(m_waitRestartLabel);
        else
            m_window.draw(m_restartButton);
        
        break;
    }
    }
}

void GameScene::restart()
{
    m_state = State::TURN_SELECT;
    m_wantRestart = false;
    m_turnSelectLabel.setText(g_isHost ? "Go first or second?" : "Host selecting turn...");
    
    m_ttt.clear();
    m_ttt.setTurn(TicTacToe::Mark::X);
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
    
    m_state = State::GAMEOVER;
}

void GameScene::sendMove(const std::pair<int, int>& move)
{
    sf::Packet packet;
    packet << static_cast<sf::Int8>(move.first) << static_cast<sf::Int8>(move.second);
    g_socket.send(packet);
}

std::optional<std::pair<int, int>> GameScene::receiveMove()
{
    std::optional<std::pair<int, int>> move;
    
    if(m_selector.wait(sf::milliseconds(10)))
    {
        sf::Packet packet;
        sf::Socket::Status status = g_socket.receive(packet);
        if(status == sf::Socket::Status::Done)
        {
            sf::Int8 receivedFirst;
            sf::Int8 receivedSecond;
            packet >> receivedFirst >> receivedSecond;
            move = std::make_pair(static_cast<int>(receivedFirst), static_cast<int>(receivedSecond));
        }
    }
    
    return move;
}

void GameScene::sendRestart()
{
    sf::Packet packet;
    packet << static_cast<sf::Int8>(0); //Don't care what is sent, just let them know to restart
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
            restart = true; //Don't care what is received, just wanted to know if they want to restart
    }
    
    return restart;
}

void GameScene::sendStartTurn(TicTacToe::Mark mark)
{
    sf::Packet packet;
    packet << static_cast<sf::Int8>(mark);
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