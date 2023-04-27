#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>

typedef enum color {
	RED = 41,
	GREEN,
	YELLOW,
	BLUE,
	PURPLE,
	CYAN,
	WHITE,
	BLACK = 0,
	ORANGE = 404, // to be modified
}Color;

typedef enum shapeID {
	EMPTY = -1,
	I,
	J,
	L,
	O,
	S,
	T,
	Z,
}ShapeID;

typedef struct {
	ShapeID shape;
	Color color;
	int size;
	char rotates[4][4][4];
} Shape;

Shape shape[7] = {
	{.shape = I, .color = CYAN, .size = 4, .rotates = {{{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
													   {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
													   {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}},
													   {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}}}},
	{.shape = J, .color = BLUE, .size = 3, .rotates = {{{1, 0, 0}, {1, 1, 1}, {0, 0, 0}},
													   {{0, 1, 1}, {0, 1, 0}, {0, 1, 0}},
													   {{0, 0, 0}, {1, 1, 1}, {0, 0, 1}},
													   {{1, 1, 0}, {0, 1, 0}, {0, 1, 0}}}},
	{.shape = L, .color = ORANGE, .size = 3, .rotates = {{{0, 0, 1}, {1, 1, 1}, {0, 0, 0}}, 
														{{0, 1, 0}, {0, 1 ,0}, {1, 1, 0}},
														{{0, 0, 0}, {1, 1, 1}, {1, 0, 0}},
														{{1, 1, 0}, {0, 1, 0}, {0, 1, 0}}}},
	{.shape = O, .color = YELLOW, .size = 2, .rotates = {{{1, 1}, {1, 1}},
														 {{1, 1}, {1, 1}},
														 {{1, 1}, {1, 1}},
														 {{1, 1}, {1, 1}}}},
	{.shape = S, .color = GREEN, .size = 3, .rotates = {{{0, 1, 1}, {1, 1, 0}, {0, 0, 0}},
														{{0, 1, 0}, {0, 1, 1}, {0, 0, 1}},
														{{0, 0, 0}, {0, 1, 1}, {1, 1, 0}},
														{{1, 0, 0}, {1, 1, 0}, {0, 1, 0}}}},
	{.shape = T, .color = PURPLE, .size = 3, .rotates = {{{0, 1, 0}, {1, 1, 1}, {0, 0, 0}},
														 {{0, 1, 0}, {0, 1, 1}, {0, 1, 0}},
														 {{0, 0, 0}, {1, 1, 1}, {0, 1, 0}},
														 {{0, 1, 0}, {1, 1, 0}, {0, 1, 0}}}},
	{.shape = Z, .color = RED, .size = 3, .rotates = {{{1, 1, 0}, {0, 1, 1}, {0, 0, 0}},
													  {{0, 0, 1}, {0, 1, 1}, {1, 0, 0}},
													  {{0, 0 ,0}, {1, 1, 0}, {0, 1, 1}},
													  {{0, 1, 0}, {1, 1, 0}, {1, 0 , 0}}}},
};

typedef struct {
	Color color;
	ShapeID shape;
}Block;


int main() {
	return 0;
}