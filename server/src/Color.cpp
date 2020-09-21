#include "Color.h"

std::string colorName(Color color) {
	return color == Color::White? "white" : "black";
}

Color otherColor(Color color) {
	return color == Color::White? Color::Black : Color::White;
}
