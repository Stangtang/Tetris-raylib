#pragma once

#include "raylib.h"
#include "game.h"

extern int fontSize;
extern Font font;
extern int scoreFontSize;
extern Font scoreFont;
extern int gameOverFontSize;
extern Font gameOverFont;

void LoadFonts();
void DrawOutlinedText(Font font, const char* text, Vector2 position, float fontSize, float spacing, Color color, int outlineSize, Color outlineColor);
void DrawScore(const Game& game);
