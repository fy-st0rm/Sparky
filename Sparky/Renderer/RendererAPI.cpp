#include "RendererAPI.h"


static RendererAPI::Stat Renderer_stat;
RendererData RendererAPI::r_Data;

/////////////////////RendererAPI initialization and main mechanism/////////////////////

void RendererAPI::init()
{
	// Creating vertex array
	GLCall(glGenVertexArrays(1, &r_Data.VAO));
	GLCall(glBindVertexArray(r_Data.VAO));

	// Creating vertex buffer
	GLCall(glGenBuffers(1, &r_Data.VBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, r_Data.VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, MaxVertexBufferSize, nullptr, GL_DYNAMIC_DRAW));

	// Creating layout for the buffer
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position)));

	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texCoords)));

	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color)));

	GLCall(glEnableVertexAttribArray(3));
	GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texID)));

	// Generating indices
	unsigned int indices[MaxIndexCount];
	for (int offset = 0, i = 0; i < MaxIndexCount; i += 6)
	{
		indices[0 + i] = 0 + offset;
		indices[1 + i] = 1 + offset;
		indices[2 + i] = 2 + offset;

		indices[3 + i] = 2 + offset;
		indices[4 + i] = 3 + offset;
		indices[5 + i] = 0 + offset;
		offset += 4;
	}

	GLCall(glGenBuffers(1, &r_Data.IBO));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r_Data.IBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	// Generating a 1x1 white texture
	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &r_Data.white_texture));
	GLCall(glBindTexture(GL_TEXTURE_2D, r_Data.white_texture));

	// Setting up some basic modes to display texture
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	unsigned int white = 0xffffffff;
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &white));

	r_Data.texture_slots[0] = r_Data.white_texture;
	for (int i = 1; i < MaxTextureSlot; i++)
	{
		r_Data.texture_slots[i] = 0;
	}

	init_shader();
}

//----Function to initialize the default engine's shader
void RendererAPI::init_shader()
{
	// Creating default shader
	r_Data.vertex_shader = 
	"#version 330 core\n"
	"\n"
	"layout(location = 0) in vec4 Position;\n"
	"layout(location = 1) in vec2 TexCoords;\n"
	"layout(location = 2) in vec4 Color;\n"
	"layout(location = 3) in float ID;\n"
	"\n"
	"out vec2 v_TexCoords;\n"
	"out vec4 v_Color;\n"
	"out float v_ID;\n"
	"\n"
	"uniform mat4 u_MVP;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	gl_Position = u_MVP * Position;\n"
	"\n"
	"	v_TexCoords = TexCoords;\n"
	"	v_Color = Color;\n"
	"	v_ID = ID;\n"
	"}\n";

	r_Data.fragment_shader = 
	"#version 330 core\n"
	"\n"
	"layout(location = 0) out vec4 color;\n"
	"\n"
	"in vec2 v_TexCoords;\n"
	"in vec4 v_Color;\n"
	"in float v_ID;\n"
	"\n"
	"uniform sampler2D u_Textures[32];\n"
	"\n"
	"void main()\n"
	"{\n"	
	"	int index = int(v_ID);\n"
	"	vec4 texColor = texture(u_Textures[index], v_TexCoords) * v_Color;\n"
	"	color = texColor;\n"
	"}\n";

	r_Data.def_shader = new Shader();
	r_Data.def_shader->create(r_Data.vertex_shader, r_Data.fragment_shader, true);
	r_Data.def_shader->Bind();

	// Provide samplers
	int samplers[MaxTextureSlot];
	for (int i = 0; i < MaxTextureSlot; i++)
	{
		samplers[i] = i;
	}
	GLCall(glUniform1iv(r_Data.def_shader->GetUniformLocation("u_Textures"), MaxTextureSlot, samplers));

	UseShader(r_Data.def_shader);
}

void RendererAPI::shutdown()
{
	GLCall(glDeleteVertexArrays(1, &r_Data.VAO));
	GLCall(glDeleteBuffers(1, &r_Data.VBO));
	GLCall(glDeleteBuffers(1, &r_Data.IBO));

	GLCall(glDeleteTextures(1, &r_Data.white_texture));

	for (auto& i : r_Data.texture_slots)
	{
		GLCall(glDeleteTextures(1, &i));
	}
}

void RendererAPI::BeginBatch()
{
	ComponentData::ClearCache();
	r_Data.QuadBuffer.clear();
}

void RendererAPI::EndBatch()
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, r_Data.VBO));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, r_Data.QuadBuffer.size() * sizeof(float), r_Data.QuadBuffer.data()));
}

void RendererAPI::Flush()
{
	r_Data.rendering_shader->Bind();

	for (unsigned int i = 0; i < r_Data.texture_slot_index; i++)
	{ 
		GLCall(glBindTextureUnit(i, r_Data.texture_slots[i]));
	}

	GLCall(glBindVertexArray(r_Data.VAO));
	GLCall(glDrawElements(GL_TRIANGLES, r_Data.index_count, GL_UNSIGNED_INT, nullptr));
	
	Renderer_stat.draw_count++;
	r_Data.index_count = 0;
}

void RendererAPI::UseShader(Shader* shader)
{
	r_Data.rendering_shader = shader;
}

/////////////////////Quads creation/////////////////////

void RendererAPI::draw_quad(Vertex* quad)
{
	// Pushing vertices into quad buffer
	for (int i = 0; i < 4; i++)
	{
		r_Data.QuadBuffer.push_back(quad[i].position.x);
		r_Data.QuadBuffer.push_back(quad[i].position.y);

		r_Data.QuadBuffer.push_back(quad[i].texCoords.x);
		r_Data.QuadBuffer.push_back(quad[i].texCoords.y);

		r_Data.QuadBuffer.push_back(quad[i].color.r);
		r_Data.QuadBuffer.push_back(quad[i].color.g);
		r_Data.QuadBuffer.push_back(quad[i].color.b);
		r_Data.QuadBuffer.push_back(quad[i].color.a);

		r_Data.QuadBuffer.push_back(quad[i].texID);
	}

	r_Data.index_count += 6;
	Renderer_stat.quad_count++;
}

/////////////////////Some Render statistics thingy/////////////////////

void RendererAPI::ResetStats()
{
	Renderer_stat.draw_count = 0;
	Renderer_stat.quad_count = 0;
}

RendererAPI::Stat RendererAPI::GetStats()
{
	return Renderer_stat;
}
