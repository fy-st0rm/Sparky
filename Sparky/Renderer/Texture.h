#pragma once

#include <fstream>
#include "Core/Dependency.h"


class Texture2D
{
public:
	static SK_Texture load_texture(const std::string& path, int vertical_flip=1);
};

