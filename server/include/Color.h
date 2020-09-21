#ifndef CHESS_COLOR_H_
#define CHESS_COLOR_H_

#include <string>

enum class Color {Black, White};
std::string colorName(Color);
Color otherColor(Color);

#endif
