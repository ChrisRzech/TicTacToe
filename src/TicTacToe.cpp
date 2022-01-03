#include "TicTacToe.hpp"

bool validCell(int row, int col)
{
    return 0 <= row && row < TicTacToe::BOARD_SIZE &&
           0 <= col && col < TicTacToe::BOARD_SIZE;
}

void TicTacToe::setMark(int row, int col)
{
    if(validCell(row, col) && !m_board[row][col].has_value())
    {
        m_board[row][col] = turn;
        turn = !turn;
    }
}

std::optional<bool> TicTacToe::getMark(int row, int col) const
{
    if(validCell(row, col))
        return m_board[row][col];
    else
        return std::nullopt;
}

void TicTacToe::clearMark(int row, int col)
{
    if(validCell(row, col))
        m_board[row][col].reset();
}

void TicTacToe::clear()
{
    for(int row = 0; row < TicTacToe::BOARD_SIZE; row++)
    {
        for(int col = 0; col < TicTacToe::BOARD_SIZE; col++)
            clearMark(row, col);
    }
}

std::optional<bool> TicTacToe::checkWin() const
{
    auto cellCheckWin =
    [](const std::optional<bool>& cell, std::optional<bool>& winningMark)
    {
        if(cell.has_value())
        {
            if(winningMark.has_value())
            {
                if(cell.value() != winningMark.value())
                {
                    winningMark.reset();
                    return false;
                }
            }
            else
            {
                winningMark = cell.value();
            }
        }
        else
        {
            winningMark.reset();
            
            //No possible winner
            return false;
        }
        
        //Possible winner exists
        return true;
    };
    
    /* Check horizontal */
    for(int row = 0; row < TicTacToe::BOARD_SIZE; row++)
    {
        std::optional<bool> winningMark;
        
        for(int col = 0; col < TicTacToe::BOARD_SIZE; col++)
        {
            if(!cellCheckWin(m_board[row][col], winningMark))
                break;
        }
        
        if(winningMark.has_value())
            return winningMark;
    }
    
    /* Check vertical */
    for(int col = 0; col < TicTacToe::BOARD_SIZE; col++)
    {
        std::optional<bool> winningMark;
        
        for(int row = 0; row < TicTacToe::BOARD_SIZE; row++)
        {
            if(!cellCheckWin(m_board[row][col], winningMark))
                break;
        }
        
        if(winningMark.has_value())
            return winningMark;
    }
    
    /* Check downward-diagonal */
    {
    std::optional<bool> winningMark;
    for(int i = 0; i < TicTacToe::BOARD_SIZE; i++)
    {
        if(!cellCheckWin(m_board[i][i], winningMark))
            break;
    }
    
    if(winningMark.has_value())
        return winningMark;
    }
    
    /* Check upward-diagonal */
    {
    std::optional<bool> winningMark;
    for(int i = 0; i < TicTacToe::BOARD_SIZE; i++)
    {
        if(!cellCheckWin(m_board[TicTacToe::BOARD_SIZE - i - 1][i], winningMark))
            break;
    }
    
    if(winningMark.has_value())
        return winningMark;
    }
    
    return std::nullopt;
}