#include "plotter.h"
#include <stdio.h>
#include <math.h>

#define WIDTH 80
#define HEIGHT 25
#define M_PI 3.14159265358979323846

void plot(const RPNExpression *rpn) {
    char canvas[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            canvas[i][j] = '.';
        }
    }

    double step = (4 * M_PI) / WIDTH;
    for (int i = 0; i < WIDTH; i++) {
        double x = i * step;
        double y = evaluate_rpn(rpn, x);
        //printf("y: %lf ", y);
        int y_coord = (int)round((y + 1) * (HEIGHT - 1) / 2);
        if (y_coord >= 0 && y_coord < HEIGHT) {
            canvas[y_coord][i] = '*';
        }
    }
    printf("\n");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", canvas[i][j]);
        }
        printf("\n");
    }
}
