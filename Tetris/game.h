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
	void AutoDropPiece();
	Piece currentPiece;
	Piece nextPiece;
	Piece heldPiece;
	bool heldPieceExists;
	bool canHoldPiece;
	bool gameOverFlag;
	unsigned long long score;

private:
	std::mt19937 randomPieceEngine;
	std::vector<Piece> pieceBag;
	Piece GetRandomPiece();
	void RefillPieceBag();
	std::vector<Piece> GetAllPieces();
	void MovePieceLeft();
	void MovePieceRight();
	Grid grid;
	void ResetGame();
	bool IsPieceOutsideGrid();
	void RotatePiece();
	void AnchorPiece();
	bool IsPieceOverlapping();
	void UpdateScore(const unsigned short& linesCleared, const short& timesMovedDown);

	Sound rotateSound;
	Sound clearSound;
	Sound gameoverSound;

	using Clock = std::chrono::steady_clock;
	using TimePoint = std::chrono::time_point<Clock>;

	const std::chrono::milliseconds initialAutoDropInterval{ 600 };
	const std::chrono::milliseconds finalAutoDropInterval{ 50 };
	const long long finalAutoDropIntervalScore = 20000;
	TimePoint lastPieceLoweringTime = Clock::now();
	std::chrono::milliseconds autoDropInterval = initialAutoDropInterval;
	const double autoDropIntervalDecreasePerScore = static_cast<double> (finalAutoDropInterval.count() - initialAutoDropInterval.count()) / finalAutoDropIntervalScore * -1;
	bool ShouldLowerPiece();

	const std::chrono::milliseconds moveDelay{ 110 };
	const std::chrono::milliseconds rotateDelay{ 180 };
	const std::chrono::milliseconds softDropDelay{ 110 };
	TimePoint lastMoveLeftTime = Clock::now();
	TimePoint lastMoveRightTime = Clock::now();
	TimePoint lastMoveDownTime = Clock::now();
	TimePoint lastRotateTime = Clock::now();
};