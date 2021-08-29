#pragma once

#include "Core/Dependency.h"
#include <vector>

class Entity;

struct ComponentData
{
	// Collider
	static std::vector<glm::vec4> BoxColliders;

	static void ClearCache();
};

class Component
{
public:
	std::string name = "<Component>";

public:
	Component() {}

	virtual void Mode(const std::string& mode) {}
	virtual void Update(Entity* entity) = 0;
};

