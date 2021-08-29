#include "Renderer.h"

// API initialization and shutdown stuff
void Renderer::Init()
{
	RendererAPI::init();
}

void Renderer::Shutdown()
{
	RendererAPI::shutdown();
}

void Renderer::ResetStats()
{
	RendererAPI::ResetStats();
}

// Shader cmds
void Renderer::UseShader(Shader* shader)
{
	RendererAPI::UseShader(shader);
}

// Render calls

void Renderer::BeginScene()
{
	RendererAPI::BeginBatch();
}

void Renderer::EndScene()
{
	RendererAPI::EndBatch();
	RendererAPI::Flush();
}

void Renderer::Clear(const glm::vec4& color)
{
	GLCall(glClearColor(color.r, color.g, color.b, color.a));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

// Tinting function
void Renderer::BeginTint(const glm::vec4& color)
{
	RendererAPI::r_Data.def_color = color;
}

void Renderer::EndTint()
{
	RendererAPI::r_Data.def_color = {1.0f, 1.0f, 1.0f, 1.0f};
}


// Renderer draw calls

// to render the plane quad with colors
void Renderer::draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	if (RendererAPI::r_Data.index_count >= MaxIndexCount)
	{
		EndScene();
		BeginScene();
	}

	float texID = 0.0f;
	
	// Creating vertices of QUAD
	Vertex v1, v2, v3, v4;

	v1.position = { position.x, position.y };
	v2.position = { position.x + size.x, position.y };
	v3.position = { position.x + size.x, position.y + size.y };
	v4.position = { position.x, position.y + size.y };

	v1.texCoords = { 0.0f, 0.0f };
	v2.texCoords = { 1.0f, 0.0f };
	v3.texCoords = { 1.0f, 1.0f };
	v4.texCoords = { 0.0f, 1.0f };

	v1.color = color;
	v2.color = color;
	v3.color = color;
	v4.color = color;

	v1.texID = texID;
	v2.texID = texID;
	v3.texID = texID;
	v4.texID = texID;

	Vertex quad[] = { v1, v2, v3, v4 };
	RendererAPI::draw_quad(quad);
}

// to render a quad with texture
void Renderer::draw_quad(const glm::vec2& position, const glm::vec2& size, unsigned int textureID)
{
	if (RendererAPI::r_Data.index_count >= MaxIndexCount)
	{
		EndScene();
		BeginScene();
	}

	float texID = 0.0f;
	glm::vec4 color = RendererAPI::r_Data.def_color;

	// Checking if the texture already exists. If yes use that else create a new one
	for (int i = 0; i < RendererAPI::r_Data.texture_slot_index; i++)
	{
		if (RendererAPI::r_Data.texture_slots[i] == textureID)
			texID = (float)i;
	}

	if (texID == 0.0f)
	{
		RendererAPI::r_Data.texture_slots[RendererAPI::r_Data.texture_slot_index] = textureID;
		texID = (float)RendererAPI::r_Data.texture_slot_index;
		RendererAPI::r_Data.texture_slot_index++;
	}

	// Creating vertices of QUAD
	Vertex v1, v2, v3, v4;

	v1.position = { position.x, position.y };
	v2.position = { position.x + size.x, position.y };
	v3.position = { position.x + size.x, position.y + size.y };
	v4.position = { position.x, position.y + size.y };

	v1.texCoords = { 0.0f, 0.0f };
	v2.texCoords = { 1.0f, 0.0f };
	v3.texCoords = { 1.0f, 1.0f };
	v4.texCoords = { 0.0f, 1.0f };

	v1.color = color;
	v2.color = color;
	v3.color = color;
	v4.color = color;

	v1.texID = texID;
	v2.texID = texID;
	v3.texID = texID;
	v4.texID = texID;

	Vertex quad[] = { v1, v2, v3, v4 };
	RendererAPI::draw_quad(quad);
}

// to render quad using spritesheets
void Renderer::draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& offset, const SK_Texture& texture)
{
	if (RendererAPI::r_Data.index_count >= MaxIndexCount)
	{
		EndScene();
		BeginScene();
	}

	float texID = 0.0f;
	glm::vec4 color = RendererAPI::r_Data.def_color;

	// Checking if the texture already exists. If yes use that else create a new one
	for (int i = 0; i < RendererAPI::r_Data.texture_slot_index; i++)
	{
		if (RendererAPI::r_Data.texture_slots[i] == texture[0])
			texID = (float)i;
	}

	if (texID == 0.0f)
	{
		RendererAPI::r_Data.texture_slots[RendererAPI::r_Data.texture_slot_index] = texture[0];
		texID = (float)RendererAPI::r_Data.texture_slot_index;
		RendererAPI::r_Data.texture_slot_index++;
	}

	// Creating vertices of QUAD
	Vertex v1, v2, v3, v4;

	v1.position = { position.x, position.y };
	v2.position = { position.x + size.x, position.y };
	v3.position = { position.x + size.x, position.y + size.y };
	v4.position = { position.x, position.y + size.y };

	v1.texCoords = { (offset.x * offset.z)/texture[1],         (offset.y * offset.w) / texture[2] };
	v2.texCoords = { ((offset.x + 1) * offset.z) / texture[1], (offset.y * offset.w) / texture[2] };
	v3.texCoords = { ((offset.x + 1) * offset.z) / texture[1], ((offset.y + 1) * offset.w) / texture[2] };
	v4.texCoords = { (offset.x * offset.z) / texture[1],       ((offset.y + 1) * offset.w) / texture[2] };

	v1.color = color;
	v2.color = color;
	v3.color = color;
	v4.color = color;

	v1.texID = texID;
	v2.texID = texID;
	v3.texID = texID;
	v4.texID = texID;

	Vertex quad[] = { v1, v2, v3, v4 };
	RendererAPI::draw_quad(quad);
}