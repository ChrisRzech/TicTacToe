#pragma once
#include "Scene.hpp"
#include "TicTacToeDrawer.hpp"
#include "Packets.hpp"

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
    void receiveMove(TicTacToeMove& move);

    TicTacToe m_ttt;
    TicTacToeDrawer m_tttDrawer;
    bool m_turn;
    bool m_packetReceived;
    bool m_waitingForPacket;
    TicTacToeMove m_packetData;
};