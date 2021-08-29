#pragma once

#include <iostream>
#include <array>
#include "GL/glew.h"

// Usefull definations
#define SK_Texture std::array<unsigned int, 3>


// Error handling stuff
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) ClearError(); \
    x;\
    ASSERT(ErrorLog(#x, __FILE__, __LINE__))

void ClearError();
bool ErrorLog(const char* function, const char* file, int line);
