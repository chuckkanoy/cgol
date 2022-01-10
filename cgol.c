#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <Windows.h>
#define sleep(x) Sleep(1000 * x)
#else
#include <unistd.h>
#endif

/* declare array with population
apply rules:
    1. any live cell 2 or 3 live neighbors lives
    2. any live cell with 1 neighbor dies
    3. any dead cell with 3 neighbors lives
    4. any live cell with more than 3 live neighbors dies
repeat */

typedef struct{
    int m[6][5];
} environment;

environment simulator = {
    .m = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    }
};

const int HEIGHT = sizeof simulator.m / sizeof simulator.m[0];
const int WIDTH = sizeof simulator.m[0] / sizeof simulator.m[0][0];

// assure index is within boundaries of array
int check_bounds (int x, int y) {
    int in_bounds = y >= 0 && y <= HEIGHT;
    if (in_bounds) 
        in_bounds = x >= 0 && x <= WIDTH;
    return in_bounds;
}

// count the number of live, adjacent cells
int count_adj (int x, int y, environment previous) {
    int count = 0, i = 0, j = 0;
    for (i = x - 1; i < x + 2; i++) {
        for (j = y - 1; j < y + 2; j++) {
            if (check_bounds(i, j)) {
                if (previous.m[i][j] == 1 && !(i == x && j == y)) {
                    count++;
                }
            }
        }
    }
    return count;
}

// apply CGOL rules to current matrix and return new matrix
environment apply_rules(environment previous) {
    environment result; 
    int i = 0, j = 0;
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            int numAdj = count_adj(i, j, previous);
            switch (numAdj) {
                case 0 ... 1:
                    result.m[i][j] = 0;
                    break;
                case 2:
                    if (previous.m[i][j] == 0){ result.m[i][j] = 0; }
                    else { result.m[i][j] = 1; }
                    continue;
                case 3:
                    result.m[i][j] = 1;
                    break;
                case 4 ... 8:
                    result.m[i][j] = 0;
                    break;
            }
        }
    }
    return result; 
}

// print the environment matrix to console
void print_env(environment env) {
    int i = 0, j = 0;
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            printf("%d ", (int) env.m[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void main() {
    int iteration = 0, limit = 10;
    environment environment = simulator;
    for (iteration; iteration < limit; iteration++) {
        system("cls");
        printf("Iteration %d\n", iteration);
        print_env(environment);

        environment = apply_rules(environment);

        sleep(.5);
    }
}