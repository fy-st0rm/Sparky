#pragma once

#include "Core/Dependency.h"
#include "Entity.h"
#include "Renderer/Renderer.h"

#include <iostream>
#include <string>


struct SK_Mode
{
	static std::string STATIC;
	static std::string DYNAMIC;
};


class BoxCollider : public Component
{
/*
	BoxCollider: a traditional box collider component which produces a quad hitbox acc to ent's pos and size and stores it.
*/

private:
	glm::vec4 m_Rect;

public:
	BoxCollider() {	name = "<BoxCollider>"; }
	~BoxCollider() {}

	void Update(Entity* entity)
	{
		m_Rect = {entity->m_Position, entity->m_Size};
		ComponentData::BoxColliders.push_back(m_Rect);
	}
};


class RigidBody : public Component
{
/*
	RigidBody: a s1mple 2D physics which includes both dynamic and static physics.
*/
private:
	glm::vec4 m_Rect;
	std::string m_Mode = SK_Mode::STATIC;

	//-- For Dynamic physics
	float gravity = 0.03f;

public:
	RigidBody() { name = "<RigidBody>"; }
	~RigidBody() {}

	void Mode(const std::string& mode) override
	{
		m_Mode = mode;
	}

	void Update(Entity* entity)
	{
		m_Rect = {entity->m_Position, entity->m_Size};

		//std::cout <<  m_Mode << std::endl;
		if (m_Mode == SK_Mode::STATIC)
			EnableCollision(entity);
		else if (m_Mode == SK_Mode::DYNAMIC)
			DynamicPhysics(entity);
	}

private:
	std::vector<glm::vec4> check_intersect();
	void EnableCollision(Entity* entity);
	void DynamicPhysics(Entity* entity);
};
