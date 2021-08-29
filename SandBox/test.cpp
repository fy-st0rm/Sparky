#include "Sparky.h"


class Box : public Entity
{
public:
	Box() {}
	~Box() {}

	void OnRender()
	{
		Renderer::draw_quad(m_Position, m_Size, m_Color);
	}
};

class Ship : public Entity
{
private:
	float speed = 0.03f;

public:
	Ship(){}
	~Ship() {}

	void OnEvent()
	{
		if (Event::key_hold(GLFW_KEY_W))
			m_Position.y += speed;
		if (Event::key_hold(GLFW_KEY_S))
			m_Position.y -= speed;
		if (Event::key_hold(GLFW_KEY_A))
			m_Position.x -= speed;
		if (Event::key_hold(GLFW_KEY_D))
			m_Position.x += speed;
	}

	void OnRender()
	{
		Renderer::draw_quad(m_Position, m_Size, m_Texture[0]);
	}

};


class Test : public WorkSpace
{
private:
	//Shader shader;
	Camera camera;

	float aspectRatio;
	float zoom = 1.0f;

	Box box;
	Ship ship;

public:
	void OnStart() override
	{
		// Camera creation
		aspectRatio = (float)GetWidth() / (float)GetHeight();
		camera.SetProjection(-aspectRatio * zoom, aspectRatio * zoom, -zoom, zoom);

		// Entity stuff
		box.Create({-0.2f, -0.2f}, {0.8f, 0.8f}, {1.0f, 1.0f, 0.0f, 1.0f});
		box.AddComponent<BoxCollider>();

		ship.Create({0.5f, 0.8f}, {0.3f, 0.3f});
		ship.SetTexture(Texture2D::load_texture("Res/ship.png"));
		ship.AddComponent<RigidBody>();
	}

	void OnUpdate() override
	{
		Renderer::Clear({0.0f, 0.8f, 0.9f, 1.0f});

		camera.Update("u_MVP");

		Renderer::ResetStats();
		Renderer::BeginScene();

		box.Update();

		camera.Follow(ship.GetPosition(), {0.0f, 0.0f});
		ship.Update();

		Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		auto stat = RendererAPI::GetStats();
		std::string quad_count = "Quads: " + std::to_string(stat.quad_count);
		std::string draw_count = "Draw Call: " + std::to_string(stat.draw_count);

		ImGui::Text(draw_count.c_str());
		ImGui::Text(quad_count.c_str());

		if (ImGui::Button("Static"))
		{
			auto* rigidBody = ship.GetComponent<RigidBody>();
			rigidBody->Mode(SK_Mode::STATIC);
		}
		if (ImGui::Button("Dyamic"))
		{
			auto* rigidBody = ship.GetComponent<RigidBody>();
			rigidBody->Mode(SK_Mode::DYNAMIC);
		}	
	}
};

int main()
{
	{
		Test test;
		test.Run("Sparky", 800, 600);
		test.Shutdown();
	}
	glfwTerminate();
}