#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define WIDTH 20
#define HEIGHT 10
#define SAVE_FILE "pacman_save.txt"
#define INITIAL_ENEMIES 3

typedef struct {
    char type; 
} Cell;

Cell grid[HEIGHT][WIDTH];
int pacmanX, pacmanY;
int foodX, foodY;
int bonusX, bonusY;
int enemyX[100], enemyY[100]; 
int numEnemies;
int score;
int doubleMoveCount;
int level;
int speed;
int dx, dy; 

void setup() {
    srand(time(0)); 
    level = 1;
    speed = 1000;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            grid[y][x].type = ' ';
        }
    }
    for (int x = 0; x < WIDTH; x++) {
        grid[0][x].type = '#';
        grid[HEIGHT-1][x].type = '#';
    }
    for (int y = 0; y < HEIGHT; y++) {
        grid[y][0].type = '#';
        grid[y][WIDTH-1].type = '#';
    }
    int wallsToAdd = (WIDTH * HEIGHT) / 4;
    while (wallsToAdd > 0) {
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;
        if (grid[y][x].type == ' ' && x > 0 && y > 0 && x < WIDTH-1 && y < HEIGHT-1) {
            grid[y][x].type = '#';
            wallsToAdd--;
        }
    }

    pacmanX = WIDTH / 2;
    pacmanY = HEIGHT / 2;
    grid[pacmanY][pacmanX].type = 'P';

    foodX = rand() % WIDTH;
    foodY = rand() % HEIGHT;
    while (grid[foodY][foodX].type != ' ') {
        foodX = rand() % WIDTH;
        foodY = rand() % HEIGHT;
    }
    grid[foodY][foodX].type = 'F';

    bonusX = rand() % WIDTH;
    bonusY = rand() % HEIGHT;
    while (grid[bonusY][bonusX].type != ' ') {
        bonusX = rand() % WIDTH;
        bonusY = rand() % HEIGHT;
    }
    grid[bonusY][bonusX].type = '$';

    numEnemies = INITIAL_ENEMIES + level;
    for (int i = 0; i < numEnemies; i++) {
        enemyX[i] = rand() % WIDTH;
        enemyY[i] = rand() % HEIGHT;
        while (grid[enemyY[i]][enemyX[i]].type != ' ') {
            enemyX[i] = rand() % WIDTH;
            enemyY[i] = rand() % HEIGHT;
        }
        grid[enemyY[i]][enemyX[i]].type = 'E';
    }

    score = 0;
    doubleMoveCount = 0;
    dx = 0; 
    dy = 0;
}

void draw() {
    system("cls");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            printf("%c", grid[y][x].type);
        }
        printf("\n");
    }
    printf("Score: %d\n", score);
    printf("Level: %d\n", level);
    if (doubleMoveCount > 0) {
        printf("Double Move Remaining: %d\n", doubleMoveCount);
    }
}

void input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'w':
                dx = 0;
                dy = -1;
                break;
            case 's':
                dx = 0;
                dy = 1;
                break;
            case 'a':
                dx = -1;
                dy = 0;
                break;
            case 'd':
                dx = 1;
                dy = 0;
                break;
        }
    }
}

void movePacman(int dx, int dy) {
    grid[pacmanY][pacmanX].type = ' ';
    pacmanX += dx;
    pacmanY += dy;
    if (pacmanX == foodX && pacmanY == foodY) {
        score++;
        foodX = rand() % WIDTH;
        foodY = rand() % HEIGHT;
        while (grid[foodY][foodX].type != ' ') {
            foodX = rand() % WIDTH;
            foodY = rand() % HEIGHT;
        }
        grid[foodY][foodX].type = 'F';
        if (score % 10 == 0) {
            level++;
            numEnemies = INITIAL_ENEMIES + level;
            speed = speed > 200 ? speed - 100 : speed;
        }
    }
    if (pacmanX == bonusX && pacmanY == bonusY) {
        doubleMoveCount = 10;
        bonusX = rand() % WIDTH;
        bonusY = rand() % HEIGHT;
        while (grid[bonusY][bonusX].type != ' ') {
            bonusX = rand() % WIDTH;
            bonusY = rand() % HEIGHT;
        }
        grid[bonusY][bonusX].type = '$';
    }
    if (pacmanX < 1) pacmanX = 1;
    if (pacmanX >= WIDTH-1) pacmanX = WIDTH-2;
    if (pacmanY < 1) pacmanY = 1;
    if (pacmanY >= HEIGHT-1) pacmanY = HEIGHT-2;
    if (grid[pacmanY][pacmanX].type == 'E') {
        printf("Game Over! You collided with an enemy.\n");
        exit(0);
    }
    grid[pacmanY][pacmanX].type = 'P';
}

void moveEnemy(int i) {
    grid[enemyY[i]][enemyX[i]].type = ' ';
    int direction = rand() % 4;
    switch (direction) {
        case 0:
            enemyY[i]--;
            break;
        case 1:
            enemyY[i]++;
            break;
        case 2:
            enemyX[i]--;
            break;
        case 3:
            enemyX[i]++;
            break;
    }
    if (enemyX[i] < 1) enemyX[i] = 1;
    if (enemyX[i] >= WIDTH-1) enemyX[i] = WIDTH-2;
    if (enemyY[i] < 1) enemyY[i] = 1;
    if (enemyY[i] >= HEIGHT-1) enemyY[i] = HEIGHT-2;
    if (grid[enemyY[i]][enemyX[i]].type == 'P') {
        printf("Game Over! You collided with an enemy.\n");
        exit(0);
    }
    grid[enemyY[i]][enemyX[i]].type = 'E';
}

void logic() {
    int moves = (doubleMoveCount > 0) ? 2 : 1;
    for (int i = 0; i < moves; i++) {
        movePacman(dx, dy);
    }
    for (int i = 0; i < numEnemies; i++) {
        moveEnemy(i);
    }
    if (doubleMoveCount > 0) {
        doubleMoveCount--;
    }
    dx = 0; 
    dy = 0;
}

void saveGame() {
    FILE *file = fopen(SAVE_FILE, "w");
    if (file) {
        fprintf(file, "%d %d %d %d %d %d %d %d %d %d\n", pacmanX, pacmanY, foodX, foodY, bonusX, bonusY, score, doubleMoveCount, level, speed);
        for (int i = 0; i < numEnemies; i++) {
            fprintf(file, "%d %d ", enemyX[i], enemyY[i]);
        }
        fclose(file);
    }
}

int loadGame() {
    FILE *file = fopen(SAVE_FILE, "r");
    if (file) {
          fscanf(file, "%d %d %d %d %d %d %d %d %d %d", &pacmanX, &pacmanY, &foodX, &foodY, &bonusX, &bonusY, &score, &doubleMoveCount, &level, &speed);
        grid[pacmanY][pacmanX].type = 'P';
        grid[foodY][foodX].type = 'F';
        grid[bonusY][bonusX].type = '$';
        for (int i = 0; i < numEnemies; i++) {
            fscanf(file, "%d %d", &enemyX[i], &enemyY[i]);
            grid[enemyY[i]][enemyX[i]].type = 'E';
        }
        fclose(file);
        return 1;
    }
    return 0;
}

void newGameOrContinue() {
    char choice;
    printf("A saved game is available. Do you want to continue (c) or start a new game (n)? ");
    choice = getchar();
    if (choice == 'c' || choice == 'C') {
        if (!loadGame()) {
            printf("Failed to load game. Starting a new game.\n");
            setup();
        }
    } else {
        setup();
    }
}

int main() {
    if (fopen(SAVE_FILE, "r")) {
        newGameOrContinue();
    } else {
        setup();
    }

    while (1) {
        draw();
        input();
        logic();
        saveGame();
        Sleep(speed); 
    }
    return 0;
}
