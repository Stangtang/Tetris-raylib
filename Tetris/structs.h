#pragma once

struct Position
{
    Position(int row, int col) : row(row), col(col) {}
    int row;
    int col;
};

struct Displacement
{
    Displacement(int x, int y) : x(x), y(y) {}
    int x;
    int y;
};