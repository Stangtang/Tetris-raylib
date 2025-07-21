#include "game.h"
#include <random>

Game::Game()
{
	grid = Grid();
	Pieces = GetAllPieces();
	currentPiece = GetRandomPiece();
	nextPiece = GetRandomPiece();
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

Piece Game::GetRandomPiece()
{
	if (Pieces.empty()) 
	{
		Pieces = GetAllPieces();
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> randPiece(0, Pieces.size() - 1);

	int randomIndex = randPiece(gen);
	Piece piece = Pieces[randomIndex];
	Pieces.erase(Pieces.begin() + randomIndex);
	return piece;
}

std::vector<Piece> Game::GetAllPieces()
{
	return { IPiece(), JPiece(), LPiece(), OPiece(), SPiece(), TPiece(), ZPiece() };
}

void Game::Draw()
{
	grid.Draw();
	currentPiece.Draw();
}

void Game::HandleInput()
{
	TimePoint now = Clock::now();

	if (gameOverFlag && IsKeyPressed(KEY_Z))
	{
		resetGame();
		return;
	}

	if (IsKeyDown(KEY_LEFT) && now - lastMoveLeftTime > moveDelay)
	{
		MovePieceLeft();
		lastMoveLeftTime = now;
	}
	if (IsKeyDown(KEY_RIGHT) && now - lastMoveRightTime > moveDelay)
	{
		MovePieceRight();
		lastMoveRightTime = now;
	}
	if (IsKeyDown(KEY_DOWN) && now - lastMoveDownTime > softDropDelay)
	{
		MovePieceDown();
		updateScore(0, 1);
		lastMoveDownTime = now;
	}
	if (IsKeyDown(KEY_Z) && now - lastRotateTime > rotateDelay)
	{
		RotatePiece();
		lastRotateTime = now;
	}

	if (IsKeyPressed(KEY_X))
	{
		DropPiece();
	}

	if (IsKeyPressed(KEY_R))
	{
		resetGame();
	}
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
		updateScore(0, 1);
	}
	currentPiece.Move(-1, 0);
	updateScore(0, -1);
	AnchorPiece();
}

bool Game::IsPieceOutsideGrid()
{
	std::vector<Position> cellPositions = currentPiece.GetCellPositions();
	for (Position pos : cellPositions)
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
	for (Position pos : cellPositions)
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

	int linesCleared = grid.ClearFullRows();
	if (linesCleared > 0)
	{
		PlaySound(clearSound);
		updateScore(linesCleared, 0);
	}

	// lastMoveLeftTime = lastMoveRightTime = lastMoveDownTime = lastRotateTime = Clock::now();
}

bool Game::IsPieceOverlapping()
{
	std::vector<Position> cellPositions = currentPiece.GetCellPositions();
	for (Position pos : cellPositions)
	{
		if (!grid.IsCellEmpty(pos.row, pos.col))
		{
			return true;
		}
	}
	return false;
}

void Game::resetGame()
{
	grid.Init();
	Pieces = GetAllPieces();
	currentPiece = GetRandomPiece();
	nextPiece = GetRandomPiece();
	gameOverFlag = false;
	score = 0;
	// lastMoveLeftTime = lastMoveRightTime = lastMoveDownTime = lastRotateTime = Clock::now();
}

void Game::updateScore(int linesCleared, int timesMovedDown)
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
}