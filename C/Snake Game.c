#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

// Console Functions

void setCursorPosition(int x, int y) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { x, y };
	SetConsoleCursorPosition(hConsole, coord);
}

void setColor(int textColor, int backgroundColor) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, textColor | (backgroundColor << 4));
}

// Variables

int playableMapSizeX;
int playableMapSizeY;

int mapSizeX;
int mapSizeY;

int borderX;
int borderY;

char up_key = 'w';
char down_key = 's';
char left_key = 'a';
char right_key = 'd';

int foodCount;

int maxSnakeSize;

int snakeSize;
int nextDirection;
int previousDirection;

struct SnakeParts {
	int PositionX;
	int PositionY;
	int Shape;
};

typedef struct {
	int** data;
	int sizeY;
	int sizeX;
} Matrix;

// Game Functions

void newConfiguration() {
	printf("\nMap Size X: ");
	scanf("%d", &playableMapSizeX);

	printf("\nMap Size Y: ");
	scanf("%d", &playableMapSizeY);

	printf("\nFood Count: ");
	scanf("%d", &foodCount);

	mapSizeX = playableMapSizeX + 2;
	mapSizeY = playableMapSizeY + 2;

	borderX = mapSizeX - 1;
	borderY = mapSizeY - 1;

	maxSnakeSize = playableMapSizeX * playableMapSizeY;
	snakeSize = 1;
	nextDirection = 2;
	previousDirection = 2;
}

Matrix createMatrix(int sizeY, int sizeX) {
	Matrix matrix;
	matrix.sizeY = sizeY;
	matrix.sizeX = sizeX;

	matrix.data = (int**)malloc(sizeY * sizeof(int*));

	if (matrix.data == NULL) {
		printf("Memory allocation for rows failed.\n");
		exit(1);
	}

	for (int i = 0; i < sizeY; i++) {
		matrix.data[i] = (int*)malloc(sizeX * sizeof(int));
		if (matrix.data[i] == NULL) {
			printf("Memory allocation for row %d failed.\n", i);
			exit(1);
		}
	}

	return matrix;
}

void freeMatrix(Matrix* matrix) {
	for (int i = 0; i < matrix->sizeY; i++) {
		free(matrix->data[i]);
	}

	free(matrix->data);
}

void createFood(int** map) {
	



	for (int Y = 0; Y < mapSizeY; Y++) {
		for (int X = 0; X < mapSizeX; X++) {
			if (map[Y][X] == 0) {
				validMap[Y][X] = 1;
			}
		}
	}


	int** validMap = newMatrix(mapSizeY, mapSizeX);

	int Y, X;
	int validPosition = 0;

	while (validPosition == 0) {
		Y = 1 + (rand() % (PlayableMapSizeY + 1));
		X = 1 + (rand() % (PlayableMapSizeX + 1));

		if (validMap[Y][X] == 0) {
			validPosition = 1;
		}
	}

	map[Y][X] = 5;
}

int main() {
	srand(time(NULL));

	while (1) {
		newConfiguration();

		Matrix mapMatrix = createMatrix(mapSizeY, mapSizeX);

		// Initialise Map // -3 = [+], -2 = [-], -1 = [|], 0 = [ ], 1 = [<], 2 = [>], 3 = [^], 4 = [v], 5 = [*]

		for (int y = 0; y < mapSizeY; y++) {
			for (int x = 0; x < mapSizeX; x++) {
				if ((y == 0 && x == 0) || (y == 0 && x == borderX) || (y == borderY && x == 0) || (y == borderY && x == borderX)) {
					mapMatrix.data[y][x] = -3;
				}
				else if ((y == 0) || (y == borderY)) {
					mapMatrix.data[y][x] = -2;
				}
				else if ((x == 0) || (x == borderX)) {
					mapMatrix.data[y][x] = -1;
				}
				else {
					mapMatrix.data[y][x] = 0;
				}
			}
		}

		// Initialise Food

		createFood();

		struct SnakeParts* SnakeArray = (struct SnakeParts*)malloc(maxSnakeSize * sizeof(struct SnakeParts));
		SnakeArray[0].PositionX = 1;
		SnakeArray[0].PositionY = 1;
		SnakeArray[0].Shape = 2;

		int run = 1;
		int exitCode = 0;

		while (run == 1) {
			clock_t start = clock();

			char c;

			if (_kbhit()) {
				c = tolower(_getch());

				if (c == left_key) {
					if (PreviousDirection == 3 || PreviousDirection == 4) {
						NextDirection = 1;
					}
				}
				else if (c == right_key) {
					if (PreviousDirection == 3 || PreviousDirection == 4) {
						NextDirection = 2;
					}
				}
				else if (c == up_key) {
					if (PreviousDirection == 1 || PreviousDirection == 2) {
						NextDirection = 3;
					}
				}
				else if (c == down_key) {
					if (PreviousDirection == 1 || PreviousDirection == 2) {
						NextDirection = 4;
					}
				}
			}

			if (start % 500 == 0) { // Tick
				// Move Head

				int* HeadPositionX = &SnakeArray[0].PositionX;
				int* HeadPositionY = &SnakeArray[0].PositionY;

				Map[*HeadPositionY][*HeadPositionX] = 0; // Previous Head Position

				int NewHeadPosX = *HeadPositionX;
				int NewHeadPosY = *HeadPositionY;

				if (NextDirection == 1) {
					NewHeadPosX = *HeadPositionX - 1;
				}
				else if (NextDirection == 2) {
					NewHeadPosX = *HeadPositionX + 1;
				}
				else if (NextDirection == 3) {
					NewHeadPosY = *HeadPositionY - 1;
				}
				else {
					NewHeadPosY = *HeadPositionY + 1;
				}

				// Check State

				setCursorPosition(0, 0);

				if (NewHeadPosX < 0 || NewHeadPosX > MapSizeX - 1 || NewHeadPosY < 0 || NewHeadPosY > MapSizeY - 1) {
					exitCode = 300; // Out of Bounds
					run = 0;
				}
				else {
					int HeadValue = Map[NewHeadPosY][NewHeadPosX];

					if ((HeadValue == -2) || (HeadValue == -1)) {
						exitCode = 200; // Hit Border
						run = 0;
					}
					else if ((HeadValue == 1) || (HeadValue == 2) || (HeadValue == 3) || (HeadValue == 4)) {
						exitCode = 201; // Hit Self
						run = 0;
					}
					else if (HeadValue == 5) { // Food
						SnakeSize++;

						SnakeArray[SnakeSize - 1].PositionX = SnakeArray[SnakeSize - 2].PositionX;
						SnakeArray[SnakeSize - 1].PositionY = SnakeArray[SnakeSize - 2].PositionY;
						SnakeArray[SnakeSize - 1].Shape = SnakeArray[SnakeSize - 2].Shape;

						if (SnakeSize == MaxSnakeSize) {
							exitCode = 100; // Win
							run = 0;
						}
						else {
							createFood();
						}
					}
				}

				// Move Parts

				Map[SnakeArray[SnakeSize - 1].PositionY][SnakeArray[SnakeSize - 1].PositionX] = 0; // Previous Tail Position

				for (int i = SnakeSize - 1; i > 0; i--) {
					int* PosX = &SnakeArray[i].PositionX;
					int* PosY = &SnakeArray[i].PositionY;
					int* Shape = &SnakeArray[i].Shape;

					*PosX = SnakeArray[i - 1].PositionX;
					*PosY = SnakeArray[i - 1].PositionY;
					*Shape = SnakeArray[i - 1].Shape;

					Map[*PosY][*PosX] = *Shape;
				}

				// Finalise head position after parts

				*HeadPositionX = NewHeadPosX;
				*HeadPositionY = NewHeadPosY;

				PreviousDirection = NextDirection;
				SnakeArray[0].Shape = NextDirection;

				Map[*HeadPositionY][*HeadPositionX] = NextDirection;

				// Render

				for (int y = 0; y < MapSizeY; y++) {
					for (int x = 0; x < MapSizeX; x++) {
						int value = Map[y][x];

						if (y == SnakeArray[0].PositionY && x == SnakeArray[0].PositionX) {
							setColor(11, 0);
						}
						else {
							setColor(9, 0);
						}

						if (value == -3) {
							setColor(12, 0);
							printf("[+]");
						}
						else if (value == -2) {
							setColor(12, 0);
							printf("[-]");
						}
						else if (value == -1) {
							setColor(12, 0);
							printf("[|]");
						}
						else if (value == 0) {
							setColor(7, 0);
							printf("[ ]");
						}
						else if (value == 1) {
							printf("[<]");
						}
						else if (value == 2) {
							printf("[>]");
						}
						else if (value == 3) {
							printf("[^]");
						}
						else if (value == 4) {
							printf("[v]");
						}
						else {
							setColor(6, 0);
							printf("[*]");
						}

						setColor(7, 0);
					}

					printf("\n");
				}

				printf("\nSnake Size: %d", SnakeSize);

				for (int i = SnakeSize - 1; i >= 0; i--) {
					printf("\n(%d, %d) + %d\n", SnakeArray[i].PositionX, SnakeArray[i].PositionY, SnakeArray[i].Shape);
				}
			}
		}

		// Game End State

		if (code == 300) {
			printf("\n\nOut of bounds\n");
		}
		else if (code == 200) {
			printf("\n\nHit Border\n");
		}
		else if (code == 201) {
			printf("\n\nHit Self\n");
		}
		else if (code == 100) {
			printf("\n\nWin\n");
		}
	}
}
