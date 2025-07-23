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
	static const std::vector<Piece> pieces = { IPiece(), JPiece(), LPiece(), OPiece(), SPiece(), TPiece(), ZPiece() };
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

void Game::HandleInput()
{
	TimePoint now = Clock::now();

	if (gameOverFlag)
	{
		if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_R)) ResetGame();
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
																	 UpdateScore(0, 1); });
	processMovement(KEY_Z, lastRotateTime, rotateDelay, [&] { RotatePiece(); });

	if (IsKeyPressed(KEY_X)) DropPiece();
	if (IsKeyPressed(KEY_R)) ResetGame();
	if (IsKeyPressed(KEY_C) && canHoldPiece) HoldPiece();
}

void Game::MovePieceLeft()
{
	if (gameOverFlag) return;

	currentPiece.Move(0, -1);
	if (IsPieceOutsideGrid() || IsPieceOverlapping()) currentPiece.Move(0, 1);
}

void Game::MovePieceRight()
{
	if (gameOverFlag) return;

	currentPiece.Move(0, 1);
	if (IsPieceOutsideGrid() || IsPieceOverlapping()) currentPiece.Move(0, -1);
}

void Game::MovePieceDown()
{
	if (gameOverFlag) return;

	currentPiece.Move(1, 0);
	if (IsPieceOutsideGrid() || IsPieceOverlapping())
	{
		currentPiece.Move(-1, 0);
		AnchorPiece();
	}
}

void Game::RotatePiece()
{
	if (gameOverFlag) return;

	currentPiece.Rotate();
	if (IsPieceOutsideGrid() || IsPieceOverlapping())
	{
		currentPiece.UndoRotation();
	}
	else
	{
		PlaySound(rotateSound);
	}
}

void Game::DropPiece()
{
	if (gameOverFlag) return;

	while (!IsPieceOutsideGrid() && !IsPieceOverlapping())
	{
		currentPiece.Move(1, 0);
		UpdateScore(0, 1);
	}
	currentPiece.Move(-1, 0);
	UpdateScore(0, -1);
	AnchorPiece();
}

void Game::HoldPiece()
{
	if (gameOverFlag) return;

	if (!heldPieceExists)
	{
		heldPiece = currentPiece;
		heldPiece = heldPiece.GetNewPieceCopy();
		currentPiece = nextPiece;
		nextPiece = GetRandomPiece();
		heldPieceExists = true;
		canHoldPiece = false;
		return;
	}

	std::swap(heldPiece, currentPiece);
	heldPiece = heldPiece.GetNewPieceCopy();
	canHoldPiece = false;
}

bool Game::IsPieceOutsideGrid()
{
	std::vector<Position> cellPositions = currentPiece.GetCellPositions();
	for (const Position& pos : cellPositions)
	{
		if (grid.IsCellOutsideGrid(pos.row, pos.col))
		{
			return true;
		}
	}
	return false;
}

void Game::AnchorPiece()
{
	std::vector<Position> cellPositions = currentPiece.GetCellPositions();
	for (const Position& pos : cellPositions)
	{
		grid.grid[pos.row][pos.col] = currentPiece.type;
	}

	currentPiece = nextPiece;
	if (IsPieceOverlapping())
	{
		gameOverFlag = true;
		PlaySound(gameoverSound);
	}
	nextPiece = GetRandomPiece();
	canHoldPiece = true;

	const unsigned short linesCleared = grid.ClearFullRows();
	if (linesCleared > 0)
	{
		PlaySound(clearSound);
		UpdateScore(linesCleared, 0);
	}

	// lastMoveLeftTime = lastMoveRightTime = lastMoveDownTime = lastRotateTime = Clock::now();
}

bool Game::IsPieceOverlapping()
{
	std::vector<Position> cellPositions = currentPiece.GetCellPositions();
	for (const Position& pos : cellPositions)
	{
		if (!grid.IsCellEmpty(pos.row, pos.col))
		{
			return true;
		}
	}
	return false;
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
	// lastMoveLeftTime = lastMoveRightTime = lastMoveDownTime = lastRotateTime = Clock::now();
}

void Game::UpdateScore(const unsigned short& linesCleared, const short& timesMovedDown)
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
	score += timesMovedDown;

	autoDropInterval = std::chrono::milliseconds{ std::max(finalAutoDropInterval.count(), static_cast<long long> (initialAutoDropInterval.count() - score * autoDropIntervalDecreasePerScore)) };
}

void Game::AutoDropPiece()
{
	if (ShouldLowerPiece()) MovePieceDown();
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