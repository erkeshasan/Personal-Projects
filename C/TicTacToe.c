#include <stdio.h>
#include <stdlib.h>

#define clrscr() printf("\e[1;1H\e[2J")

int gameFinished = 0;

void draw(int coordinates[]) {
  clrscr();

  int j, i;
  int emptyCoordinate = 0;

  for (j=0; j<3; j++) {
      for (i=0; i<3; i++) {
          if (coordinates[i + (j * 3)] == 0) {
            printf("%s", "☐ ");
            emptyCoordinate = 1;
          }
          else if (coordinates[i + (j * 3)] == 1) {
            printf("%s", "X ");
          }
          else {
            printf("%s", "O ");
          }
      }

      printf("\n");
  }

  if (emptyCoordinate == 0) {
    gameFinished = 1;
  }
}

int main() {
  int coordinates[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

  draw(coordinates);
  
  while (gameFinished == 0) {
    printf("Player X\nChoose a coordinate from 1 to 9: ");

    int chosenCoordinate = 0;
    int validChoice = 0;
    
    while (validChoice == 0) {
      scanf("%d", &chosenCoordinate);
      validChoice = 1;
      
      if (chosenCoordinate < 1 || chosenCoordinate > 9) {
        printf("Chosen coordinate must be from 1 to 9.\n");
        validChoice = 0;
      }

      if (coordinates[chosenCoordinate-1] == 1 || coordinates[chosenCoordinate-1] == 2) {
        printf("Chosen coordinate has already been assigned.\n");
        validChoice = 0;
      }
    }

    coordinates[chosenCoordinate-1] = 1;

    draw(coordinates);

    chosenCoordinate = 0;
    validChoice = 0;
    
    printf("Player O\nChoose a coordinate from 1 to 9: ");

    while (validChoice == 0) {
      scanf("%d", &chosenCoordinate);
      validChoice = 1;

      if (chosenCoordinate < 1 || chosenCoordinate > 9) {
        printf("Chosen coordinate must be from 1 to 9.\n");
        validChoice = 0;
      }

      if (coordinates[chosenCoordinate-1] == 1 || coordinates[chosenCoordinate-1] == 2) {
        printf("Chosen coordinate has already been assigned.\n");
        validChoice = 0;
      }
    }

    coordinates[chosenCoordinate-1] = 2;

    draw(coordinates);
  }

  printf("Game finished");
  
  return 0;
}
