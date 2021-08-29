#include "Camera.h"


Camera::Camera()
	:m_ProjectionMat(glm::ortho(0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f)), m_ViewMat(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)))
{
	m_ViewProjectionMat = m_ProjectionMat * m_ViewMat;
}

Camera::~Camera()
{

}

void Camera::SetProjection(float left, float right, float bottom, float top)
{
	m_ProjectionMat = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	m_ViewProjectionMat = m_ProjectionMat * m_ViewMat;
}

void Camera::SetPosition(const glm::vec3& position)
{
	m_Position = position;
	CalculateMatrix();
}

void Camera::SetRotation(float rotation)
{
	m_Rotation = rotation;
	CalculateMatrix();
}

void Camera::CalculateMatrix()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
	m_ViewMat = glm::inverse(transform);
	m_ViewProjectionMat = m_ProjectionMat * m_ViewMat;
}

void Camera::Follow(const glm::vec2& position, const glm::vec2& offset)
{
	m_Position.x += (position.x - m_Position.x - offset.x);/// 10;
	m_Position.y += (position.y - m_Position.y - offset.y);// / 10;
	CalculateMatrix();
}

void Camera::Update(const std::string& matrix_uniform)
{
	glm::mat4 mvp = GetMVP();
	GLCall(glUniformMatrix4fv(Renderer::GetShader()->GetUniformLocation(matrix_uniform), 1, GL_FALSE, &mvp[0][0]));
}