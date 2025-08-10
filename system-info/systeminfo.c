/**
 * System Info
 */

#include <c64.h>
#include <cc65.h>
#include <conio.h>
#include <ctype.h>
#include <modload.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgi.h>

int main(void)
{
    // setup tgi
    tgi_install(tgi_static_stddrv);
    tgi_init();

    // output system info
    printf("char, int, long sizes: %d, %d, %d\n",
           sizeof((char) 0), sizeof((int) 0), sizeof((long) 0));
    printf("x res: %d, y res: %d, colors: %d\n",
           tgi_getxres(), tgi_getyres(), tgi_getcolorcount());

    // cleanup tgi
    tgi_uninstall();

    return EXIT_SUCCESS;
}
