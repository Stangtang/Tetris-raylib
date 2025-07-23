#pragma once

#include <vector>
#include "raylib.h"

class Grid
{
public:
	Grid();
	void Init();
	void Print();
	void Draw();
	bool IsCellOutsideGrid(int row, int col);
	bool IsCellEmpty(int row, int col);
	int ClearFullRows();
	int grid[20][10];

private:
	int numRows;
	int numCols;
	int cellSize;
	static std::vector<Color> colors;
	bool IsRowFull(int row);
	void ClearRow(int row);
	void ShiftRowDown(int row, int numRows);
};