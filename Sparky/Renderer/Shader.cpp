#include "Shader.h"
#include "Core/Core.h"

#include <iostream>
#include <sstream>
#include <fstream>

Shader::Shader()
    :m_RendererID(0)
{

}

Shader::~Shader()
{
    Destroy();
}

void Shader::create(const std::string& VertexShader, const std::string& FragmentShader, bool raw)
{
    m_RendererID = CreateShader(VertexShader, FragmentShader, raw);   
}

void Shader::Destroy()
{
    std::cout << m_RendererID << std::endl;
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::change(const std::string& VertexShader, const std::string& FragmentShader, bool raw)
{
    Destroy();
    m_RendererID = CreateShader(VertexShader, FragmentShader, raw);   
}


void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}


int Shader::GetUniformLocation(const std::string& name)
{
    // Searching if the location already exists in the cache
    if (m_LocationCache.find(name) != m_LocationCache.end())
        return m_LocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Uniform " << name << " doesn't exist!!" << std::endl;
    else
        m_LocationCache[name] = location;

    return location;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& sourceFile, bool raw)
{
    std::stringstream shaderSrc;
    std::string shader;

    if (raw)    // Raw mode to directly providing the shader source code
    {
        shader = sourceFile;
    }
    else
    {
        // Reading the file
        std::ifstream stream(sourceFile);
        std::string line;
        while (std::getline(stream, line))
        {
            shaderSrc << line << "\n";
        }
        shader = shaderSrc.str();
    }

    GLCall(unsigned int id = glCreateShader(type)); // Creating the shader of the required type
    const char* src = shader.c_str();               // Converting the source into const char*
    GLCall(glShaderSource(id, 1, &src, nullptr));   // Providing our shader source code
    GLCall(glCompileShader(id));                    // Compiling the shader

    // Error handleling
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));  // Getting the result of the compilation of shader
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length)); // Getting the length of the error
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        // Printing out the error msg
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;

        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader, bool raw)
{
    GLCall(unsigned int program = glCreateProgram());    // Creating our shader program

    // Compiling our shaders
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader, raw);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader, raw);

    // Attaching the shaders into one
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    // Deleting the shader
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}
