#include "Core.h"


// Error handling
void ClearError()
{
    while(glGetError());
}

bool ErrorLog(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[Error code]: " << error << std::endl;
        std::cout << "[Opengl error]: " << function << " " << file << ": " << line << std::endl;
        return false;
    }
    return true;
}