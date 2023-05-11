// define constants
#define CANVAS_HEIGHT 20
#define CANVAS_WIDTH 10
#define LEFT_KEY 0x25
#define RIGHT_KEY 0x27
#define ROTATE_KEY 0x26
#define SOFT_DROP_KEY 0x28
#define HARD_DROP_KEY 0x20

// include libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// system detection
#ifdef _WIN32
#include <windows.h>
#define CLS() system("cls")
#define LEFT_FUNC() GetAsyncKeyState(LEFT_KEY) & 0x8000
#define RIGHT_FUNC() GetAsyncKeyState(RIGHT_KEY) & 0x8000
#define ROTATE_FUNC() GetAsyncKeyState(ROTATE_KEY) & 0x8000
#define SOFT_DROP_FUNC() GetAsyncKeyState(SOFT_DROP_KEY) & 0x8000
#define HARD_DROP_FUNC() GetAsyncKeyState(HARD_DROP_KEY) & 0x8000
#define SLEEP(x) Sleep(x)
#elif __APPLE__
#error "MacOS is not supported."
#elif __linux__ 
#error "Linux is not supported."
#endif

typedef enum color {
	RED = 1,
	GREEN,
	YELLOW = 220,
	BLUE = 21,
	PURPLE = 93,
	CYAN = 6,
	WHITE = 231,
	BLACK = 0,
	ORANGE = 208, // to be modified
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

typedef struct {
	int x;
	int y;
	int score;
	int rotate;
	int falltime;
	ShapeID queue[4];
}State;

typedef struct {
	Color color;
	ShapeID shape;
	bool current;
}Block;
 
Shape shape[7] = {
	{.shape = I, .color = CYAN, .size = 4, .rotates = {{{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
													   {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
													   {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}},
													   {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}}}},
	{.shape = J, .color = BLUE, .size = 3, .rotates = {{{1, 0, 0}, {1, 1, 1}, {0, 0, 0}},
													   {{0, 1, 0}, {0, 1, 0}, {0, 1, 1}},
													   {{0, 0, 0}, {1, 1, 1}, {1, 0, 0}},
													   {{1, 1, 0}, {0, 1, 0}, {0, 1, 0}}}},
	{.shape = L, .color = ORANGE, .size = 3, .rotates = {{{0, 0, 1}, {1, 1, 1}, {0, 0, 0}}, 
														{{0, 1, 0}, {0, 1 ,0}, {0, 1, 1}},
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

void setBlock(Block* block, Color color, ShapeID shape, bool current) {
	block->color = color;
	block->shape = shape;
	block->current = current;
}

void resetBlock(Block* block) {
	block->color = BLACK;
	block->shape = EMPTY;
	block->current = false;
}

// bool move(int height, int width, Block canvas[height][width], int originalX, int originalY, int originalRotate, int newX, int newY, int newRotate, ShapeID shapeId) {
// 
// }

int main() {
	srand(tim(NULL));
	State state = {
		.x = CANVAS_HEIGHT,
		.y = 0,
		.score = 0,
		.rotate = 0,
		.falltime = 0
	};
	Block canvas[CANVAS_HEIGHT][CANVAS_WIDTH];
	for (int i = 0; i < CANVAS_HEIGHT; i++) {
		for (int j = 0; j < CANVAS_WIDTH; j++) {
			resetBlock(&canvas[i][j]);
		}
	}

	system("cls");
	printf("\e[?25l"); // hide cursor */
	return 0;
}