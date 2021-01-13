/*
	Colour.h
*/

#pragma once

// struct for storing colours
struct Colour {
	float r, g, b;
};
bool operator==(Colour c1, Colour c2) {
	return (c1.r == c2.r && c1.g == c2.g && c1.b == c2.b);
}

bool operator!=(Colour c1, Colour c2) {
	return (c1.r != c2.r || c1.g != c2.g || c1.b != c2.b);
}