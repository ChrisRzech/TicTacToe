#pragma once
#include <sfml/Network.hpp>

struct TicTacToeMove
{
    sf::Int32 row;
    sf::Int32 col;
};

sf::Packet& operator<<(sf::Packet& packet, const TicTacToeMove& move)
{
    return packet << move.row << move.col;
}

sf::Packet& operator>>(sf::Packet& packet, TicTacToeMove& move)
{
    return packet >> move.row >> move.col;
}