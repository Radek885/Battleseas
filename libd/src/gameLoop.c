#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>

void render_triangle()
{
	// Begin OpenGL compatibility with the RDP
	gl_context_begin();

	// Set the camera's position
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		0, -10.0, 0.0,
		0, 0, 0,
		0, 0, 1
		);

	// Draw the triangle
	glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f); // Red color
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
	glEnd();

    glBegin(GL_TRIANGLES);
		glColor3f(0.0f, 1.0f, 1.0f);
		glVertex3f(-0.5f, 1.0f, -1.0f);
		glVertex3f(-0.5f, 1.0f, 1.0f);
		glVertex3f(1.5f, 1.0f, 1.0f);
	glEnd();

	// Does nothing for now, but keep it in case
	gl_context_end();
}

void gameLoop() {
    rdpq_init();
	gl_init();

    // Setup
    float aspect_ratio = (float)display_get_width() / (float)display_get_height();
    float near_plane = 1.0f;
    float far_plane = 50.0f;

    // Set the viewing area
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(
        -near_plane*aspect_ratio,
        near_plane*aspect_ratio,
        -near_plane,
        near_plane,
        near_plane,
        far_plane
        );

	// Main loop
	while(1) {
		// Start a new frame
		// Get the frame buffer and z-buffer
		surface_t* disp = display_get();
		surface_t* zbuf = display_get_zbuf();
		// Attach the buffers to the RDP
		rdpq_attach_clear(disp, zbuf);

		// Fill the background with white
		graphics_fill_screen(disp, graphics_make_color(0,0,0,0));

		// Render a triangle with OpenGL using the function above
		render_triangle();

		// Send frame buffer to display (TV)
		rdpq_detach_show();
	}
}

//hue hue