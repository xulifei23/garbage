#include <glad/glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include "global.h"
#include "shader.h"
#include "stb_image.h"
#include "camera.h"

using namespace std;

int main()
{
	if (!InitOpenGL(3, 3))
	{
		return -1;
	}

	int monitorCount = 0;
	GLFWmonitor** pm = glfwGetMonitors(&monitorCount);
	for (int i = 0; i < monitorCount; ++i)
	{
		const GLFWvidmode* pv = glfwGetVideoMode(pm[i]);
		screenWid = pv->width > screenWid ? pv->width : screenWid;
		screenHei = pv->height > screenHei ? pv->height : screenHei;
	}

	screenWid = 800;
	screenHei = 600;

	lastX = (float)screenWid / 2;
	lastY = (float)screenHei / 2;
	GLFWwindow* window = CreateWnd(screenWid, screenHei, nullptr/*pm[0]*/);
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

	glViewport(0, 0, screenWid, screenHei);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int w, int h) 
	{
		glViewport(0, 0, w, h);
	});

	glfwSetCursorPosCallback(window, CursorPosCallback);

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	glm::vec3 lambPos = glm::vec3(1.2f, 1.0f, 2.0f);
	glm::vec3 lightingPos = glm::vec3(0.0f, 0.0f, 0.0f);

	unsigned int lightingVAO = 0;
	glGenVertexArrays(1, &lightingVAO);
	unsigned int VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(lightingVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 + 3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 + 6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int lambVAO = 0;
	glGenVertexArrays(1, &lambVAO);
	glBindVertexArray(lambVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	unsigned int diffuseMap = loadTexture("../textures/container2.png");
	unsigned int specularMap = loadTexture("../textures/container2_specular.png");
	
	Shader lambShader("../shaders/lamb.vs", "../shaders/lamb.fg");	
	Shader lightingShader("../shaders/lighting.vs", "../shaders/lighting.fg");
	glEnable(GL_DEPTH_TEST);
	float lastFrame = glfwGetTime();
	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);

	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		ProcessInput(window);

		glClearColor(FLTRGB(15), FLTRGB(37), FLTRGB(64), 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glBindVertexArray(lightingVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		lightingShader.use();
		// view matrix
		glm::mat4 view;
		view = cam.ViewMatrix();
		lightingShader.setMat4("view", view);
		// projection matrix
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)screenWid / screenHei, 0.1f, 100.0f);
		lightingShader.setMat4("projection", projection);

		glm::vec3 cubeColor = glm::vec3(1.0f, 0.5f, 0.31f);

		lightingShader.setVec3("cubeColor", cubeColor);
		lightingShader.setVec3("lambPos", lambPos);
		lightingShader.setVec3("viewPos", cam.m_cameraPos);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setVec3("light.position", lambPos);
		lightingShader.setVec3("light.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
		lightingShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		lightingShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		glm::mat4 lightModel;
		lightModel = glm::translate(lightModel, lightingPos);
		lightingShader.setMat4("model", lightModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(lambVAO);
		lambShader.use();
		lambShader.setMat4("view", view);
		lambShader.setMat4("projection", projection);
		glm::mat4 lambModel;
		lambModel = glm::translate(lambModel, lambPos);
		lambModel = glm::scale(lambModel, glm::vec3(0.2f));
		lambShader.setMat4("model", lambModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		
		glfwSwapBuffers(window);
		glfwPollEvents();

		glBindVertexArray(0);
	}

	glDeleteVertexArrays(1, &lightingVAO);
	glDeleteVertexArrays(1, &lambVAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}