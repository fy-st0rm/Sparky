#include "ComponentsAPI.h"

// Box Collider
std::vector<glm::vec4> ComponentData::BoxColliders;

void ComponentData::ClearCache()
{
	ComponentData::BoxColliders.clear();
}