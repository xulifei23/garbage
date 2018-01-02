#ifndef CAMERA_H
#define CAMERA_H

#include <glm.hpp>

enum CameraMovement
{
	FORWARD = 0, 
	BACKWARD, 
	LEFT, 
	RIGHT,
};

class Camera
{
public:
	Camera(glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, -1.0f));
	~Camera() = default;

	glm::mat4 ViewMatrix();
	void ProcessKeyboard(CameraMovement movement, float deltaTime);
	void ProcessMouse(float xOffset, float yOffset, bool constrainPitch = true);
	void UpdateCameraVector();

public:
	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraRight;
	glm::vec3 m_cameraUp;
	float m_yaw;
	float m_pitch;
	const float m_moveSpeed = 2.5f;
	const float m_mouseSensitivity = 0.1f;
	const glm::vec3 m_upWorld = glm::vec3(0.0f, 1.0f, 0.0f);
};

#endif // CAMERA_H