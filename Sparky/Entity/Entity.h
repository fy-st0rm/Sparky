#pragma once

#include "Core/Dependency.h"
#include "ComponentsAPI.h"

#include <vector>
#include <string>
#include <functional>


class Entity
{
private:
	std::vector<std::pair<std::string, Component*>> m_Components;

public:
	glm::vec2 m_Position;
	glm::vec2 m_Size;
	glm::vec4 m_Color;

	// For Spritesheet or texture
	glm::vec2 m_Offset;
	SK_Texture m_Texture;

public:
	Entity();
	~Entity();


	void Create(const glm::vec2& position, const glm::vec2& size);
	void Create(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
	void Create(const glm::vec2& position, const glm::vec2& size, const SK_Texture& texture);

	// Getters and setters
	void SetPosition(const glm::vec2& position) { m_Position = position; }
	void SetSize(const glm::vec2& size) { m_Size = size; }
	void SetOffset(const glm::vec4& offset) { m_Offset = offset; }
	void SetTexture(const SK_Texture& texture) { m_Texture = texture; }

	glm::vec2 GetPosition() { return m_Position; }
	glm::vec2 GetSize() { return m_Size; }
	glm::vec2 GetOffset() { return m_Offset; }
	SK_Texture GetTexture() { return m_Texture; }

	// Component stuff
	template<typename T>
	void AddComponent()
	{
		T* new_comp = new T();
		m_Components.push_back(std::make_pair(T().name, new_comp));
	}

	template<typename T>
	void RemoveComponent()
	{
		int index = 0;
		for (auto& i : m_Components)
		{
			if (i.first == T().name)
			{
				m_Components.erase(m_Components.begin() + index);
			}
			index++;
		}
	}

	template<typename T>
	Component* GetComponent()
	{
		for (auto& i : m_Components)
		{
			if (i.first == T().name)
			{
				return i.second;
			}
		}
		std::cout << T().name << " cannot be found!" << std::endl;
		return nullptr;
	}

	// Updating functions
	virtual void OnEvent() {}
	virtual void OnRender() = 0;

	void UpdateComponents();
	void Update()
	{
		OnEvent();
		UpdateComponents();
		OnRender();
	}
};
