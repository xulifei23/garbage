#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>

struct GLFWwindow;

bool InitOpenGL(int vmarjor, int vminor);
GLFWwindow* CreateWnd(int width, int height);
bool InitGLAD();
void ProcessInput(GLFWwindow* window);

#endif // GLOBAL_H
