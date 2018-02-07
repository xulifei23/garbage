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
#include "model.h"

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
	glfwSetScrollCallback(window, scroll_callback);
	glEnable(GL_DEPTH_TEST);

	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	Model mod("../resource/car/Street environment_V01.obj");
	Shader shader("../shaders/lighting.vs", "../shaders/lighting.fg");

	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		ProcessInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		shader.setVec3("viewPos", cam.m_cameraPos);
		shader.setFloat("material.shininess", 32.0f);
		
		// directional light.
		shader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		shader.setVec3("dirLight.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
		shader.setVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		shader.setVec3("dirLight.sepcular", glm::vec3(0.5f, 0.5f, 0.5f));

		// point light 1.
		shader.setVec3("pointLight[0].position", pointLightPositions[0]);
		shader.setVec3("pointLight[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		shader.setVec3("pointLight[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		shader.setVec3("pointLight[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setFloat("pointLight[0].constant", 1.0f);
		shader.setFloat("pointLight[0].linear", 0.09);
		shader.setFloat("pointLight[0].quadratic", 0.032);

		// point light 2.
		shader.setVec3("pointLight[1].position", pointLightPositions[1]);
		shader.setVec3("pointLight[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		shader.setVec3("pointLight[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		shader.setVec3("pointLight[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setFloat("pointLight[1].constant", 1.0f);
		shader.setFloat("pointLight[1].linear", 0.09);
		shader.setFloat("pointLight[1].quadratic", 0.032);

		// point light 3.
		shader.setVec3("pointLight[2].position", pointLightPositions[2]);
		shader.setVec3("pointLight[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		shader.setVec3("pointLight[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		shader.setVec3("pointLight[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setFloat("pointLight[2].constant", 1.0f);
		shader.setFloat("pointLight[2].linear", 0.09);
		shader.setFloat("pointLight[2].quadratic", 0.032);

		// point light 4.
		shader.setVec3("pointLight[3].position", pointLightPositions[3]);
		shader.setVec3("pointLight[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		shader.setVec3("pointLight[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		shader.setVec3("pointLight[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setFloat("pointLight[3].constant", 1.0f);
		shader.setFloat("pointLight[3].linear", 0.09);
		shader.setFloat("pointLight[3].quadratic", 0.032);

		// spotLight
		shader.setVec3("spotLight.position", cam.m_cameraPos);
		shader.setVec3("spotLight.direction", cam.m_cameraTarget);
		shader.setVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("spotLight.diffuse", glm::vec3( 1.0f, 1.0f, 1.0f));
		shader.setVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setFloat("spotLight.constant", 1.0f);
		shader.setFloat("spotLight.linear", 0.09);
		shader.setFloat("spotLight.quadratic", 0.032);
		shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		glm::mat4 projection = glm::perspective(glm::radians(cam.m_Zoom),
			(float)screenWid / (float)screenHei, 0.1f, 100.0f);
		glm::mat4 view = cam.ViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setMat4("model", model);
		
		mod.Draw(shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}

	glfwTerminate();
	return 0;
}