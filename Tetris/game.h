#pragma once

#include "grid.h"
#include "pieces.cpp"
#include <random>
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
	void HoldPiece();
	Piece currentPiece;
	Piece nextPiece;
	Piece heldPiece;
	bool heldPieceExists;
	bool canHoldPiece;
	bool gameOverFlag;
	int score;

private:
	std::mt19937 randomEngine;
	std::vector<Piece> pieceBag;
	Piece GetRandomPiece();
	std::vector<Piece> GetAllPieces();
	void MovePieceLeft();
	void MovePieceRight();
	Grid grid;
	void ResetGame();
	bool IsPieceOutsideGrid();
	void RotatePiece();
	void AnchorPiece();
	bool IsPieceOverlapping();
	void UpdateScore(int linesCleared, int timesMovedDown);

	Sound rotateSound;
	Sound clearSound;
	Sound gameoverSound;

	using Clock = std::chrono::steady_clock;
	using TimePoint = std::chrono::time_point<Clock>;
	std::chrono::milliseconds moveDelay{ 110 };
	std::chrono::milliseconds rotateDelay{ 180 };
	std::chrono::milliseconds softDropDelay{ 110 };
	TimePoint lastMoveLeftTime = Clock::now();
	TimePoint lastMoveRightTime = Clock::now();
	TimePoint lastMoveDownTime = Clock::now();
	TimePoint lastRotateTime = Clock::now();
};