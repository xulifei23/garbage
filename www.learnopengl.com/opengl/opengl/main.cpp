#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include "global.h"
#include "shader.h"

using namespace std;

int main()
{
	if (!InitOpenGL(3, 3))
	{
		return -1;
	}

	GLFWwindow* window = CreateWnd(800, 600);
	if (window == nullptr)
	{
		glfwTerminate();
		return -1;
	}

	if (!InitGLAD())
	{
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int w, int h) 
	{
		glViewport(0, 0, w, h);
	});

	float vertices[] = 
	{
		0.0f, 0.5f, 0.0f, 
		0.5f, -0.5f, 0.0f, 
		-0.5f, -0.5f, 0.0f
	};

	float texCoords[] = 
	{
		0.0f, 0.0f, 
		1.0f, 0.0f, 
		0.5f, 1.0f
	};

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned int VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	unsigned int VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Shader shader("../shaders/shader.vs", "../shaders/shader.fg");

	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);

		glClearColor(FLTRGB(137), FLTRGB(145), FLTRGB(107), 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}