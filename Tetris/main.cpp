#include "raylib.h"
#include "game.h"
#include <chrono>
#include "text.h"
#include <iostream>

int main()
{
	InitWindow(15 + 400 + 250, 15 + 800 + 15, "Tetris");
	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
	
	LoadFonts();

	Game game = Game();

	while (!WindowShouldClose())
	{
		game.ProcessInput();
		game.AutoDropPiece();

		BeginDrawing();
			ClearBackground(LIGHTGRAY);

			DrawSidePanel(game);

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