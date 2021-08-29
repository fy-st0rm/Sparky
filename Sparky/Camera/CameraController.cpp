#include "CameraController.h"


CameraController::CameraController()
{

}

CameraController::~CameraController()
{

}

void CameraController::Create(float aspectRatio)
{
	m_AspectRatio = aspectRatio;

	camera.SetProjection(-m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom);
	camera.SetPosition(m_CamPos);
}

void CameraController::Update()
{
	//m_Speed = m_Zoom / 5.0f;

	if (Event::key_hold(GLFW_KEY_W))
		m_CamPos.y += m_Speed;
	if (Event::key_hold(GLFW_KEY_S))
		m_CamPos.y -= m_Speed;
	if (Event::key_hold(GLFW_KEY_A))
		m_CamPos.x -= m_Speed;
	if (Event::key_hold(GLFW_KEY_D))
		m_CamPos.x += m_Speed;

	if (Event::key_hold(GLFW_KEY_Q))
		m_Zoom -= 0.02f;
	if (Event::key_hold(GLFW_KEY_E))
		m_Zoom += 0.02f;

	m_Zoom = std::max(m_Zoom, 0.25f);
	camera.SetProjection(-m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom);
	camera.SetPosition(m_CamPos);
}