#pragma once
#include "Scene.hpp"
#include "TicTacToeDrawer.hpp"
#include "Button.hpp"
#include "TextLabel.hpp"
#include <sfml/Network/SocketSelector.hpp>

class GameScene : public Scene
{
public:
    GameScene(sf::RenderWindow&);
    
    void enter() override;
    void update(const Input&) override;
    void exit() override;
    
    void draw() const override;

private:
    enum class State { TURN_SELECT, PLAY, GAMEOVER };
    State m_state;
    
    void restart();
    void handleWinner(TicTacToe::WinCondition winner);
    
    void sendMove(const std::pair<int, int>& move);
    std::optional<std::pair<int, int>> receiveMove();
    
    void sendRestart();
    bool receiveRestart();
    
    void sendStartTurn(TicTacToe::Mark);
    std::optional<TicTacToe::Mark> receiveStartTurn();

    TicTacToe m_ttt;
    TicTacToeDrawer m_tttDrawer;
    TicTacToe::Mark m_myMark;
    
    sf::SocketSelector m_selector;
    
    TextLabel m_winnerLabel;
    Button m_restartButton;
    bool m_wantRestart;
    TextLabel m_waitRestartLabel;
    
    TextLabel m_turnSelectLabel;
    Button m_selectFirstButton;
    Button m_selectSecondButton;
};