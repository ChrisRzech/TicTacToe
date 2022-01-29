#include "TicTacToe.hpp"

TicTacToe::TicTacToe()
{
    clear();
}

void TicTacToe::setTurn(Mark turn)
{
    m_turn = turn;
}

TicTacToe::Mark TicTacToe::getTurn() const
{
    return m_turn;
}

void TicTacToe::setMark(int row, int col)
{
    if(validCell(row, col) && m_board[row][col] == Mark::EMPTY)
    {
        m_board[row][col] = m_turn;
        switch(m_turn)
        {
        case Mark::X:
        {
            m_turn = Mark::O;
            break;
        }
        
        case Mark::O:
        {
            m_turn = Mark::X;
            break;
        }
        }
    }
}

TicTacToe::Mark TicTacToe::getMark(int row, int col) const
{
    if(validCell(row, col))
        return m_board[row][col];
    else
        return Mark::EMPTY;
}

void TicTacToe::clearMark(int row, int col)
{
    if(validCell(row, col))
        m_board[row][col] = Mark::EMPTY;
}

void TicTacToe::clear()
{
    for(int row = 0; row < BOARD_SIZE; row++)
    {
        for(int col = 0; col < BOARD_SIZE; col++)
            clearMark(row, col);
    }
}

TicTacToe::WinCondition TicTacToe::checkWin() const
{   
    auto cellCheckWin =
    []
    (Mark mark, WinCondition& winner)
    {
        if(mark == Mark::EMPTY)
        {
            winner = WinCondition::NOT_DONE;
            return false; //No possible winner
        }
        else
        {
            /* Is this the first cell we check? */
            if(winner == WinCondition::NOT_DONE)
            {
                winner = (mark == Mark::X ? WinCondition::X : WinCondition::O);
            }
            else
            {
                if((mark == Mark::X && winner == WinCondition::O) || (mark == Mark::O && winner == WinCondition::X))
                {
                    winner = WinCondition::DRAW;
                    return false; //No possible winner
                }
            }
        }
        
        return true; //Possible winner
    };
    
    WinCondition winner;
    
    /* Check horizontal */
    for(int row = 0; row < BOARD_SIZE; row++)
    {
        winner = WinCondition::NOT_DONE;
        
        for(int col = 0; col < BOARD_SIZE; col++)
        {
            if(!cellCheckWin(m_board[row][col], winner))
                break;
        }
        
        if(winner == WinCondition::X || winner == WinCondition::O)
            return winner;
    }
    
    /* Check vertical */
    for(int col = 0; col < BOARD_SIZE; col++)
    {
        winner = WinCondition::NOT_DONE;
        
        for(int row = 0; row < BOARD_SIZE; row++)
        {
            if(!cellCheckWin(m_board[row][col], winner))
                break;
        }
        
        if(winner == WinCondition::X || winner == WinCondition::O)
            return winner;
    }
    
    /* Check downward-diagonal */
    winner = WinCondition::NOT_DONE;
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        if(!cellCheckWin(m_board[i][i], winner))
            break;
    }
    
    if(winner == WinCondition::X || winner == WinCondition::O)
        return winner;
    
    /* Check upward-diagonal */
    winner = WinCondition::NOT_DONE;
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        if(!cellCheckWin(m_board[TicTacToe::BOARD_SIZE - i - 1][i], winner))
            break;
    }

    if(winner == WinCondition::X || winner == WinCondition::O)
        return winner;
    
    /* Check if there is an empty cell */
    for(int row = 0; row < BOARD_SIZE; row++)
    {
        for(int col = 0; col < BOARD_SIZE; col++)
        {
            if(m_board[row][col] == Mark::EMPTY)
                return WinCondition::NOT_DONE;
        }
    }
    
    //No empty cell, it is a draw
    return WinCondition::DRAW;
}


bool TicTacToe::validCell(int row, int col)
{
    return 0 <= row && row < BOARD_SIZE &&
           0 <= col && col < BOARD_SIZE;
}