#pragma once

#include "raylib.h"
#include "game.h"

extern const int fontSize;
extern Font font;
extern const int scoreFontSize;
extern Font scoreFont;
extern const int gameOverFontSize;
extern Font gameOverFont;

extern Vector2 scoreTextSize;
extern Vector2 nextTextSize;
extern Vector2 heldTextSize;
extern Vector2 gameOverTextSize;

void LoadFonts();
void UnloadFonts();
void DrawOutlinedText(Font font, const char* text, Vector2 position, const float& fontSize, const float& spacing, Color color, const int& outlineSize, Color outlineColor);
void DrawSidePanel(const Game& game);
void DrawScore(const Game& game);
void DrawNext(const Game& game);
void DrawHeld(const Game& game, const bool& heldPieceExists);
void DrawGameOver();