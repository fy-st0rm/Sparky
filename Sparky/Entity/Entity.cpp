#include "Entity.h"


Entity::Entity() 
{

}

Entity::~Entity() 
{
	m_Components.clear();
}

void Entity::Create(const glm::vec2& position, const glm::vec2& size)
{
	m_Position = position;
	m_Size = size;
}

void Entity::Create(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	m_Position = position;
	m_Size = size;
	m_Color = color;
}

void Entity::Create(const glm::vec2& position, const glm::vec2& size, const SK_Texture& texture)
{
	m_Position = position;
	m_Size = size;
	m_Texture = texture;
}

void Entity::UpdateComponents()
{
	for (auto& component : m_Components)
	{
		auto* comp = component.second;
		comp->Update(this);
	}
}