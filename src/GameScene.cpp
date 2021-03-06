#include "GameScene.hpp"
#include "Resources.hpp"
#include "SceneManager.hpp"
#include <sfml/Network/TcpSocket.hpp>
#include <sfml/Network/Packet.hpp>

extern sf::TcpSocket g_socket;
extern bool g_isHost;

GameScene::GameScene(const sf::Vector2u& size)
    : Scene{size},
      m_tttDrawer{m_ttt, sf::Vector2f{0, 0}, size},
      m_winnerLabel{Resources::get().font, ""},
      m_restartButton{Resources::get().font, "Restart"},
      m_waitRestartLabel{Resources::get().font, "Waiting for opponent..."},
      m_turnSelectLabel{Resources::get().font, ""},
      m_selectFirstButton{Resources::get().font, "First"},
      m_selectSecondButton{Resources::get().font, "Second"},
      m_opponentDisconnectedLabel{Resources::get().font, "Opponent disconnected"},
      m_opponentDisconnectedButton{Resources::get().font, "Back"}
{
    m_winnerLabel.setBackgroundSize(300, 175);
    m_winnerLabel.setPositionCenter(size.x * 0.5, size.y * 0.5);
    m_winnerLabel.setBorderThickness(3);
    m_winnerLabel.setTextColor(sf::Color::White);
    m_winnerLabel.setBackgroundColor(sf::Color::Black);
    m_winnerLabel.setBorderColor(sf::Color::White);
    
    m_restartButton.setBackgroundSize(120, 40);
    m_restartButton.setPositionCenter(size.x * 0.5, size.y * 0.6);
    m_restartButton.setBorderThickness(1);
    m_restartButton.setTextColor(sf::Color::White);
    m_restartButton.setBackgroundColor(sf::Color::Black);
    m_restartButton.setBorderColor(sf::Color::White);
    
    m_waitRestartLabel.setPositionCenter(size.x * 0.5, size.y * 0.6);
    m_waitRestartLabel.setTextColor(sf::Color::White);
    m_waitRestartLabel.setBackgroundColor(sf::Color::Black);
    
    m_turnSelectLabel.setBackgroundSize(300, 175);
    m_turnSelectLabel.setPositionCenter(size.x * 0.5, size.y * 0.5);
    m_turnSelectLabel.setBorderThickness(3);
    m_turnSelectLabel.setTextColor(sf::Color::White);
    m_turnSelectLabel.setBackgroundColor(sf::Color::Black);
    m_turnSelectLabel.setBorderColor(sf::Color::White);
    
    m_selectFirstButton.setBackgroundSize(110, 40);
    m_selectFirstButton.setPositionCenter(size.x * 0.38, size.y * 0.6);
    m_selectFirstButton.setBorderThickness(1);
    m_selectFirstButton.setTextColor(sf::Color::White);
    m_selectFirstButton.setBackgroundColor(sf::Color::Black);
    m_selectFirstButton.setBorderColor(sf::Color::White);
    
    m_selectSecondButton.setBackgroundSize(110, 40);
    m_selectSecondButton.setPositionCenter(size.x * 0.62, size.y * 0.6);
    m_selectSecondButton.setBorderThickness(1);
    m_selectSecondButton.setTextColor(sf::Color::White);
    m_selectSecondButton.setBackgroundColor(sf::Color::Black);
    m_selectSecondButton.setBorderColor(sf::Color::White);
    
    m_opponentDisconnectedLabel.setBackgroundSize(350, 175);
    m_opponentDisconnectedLabel.setPositionCenter(size.x * 0.5, size.y * 0.5);
    m_opponentDisconnectedLabel.setBorderThickness(3);
    m_opponentDisconnectedLabel.setTextColor(sf::Color::White);
    m_opponentDisconnectedLabel.setBackgroundColor(sf::Color::Black);
    m_opponentDisconnectedLabel.setBorderColor(sf::Color::White);
    
    m_opponentDisconnectedButton.setBackgroundSize(110  , 40);
    m_opponentDisconnectedButton.setPositionCenter(size.x * 0.5, size.y * 0.6);
    m_opponentDisconnectedButton.setBorderThickness(1);
    m_opponentDisconnectedButton.setTextColor(sf::Color::White);
    m_opponentDisconnectedButton.setBackgroundColor(sf::Color::Black);
    m_opponentDisconnectedButton.setBorderColor(sf::Color::White);
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
    
    case State::DISCONNECTED:
    {
        if(m_opponentDisconnectedButton.isPressed(input))
            SceneManager::changeScene("MainMenu");
        
        break;
    }
    }
}

void GameScene::exit()
{
    m_selector.clear();
}

void GameScene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_tttDrawer, states);
    
    switch(m_state)
    {
    case State::TURN_SELECT:
    {
        target.draw(m_turnSelectLabel);
        
        if(g_isHost)
        {
            target.draw(m_selectFirstButton, states);
            target.draw(m_selectSecondButton, states);
        }
        
        break;
    }
    
    case State::PLAY:
    {
        break;
    }
    
    case State::GAMEOVER:
    {
        target.draw(m_winnerLabel, states);
        
        if(m_wantRestart)
            target.draw(m_waitRestartLabel, states);
        else
            target.draw(m_restartButton, states);
        
        break;
    }
    
    case State::DISCONNECTED:
    {
        target.draw(m_opponentDisconnectedLabel, states);
        target.draw(m_opponentDisconnectedButton, states);
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

//TODO: could be a templated lambda in C++20, maybe?
template<typename DataType, typename... DataTypes>
void sendData_helper(sf::Packet& packet, const DataType& data, const DataTypes&... datas)
{
    packet << data;
    ((packet << datas), ...);
}

template<typename... DataTypes>
void GameScene::sendData(const DataTypes&... datas)
{
    sf::Packet packet;
    sendData_helper(packet, datas...);
    g_socket.send(packet);
}

template<typename... DataType>
std::optional<std::tuple<DataType...>> GameScene::receiveData()
{
    std::optional<std::tuple<DataType...>> data;
    
    if(m_selector.wait(sf::milliseconds(10)))
    {
        sf::Packet packet;
        sf::Socket::Status status = g_socket.receive(packet);
        switch(status)
        {
        case sf::Socket::Status::Done:
        {
            //NOTE: std::apply() folds the parameter packs for us specifically for tuples
            std::tuple<DataType...> receivedData;
            std::apply
            (
                [&packet]
                (DataType&... datas)
                {
                    ((packet >> datas), ...);
                }, receivedData
            );
            data = receivedData;

            break;
        }
        
        case sf::Socket::Status::Disconnected:
        {
            m_state = State::DISCONNECTED;
            break;
        }
        
        default:
        {
            break;
        }
        }
    }
    
    return data;
}

void GameScene::sendMove(const std::pair<int, int>& move)
{
    sendData(static_cast<sf::Int8>(move.first), static_cast<sf::Int8>(move.second));
}

std::optional<std::pair<int, int>> GameScene::receiveMove()
{
    auto data = receiveData<sf::Int8, sf::Int8>();
    return (data.has_value() ? std::make_optional(std::make_pair(std::get<0>(data.value()), std::get<1>(data.value()))) : std::nullopt);
}

void GameScene::sendRestart()
{
    sendData(static_cast<sf::Int8>(0)); //Don't care what is sent, just let them know that we want to restart
}

bool GameScene::receiveRestart()
{
    auto data = receiveData<sf::Int8>();
    return data.has_value();
}

void GameScene::sendStartTurn(TicTacToe::Mark mark)
{
    sendData(static_cast<sf::Int8>(mark));
}

std::optional<TicTacToe::Mark> GameScene::receiveStartTurn()
{
    auto data = receiveData<sf::Int8>();
    return (data.has_value() ? std::make_optional(static_cast<TicTacToe::Mark>(std::get<0>(data.value()))) : std::nullopt);
}