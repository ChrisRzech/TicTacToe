#pragma once
#include <array>
#include <optional>

class TicTacToe
{
public:
    static constexpr int BOARD_SIZE = 3;
    
    void setMark(int row, int col);
    std::optional<bool> getMark(int row, int col) const;
    
    void clearMark(int row, int col);
    void clear();
    
    std::optional<bool> checkWin() const;
    
    bool turn = false;

private:
    std::array<std::array<std::optional<bool>, BOARD_SIZE>, BOARD_SIZE> m_board;
};