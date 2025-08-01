#include "game.h"
#include <algorithm>

Game::Game()
{
	grid = Grid();
	std::random_device rd;
	randomPieceEngine = std::mt19937(rd());
	RefillPieceBag();
	std::shuffle(pieceBag.begin(), pieceBag.end(), randomPieceEngine);
	currentPiece = GetRandomPiece();
	nextPiece = GetRandomPiece();
	heldPiece;
	heldPieceExists = false;
	canHoldPiece = true;
	gameOverFlag = false;
	score = 0;

	InitAudioDevice();
	rotateSound = LoadSound("Sounds/rotate1.wav");
	clearSound = LoadSound("Sounds/clear1.wav");
	SetSoundVolume(clearSound, 0.5);
	gameoverSound = LoadSound("Sounds/game_over1.wav");
	SetSoundVolume(gameoverSound, 0.5);
}

Game::~Game()
{
	UnloadSound(rotateSound);
	UnloadSound(clearSound);
	UnloadSound(gameoverSound);
	CloseAudioDevice();
}

std::vector<Piece> Game::GetAllPieces()
{
	static const std::vector<Piece> pieces = { IPiece(), OPiece(), TPiece(), LPiece(), JPiece(), SPiece(), ZPiece() };
	return pieces;
}

Piece Game::GetRandomPiece()
{
	if (pieceBag.empty()) {
		RefillPieceBag();
	}

	Piece piece = pieceBag.back();
	pieceBag.pop_back();
	return piece;
}

void Game::RefillPieceBag()
{
	pieceBag = GetAllPieces();
	std::shuffle(pieceBag.begin(), pieceBag.end(), randomPieceEngine);
}

void Game::Draw()
{
	grid.Draw();
	currentPiece.Draw();
}

void Game::ProcessInput()
{
	TimePoint now = Clock::now();

	if (gameOverFlag)
	{
		if (IsKeyPressed(KEY_R)) ResetGame();
		return;
	}

	const auto processMovement = [&](int key, auto& lastActionTime, const auto& delay, auto action) {
		if (IsKeyDown(key) && now - lastActionTime > delay) {
			action();
			lastActionTime = now;
		}
	};
	processMovement(KEY_LEFT, lastMoveLeftTime, moveDelay, [&] { MovePieceLeft(); });
	processMovement(KEY_RIGHT, lastMoveRightTime, moveDelay, [&] { MovePieceRight(); });
	processMovement(KEY_DOWN, lastMoveDownTime, softDropDelay, [&] { MovePieceDown();
																	 UpdateScoreMoveDown(1); });
	
	if (IsKeyPressed(KEY_X)) RotatePieceClockwise();
	if (IsKeyPressed(KEY_Z)) RotatePieceCounterclockwise();
	if (IsKeyPressed(KEY_A)) RotatePiece180();

	if (IsKeyPressed(KEY_SPACE)) DropPiece();
	if (IsKeyPressed(KEY_R)) ResetGame();
	if (IsKeyPressed(KEY_C) && canHoldPiece) HoldPiece();
}

void Game::MovePieceLeft()
{
	currentPiece.Move(0, -1);
	if (!IsPiecePositionValid()) currentPiece.Move(0, 1);
}

void Game::MovePieceRight()
{
	currentPiece.Move(0, 1);
	if (!IsPiecePositionValid()) currentPiece.Move(0, -1);
}

void Game::MovePieceDown()
{
	currentPiece.Move(1, 0);
	if (!IsPiecePositionValid())
	{
		currentPiece.Move(-1, 0);
		AnchorPiece();
	}
}

void Game::DropPiece()
{
	while (IsPiecePositionValid())
	{
		currentPiece.Move(1, 0);
		UpdateScoreMoveDown(1);
	}
	currentPiece.Move(-1, 0);
	UpdateScoreMoveDown(false);
	AnchorPiece();
}

void Game::RotatePieceClockwise()
{
	currentPiece.Rotate(1);
	if (IsPiecePositionValid() || currentPiece.type == 2)
	{
		PlaySound(rotateSound);
		return;
	}

	const int from = currentPiece.GetRotation(-1);
	const int to = currentPiece.GetRotation(0);
	TryWallKickTests(from, to);
}

void Game::RotatePieceCounterclockwise()
{
	currentPiece.Rotate(-1);
	if (IsPiecePositionValid() || currentPiece.type == 2)
	{
		PlaySound(rotateSound);
		return;
	}

	const int from = currentPiece.GetRotation(1);
	const int to = currentPiece.GetRotation(0);
	TryWallKickTests(from, to);
}

void Game::RotatePiece180()
{
	currentPiece.Rotate(2);
	if (IsPiecePositionValid() || currentPiece.type == 2)
	{
		PlaySound(rotateSound);
		return;
	}

	const int from = currentPiece.GetRotation(-2);
	const int to = currentPiece.GetRotation(0);
	TryWallKickTests(from, to);
}

void Game::TryWallKickTests(const int& from, const int& to)
{
	const auto wallKickTests = currentPiece.wallKickTable[std::make_pair(from, to)];
	for (auto dis : wallKickTests)
	{
		currentPiece.Move(dis.y * -1, dis.x);
		if (IsPiecePositionValid())
		{
			PlaySound(rotateSound);
			return;
		}
		currentPiece.Move(dis.y, dis.x * -1);
	}
}

void Game::HoldPiece()
{
	if (!heldPieceExists)
	{
		heldPiece = currentPiece;
		heldPiece = heldPiece.NewCopy();
		currentPiece = nextPiece;
		nextPiece = GetRandomPiece();
		heldPieceExists = true;
		canHoldPiece = false;
		return;
	}

	std::swap(heldPiece, currentPiece);
	heldPiece = heldPiece.NewCopy();
	canHoldPiece = false;
}

void Game::AnchorPiece()
{
	const std::vector<Position> cellPositions = currentPiece.GetCellPositions();
	for (const Position& pos : cellPositions)
	{
		grid.grid[pos.row][pos.col] = currentPiece.type;
	}

	currentPiece = nextPiece;
	if (IsPieceOverlapping())
	{
		while (IsPieceOverlapping())
		{
			currentPiece.Move(-1, 0);
		}

		gameOverFlag = true;
		PlaySound(gameoverSound);
		return;
	}
	nextPiece = GetRandomPiece();
	canHoldPiece = true;

	const unsigned short linesCleared = grid.ClearFullRows();
	if (linesCleared > 0)
	{
		PlaySound(clearSound);
		UpdateScoreLineClear(linesCleared);
	}
}

bool Game::IsPieceOutsideGrid()
{
	const std::vector<Position> cellPositions = currentPiece.GetCellPositions();
	for (const Position& pos : cellPositions)
	{
		if (grid.IsCellOutsideGrid(pos.row, pos.col))
		{
			return true;
		}
	}
	return false;
}

bool Game::IsPieceOverlapping()
{
	const std::vector<Position> cellPositions = currentPiece.GetCellPositions();
	for (const Position& pos : cellPositions)
	{
		if (!grid.IsCellEmpty(pos.row, pos.col))
		{
			return true;
		}
	}
	return false;
}

bool Game::IsPiecePositionValid()
{
	const std::vector<Position>& cellPositions = currentPiece.GetCellPositions();
	for (const Position& pos : cellPositions)
	{
		if (grid.IsCellOutsideGrid(pos.row, pos.col) || !grid.IsCellEmpty(pos.row, pos.col))
		{
			return false;
		}
	}
	return true;
}

void Game::ResetGame()
{
	grid.Init();
	RefillPieceBag();
	currentPiece = GetRandomPiece();
	nextPiece = GetRandomPiece();
	heldPieceExists = false;
	canHoldPiece = true;
	gameOverFlag = false;
	score = 0;
	autoDropInterval = initialAutoDropInterval;
	lastPieceLoweringTime = Clock::now();
}

void Game::UpdateScoreLineClear(const unsigned short& linesCleared)
{
	switch (linesCleared)
	{
	case 1:
		score += 100;
		break;
	case 2:
		score += 300;
		break;
	case 3:
		score += 500;
		break;
	case 4:
		score += 800;
		break;
	default:
		break;
	}
	UpdateAutoDropInterval();
}

void Game::UpdateScoreMoveDown(const bool& movedDown)
{
	score += 2 * movedDown - 1;
	UpdateAutoDropInterval();
}

void Game::UpdateAutoDropInterval()
{
	// only recalculate every 100 points
	if (!(score / 100 > lastIntervalScore / 100)) return;

	autoDropInterval = std::chrono::milliseconds{ std::max(finalAutoDropInterval.count(), static_cast<long long> (initialAutoDropInterval.count() - score * autoDropIntervalDecreasePerScore)) };
	lastIntervalScore = score;
}

void Game::AutoDropPiece()
{
	if (ShouldLowerPiece() && !gameOverFlag) MovePieceDown();
}

bool Game::ShouldLowerPiece()
{
	using namespace std::chrono;
	const auto now = steady_clock::now();
	const auto elapsed = duration_cast<milliseconds>(now - lastPieceLoweringTime);
	if (elapsed.count() >= autoDropInterval.count()) {
		lastPieceLoweringTime = now;
		return true;
	}
	return false;
}