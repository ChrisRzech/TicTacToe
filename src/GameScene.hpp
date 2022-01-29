#pragma once
#include "Scene.hpp"
#include "TicTacToeDrawer.hpp"
#include "Packets.hpp"
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
    void sendMove(const TicTacToeMove& move);
    std::optional<TicTacToeMove> receiveMove();
    
    void handleWinner(TicTacToe::WinCondition winner);

    TicTacToe m_ttt;
    TicTacToeDrawer m_tttDrawer;
    TicTacToe::Mark m_myMark;
    
    sf::SocketSelector m_selector;
    
    bool m_gameover;
    TextLabel m_winnerLabel;
    Button m_restartButton;
};