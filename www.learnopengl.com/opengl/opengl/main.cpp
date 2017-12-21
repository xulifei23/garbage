#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include "global.h"
#include "shader.h"
#include "stb_image.h"

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
		0.5f, 0.5f, 0.0f,      1.0f, 1.0f,     1.0f, 0.0f, 0.0f, 
		0.5f, -0.5f, 0.0f,     1.0f, 0.0f,     0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,    0.0f, 0.0f,     0.0f, 0.0f, 1.0f, 
		-0.5f, 0.5f, 0.0f,     0.0f, 1.0f,     0.0f, 1.0f, 0.0f
	};

	unsigned int indices[] = 
	{
		0, 1, 2, 
		2, 3, 0
	};

	float texCoords[] = 
	{
		0.0f, 0.0f, 
		1.0f, 0.0f, 
		0.5f, 1.0f
	};

	int width = 0, height = 0, nrChannels = 0;
	unsigned char* data = stbi_load("../textures/wall.jpg", &width, &height, &nrChannels, 0);
	unsigned int texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned int VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	unsigned int VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	unsigned int EBO = 0;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(5 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
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
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}