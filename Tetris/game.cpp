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
	SetSoundVolume(clearSound, 0.3);

}

Game::~Game()
{
	UnloadSound(rotateSound);
	UnloadSound(clearSound);
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
	std::uniform_int_distribution<> randBlock(0, Pieces.size() - 1);

	int randomIndex = randBlock(gen);
	Piece block = Pieces[randomIndex];
	Pieces.erase(Pieces.begin() + randomIndex);
	return block;
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
	int inputKey = GetKeyPressed();

	if (gameOverFlag && inputKey == KEY_Z)
	{
		resetGame();
	}

	switch (inputKey)
	{
	case KEY_LEFT:
		MovePieceLeft();
		break;
	case KEY_RIGHT:
		MovePieceRight();
		break;
	case KEY_DOWN:
		MovePieceDown();
		updateScore(0, 1);
		break;
	case KEY_Z:
		RotatePiece();
		break;
	case KEY_X:
		DropPiece();
		break;
	case KEY_R:
		resetGame();
		break;
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
	std::vector<Position> tiles = currentPiece.GetCellPositions();
	for (Position item : tiles)
	{
		if (grid.IsCellOutsideGrid(item.row, item.col))
		{
			return true;
		}
	}
	return false;
}

void Game::AnchorPiece()
{
	std::vector<Position> tiles = currentPiece.GetCellPositions();
	for (Position item : tiles)
	{
		grid.grid[item.row][item.col] = currentPiece.type;
	}
	currentPiece = nextPiece;
	if (IsPieceOverlapping())
	{
		gameOverFlag = true;
	}
	nextPiece = GetRandomPiece();

	int linesCleared = grid.ClearFullRows();
	if (linesCleared > 0)
	{
		PlaySound(clearSound);
		updateScore(linesCleared, 0);
	}
}

bool Game::IsPieceOverlapping()
{
	std::vector<Position> tiles = currentPiece.GetCellPositions();
	for (Position item : tiles)
	{
		if (!grid.IsCellEmpty(item.row, item.col))
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