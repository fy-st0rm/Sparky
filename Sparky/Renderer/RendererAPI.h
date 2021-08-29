#pragma once

#include "Core/Dependency.h"
#include "Entity/ComponentsAPI.h"
#include "Shader.h"

#include <array>
#include <vector>
#include <memory>
#include <algorithm>


struct Vertex
{
	glm::vec2 position;
	glm::vec2 texCoords;
	glm::vec4 color;
	float texID;
};

// Some constants references
const int MaxQuadCount = 1000;
const int MaxIndexCount = MaxQuadCount * 6;
const int MaxVertexBufferSize = MaxQuadCount * sizeof(Vertex) * 4;
const int MaxTextureSlot = 32;

// Rendering data storage
struct RendererData
{
	// Buffer objects IDs
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;

	int index_count = 0;

	// Quad storage
	std::vector<float> QuadBuffer;

	unsigned int white_texture = 0;
	unsigned int texture_slot_index = 1;
	std::array<unsigned int, MaxTextureSlot> texture_slots;

	// Shader
	Shader* def_shader;			// The shader we r gona create in runtime of the engine
	std::string vertex_shader;
	std::string fragment_shader;

	Shader* rendering_shader;	// the memory address of the currently using shader

	// Color
	glm::vec4 def_color = { 1.0f, 1.0f, 1.0f, 1.0f };
};


class RendererAPI
{
public:

	static RendererData r_Data;
	
	static void init();
	static void shutdown();

	// Shader thingy
	static void init_shader();
	static void UseShader(Shader* shader);

	// Render start and end function
	static void BeginBatch();
	static void EndBatch();
	static void Flush();
	
	// Quad drawing function
	static void draw_quad(Vertex* quad);

	// Statistics
	struct Stat
	{
		int quad_count = 0;
		int draw_count = 0;
	};

	static void ResetStats();
	static Stat GetStats();

};
