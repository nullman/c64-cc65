/**
 * Bit Array
 */

#include <cbm.h>
#include <stdio.h>
#include <stdlib.h>

#define TRUE       1
#define FALSE      0
#define X_SIZE     320
#define Y_SIZE     200
#define ARRAY_SIZE (X_SIZE * Y_SIZE)    // 64000

typedef unsigned char bool;
typedef unsigned char byte;
typedef unsigned short ushort;

// globals
byte array[ARRAY_SIZE / 8];

void set_bit_pos(const ushort pos, const bool val)
{
    if (val == TRUE)
        array[pos / 8] |= 1 << (pos % 8);
    else
        array[pos / 8] &= ~(1 << (pos % 8));
}

void set_bit_xy(const short x, const short y, const bool val)
{
    ushort pos = y * X_SIZE + x;

    set_bit_pos(pos, val);
}

bool get_bit_pos(const ushort pos)
{
    return (((array[pos / 8] & (1 << (pos % 8))) == 0) ? 0 : 1);
}

bool get_bit_xy(const short x, const short y)
{
    ushort pos = y * X_SIZE + x;

    return get_bit_pos(pos);
}

void bit_test()
{
    byte b;

    printf("\nbit test\n\n");

    b = 0;
    printf("add even bits: %d", b);
    b |= 1 << 2;
    printf(" -> %d", b);
    b |= 1 << 4;
    printf(" -> %d", b);
    b |= 1 << 6;
    printf(" -> %d\n", b);
    printf("remove even bits: %d", b);
    b &= ~(1 << 2);
    printf(" -> %d", b);
    b &= ~(1 << 4);
    printf(" -> %d", b);
    b &= ~(1 << 6);
    printf(" -> %d\n", b);
}

void bit_array_test()
{
    ushort p;
    short x, y;

    printf("\nbit array test\n\n");

    p = 14405;
    set_bit_pos(p, TRUE);
    if (get_bit_pos(p) != TRUE)
        printf("error: ");
    printf("pos: %5u, set: %d, get: %d\n", p, TRUE, get_bit_pos(p));
    set_bit_pos(p, FALSE);
    if (get_bit_pos(p) != FALSE)
        printf("error: ");
    printf("pos: %5u, set: %d, get: %d\n", p, FALSE, get_bit_pos(p));

    p = 60810;
    set_bit_pos(p, TRUE);
    if (get_bit_pos(p) != TRUE)
        printf("error: ");
    printf("pos: %5u, set: %d, get: %d\n", p, TRUE, get_bit_pos(p));
    set_bit_pos(p, FALSE);
    if (get_bit_pos(p) != FALSE)
        printf("error: ");
    printf("pos: %5u, set: %d, get: %d\n", p, FALSE, get_bit_pos(p));

    x = 10;
    y = 90;
    set_bit_xy(x, y, TRUE);
    if (get_bit_xy(x, y) != TRUE)
        printf("error: ");
    printf("x,y: %3d,%3d, set: %d, get: %d\n", x, y, TRUE, get_bit_xy(x, y));
    set_bit_xy(x, y, FALSE);
    if (get_bit_xy(x, y) != FALSE)
        printf("error: ");
    printf("x,y: %3d,%3d, set: %d, get: %d\n", x, y, FALSE, get_bit_xy(x, y));

    x = 10;
    y = 190;
    set_bit_xy(x, y, TRUE);
    if (get_bit_xy(x, y) != TRUE)
        printf("error: ");
    printf("x,y: %3d,%3d, set: %d, get: %d\n", x, y, TRUE, get_bit_xy(x, y));
    set_bit_xy(x, y, FALSE);
    if (get_bit_xy(x, y) != FALSE)
        printf("error: ");
    printf("x,y: %3d,%3d, set: %d, get: %d\n", x, y, FALSE, get_bit_xy(x, y));
}

bit_array_test_pos()
{
    ushort p;

    printf("\nbit array test: pos\n\n");

    for (p = 0; p < ARRAY_SIZE; p += 1000) {
        set_bit_pos(p, TRUE);
        if (get_bit_pos(p) != TRUE)
            printf("error: ");
        printf("pos: %5u, set: %d, get: %d\n", p, TRUE, get_bit_pos(p));
    }

    for (p = 0; p < ARRAY_SIZE; p += 1000) {
        set_bit_pos(p, FALSE);
        if (get_bit_pos(p) != FALSE)
            printf("error: ");
        printf("pos: %5u, set: %d, get: %d\n", p, FALSE, get_bit_pos(p));
    }
}

bit_array_test_xy()
{
    short x, y;

    printf("\nbit array test: xy\n\n");

    for (y = 0; y < Y_SIZE; y += 10) {
        for (x = 0; x < X_SIZE; x += 120) {
            set_bit_xy(x, y, TRUE);
            if (get_bit_xy(x, y) != TRUE)
                printf("error: ");
            printf("x,y: %3d,%3d, set: %d, get: %d\n", x, y, TRUE, get_bit_xy(x, y));
        }
    }

    for (y = 0; y < Y_SIZE; y += 10) {
        for (x = 0; x < X_SIZE; x += 120) {
            set_bit_xy(x, y, FALSE);
            if (get_bit_xy(x, y) != FALSE)
                printf("error: ");
            printf("x,y: %3d,%3d, set: %d, get: %d\n", x, y, FALSE, get_bit_xy(x, y));
        }
    }
}

int main(void)
{
    cbm_k_bsout(CH_FONT_UPPER);

    bit_test();
    bit_array_test();
    bit_array_test_pos();
    bit_array_test_xy();

    return EXIT_SUCCESS;
}
