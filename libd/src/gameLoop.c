#include "gameLoop.h"
#include <libdragon.h>
#include <stdio.h>
#include <math.h> // Potrzebne do funkcji sin/cos
#include "teapot.h"

#define PI 3.14159265

void compute_lighting(float vertex[3], float normal[3], uint8_t *color) {
    float light_pos[3] = {0, 50, 50}; // Pozycja światła
    float light_dir[3] = {
        light_pos[0] - vertex[0],
        light_pos[1] - vertex[1],
        light_pos[2] - vertex[2]
    };
    
    float length = sqrt(light_dir[0] * light_dir[0] + light_dir[1] * light_dir[1] + light_dir[2] * light_dir[2]);
    light_dir[0] /= length;
    light_dir[1] /= length;
    light_dir[2] /= length;
    
    float dot = light_dir[0] * normal[0] + light_dir[1] * normal[1] + light_dir[2] * normal[2];
    if (dot < 0) dot = 0;
    
    color[0] = (uint8_t)(255); // Czerwony
    color[1] = (uint8_t)(dot * 255); // Zielony
    color[2] = (uint8_t)(dot * 255); // Niebieski
}

void gameLoop() {
    rdpq_init(); // Inicjalizacja RDP
    surface_t *screen;
	surface_t *zbuf;
    float angleX = 0.0f, angleY = 0.0f, angleZ = 0.0f; // Kąty obrotu dla trzech osi

    while (1) {
        screen = display_get();
		zbuf = display_get_zbuf();
        rdpq_attach(screen, zbuf);
        rdpq_clear(RGBA32(0, 0, 0, 255)); // Czyszczenie ekranu

        rdpq_set_mode_standard();
        rdpq_mode_combiner(RDPQ_COMBINER_SHADE);
		rdpq_mode_zbuf(true, false);
		rdpq_set_z_image(zbuf);

        // Obliczenie macierzy rotacji dla każdej osi
        float cosX = cos(angleX), sinX = sin(angleX);
        float cosY = cos(angleY), sinY = sin(angleY);
        float cosZ = cos(angleZ), sinZ = sin(angleZ);

        // Rysowanie modelu 3D z rotacją
        for (int i = 0; i < triangle_count; i++) {
            float v[3][3];
            float normal[3];
            uint8_t colors[3][3];

            for (int j = 0; j < 3; j++) {
                float x = vertices[triangles[i][j]][0];
                float y = vertices[triangles[i][j]][1];
                float z = vertices[triangles[i][j]][2];

                // Rotacja wokół osi X
                float y1 = y * cosX - z * sinX;
                float z1 = y * sinX + z * cosX;

                // Rotacja wokół osi Y
                float x2 = x * cosY + z1 * sinY;
                float z2 = -x * sinY + z1 * cosY;

                // Rotacja wokół osi Z
                float x3 = x2 * cosZ - y1 * sinZ;
                float y3 = x2 * sinZ + y1 * cosZ;

                v[j][0] = x3;
                v[j][1] = y3;
                v[j][2] = z2;
            }

            // Obliczenie normalnej trójkąta
            float u[3] = {v[1][0] - v[0][0], v[1][1] - v[0][1], v[1][2] - v[0][2]};
            float w[3] = {v[2][0] - v[0][0], v[2][1] - v[0][1], v[2][2] - v[0][2]};
            normal[0] = u[1] * w[2] - u[2] * w[1];
            normal[1] = u[2] * w[0] - u[0] * w[2];
            normal[2] = u[0] * w[1] - u[1] * w[0];
            float length = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
            normal[0] /= length;
            normal[1] /= length;
            normal[2] /= length;

			// Sprawdzenie, czy trójkąt jest widoczny
			float view_dir[3] = { 0, 0, -1 }; // Kierunek patrzenia kamery
			float dot_product = normal[0] * view_dir[0] + normal[1] * view_dir[1] + normal[2] * view_dir[2];

			if (dot_product > 0) continue; // Pomijamy trójkąty zwrócone do tyłu

            // Obliczanie cieniowania dla każdego wierzchołka
            for (int j = 0; j < 3; j++) {
                compute_lighting(v[j], normal, colors[j]);
            }

            // Przekształcenie współrzędnych do przestrzeni ekranu
            float screen_v1[7] = { v[0][0] * 10 + 160, v[0][1] * 10 + 120, v[0][2] * 10, 255, colors[0][0], colors[0][1], colors[0][2] };
            float screen_v2[7] = { v[1][0] * 10 + 160, v[1][1] * 10 + 120, v[1][2] * 10, 255, colors[1][0], colors[1][1], colors[1][2] };
            float screen_v3[7] = { v[2][0] * 10 + 160, v[2][1] * 10 + 120, v[2][2] * 10, 255, colors[2][0], colors[2][1], colors[2][2] };

            rdpq_triangle(&TRIFMT_ZBUF_SHADE, screen_v1, screen_v2, screen_v3);
        }

        display_show(screen);
        rdpq_detach();

        // Zwiększanie kątów obrotu
        angleX += 0.02f;
        angleY += 0.01f;
        angleZ += 0.01f;

        if (angleX > 2 * PI) angleX -= 2 * PI;
        if (angleY > 2 * PI) angleY -= 2 * PI;
        if (angleZ > 2 * PI) angleZ -= 2 * PI;
    }
}
