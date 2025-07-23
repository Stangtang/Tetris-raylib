#include "text.h"
#include <iostream>

const int fontSize = 50;
Font font;
const int scoreFontSize = 45;
Font scoreFont;
const int gameOverFontSize = 125;
Font gameOverFont;

Vector2 scoreTextSize;
Vector2 nextTextSize;
Vector2 heldTextSize;
Vector2 gameOverTextSize;

void LoadFonts()
{
	font = LoadFontEx("Font/monogram.ttf", fontSize, 0, 0);
	scoreFont = LoadFontEx("Font/monogram.ttf", scoreFontSize, 0, 0);
	gameOverFont = LoadFontEx("Font/monogram.ttf", gameOverFontSize, 0, 0);

	scoreTextSize = MeasureTextEx(font, "Score", fontSize, 2);
	nextTextSize = MeasureTextEx(font, "Next", fontSize, 2);
	heldTextSize = MeasureTextEx(font, "Held", fontSize, 2);
	gameOverTextSize = MeasureTextEx(font, "GAME OVER", gameOverFontSize, 5);
}

void UnloadFonts() {
	UnloadFont(font);
	UnloadFont(scoreFont);
	UnloadFont(gameOverFont);
}

void DrawOutlinedText(Font font, const char* text, Vector2 position, const float& fontSize, const float& spacing, Color color, const int& outlineSize, Color outlineColor) {
	const Vector2 offsets[] = {
		{-outlineSize, -outlineSize},
		{ outlineSize, -outlineSize},
		{-outlineSize,  outlineSize},
		{ outlineSize,  outlineSize}
	};

	for (const Vector2& offset : offsets) {
		const Vector2 outlinePos = { position.x + offset.x, position.y + offset.y };
		DrawTextEx(font, text, outlinePos, fontSize, spacing, outlineColor);
	}

	DrawTextEx(font, text, position, fontSize, spacing, color);
}

void DrawSidePanel(const Game& game)
{
	DrawScore(game);
	DrawNext(game);
	DrawHeld(game, game.heldPieceExists);
}

void DrawScore(const Game& game)
{
	DrawTextEx(font, "Score", { 540 - scoreTextSize.x / 2, 5 }, fontSize, 2, BLACK);
	DrawRectangleRounded({ 540 - 225 / 2, 70, 225, 80 }, 0.3, 6, GRAY);
	char scoreNumberText[10];
	snprintf(scoreNumberText, sizeof(scoreNumberText), "%d", game.score);
	DrawTextEx(scoreFont, scoreNumberText, { 540 - MeasureTextEx(scoreFont, scoreNumberText, scoreFontSize, 2).x / 2, (70 + 80) / 2 + 13 }, scoreFontSize, 2, BLACK);
}

void DrawNext(const Game& game)
{
	DrawTextEx(font, "Next", { 540 - nextTextSize.x / 2, 190 }, fontSize, 2, BLACK);
	DrawRectangleRounded({ 540 - 225 / 2, 190 + 65, 225, 225 }, 0.3, 6, GRAY);

	Piece piece = game.nextPiece;
	switch (piece.type)
	{
	case 1: // I-piece
		piece.Draw(360 - 19, 370 - 22);
		break;
	case 2: // O-piece
		piece.Draw(360 - 19, 330 - 2);
		break;
	default:
		piece.Draw(360 + 1, 330 - 2);
		break;
	}
}

void DrawHeld(const Game& game, const bool& heldPieceExists)
{
	DrawTextEx(font, "Held", { 540 - heldTextSize.x / 2, 515 }, fontSize, 2, BLACK);
	DrawRectangleRounded({ 540 - 225 / 2, 515 + 65, 225, 225 }, 0.3, 6, GRAY);

	if (!heldPieceExists) return;

	Piece piece = game.heldPiece;
	switch (piece.type)
	{
	case 1: // I-piece
		piece.Draw(360 - 19, 370 - 22 + 325);
		break;
	case 2: // O-piece
		piece.Draw(360 - 19, 330 - 2 + 325);
		break;
	default:
		piece.Draw(360 + 1, 330 - 2 + 325);
		break;
	}
}

void DrawGameOver()
{
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));
	DrawOutlinedText(gameOverFont, "GAME OVER", { (15 + 400 + 250) / 2 - gameOverTextSize.x / 2, (15 + 800 + 15) / 2 - gameOverTextSize.y / 2 - 6}, gameOverFontSize, 5, BLACK, 4, WHITE);
}
