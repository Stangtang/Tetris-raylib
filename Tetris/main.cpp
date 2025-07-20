#include "raylib.h"
#include "game.h"
#include <chrono>
#include "text.h"

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

			DrawTextEx(font, "Next", { 540 - MeasureTextEx(font, "Next", fontSize, 2).x / 2, 200 }, fontSize, 2, BLACK);
			DrawRectangleRounded({ 540 - 225 / 2, 265, 225, 225 }, 0.3, 6, GRAY);
			switch (game.nextPiece.type)
			{
			case 3: // I-piece
				game.nextPiece.Draw(360 - 18, 380);
				break;
			case 4: // O-piece
				game.nextPiece.Draw(360 - 15, 340);
				break;
			default:
				game.nextPiece.Draw(360, 340);
				break;
			}

			game.Draw();

			if (game.gameOverFlag)
			{
				DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));
				// DrawTextEx(gameOverFont, "GAME OVER", { (15 + 400 + 250) / 2 - MeasureTextEx(font, "GAME OVER", gameOverFontSize, 5).x / 2, (15 + 800 + 15) / 2 - MeasureTextEx(font, "GAME OVER", gameOverFontSize, 5).y / 2 }, gameOverFontSize, 5, BLACK);
				DrawOutlinedText(gameOverFont, "GAME OVER", { (15 + 400 + 250) / 2 - MeasureTextEx(font, "GAME OVER", gameOverFontSize, 5).x / 2, (15 + 800 + 15) / 2 - MeasureTextEx(font, "GAME OVER", gameOverFontSize, 5).y / 2 }, gameOverFontSize, 5, BLACK, 4, WHITE);
			}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}