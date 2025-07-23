#include "grid.h"
#include <iostream>
#include "colors.h"

std::vector<Color> Grid::colors = GetColors();
Grid::Grid()
{
	numRows = 20;
	numCols = 10;
	cellSize = 40;
	Init();
}

void Grid::Init()
{
	for (size_t row = 0; row < numRows; row++)
	{
		for (size_t col = 0; col < numCols; col++)
		{
			grid[row][col] = 0;
		}
	}
}

void Grid::Print()
{
	for (size_t row = 0; row < numRows; row++)
	{
		for (size_t col = 0; col < numCols; col++)
		{
			std::cout << grid[row][col] << " ";
		}
		std::cout << std::endl;
	}
}

void Grid::Draw()
{
	for (size_t row = 0; row < numRows; row++)
	{
		for (size_t col = 0; col < numCols; col++)
		{
			int cellValue = grid[row][col];
			DrawRectangle(col * cellSize + 1 + 15, row * cellSize + 1 + 15, cellSize - 1, cellSize - 1, colors[cellValue]);
		}
	}
}

bool Grid::IsCellOutsideGrid(int row, int col)
{
	if (row < 0 || row >= numRows || col < 0 || col >= numCols) return true;
	return false;
}

bool Grid::IsCellEmpty(int row, int col)
{
	if (grid[row][col] == 0) return true;
	return false;
}

unsigned short Grid::ClearFullRows()
{
	unsigned short completedRows = 0;
	for (int row = numRows - 1; row >= 0; row--)
	{
		if (IsRowFull(row))
		{
			ClearRow(row);
			completedRows++;
		}
		else if (completedRows > 0)
		{
			ShiftRowDown(row, completedRows);
		}
	}
	return completedRows;
}

bool Grid::IsRowFull(int row)
{
	for (int column = 0; column < numCols; column++)
	{
		if (grid[row][column] == 0)
		{
			return false;
		}
	}
	return true;
}

void Grid::ClearRow(int row)
{
	for (int column = 0; column < numCols; column++)
	{
		grid[row][column] = 0;
	}
}

void Grid::ShiftRowDown(int row, int numRows)
{
	for (int col = 0; col < numCols; col++)
	{
		grid[row + numRows][col] = grid[row][col];
		grid[row][col] = 0;
	}
}
