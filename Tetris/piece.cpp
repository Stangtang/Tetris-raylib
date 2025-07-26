#include "piece.h"
#include "raylib.h"
#include "game.h"

std::vector<Color> Piece::colors = GetColors();
Piece::Piece()
{
	cellSize = 40;
	rotation = 0;
	rowOffset = 0;
	colOffset = 0;
}

void Piece::Draw()
{
	const std::vector<Position> cellPositions = GetCellPositions();
	for (const Position& pos : cellPositions)
	{
		DrawRectangle(pos.col * cellSize + 1 + 15, pos.row * cellSize + 1 + 15, cellSize - 1, cellSize - 1, colors[type]);
	}
}

void Piece::Draw(int PixelOffsetX, int PixelOffsetY)
{
	const std::vector<Position> cellPositions = GetCellPositions();
	for (const Position& pos : cellPositions)
	{
		DrawRectangle(pos.col * cellSize + PixelOffsetX, pos.row * cellSize + PixelOffsetY, cellSize - 1, cellSize - 1, colors[type]);
	}
}

void Piece::Move(int rows, int cols)
{
	rowOffset += rows;
	colOffset += cols;
}

void Piece::Rotate(int numTimes)
{
	rotation = (rotation + numTimes + 4) % 4;
}

std::vector<Position> Piece::GetCellPositions()
{
	const std::vector<Position> cellPositions = occupiedCells[rotation];
	std::vector<Position> movedTiles;
	for (const Position& pos : cellPositions)
	{
		Position newPos = Position(pos.row + rowOffset, pos.col + colOffset);
		movedTiles.push_back(newPos);
	}
	return movedTiles;
}

Piece Piece::GetNewPieceCopy()
{
	switch (type)
	{
	case 1: return IPiece();
	case 2: return OPiece();
	case 3: return TPiece();
	case 4: return LPiece();
	case 5: return JPiece();
	case 6: return SPiece();
	case 7: return ZPiece();
	}
}