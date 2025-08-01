#pragma once

#include "position.h"
#include <vector>
#include <map>
#include "colors.h"
#include "displacement.h"
#include <utility>

class Piece
{
public:
	Piece();
	unsigned short type;
	void Draw();
	void Draw(int PixelOffsetX, int PixelOffsetY);
	void Move(int rows, int cols);
	void Rotate(int numTimes);
	int GetRotation(int numTimes);
	std::vector<Position> GetCellPositions();
	std::map<unsigned short, std::vector<Position>> occupiedCells;
	Piece NewCopy();
	std::map<std::pair<int, int>, std::vector<Displacement>> wallKickTable;

private:
	int cellSize;
	int rotation;
	static std::vector<Color> colors;
	int rowOffset;
	int colOffset;
};