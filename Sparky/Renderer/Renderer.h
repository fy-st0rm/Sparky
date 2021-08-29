#pragma once

// API stuff
#include "RendererAPI.h"


class Renderer
{
public:
	// Initiailization 
	static void Init();
	static void Shutdown();
	static void ResetStats();

	// Shader cmds
	inline static Shader* GetShader() { return RendererAPI::r_Data.rendering_shader; }
	static void UseShader(Shader* shader);

	// Render calls
	static void BeginScene();
	static void EndScene();

	static void Clear(const glm::vec4& color);

	// Tint functions
	static void BeginTint(const glm::vec4& color);
	static void EndTint();


	// Draw calls
	static void draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
	static void draw_quad(const glm::vec2& position, const glm::vec2& size, unsigned int textureID);
	static void draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& offset, const SK_Texture& texture);
};