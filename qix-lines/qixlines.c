/**
 * QIX Lines
 *
 * Copyright © 2023 Kyle W T Sherman
 * MIT License
 */

#include <c64.h>
#include <cc65.h>
#include <conio.h>
#include <ctype.h>
#include <modload.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgi.h>

#define MAX_COLORS 16
#define COLOR_BG TGI_COLOR_BLACK
#define COLOR_FG TGI_COLOR_WHITE
#define MAX_SIN 180
#define HISTORY_SIZE 10                 // how many lines to display at once
#define STEP 8                          // line spacing
#define STEP_RANGE 6                    // spacing plus/minus range

// use all colors except black (0)
#define RANDOM_COLOR() (rand() % (MAX_COLORS - 1) + 1)

typedef unsigned char byte;
typedef unsigned short ushort;

// line
typedef struct {
    short x1;
    short y1;
    short x2;
    short y2;
} line_s;

// globals
static ushort x_size;
static ushort y_size;

void set_color(const byte color) {
    byte palette[2];
    palette[0] = COLOR_BG;
    palette[1] = color;
    tgi_setpalette(palette);
    tgi_setcolor(COLOR_FG);
}

ushort next_degree(const ushort degree) {
    // add randomly to the degree
    ushort d = degree + STEP + (int)rand() % (STEP_RANGE * 2 + 1) - STEP_RANGE;
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
    line->x1 += (ushort)(((long)line_delta->x1 * _sin(line_degree->x1)) / 256);
    line->y1 += (ushort)(((long)line_delta->y1 * _sin(line_degree->y1)) / 256);
    line->x2 += (ushort)(((long)line_delta->x2 * _sin(line_degree->x2)) / 256);
    line->y2 += (ushort)(((long)line_delta->y2 * _sin(line_degree->y2)) / 256);

    // if any coordinates are out of range, reverse their direction and change color
    if (line->x1 < 0) {
        line->x1 = 0 - line->x1;
        line_delta->x1 = -line_delta->x1;
        set_color(RANDOM_COLOR());
    }
    if (line->x1 >= x_size) {
        line->x1 = x_size - (line->x1 - x_size);
        line_delta->x1 = -line_delta->x1;
        set_color(RANDOM_COLOR());
    }
    if (line->y1 < 0) {
        line->y1 = 0 - line->y1;
        line_delta->y1 = -line_delta->y1;
        set_color(RANDOM_COLOR());
    }
    if (line->y1 >= y_size) {
        line->y1 = y_size - (line->y1 - y_size);
        line_delta->y1 = -line_delta->y1;
        set_color(RANDOM_COLOR());
    }
    if (line->x2 < 0) {
        line->x2 = 0 - line->x2;
        line_delta->x2 = -line_delta->x2;
        set_color(RANDOM_COLOR());
    }
    if (line->x2 >= x_size) {
        line->x2 = x_size - (line->x2 - x_size);
        line_delta->x2 = -line_delta->x2;
        set_color(RANDOM_COLOR());
    }
    if (line->y2 < 0) {
        line->y2 = 0 - line->y2;
        line_delta->y2 = -line_delta->y2;
        set_color(RANDOM_COLOR());
    }
    if (line->y2 >= y_size) {
        line->y2 = y_size - (line->y2 - y_size);
        line_delta->y2 = -line_delta->y2;
        set_color(RANDOM_COLOR());
    }
}

// draw lines until a key is pressed
void draw_lines() {
    line_s line, line_delta, line_degree, line_history[HISTORY_SIZE];
    ushort history_index;

    // set random color
    tgi_setcolor(RANDOM_COLOR());

    // randomize starting values
    line.x1 = rand() % x_size;
    line.y1 = rand() % y_size;
    line.x2 = rand() % x_size;
    line.y2 = rand() % y_size;

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
        tgi_setcolor(COLOR_FG);
        tgi_line(line.x1, line.y1, line.x2, line.y2);

        // undraw oldest line
        tgi_setcolor(COLOR_BG);
        tgi_line(line_history[history_index].x1, line_history[history_index].y1,
                 line_history[history_index].x2, line_history[history_index].y2);

        // add to history
        line_history[history_index++] = line;
        if (history_index >= HISTORY_SIZE) history_index = 0;
    }

    // consume key-press
    cgetc();
}

int main(void) {
    byte border_color;

    // setup tgi
    tgi_install(tgi_static_stddrv);
    tgi_init();
    tgi_clear();

    // set globals
    x_size = tgi_getxres();
    y_size = tgi_getyres();

    // persist border color
    border_color = bordercolor(COLOR_BG);

    // main loop
    draw_lines();

    // restore border color
    bordercolor(border_color);

    // cleanup tgi
    tgi_uninstall();
    clrscr();

    return EXIT_SUCCESS;
}
