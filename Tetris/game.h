#pragma once

#include "grid.h"
#include "pieces.cpp"
#include <chrono>

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

	using Clock = std::chrono::steady_clock;
	using TimePoint = std::chrono::time_point<Clock>;
	std::chrono::milliseconds moveDelay{ 80 };
	std::chrono::milliseconds rotateDelay{ 180 };
	std::chrono::milliseconds softDropDelay{ 120 };
	TimePoint lastMoveLeftTime = Clock::now();
	TimePoint lastMoveRightTime = Clock::now();
	TimePoint lastMoveDownTime = Clock::now();
	TimePoint lastRotateTime = Clock::now();
};