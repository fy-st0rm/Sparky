#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"


class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_Filepath;
	std::unordered_map<std::string, int> m_LocationCache;

public:
	Shader(const std::string& VertexShader, const std::string& FragmentShader);
	Shader();
	~Shader();

	void create(const std::string& VertexShader, const std::string& FragmentShader, bool raw=false);
	void Destroy();
	void change(const std::string& VertexShader, const std::string& FragmentShader, bool raw=false);

	void Bind() const;
	void Unbind() const;

	int GetUniformLocation(const std::string& name);

private:
	unsigned int CompileShader(unsigned int type, const std::string& source, bool raw=false);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader, bool raw=false);
};