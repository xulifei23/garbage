#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>

struct GLFWwindow;

bool InitOpenGL(int vmarjor, int vminor);
GLFWwindow* CreateWnd(int width, int height);
bool InitGLAD();
void ProcessInput(GLFWwindow* window);

#define FLTRGB(x) ((float)x / 255)

#endif // GLOBAL_H
