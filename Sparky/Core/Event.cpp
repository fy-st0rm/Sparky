#include "Event.h"

// Global vars
std::map<int, int> Event::keys;
std::map<int, int> Event::buttons;

glm::vec2 Event::mouse_pos;

int Event::win_width;
int Event::win_height;


// Some event callbacks
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Event::buttons[button] = action;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Event::keys[key] = action;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Event::mouse_pos.x = xpos;
	Event::mouse_pos.y = ypos;
}

void window_resize_callback(GLFWwindow* window, int width, int height)
{
	Event::win_width = width;
	Event::win_height = height;
}


// Event class
bool Event::key_pressed(int key)
{
	if (Event::keys.find(key) != Event::keys.end())
	{
		if (Event::keys[key] == GLFW_PRESS)
		{
			Event::keys[key] = GLFW_RELEASE;
			return true;
		}
	}
	return false;
}

bool Event::key_hold(int key)
{
	if (Event::keys.find(key) != Event::keys.end())
	{
		if ((Event::keys[key] == GLFW_REPEAT) || (Event::keys[key] == GLFW_PRESS))
		{
			return true;
		}
	}
	return false;
}

bool Event::key_not_pressed(const std::vector<int>& keys)
{
	for (int i : keys)
	{
		if (Event::keys.find(i) != Event::keys.end())
		{
			if ((Event::keys[i] == GLFW_PRESS) || (Event::keys[i] == GLFW_REPEAT))
				return false;
		}
	}

	return true;
}

bool Event::mouse_button_pressed(int button)
{
	if (Event::buttons.find(button) != Event::buttons.end())
	{
		if (Event::buttons[button] == GLFW_PRESS)
		{
			Event::buttons[button] = GLFW_RELEASE;
			return true;
		}
	}
	return false;
}

bool Event::mouse_button_hold(int button)
{
	if (Event::buttons.find(button) != Event::buttons.end())
	{
		if ((Event::buttons[button] == GLFW_REPEAT) || (Event::buttons[button] == GLFW_PRESS))
		{
			return true;
		}
	}
	return false;
}

