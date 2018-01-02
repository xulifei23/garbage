#include "camera.h"
#include <glfw3.h>
#include <gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 cameraPos /* = glm::vec3(0.0f, 0.0f, 3.0f) */, glm::vec3 cameraTarget /* = glm::vec3(0.0f, 0.0f, -1.0f) */)
	:m_cameraPos(cameraPos)
	,m_cameraTarget(cameraTarget)
	,m_yaw(-90.0f)
	,m_pitch(0.0f)
{
	m_cameraRight = glm::normalize(glm::cross(m_cameraTarget, m_upWorld));
	m_cameraUp = glm::normalize(glm::cross(m_cameraRight, m_cameraTarget));
}

glm::mat4 Camera::ViewMatrix()
{
	return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraTarget, m_cameraUp);
}

void Camera::ProcessKeyboard(CameraMovement movement, float deltaTime)
{
	float velocity = m_moveSpeed * deltaTime;
	if (movement == FORWARD)
	{
		m_cameraPos += m_cameraTarget * velocity;
	}
	else if (movement == BACKWARD)
	{
		m_cameraPos -= m_cameraTarget * velocity;
	}
	else if (movement == LEFT)
	{
		m_cameraPos -= m_cameraRight * velocity;
	}
	else if (movement == RIGHT)
	{
		m_cameraPos += m_cameraRight * velocity;
	}

	return;
}

void Camera::ProcessMouse(float xOffset, float yOffset, bool constrainPitch)
{
	xOffset *= m_mouseSensitivity;
	yOffset *= m_mouseSensitivity;

	m_yaw += xOffset;
	m_pitch += yOffset;

	if (constrainPitch)
	{
		if (m_pitch < -89.0f)
		{
			m_pitch = -89.0f;
		}
		else if (m_pitch > 89.0f)
		{
			m_pitch = 89.0f;
		}
	}

	UpdateCameraVector();
}

void Camera::UpdateCameraVector()
{
	glm::vec3 target;
	target.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	target.y = sin(glm::radians(m_pitch));
	target.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	m_cameraTarget = glm::normalize(target);
	m_cameraRight = glm::normalize(glm::cross(m_cameraTarget, m_upWorld));
	m_cameraUp = glm::normalize(glm::cross(m_cameraRight, m_cameraTarget));
}
