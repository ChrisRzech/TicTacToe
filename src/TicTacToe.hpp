#pragma once
#include <array>

class TicTacToe
{
public:
    enum class Mark { EMPTY, X, O };
    enum class WinCondition { NOT_DONE, X, O, DRAW };
    
    static constexpr int BOARD_SIZE = 3;
    
    TicTacToe();
    
    void setTurn(Mark);
    Mark getTurn() const;
    
    void setMark(int row, int col);
    Mark getMark(int row, int col) const;
    
    void clearMark(int row, int col);
    void clear();
    
    WinCondition checkWin() const;

private:
    static bool validCell(int row, int col);
    
    Mark m_turn;
    std::array<std::array<Mark, BOARD_SIZE>, BOARD_SIZE> m_board;
};