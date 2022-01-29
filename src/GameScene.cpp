#include "GameScene.hpp"
#include "Resources.hpp"
#include <sfml/Network/TcpSocket.hpp>

extern sf::TcpSocket g_socket;
extern bool g_isHost;

GameScene::GameScene(sf::RenderWindow& window)
    : Scene{window},
      m_tttDrawer{m_ttt, sf::Vector2f{0, 0}, window.getSize()},
      m_winnerLabel{Resources::getResources().font, ""},
      m_restartButton{Resources::getResources().font, "Restart"}
{
    sf::Vector2u windowSize = window.getSize();
    
    m_winnerLabel.setBackgroundSize(350, 175);
    m_winnerLabel.setPositionCenter(windowSize.x * 0.5, windowSize.y * 0.45);
    m_winnerLabel.setBorderThickness(1);
    m_winnerLabel.setTextColor(sf::Color::White);
    m_winnerLabel.setBackgroundColor(sf::Color::Black);
    m_winnerLabel.setBorderColor(sf::Color::White);
    
    m_restartButton.setBackgroundSize(120, 40);
    m_restartButton.setPositionCenter(windowSize.x * 0.5, windowSize.y * 0.55);
    m_restartButton.setBorderThickness(1);
    m_restartButton.setTextColor(sf::Color::White);
    m_restartButton.setBackgroundColor(sf::Color::Black);
    m_restartButton.setBorderColor(sf::Color::White);
}

void GameScene::enter()
{
    m_myMark = (g_isHost ? TicTacToe::Mark::X : TicTacToe::Mark::O);
    m_ttt.setTurn(TicTacToe::Mark::X);
    
    m_selector.add(g_socket);
    
    m_gameover = false;
}

void GameScene::update(const Input& input)
{
    if(m_gameover)
    {
        if(m_restartButton.isPressed(input))
        {
            m_ttt.clear();
            
            m_ttt.setTurn(TicTacToe::Mark::X);
            m_gameover = false;
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
        m_window.draw(m_restartButton);
    }
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

void GameScene::handleWinner(TicTacToe::WinCondition winner)
{
    switch(winner)
    {
    case TicTacToe::WinCondition::NOT_DONE:
    {
        break;
    }
    
    case TicTacToe::WinCondition::X:
    {
        m_winnerLabel.setText("X wins!");
        m_gameover = true;
        break;
    }
    
    case TicTacToe::WinCondition::O:
    {
        m_winnerLabel.setText("O wins!");
        m_gameover = true;
        break;
    }
    
    case TicTacToe::WinCondition::DRAW:
    {
        m_winnerLabel.setText("Draw!");
        m_gameover = true;
        break;
    }
    }
}