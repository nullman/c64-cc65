/**
 * QIX Lines (Multi-Color)
 */

#include <c64.h>
#include <cc65.h>
#include <conio.h>
#include <ctype.h>
#include <modload.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgi.h>

#include "mcbitmap.h"

#define X_SIZE 160
#define Y_SIZE 192
#define MAX_COLORS 16
#define COLOR_BG TGI_COLOR_BLACK
#define MAX_SIN 180
#define HISTORY_SIZE 10                 // how many lines to display at once
#define STEP 10                         // line spacing
#define STEP_RANGE 9                    // spacing plus/minus range
#define QIX_COUNT 3                     // number of qixs to display

// use all colors except black (0)
#define RANDOM_COLOR() (rand() % (MAX_COLORS - 1) + 1)

// line
typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
    byte color;
} line_s;

int next_degree(int degree) {
    // add randomly to the degree
    int d = degree + STEP + (int) rand() % (STEP_RANGE * 2 + 1) - STEP_RANGE;
    if (d >= MAX_SIN) d = d - MAX_SIN;
    return d;
}

void next_line(line_s *line, line_s *line_delta, line_s *line_degree) {
    // randomly add to the degrees
    line_degree->x1 = next_degree(line_degree->x1);
    line_degree->y1 = next_degree(line_degree->y1);
    line_degree->x2 = next_degree(line_degree->x2);
    line_degree->y2 = next_degree(line_degree->y2);

    // add using sin modified by a delta for each coordinate dimension
    line->x1 += (int) (((long) line_delta->x1 * _sin(line_degree->x1)) / 256);
    line->y1 += (int) (((long) line_delta->y1 * _sin(line_degree->y1)) / 256);
    line->x2 += (int) (((long) line_delta->x2 * _sin(line_degree->x2)) / 256);
    line->y2 += (int) (((long) line_delta->y2 * _sin(line_degree->y2)) / 256);

    // if any coordinates are out of range, reverse their direction and change color
    if (line->x1 < 0) {
        line->x1 = 0 - line->x1;
        line_delta->x1 = -line_delta->x1;
        line->color = RANDOM_COLOR();
    }
    if (line->x1 >= X_SIZE) {
        line->x1 = X_SIZE - (line->x1 - X_SIZE);
        line_delta->x1 = -line_delta->x1;
        line->color = RANDOM_COLOR();
    }
    if (line->y1 < 0) {
        line->y1 = 0 - line->y1;
        line_delta->y1 = -line_delta->y1;
        line->color = RANDOM_COLOR();
    }
    if (line->y1 >= Y_SIZE) {
        line->y1 = Y_SIZE - (line->y1 - Y_SIZE);
        line_delta->y1 = -line_delta->y1;
        line->color = RANDOM_COLOR();
    }
    if (line->x2 < 0) {
        line->x2 = 0 - line->x2;
        line_delta->x2 = -line_delta->x2;
        line->color = RANDOM_COLOR();
    }
    if (line->x2 >= X_SIZE) {
        line->x2 = X_SIZE - (line->x2 - X_SIZE);
        line_delta->x2 = -line_delta->x2;
        line->color = RANDOM_COLOR();
    }
    if (line->y2 < 0) {
        line->y2 = 0 - line->y2;
        line_delta->y2 = -line_delta->y2;
        line->color = RANDOM_COLOR();
    }
    if (line->y2 >= Y_SIZE) {
        line->y2 = Y_SIZE - (line->y2 - Y_SIZE);
        line_delta->y2 = -line_delta->y2;
        line->color = RANDOM_COLOR();
    }
}

// draw lines until a key is pressed
void draw_lines() {
    line_s line, line_delta, line_degree, line_history[HISTORY_SIZE];
    int history_index;

    // randomize starting values
    line.x1 = rand() % X_SIZE;
    line.y1 = rand() % Y_SIZE;
    line.x2 = rand() % X_SIZE;
    line.y2 = rand() % Y_SIZE;
    line.color = RANDOM_COLOR();

    line_delta.x1 = STEP;
    line_delta.y1 = STEP;
    line_delta.x2 = STEP;
    line_delta.y2 = STEP;

    line_degree.x1 = rand() % MAX_SIN;
    line_degree.y1 = rand() % MAX_SIN;
    line_degree.x2 = rand() % MAX_SIN;
    line_degree.y2 = rand() % MAX_SIN;
    history_index = 0;

    // loop until key-press
    while (!kbhit()) {
        // get next line
        next_line(&line, &line_delta, &line_degree);

        // draw line
        draw_line(line.x1, line.y1, line.x2, line.y2, line.color);

        // remove from history
        draw_line(line_history[history_index].x1, line_history[history_index].y1,
                  line_history[history_index].x2, line_history[history_index].y2,
                  COLOR_BG);

        // add to history
        line_history[history_index++] = line;
        if (history_index >= HISTORY_SIZE) history_index = 0;
    }
    cgetc();                            // consume key-press
}

int main(void) {
    unsigned char bg_color, border_color;

    // setup multi-color bitmap
    setup_bitmap_multi();

    // persist background and border color
    bg_color = bgcolor(COLOR_BG);
    border_color = bordercolor(COLOR_BG);

    // clear screen
    clrscr();

    // main loop
    draw_lines();

    // restore background and border color
    bgcolor(bg_color);
    bordercolor(border_color);

    return EXIT_SUCCESS;
}
