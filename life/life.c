/**
 * Game of Life
 *
 * Copyright © 2023-2025 Kyle W T Sherman
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

#define TRUE      1
#define FALSE     0
#define COLOR_BG  TGI_COLOR_BLACK
#define COLOR_FG  TGI_COLOR_WHITE
#define X_SIZE    320
#define Y_SIZE    200
#define CELL_SIZE 1000
#define GRID_SIZE (X_SIZE * Y_SIZE)

// predefined cell types
#define CT_RANDOM    0
#define CT_GLIDER_NW 1
#define CT_GLIDER_NE 2
#define CT_GLIDER_SE 3
#define CT_GLIDER_SW 4

typedef unsigned char bool;
typedef unsigned char byte;
typedef unsigned short ushort;

// globals
byte work[GRID_SIZE / 8];
ushort cell[CELL_SIZE];
ushort next[CELL_SIZE];
ushort cell_p, next_p;

void set_work_bit(const short x, const short y, const bool val)
{
    ushort pos = y * X_SIZE + x;

    if (x >= 0 && y >= 0 && x < X_SIZE && y < Y_SIZE) {
        if (val)
            work[pos / 8] |= 1 << (pos % 8);
        else
            work[pos / 8] &= ~(1 << (pos % 8));
    }
}

bool get_work_bit(const short x, const short y)
{
    ushort pos = y * X_SIZE + x;

    if (x >= 0 && y >= 0 && x < X_SIZE && y < Y_SIZE)
        return (((work[pos / 8] & (1 << (pos % 8))) == 0) ? 0 : 1);
    else
        return 0;
}

byte empty_cell(const short x, const short y)
{
    return (x < 0 || y < 0 || x >= X_SIZE || y >= Y_SIZE ||
            (tgi_getpixel(x, y) == COLOR_BG));
}

// check cell for life (by counting neighbors)
// state is either 0 = off or >0 = on
// return 1 for life (on and 2 or 3 neighbors, or off and 3 neighbors)
// otherwise, return 0
byte check_cell(const short x, const short y, const byte state)
{
    short xx, yy;
    byte n = 0;

    for (xx = x - 1; xx <= x + 1; xx++)
        for (yy = y - 1; yy <= y + 1; yy++)
            if ((xx != x || yy != y) && !empty_cell(xx, yy))
                n++;
    return (n == 3 || (n == 2 && state));
}

// check for cell life of a neighbor
// only check dead cells
byte check_neighbor(const short x, const short y)
{
    return (x >= 0 && y >= 0 && x < X_SIZE && y < Y_SIZE &&
            (tgi_getpixel(x, y) == COLOR_BG) && check_cell(x, y, FALSE));
}

void clear_cells()
{
    ushort i;

    tgi_setcolor(COLOR_BG);
    for (i = 0; i < cell_p; i++)
        tgi_setpixel(cell[i] % X_SIZE, cell[i] / X_SIZE);
}

void draw_next_cells()
{
    ushort i;

    tgi_setcolor(COLOR_FG);
    for (i = 0; i < next_p; i++) {
        tgi_setpixel(next[i] % X_SIZE, next[i] / X_SIZE);
        cell[i] = next[i];
    }
    cell_p = next_p;
}

void add_cell(short x, short y)
{
    ushort pos = y * X_SIZE + x;

    if (cell_p < CELL_SIZE - 1) {
        cell[cell_p++] = pos;
        tgi_setpixel(x, y);
    }
}

void add_next(short x, short y)
{
    ushort pos = y * X_SIZE + x;

    if (next_p < CELL_SIZE - 1) {
        next[next_p++] = pos;
    }
}

void add_random(short x, short y)
{
    ushort xx, yy;

    x = (x < 10) ? 10 : (x >= X_SIZE - 10) ? X_SIZE - 11 : x;
    y = (y < 10) ? 10 : (y >= Y_SIZE - 10) ? (Y_SIZE - 11) : y;
    for (yy = y - 10; yy < y + 10; yy++) {
        for (xx = x - 10; xx < x + 10; xx++) {
            if ((rand() % 2) == 0) {
                add_cell(xx, yy);
            }
        }
    }
}

void add_glider(short x, short y, byte dir)
{
    x = (x < 1) ? 1 : (x >= X_SIZE - 1) ? X_SIZE - 2 : x;
    y = (y < 1) ? 1 : (y >= Y_SIZE - 1) ? Y_SIZE - 2 : y;
    switch (dir) {
        case CT_GLIDER_NW:
            add_cell(x - 1, y - 1);
            add_cell(x - 1, y);
            add_cell(x, y - 1);
            add_cell(x, y + 1);
            add_cell(x + 1, y - 1);
            break;
        case CT_GLIDER_NE:
            add_cell(x - 1, y);
            add_cell(x, y - 1);
            add_cell(x + 1, y - 1);
            add_cell(x + 1, y);
            add_cell(x + 1, y + 1);
            break;
        case CT_GLIDER_SE:
            add_cell(x - 1, y + 1);
            add_cell(x, y - 1);
            add_cell(x, y + 1);
            add_cell(x + 1, y);
            add_cell(x + 1, y + 1);
            break;
        case CT_GLIDER_SW:
            add_cell(x - 1, y - 1);
            add_cell(x - 1, y);
            add_cell(x - 1, y + 1);
            add_cell(x, y + 1);
            add_cell(x + 1, y);
            break;
    }
}

void add_shape(const byte type, const short x, const short y)
{
    ushort xx, yy;

    xx = x;
    yy = y;

    switch (type) {
        case CT_RANDOM:
            add_random(xx, yy);
            break;
        case CT_GLIDER_NW:
            add_glider(xx, yy, CT_GLIDER_NW);
            break;
        case CT_GLIDER_NE:
            add_glider(xx, yy, CT_GLIDER_NE);
            break;
        case CT_GLIDER_SE:
            add_glider(xx, yy, CT_GLIDER_SE);
            break;
        case CT_GLIDER_SW:
            add_glider(xx, yy, CT_GLIDER_SW);
            break;
    }
}

// draw cells in a loop, controlled by key presses
void draw_loop()
{
    byte key, key1, key2, mode;
    ushort i, pos;
    short x, y, xx, yy, cx, cy;

    cell_p = 0;
    cx = X_SIZE / 2;
    cy = Y_SIZE / 2;

    // randomize initial state
    //add_shape(CT_RANDOM, cx, cy);
    add_shape(CT_GLIDER_SE, cx, cy);

    // mode 0: exit
    // mode 1: pause until key-press
    // mode 2: run continuously
    mode = 1;
    key1 = key2 = 0;
    while (mode > 0) {
        tgi_setcolor(COLOR_FG);

        if (kbhit())
            key = cgetc();

        switch (key) {
            case 'q':
                mode = 0;
                break;
            case 's':
                if (mode == 1) mode = 2;
                else mode = 1;
                break;
            case 'c':
                clear_cells();
                cell_p = 0;
                break;
        }

        if (key >= '0' && key <= '9') {
            if (key1 == 0)
                key1 = key;
            else
                key2 = key;

            if (key2 == 'q')
                mode = 0;
            else if (key2 > 0) {
                add_shape((key1 - '0') * 10 + key2 - '0', cx, cy);
                key1 = key2 = 0;
            }
        }

        if (mode == 2 || key == ' ') {
            next_p = 0;

            // process cells
            for (i = 0; i < cell_p; i++) {
                pos = cell[i];
                x = pos % X_SIZE;
                y = pos / X_SIZE;

                // add all neighbors to work
                for (xx = x - 1; xx <= x + 1; xx++)
                    for (yy = y - 1; yy <= y + 1; yy++) {
                        set_work_bit(xx, yy, TRUE);
                    }

                // if cell is still alive, add to next
                if (check_cell(x, y, TRUE))
                    add_next(x, y);
            }

            // process cell neighbors
            for (i = 0; i < cell_p; i++) {
                pos = cell[i];
                x = pos % X_SIZE;
                y = pos / X_SIZE;
                // check neighbor cells for life and add to next as appropriate
                for (xx = x - 1; xx <= x + 1; xx++)
                    for (yy = y - 1; yy <= y + 1; yy++) {
                        if (get_work_bit(xx, yy) && check_neighbor(xx, yy))
                            add_next(xx, yy);
                        set_work_bit(xx, yy, FALSE);
                    }
            }

            // remove dead cells from screen
            clear_cells();

            // draw life cells to screen
            // and copy next array to cell array
            draw_next_cells();
        }

        key = 0;
    }
}

int main(void)
{
    byte border_color;

    // setup tgi
    tgi_install(tgi_static_stddrv);
    tgi_init();
    tgi_clear();

    // persist border color
    border_color = bordercolor(COLOR_BG);

    // main loop
    draw_loop();

    // restore border color
    bordercolor(border_color);

    // cleanup tgi
    tgi_uninstall();
    clrscr();

    return EXIT_SUCCESS;
}
