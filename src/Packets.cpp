#include "Packets.hpp"

sf::Packet& operator<<(sf::Packet& packet, const TicTacToeMove& move)
{
    return packet << move.row << move.col;
}

sf::Packet& operator>>(sf::Packet& packet, TicTacToeMove& move)
{
    return packet >> move.row >> move.col;
}