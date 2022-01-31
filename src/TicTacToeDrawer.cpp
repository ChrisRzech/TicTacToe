#include "TicTacToeDrawer.hpp"
#include "Resources.hpp"
#include <sfml/Graphics/VertexArray.hpp>
#include <sfml/Graphics/Text.hpp>

TicTacToeDrawer::TicTacToeDrawer(const TicTacToe& TicTacToe)
    : TicTacToeDrawer{TicTacToe, sf::Vector2f{0, 0}, sf::Vector2u{0, 0}}
{
    //Do nothing
}

TicTacToeDrawer::TicTacToeDrawer(const TicTacToe& TicTacToe, sf::Vector2f position, sf::Vector2u size)
    : m_TicTacToe{TicTacToe}, position{position}, size{size}
{
    //Do nothing
}

std::pair<int, int> TicTacToeDrawer::convertPointToCell(sf::Vector2f point) const
{
    float spacingX = size.x / TicTacToe::BOARD_SIZE;
    float spacingY = size.y / TicTacToe::BOARD_SIZE;
    
    point -= position;
    
    //return (row, col)
    return std::make_pair(point.y / spacingY, point.x / spacingX);
}

void TicTacToeDrawer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    /* Draw board */
    sf::VertexArray lines(sf::Lines, (TicTacToe::BOARD_SIZE - 1) * 2);
    float spacingX = size.x / TicTacToe::BOARD_SIZE;
    float spacingY = size.y / TicTacToe::BOARD_SIZE;
    float startingX = spacingX + position.x;
    float startingY = spacingY + position.y;
    
    for(int i = 0; i < TicTacToe::BOARD_SIZE - 1; i++)
    {
        sf::Color color = sf::Color::White;
        
        /* Vertical line */
        lines.append(sf::Vertex{sf::Vector2f{startingX + spacingX * i, position.y}, color});
        lines.append(sf::Vertex{sf::Vector2f{startingX + spacingX * i, position.y + size.y}, color});
        
        /* Horizontal line */
        lines.append(sf::Vertex{sf::Vector2f{position.x, startingY + spacingY * i}, color});
        lines.append(sf::Vertex{sf::Vector2f{position.x + size.x, startingY + spacingY * i}, color});
    }
    
    target.draw(lines);
    
    /* Draw symbols */
    sf::Text text;
    text.setFont(Resources::get().font);
    text.setCharacterSize(75);
    for(int row = 0; row < TicTacToe::BOARD_SIZE; row++)
    {
        for(int col = 0; col < TicTacToe::BOARD_SIZE; col++)
        {
            TicTacToe::Mark cellMark = m_TicTacToe.getMark(row, col);
            
            if(cellMark != TicTacToe::Mark::EMPTY)
            {
                text.setString(cellMark == TicTacToe::Mark::X ? "X" : "O");
                
                /* Center text in cell */
                float textWidth = text.getLocalBounds().width;
                float textHeight = text.getLocalBounds().height;
                float leftExtraSpace = text.getLocalBounds().left;
                float topExtraSpace = text.getLocalBounds().top;
                float offsetX = spacingX / 2 - textWidth / 2;
                float offsetY = spacingY / 2 - textHeight / 2;
                text.setPosition(spacingX * col + offsetX - leftExtraSpace, spacingY * row + offsetY - topExtraSpace);
                
                target.draw(text);
            }
        }
    }
}