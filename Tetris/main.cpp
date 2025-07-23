#include "raylib.h"
#include "game.h"
#include <chrono>
#include "text.h"
#include <iostream>

// for debug
//void PrintPieceSequence(Game& game)
//{
//	std::cout << "Piece sequence: ";
//	for (int i = 0; i < 14; i++) {  // 2 full bags
//		Piece p = game.GetRandomPiece();
//		std::cout << p.type << " ";
//	}
//	std::cout << "\n";
//}

auto lastPieceLoweringTime = std::chrono::steady_clock::now();
bool ShouldLowerPiece(std::chrono::steady_clock::time_point& lastPieceLoweringTime)
{
	using namespace std::chrono;
	auto now = steady_clock::now();
	auto elapsed = duration_cast<milliseconds>(now - lastPieceLoweringTime);
	if (elapsed.count() >= 600) { // lower piece every 600 ms
		lastPieceLoweringTime = now;
		return true;
	}
	return false;
}

int main()
{
	InitWindow(15 + 400 + 250, 15 + 800 + 15, "Tetris");
	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
	
	LoadFonts();

	Game game = Game();

	while (!WindowShouldClose())
	{
		game.HandleInput();

		if (ShouldLowerPiece(lastPieceLoweringTime))
		{
			game.MovePieceDown();
		}

		BeginDrawing();
			ClearBackground(LIGHTGRAY);

			DrawScore(game); 
			DrawNext(game);
			DrawHeld(game, game.heldPieceExists);

			game.Draw();

			if (game.gameOverFlag)
			{
				DrawGameOver();
			}

		EndDrawing();
	}
	UnloadFonts();
	CloseWindow();

	return 0;
}