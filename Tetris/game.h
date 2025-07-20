#pragma once

#include "grid.h"
#include "pieces.cpp"
class Game
{
public:
	Game();
	~Game();
	void Draw();
	void HandleInput();
	void MovePieceDown();
	void DropPiece();
	Piece currentPiece;
	Piece nextPiece;
	bool gameOverFlag;
	int score;

private:
	Piece GetRandomPiece();
	std::vector<Piece> GetAllPieces();
	void MovePieceLeft();
	void MovePieceRight();
	Grid grid;
	void resetGame();
	std::vector<Piece> Pieces;
	bool IsPieceOutsideGrid();
	void RotatePiece();
	void AnchorPiece();
	bool IsPieceOverlapping();
	void updateScore(int linesCleared, int timesMovedDown);
	Sound rotateSound;
	Sound clearSound;
};