#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

// Constants

typedef struct {
	int PositionX;
	int PositionY;
	int Shape;
} SnakeParts;

typedef struct {
	int** data;
	int sizeY;
	int sizeX;
} MatrixObject;

#define TickSecond 0.5 * CLOCKS_PER_SEC

// Variables

char up_key = 'w';
char down_key = 's';
char left_key = 'a';
char right_key = 'd';

int playableMapSizeX;
int playableMapSizeY;

int mapSizeX;
int mapSizeY;

int borderX;
int borderY;

int maxFoodCount;
int foodCount;

int maxSnakeSize;
int snakeSize;

int previousDirection;
int nextDirection;

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

// Game Functions

void newConfiguration() {
	printf("\nMap Size X: ");
	scanf_s("%d", &playableMapSizeX);

	printf("\nMap Size Y: ");
	scanf_s("%d", &playableMapSizeY);

	printf("\nMax Food Count: ");
	scanf_s("%d", &maxFoodCount);
	foodCount = 0;

	mapSizeX = playableMapSizeX + 2;
	mapSizeY = playableMapSizeY + 2;

	borderX = mapSizeX - 1;
	borderY = mapSizeY - 1;

	snakeSize = 1;
	maxSnakeSize = playableMapSizeX * playableMapSizeY;

	nextDirection = 2;
	previousDirection = 2;
}

MatrixObject createMatrix(int sizeY, int sizeX) {
	MatrixObject matrix;
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

void freeMatrixMemory(MatrixObject* matrixObject) {
	for (int i = 0; i < matrixObject->sizeY; i++) {
		free(matrixObject->data[i]);
	}

	free(matrixObject->data);
}

void createFood(MatrixObject* mapMatrix) {
	int sizeY = mapMatrix->sizeY;
	int sizeX = mapMatrix->sizeX;

	int* validY = (int*)malloc(sizeY * sizeof(int));
	int* validCorrespondingX = (int*)malloc(sizeY * sizeof(int));
	int validSizeY = 0;

	for (int Y = 0; Y < sizeY; Y++) {
		int* validX = (int*)malloc(sizeX * sizeof(int));
		int validSizeX = 0;

		for (int X = 0; X < sizeX; X++) {
			if (mapMatrix->data[Y][X] == 0) {
				validX[validSizeX] = X;
				validSizeX = validSizeX + 1;
			}
		}

		if (validSizeX > 0) {
			validY[validSizeY] = Y;
			validCorrespondingX[validSizeY] = validX[(rand() % validSizeX)];

			validSizeY = validSizeY + 1;
		}

		free(validX);
	}

	if (validSizeY > 0) {
		int Y = validY[(rand() % validSizeY)];
		int X = validCorrespondingX[(rand() % validSizeY)];

		mapMatrix->data[Y][X] = 5;
		foodCount = foodCount + 1;
	}

	free(validY);
	free(validCorrespondingX);
}

int main() {
	srand(time(NULL));

	while (1) {
		// Initialise Config

		newConfiguration();

#ifdef _WIN32
		system("cls");  // Windows
#else
		system("clear");  // Unix/Linux/Mac
#endif

		// Initialise Map // -3 = [+], -2 = [-], -1 = [|], 0 = [ ], 1 = [<], 2 = [>], 3 = [^], 4 = [v], 5 = [*]

		MatrixObject mapMatrix = createMatrix(mapSizeY, mapSizeX);

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

		// Initialise Snake

		SnakeParts* SnakeArray = (SnakeParts*)malloc(maxSnakeSize * sizeof(SnakeParts));
		SnakeArray[0].PositionY = 1;
		SnakeArray[0].PositionX = 1;
		SnakeArray[0].Shape = 2;

		mapMatrix.data[1][1] = 2;

		// Initialise Food

		for (int i = 0; i < maxFoodCount; i++) {
			createFood(&mapMatrix);
		}

		// Begin

		int run = 1;
		int exitCode = 0;

		clock_t lastTick = clock();

		while (run == 1) {
			clock_t newTick = clock();

			// Movement Input

			char c;

			if (_kbhit()) {
				c = tolower(_getch());

				if (c == left_key) {
					if (previousDirection == 3 || previousDirection == 4) {
						nextDirection = 1;
					}
				}
				else if (c == right_key) {
					if (previousDirection == 3 || previousDirection == 4) {
						nextDirection = 2;
					}
				}
				else if (c == up_key) {
					if (previousDirection == 1 || previousDirection == 2) {
						nextDirection = 3;
					}
				}
				else if (c == down_key) {
					if (previousDirection == 1 || previousDirection == 2) {
						nextDirection = 4;
					}
				}
			}

			// Game Tick

			if (newTick - lastTick > TickSecond) {
				lastTick = newTick;

				previousDirection = nextDirection;

				// Move Head

				int* HeadPositionY = &SnakeArray[0].PositionY;
				int* HeadPositionX = &SnakeArray[0].PositionX;
				mapMatrix.data[*HeadPositionY][*HeadPositionX] = 0; // Empty Previous Head Position

				int NewHeadPosY = *HeadPositionY;
				int NewHeadPosX = *HeadPositionX;

				if (nextDirection == 1) {
					NewHeadPosX = *HeadPositionX - 1;
				}
				else if (nextDirection == 2) {
					NewHeadPosX = *HeadPositionX + 1;
				}
				else if (nextDirection == 3) {
					NewHeadPosY = *HeadPositionY - 1;
				}
				else {
					NewHeadPosY = *HeadPositionY + 1;
				}

				// Check State

				setCursorPosition(0, 0);

				if (NewHeadPosY < 0 || NewHeadPosY > borderY || NewHeadPosX < 0 || NewHeadPosX > borderX) {
					exitCode = 300; // Out of Bounds
					run = 0;
				}
				else {
					int HeadValue = mapMatrix.data[NewHeadPosY][NewHeadPosX];

					if ((HeadValue == -2) || (HeadValue == -1)) {
						exitCode = 200; // Hit Border
						run = 0;
					}
					else if ((HeadValue == 1) || (HeadValue == 2) || (HeadValue == 3) || (HeadValue == 4)) {
						exitCode = 201; // Hit Self
						run = 0;
					}
					else if (HeadValue == 5) { // Hit Food
						snakeSize++;

						SnakeArray[snakeSize - 1].PositionY = SnakeArray[snakeSize - 2].PositionY;
						SnakeArray[snakeSize - 1].PositionX = SnakeArray[snakeSize - 2].PositionX;
						SnakeArray[snakeSize - 1].Shape = SnakeArray[snakeSize - 2].Shape;

						if (snakeSize == maxSnakeSize) {
							exitCode = 100; // Win
							run = 0;
						}

						foodCount = foodCount - 1;
					}
				}

				// Move Parts

				mapMatrix.data[SnakeArray[snakeSize - 1].PositionY][SnakeArray[snakeSize - 1].PositionX] = 0; // Empty Previous Tail Position

				for (int i = snakeSize - 1; i > 0; i--) {
					int* PosY = &SnakeArray[i].PositionY;
					int* PosX = &SnakeArray[i].PositionX;
					int* Shape = &SnakeArray[i].Shape;

					*PosY = SnakeArray[i - 1].PositionY;
					*PosX = SnakeArray[i - 1].PositionX;
					*Shape = SnakeArray[i - 1].Shape;

					mapMatrix.data[*PosY][*PosX] = *Shape;
				}

				// Finalise Head Position

				*HeadPositionX = NewHeadPosX;
				*HeadPositionY = NewHeadPosY;
				SnakeArray[0].Shape = nextDirection;

				mapMatrix.data[*HeadPositionY][*HeadPositionX] = nextDirection;

				// New Food

				if (maxFoodCount - foodCount > 0) {
					createFood(&mapMatrix);
				}

				// Render

				for (int y = 0; y < mapSizeY; y++) {
					for (int x = 0; x < mapSizeX; x++) {
						int value = mapMatrix.data[y][x];

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

				printf("\nSnake Size: %d / %d", snakeSize, maxSnakeSize);
			}
		}

		// Game End State

		if (exitCode == 300) {
			printf("\n\nOut of bounds\n");
		}
		else if (exitCode == 200) {
			printf("\n\nHit Border\n");
		}
		else if (exitCode == 201) {
			printf("\n\nHit Self\n");
		}
		else if (exitCode == 100) {
			printf("\n\nWin\n");
		}

		// Free Map Memory

		freeMatrixMemory(&mapMatrix);
	}
}
