#include <glad/glad.h>
#include "global.h"
#include <glfw3.h>
#include <iostream>
using namespace std;

bool InitOpenGL(int vmarjor, int vminor)
{
	if (glfwInit() == GLFW_FALSE)
	{
		cout << "initialize glfw failed." << endl;
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, vmarjor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, vminor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	return true;
}

GLFWwindow* CreateWnd(int width, int height,GLFWmonitor* pm)
{
	GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", pm, nullptr);
	if (window == nullptr)
	{
		cout << "create window failed." << endl;
		return window;
	}

	glfwMakeContextCurrent(window);
	return window;
}

bool InitGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "initialize glad failed." << endl;
		return false;
	}

	return true;
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}
