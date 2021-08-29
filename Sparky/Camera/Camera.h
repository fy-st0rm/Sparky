#pragma once

#include "Core/Dependency.h"
#include "Renderer/Renderer.h"

class Camera
{
private:
	glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
	float m_Rotation = 0.0f;

	glm::mat4 m_ProjectionMat;
	glm::mat4 m_ViewMat;
	glm::mat4 m_ViewProjectionMat;	

public:
	Camera();
	~Camera();

	void Update(const std::string& matrix_uniform);

	void SetProjection(float left, float right, float bottom, float top);
	void SetPosition(const glm::vec3& position);
	void SetRotation(float rotation);

	inline glm::vec3 GetPosition() const  { return m_Position; }
	inline glm::mat4 GetMVP() { return m_ViewProjectionMat; }

	void Follow(const glm::vec2& position, const glm::vec2& offset);
	void CalculateMatrix();
};
