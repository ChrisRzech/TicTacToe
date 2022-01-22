#pragma once
#include "TicTacToe.hpp"
#include <sfml/Graphics/RenderTarget.hpp>
#include <sfml/Graphics/Drawable.hpp>

class TicTacToeDrawer : public sf::Drawable
{
public:
    TicTacToeDrawer(const TicTacToe&);
    TicTacToeDrawer(const TicTacToe&, sf::Vector2f position, sf::Vector2u size);
    
    std::pair<int, int> convertPointToCell(sf::Vector2f) const;
    
    void draw(sf::RenderTarget&, sf::RenderStates states) const override;
    
    sf::Vector2f position;
    sf::Vector2u size;

private:
    const TicTacToe& m_TicTacToe;
};