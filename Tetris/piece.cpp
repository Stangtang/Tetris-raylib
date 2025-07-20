#include "piece.h"
#include "raylib.h"

Piece::Piece()
{
	cellSize = 40;
	rotationState = 0;
	colors = GetColors();
	rowOffset = 0;
	colOffset = 0;
}

void Piece::Draw()
{
	std::vector<Position> cellPositions = GetCellPositions();
	for (Position pos : cellPositions)
	{
		DrawRectangle(pos.col * cellSize + 1 + 15, pos.row * cellSize + 1 + 15, cellSize - 1, cellSize - 1, colors[type]);
	}
}

void Piece::Draw(int offsetX, int offsetY)
{
	std::vector<Position> cellPositions = GetCellPositions();
	for (Position pos : cellPositions)
	{
		DrawRectangle(pos.col * cellSize + offsetX, pos.row * cellSize + offsetY, cellSize - 1, cellSize - 1, colors[type]);
	}
}

void Piece::Move(int rows, int cols)
{
	rowOffset += rows;
	colOffset += cols;
}

void Piece::Rotate()
{
	rotationState++;
	if (rotationState >= cells.size()) rotationState = 0;
}

void Piece::UndoRotation()
{
	rotationState--;
	if (rotationState < 0) rotationState = cells.size() - 1;
}

std::vector<Position> Piece::GetCellPositions()
{
	std::vector<Position> cellPositions = cells[rotationState];
	std::vector<Position> movedTiles;
	for (Position pos : cellPositions)
	{
		Position newPos = Position(pos.row + rowOffset, pos.col + colOffset);
		movedTiles.push_back(newPos);
	}
	return movedTiles;
}

