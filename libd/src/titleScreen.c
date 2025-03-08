#include "titleScreen.h"
#include <libdragon.h>
#include "gameLoop.h"
#include <stdio.h>

typedef struct 
{
    uint16_t x, y;
    uint16_t width, height;
    uint32_t color;
} Rectangle;


void showTitleScreen() {
    Rectangle prostokat;
    prostokat.color = graphics_make_color(70,70,70,255);
    prostokat.x = 320/6;
    prostokat.y = 240/5;
    prostokat.width = (320/3)*2;
    prostokat.height = 240/3;

    uint32_t titleTxtColor = graphics_make_color(200,10,10,255);
    graphics_set_color(titleTxtColor, 0);
    
    surface_t *screen = display_get();
    graphics_draw_box(screen, prostokat.x, prostokat.y, prostokat.width, prostokat.height, prostokat.color);
    graphics_draw_text(screen,130, 80, "Battleseas");

    //debug wyświetlenie rozdzielczości ekranu
    // int szer = display_get_width();
    // int wys = display_get_height();

    // char buffer[32];
    // sprintf(buffer, "Width: %d", szer);
    // graphics_draw_text(screen, 10, 10, buffer);
    
    // sprintf(buffer, "Height: %d", wys);
    // graphics_draw_text(screen, 10, 30, buffer);
    //

    graphics_draw_text(screen,130, 160, "Press Start");
    display_show(screen);

    controller_init();
    while(1) 
    {
        controller_scan();
        struct controller_data keys = get_keys_down();

        if(keys.c[0].start || keys.c[0].A)
            gameLoop();

        titleTxtColor += 256;
        graphics_set_color(titleTxtColor, 0);
        graphics_draw_text(screen,130, 160, "Press Start");
        wait_ms(16);
    }
}
