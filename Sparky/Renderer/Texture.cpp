#include "Texture.h"


SK_Texture Texture2D::load_texture(const std::string& path, int vertical_flip)
{
	if (vertical_flip)
		stbi_set_flip_vertically_on_load(vertical_flip);

	// Temp vars
	unsigned int ID;
	unsigned char* m_LocalBuffer = nullptr;
	int m_Width, m_Height, m_BPP;

	std::ifstream ifile;
	ifile.open(path);
	if (ifile)
		m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);	// Loading the texture
	else
		std::cout << path << " not found!" << std::endl;

	// Generating the texture buffer
	GLCall(glGenTextures(1, &ID));
	GLCall(glBindTexture(GL_TEXTURE_2D, ID));

	// Setting up some basic modes to display texture
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	// Sending the pixel data to opengl
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);

	return { ID, (unsigned int)m_Width, (unsigned int)m_Height };
}