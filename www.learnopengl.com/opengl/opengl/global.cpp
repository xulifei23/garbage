#include <glad/glad.h>
#include "global.h"
#include <glfw3.h>
#include <iostream>
#include <glm.hpp>

using namespace std;

int screenWid = 0;
int screenHei = 0;

float lastX = 0.0f;
float lastY = 0.0f;

extern float deltaTime = 0.0f;
Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));

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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cam.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cam.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cam.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cam.ProcessKeyboard(RIGHT, deltaTime);
	}
}

void CursorPosCallback(GLFWwindow* wnd, double posx, double posy)
{
	static bool firstMouse = true;

	if (firstMouse)
	{
		lastX = posx;
		lastY = posy;
		firstMouse = false;
	}

	float xOffset = posx - lastX;
	float yOffset = lastY - posy;

	lastX = posx;
	lastY = posy;

	cam.ProcessMouse(xOffset, yOffset);
}

void printVec(glm::vec3 vec)
{
	cout << vec.x << ' ' << vec.y << ' ' << vec.z << endl;
}

