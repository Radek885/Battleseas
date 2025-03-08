#ifndef CAMERA_H
#define CAMERA_H

typedef struct
{
    float x, y, z; //pozycja kamery
    float katx, katy;
} Camera;

Camera NewCamera();
void setPosition(Camera camera);

#endif