#include "text.h"
#include <iostream>
#include "piece.h"
#include "game.h"

int fontSize = 50;
Font font;
int scoreFontSize = 45;
Font scoreFont;
int gameOverFontSize = 125;
Font gameOverFont;

void LoadFonts()
{
	font = LoadFontEx("Font/monogram.ttf", fontSize, 0, 0);
	scoreFont = LoadFontEx("Font/monogram.ttf", scoreFontSize, 0, 0);
	gameOverFont = LoadFontEx("Font/monogram.ttf", gameOverFontSize, 0, 0);
}

void DrawOutlinedText(Font font, const char* text, Vector2 position, float fontSize, float spacing, Color color, int outlineSize, Color outlineColor) {
	Vector2 offsets[] = {
		{-outlineSize, -outlineSize},
		{ outlineSize, -outlineSize},
		{-outlineSize,  outlineSize},
		{ outlineSize,  outlineSize}
	};

	for (Vector2 offset : offsets) {
		Vector2 outlinePos = { position.x + offset.x, position.y + offset.y };
		DrawTextEx(font, text, outlinePos, fontSize, spacing, outlineColor);
	}

	DrawTextEx(font, text, position, fontSize, spacing, color);
}

void DrawScore(const Game& game)
{
	DrawTextEx(font, "Score", { 540 - MeasureTextEx(font, "Score", fontSize, 2).x / 2, 15 }, fontSize, 2, BLACK);
	DrawRectangleRounded({ 540 - 225 / 2, 80, 225, 80 }, 0.3, 6, GRAY);
	char scoreText[10];
	snprintf(scoreText, sizeof(scoreText), "%d", game.score);
	DrawTextEx(scoreFont, scoreText, { 540 - MeasureTextEx(scoreFont, scoreText, scoreFontSize, 2).x / 2, (80 + 80) / 2 + 13 }, scoreFontSize, 2, BLACK);
}