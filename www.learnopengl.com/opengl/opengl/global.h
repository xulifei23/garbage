#ifndef GLOBAL_H
#define GLOBAL_H

#include <vector>
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
unsigned int loadTexture(const char* path);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#define FLTRGB(x) ((float)x / 255)

extern int screenWid;
extern int screenHei;

extern float lastX;
extern float lastY;

extern float deltaTime;
extern float lastFrame;

extern Camera cam;

template <typename T>
std::vector<T>& operator <<(std::vector<T>& vec, T& t)
{
	vec.push_back(t);
	return vec;
}

#endif // GLOBAL_H
