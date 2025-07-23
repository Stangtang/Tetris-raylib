#include "colors.h"
#include <vector>

const Color darkGray = { 26, 31, 40, 255 };

const Color cyan = { 12, 230, 237, 255 };
const Color yellow = { 234, 233, 15, 255 };
const Color purple = { 166, 0, 247, 255 };
const Color green = { 37, 228, 12, 255 };
const Color red = { 232, 18, 18, 255 };
const Color blue = { 12, 64, 216, 255 };
const Color orange = { 226, 116, 17, 255 };

std::vector<Color> GetColors()
{
	return { darkGray, orange, blue, cyan, yellow, green, purple, red };
}