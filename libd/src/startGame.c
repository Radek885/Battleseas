#include "startGame.h"
#include "titleScreen.h"
#include <libdragon.h>
#include <stdio.h>

void startGame() {
    //inicjalizacja ekranu
    display_init(RESOLUTION_320x240, DEPTH_32_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    console_set_render_mode(RENDER_MANUAL);

    debug_init_usblog();
    console_set_debug(true);

    showTitleScreen();
}
