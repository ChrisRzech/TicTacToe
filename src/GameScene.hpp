#pragma once
#include "Scene.hpp"
#include "TicTacToeDrawer.hpp"
#include "Packets.hpp"
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

    TicTacToe m_ttt;
    TicTacToeDrawer m_tttDrawer;
    
    bool m_turn;
    sf::SocketSelector m_selector;
};