#include "Components.h"


//---------Mode declaration-------
std::string SK_Mode::STATIC = "<static>";
std::string SK_Mode::DYNAMIC = "<dynamic>";

//----------RigidBody----------
std::vector<glm::vec4> RigidBody::check_intersect()
{
	std::vector<glm::vec4> hit_list;

	for (auto& rect : ComponentData::BoxColliders)
	{
		if (((m_Rect.x <= rect.x) && (rect.x <= m_Rect.x + m_Rect.z)) || ((rect.x <= m_Rect.x) && (m_Rect.x <= rect.x + rect.z)))
			if (((m_Rect.y <= rect.y) && (rect.y <= m_Rect.y + m_Rect.w)) || ((rect.y <= m_Rect.y) && (m_Rect.y <= rect.y + rect.w)))
				hit_list.push_back(rect);
	}
	return hit_list;
}

void RigidBody::EnableCollision(Entity* entity)
{
	auto hit_list = check_intersect();
	for (auto& rect : hit_list)
	{
		// Offsets of pixel in both x and y axis to cooperate x-axis and y-axis collider calculations (idk wat does this mean)
		float offsets[2] = { 0.1f, 0.1f };

		// For left and right collision
		if (((m_Rect.y <= rect.y) && (rect.y <= (m_Rect.y + m_Rect.w)-offsets[1])) || ((rect.y <= m_Rect.y+offsets[1]) && (m_Rect.y+offsets[1] <= rect.y + rect.w)))
		{
			// For left
			if ((m_Rect.x <= rect.x) && (rect.x <= (m_Rect.x + m_Rect.z )))
				entity->m_Position.x = rect.x - m_Rect.z;

			// For right
			if ((rect.x <= m_Rect.x) && (m_Rect.x <= (rect.x + rect.z)))
				entity->m_Position.x = rect.x + rect.z;
		}

		// For up and down collision
		if (((m_Rect.x <= rect.x) && (rect.x <= (m_Rect.x + m_Rect.z)-offsets[0])) || ((rect.x <= m_Rect.x+offsets[0]) && (m_Rect.x+offsets[0] <= rect.x + rect.z)))
		{
			// For up
			if ((m_Rect.y <= rect.y) && (rect.y <= (m_Rect.y + m_Rect.w)))
				entity->m_Position.y = rect.y - m_Rect.w;

			// For right
			if ((rect.y <= m_Rect.y) && (m_Rect.y <= (rect.y + rect.w)))
				entity->m_Position.y = rect.y + rect.w;
		}
	}
}

void RigidBody::DynamicPhysics(Entity* entity)
{
	entity->m_Position.y -= gravity;
	EnableCollision(entity);
}