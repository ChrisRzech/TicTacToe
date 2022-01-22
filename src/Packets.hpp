#pragma once
#include <sfml/Network/Packet.hpp>

struct TicTacToeMove
{
    sf::Int32 row;
    sf::Int32 col;
};

sf::Packet& operator<<(sf::Packet& packet, const TicTacToeMove& move);
sf::Packet& operator>>(sf::Packet& packet, TicTacToeMove& move);