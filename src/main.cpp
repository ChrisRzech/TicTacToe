#include "TicTacToeDrawer.hpp"
#include "Input.hpp"
#include <iostream>

int main()
{
    sf::VideoMode windowVideoMode(600, 600);
    sf::RenderWindow window(windowVideoMode, "TicTacToe", sf::Style::Close);
    window.setFramerateLimit(60);
    bool windowIsFocused = true;
    
    const std::vector<Input::Key> polledInputs =
    {
        Input::Key::LeftClick
    };
    Input input(window, 10, polledInputs);

    TicTacToe ttt;
    TicTacToeDrawer tttDrawer(ttt, sf::Vector2f{0, 0}, sf::Vector2u{600, 600});
    
    while(window.isOpen())
    {
        /* Window event */
        sf::Event windowEvent;
        while(window.pollEvent(windowEvent))
        {
            switch(windowEvent.type)
            {
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }
                
                case sf::Event::LostFocus:
                {
                    windowIsFocused = false;
                    break;
                }
                
                case sf::Event::GainedFocus:
                {
                    windowIsFocused = true;
                    break;
                }
                
                default:
                {
                    break;
                }
            }
        }
        
        /* Input */
        if(windowIsFocused)
            input.poll();
        
        /* Logic */
        if(input.isPressed(Input::Key::LeftClick))
        {
            std::pair<int, int> cell = tttDrawer.convertPointToCell(static_cast<sf::Vector2f>(input.mousePosition()));
            
            if(!ttt.getMark(cell.first, cell.second).has_value())
            {
                ttt.setMark(cell.first, cell.second);
                
                std::optional<bool> winner = ttt.checkWin();
                
                if(winner.has_value())
                {
                    if(winner.value())
                    {
                        std::cout << "X" << std::endl;
                    }
                    else
                    {
                        std::cout << "O" << std::endl;
                    }
                }
                else
                {
                    std::cout << "No winner" << std::endl;
                }
            }
        }
        
        /* Render */
        window.clear();
        window.draw(tttDrawer);
        window.display();
    }
}