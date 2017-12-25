#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <glm.hpp>
#include "camera.h"

struct GLFWwindow;
struct GLFWmonitor;

bool InitOpenGL(int vmarjor, int vminor);
GLFWwindow* CreateWnd(int width, int height, GLFWmonitor* pm);
bool InitGLAD();
void ProcessInput(GLFWwindow* window);
void CursorPosCallback(GLFWwindow* wnd, double posx, double posy);
void printVec(glm::vec3 vec);

#define FLTRGB(x) ((float)x / 255)

extern int screenWid;
extern int screenHei;

extern float lastX;
extern float lastY;

extern float deltaTime;

extern Camera cam;

#endif // GLOBAL_H
