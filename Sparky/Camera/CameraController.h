#pragma once

#include "Core/Dependency.h"
#include "Camera.h"
#include "Core/Event.h"


class CameraController
{
private:
	float m_AspectRatio;

	glm::vec3 m_CamPos = {0.0f, 0.0f, 0.0f};
	float m_Zoom = 1.0f;
	float m_Speed = 0.01f;

	Camera camera;

public:
	CameraController();
	~CameraController();

	void Create(float aspectRatio);
	void Update();

	inline Camera* GetCamera() { return &camera; }
};