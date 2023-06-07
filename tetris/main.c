// TODO Fix render and clear 
// define constants
#define CANVAS_HEIGHT 20
#define CANVAS_WIDTH 10
#define RENDER_DELAY 75
#define FALL_DELAY 750

// define key maps
#define LEFT_KEY 0x25
#define RIGHT_KEY 0x27
#define ROTATE_KEY 0x26
#define SOFT_DROP_KEY 0x28
#define HARD_DROP_KEY 0x20
#define HOLD_KEY 0xA0

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
#define HOLD_FUNC() GetAsyncKeyState(HOLD_KEY) & 0x8000
#define SLEEP(x) Sleep(x)
#elif __APPLE__
#error "MacOS is not supported."
#elif __linux__ 
#error "Linux is not supported."
#endif

bool gameState = true;
int highestScore = 0;

typedef enum color { // color code
	RED = 1,
	GREEN,
	YELLOW = 220,
	BLUE = 21,
	PURPLE = 93,
	CYAN = 6,
	WHITE = 231,
	BLACK = 0,
	ORANGE = 208,
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

typedef struct { // define shape structure form
	ShapeID shape;
	Color color;
	int size;
	char rotates[4][4][4];
} Shape;

typedef struct { // define state structure form
	int x;
	int y;
	int score;
	int rotate;
	int falltime;
	bool isHold;
	ShapeID queue[5];
	ShapeID hold;
} State;

typedef struct { // define block structure form
	Color color;
	ShapeID shape;
	bool current;
} Block;

Shape shape[7] = { // shape illustrating
	{.shape = I, .color = CYAN, .size = 4, .rotates = {{{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
													   {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
													   {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}},
													   {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}}}},
	{.shape = J, .color = BLUE, .size = 3, .rotates = {{{1, 0, 0}, {1, 1, 1}, {0, 0, 0}},
													   {{0, 1, 0}, {0, 1, 0}, {1, 1, 0}},
													   {{0, 0, 0}, {1, 1, 1}, {0, 0, 1}},
													   {{0, 1, 1}, {0, 1, 0}, {0, 1, 0}}}},
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
													  {{0, 0, 1}, {0, 1, 1}, {0, 1, 0}},
													  {{0, 0 ,0}, {1, 1, 0}, {0, 1, 1}},
													  {{0, 1, 0}, {1, 1, 0}, {1, 0 , 0}}}},
};

void setBlock(Block* block, Color color, ShapeID shape, bool current) { // block setup function
	block->color = color;
	block->shape = shape;
	block->current = current;
}

void resetBlock(Block* block) { // block reset to empty function
	block->color = BLACK;
	block->shape = EMPTY;
	block->current = false;
}

bool move(Block canvas[CANVAS_HEIGHT][CANVAS_WIDTH], int originalX, int originalY, int originalRotate, int newX, int newY, int newRotate, ShapeID shapeID) {
	// move block function
	Shape shapeData = shape[shapeID];
	int size = shapeData.size;

	// check if the new position is valid to place the block 
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (shapeData.rotates[newRotate][i][j]) {
				if (newX + j < 0 || newX + j >= CANVAS_WIDTH || newY + i < 0 || newY + i >= CANVAS_HEIGHT) {
					return false;
				}
				if (!canvas[newY + i][newX + j].current && canvas[newY + i][newX + j].shape != EMPTY) {
					return false;
				}
			}
		}
	}

	// remove the old position
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (shapeData.rotates[originalRotate][i][j]) {
				resetBlock(&canvas[originalY + i][originalX + j]);
			}
		}
	}

	// move the block
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (shapeData.rotates[newRotate][i][j]) {
				setBlock(&canvas[newY + i][newX + j], shapeData.color, shapeID, true);
			}
		}
	}

	return true;
}

int clearLine(Block canvas[CANVAS_HEIGHT][CANVAS_WIDTH]) {
	for (int i = 0; i < CANVAS_HEIGHT; i++) {
		for (int j = 0; j < CANVAS_WIDTH; j++) {
			if (canvas[i][j].current) {
				canvas[i][j].current = false;
			}
		}
	}
	int linesCleared = 0;

	for (int i = CANVAS_HEIGHT - 1, isFull = true; i >= 0; isFull = true) {
		for (int j = 0; j < CANVAS_WIDTH; j++) {
			if (canvas[i][j].shape == EMPTY) {
				isFull = false;
				break;
			}
		}

		if (isFull) {
			linesCleared++;

			for (int j = 0; j < CANVAS_WIDTH; j++) {
				resetBlock(&canvas[i][j]);
			}

			for (int k = i; k > 0; k--) {
				for (int j = 0; j < CANVAS_WIDTH; j++) {
					setBlock(&canvas[k][j], canvas[k - 1][j].color, canvas[k - 1][j].shape, false);
					resetBlock(&canvas[k - 1][j]);
				}
			}
		} else {
			i--;
		}
	}

	return linesCleared;
}

void hold(Block canvas[CANVAS_HEIGHT][CANVAS_WIDTH], State* state) {
	if (!state->isHold && HOLD_FUNC()) {
		state->isHold = true;
		for (int i = 0; i < shape[state->queue[0]].size; i++) {
			for (int j = 0; j < shape[state->queue[0]].size; j++) {
				resetBlock(&canvas[state->y + i][state->x + j]);
			}
		}
		state->x = CANVAS_WIDTH / 2;
		state->y = 0;
		state->rotate = 0;
		state->falltime = 0;

		if (state->hold == EMPTY) {
			state->hold = state->queue[0];
			state->queue[0] = state->queue[1];
			state->queue[1] = state->queue[2];
			state->queue[2] = state->queue[3];
			state->queue[3] = state->queue[4];
			state->queue[4] = rand() % 7;
		}
		else {
			ShapeID tmp = state->hold;
			state->hold = state->queue[0];
			state->queue[0] = tmp;
		}
		move(canvas, state->x, state->y, state->rotate, state->x, state->y, state->rotate, state->queue[0]);
	}
}

void printStart() {
	printf("\033[?25l");
	printf("\n\n\n");
	printf("\t\033[48;5;12m ______    ______   ______    ______   __   ______   \n\t/\\__  __\\ /\\  ___\\ /\\__  __\\ /\\  __ \\ /\\ \\ /\\  ___\\  \n\t\\/_/\\ \\_/ \\ \\  ___\\\\/_/\\ \\_/ \\ \\    / \\ \\ \\\\ \\____ \\ \033[0m\n\t\033[48;5;227m   \\ \\_\\   \\ \\_____\\  \\ \\_\\   \\ \\_\\_\\  \\ \\ \\\\/\\_____\\\n\t    \\/_/    \\/_____/   \\/_/    \\/_/_/   \\/_/ \\/_____/\033[0m\n");
	printf("\n\n");
		
	printf("\t\tPress SPACE key to start game.");
	while (1) {
		if (_kbhit() && getch() == 0x20) {
			CLS();
			return;
		}
	}
}
/*
 ______    ______   ______    ______   __   ______
/\__  __\ /\  ___\ /\__  __\ /\  __ \ /\ \ /\  ___\
\/_/\ \_/ \ \  ___\\/_/\ \_/ \ \    / \ \ \\ \____ \
   \ \_\   \ \_____\  \ \_\   \ \_\_\  \ \ \\/\_____\
    \/_/    \/_____/   \/_/    \/_/_/   \/_/ \/_____/
*/

void printCanvas(Block canvas[CANVAS_HEIGHT][CANVAS_WIDTH], State* state) {
	printf("\033[0;0H\n");
	for (int i = 0; i < CANVAS_HEIGHT; i++) {
		printf("|");
		for (int j = 0; j < CANVAS_WIDTH; j++) {
			printf("\033[48;5;%dm\u3000\033[0m", canvas[i][j].color);
		}
		printf("\x1b[0m|\n");
	}

	// print next tetrimino
	Shape shapeData = shape[state->queue[1]];
	printf("\033[%d;%dHNEXT:", 3, CANVAS_WIDTH * 2 + 5);
	for (int i = 1; i <= 4; i++) {
		shapeData = shape[state->queue[i]];
		for (int j = 0; j < 4; j++) {
			printf("\033[%d;%dH", i * 4 + j, CANVAS_WIDTH * 2 + 15);
			for (int k = 0; k < 4; k++) {
				
				if (j < shapeData.size && k < shapeData.size && shapeData.rotates[0][j][k]) {
					printf("\033[48;5;%dm\u3000\033[0m", shapeData.color);
				}
				else {
					printf("\033[48;5;0m \033[0m");
					printf("\x1b[0m ");
				}
			}
		}
	}

	Shape holdData = shape[state->hold];
	printf("\033[%d;%dHHOLD:", 3, CANVAS_WIDTH * 2 + 25);
	for (int i = 0; i < 4; i++) {
		printf("\033[%d;%dH", 4 + i, CANVAS_WIDTH * 2 + 30);
		for (int j = 0; j < 4; j++) {

			if (i < holdData.size && j < holdData.size && holdData.rotates[0][i][j]) {
				printf("\033[48;5;%dm\u3000\033[0m", holdData.color);
			}
			else {
				printf("\033[48;5;0m \033[0m");
				printf("\x1b[0m ");
			}
		}
	}

	printf("\033[%d;%dHSCORE: %10d\n", CANVAS_HEIGHT, CANVAS_WIDTH * 2 + 5, state->score);
	if (highestScore < state->score) highestScore = state->score;
	printf("\033[%d;%dHHIGHEST SCORE: %10d\n", CANVAS_HEIGHT + 5, CANVAS_WIDTH * 2 + 5, highestScore);
}

void logic(Block canvas[CANVAS_HEIGHT][CANVAS_WIDTH], State* state) {
	if (ROTATE_FUNC()) {
		int newRotate = (state->rotate + 1) % 4;
		if (move(canvas, state->x, state->y, state->rotate, state->x, state->y, newRotate, state->queue[0])) {
			state->rotate = newRotate;
		}
	}
	else if (LEFT_FUNC()) {
		if (move(canvas, state->x, state->y, state->rotate, state->x - 1, state->y, state->rotate, state->queue[0])) {
			state->x--;
		}
	}
	else if (RIGHT_FUNC()) {
		if (move(canvas, state->x, state->y, state->rotate, state->x + 1, state->y, state->rotate, state->queue[0])) {
			state->x++;
		}
	}
	else if (SOFT_DROP_FUNC()) {
		state->falltime = FALL_DELAY;
	}
	else if (HARD_DROP_FUNC()) {
		state->falltime = FALL_DELAY * CANVAS_HEIGHT;
		//state->isHold = false;
	}
	if (HOLD_FUNC()) {
		hold(canvas, state);
	}

	state->falltime += RENDER_DELAY;
	while (state->falltime >= FALL_DELAY) {
		state->falltime -= FALL_DELAY;

		if (move(canvas, state->x, state->y, state->rotate, state->x, state->y + 1, state->rotate, state->queue[0])) {
			state->y++;
		}
		else {
			
			state->score += clearLine(canvas) * 100;

			state->x = CANVAS_WIDTH / 2;
			state->y = 0;
			state->rotate = 0;
			state->falltime = 0;
			state->isHold = false;
			state->queue[0] = state->queue[1];
			state->queue[1] = state->queue[2];
			state->queue[2] = state->queue[3];
			state->queue[3] = state->queue[4];
			state->queue[4] = rand() % 7;

			if (!move(canvas, state->x, state->y, state->rotate, state->x, state->y, state->rotate, state->queue[0])) {
				printf("\033[%d;%dH\x1b[48;5;160m GAME OVER \x1b[0m\033[%d;%dH", CANVAS_HEIGHT - 1, CANVAS_WIDTH * 2 + 5, CANVAS_HEIGHT + 3, 0);
				printf("\033[%d;%dH\x1b[48;5;34m Press SPACE key to play again \x1b[0m\033[%d;%dH", CANVAS_HEIGHT + 1, CANVAS_WIDTH * 2 + 5, CANVAS_HEIGHT + 1, 0);
				gameState = false;
				system("pause");
				break;
			}
		}
	}

	return;
}

void gameInit() {
	srand(time(NULL));
	State state = {
		.x = CANVAS_WIDTH / 2,
		.y = 0,
		.score = 0,
		.rotate = 0,
		.falltime = 0,
		.hold = -1,
		.isHold = false
	};

	for (int i = 0; i < 5; i++) {
		state.queue[i] = rand() % 7;
	}

	Block canvas[CANVAS_HEIGHT][CANVAS_WIDTH];
	for (int i = 0; i < CANVAS_HEIGHT; i++) {
		for (int j = 0; j < CANVAS_WIDTH; j++) {
			resetBlock(&canvas[i][j]);
		}
	}

	CLS();
	printf("\033[e?25l");
	move(canvas, state.x, state.y, state.rotate, state.x, state.y, state.rotate, state.queue[0]);

	while (gameState) {
		logic(canvas, &state);
		printCanvas(canvas, &state);
		SLEEP(RENDER_DELAY);
	}
}

int main() {

	printStart();

	while (1) {
		gameInit();
		if (_kbhit() && getch() == 0x20) {
			gameState = true;
			continue;
		}
		else if (_kbhit() && getch() == 0x58) {
			break;
		}
	}
	return 0;

}