#pragma once

#include "position.h"
#include <vector>
#include <map>
#include "colors.h"

class Piece
{
public:
	Piece();
	int type;
	void Draw();
	void Draw(int XOffset, int YOffset);
	void Move(int rows, int cols);
	void Rotate();
	void UndoRotation();
	std::vector<Position> GetCellPositions();
	std::map<int, std::vector<Position>> occupiedCells;
	Piece GetNewPieceCopy();

private:
	int cellSize;
	int rotation;
	std::vector<Color> colors;
	int rowOffset;
	int colOffset;
};