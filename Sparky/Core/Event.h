#pragma once

#include "Dependency.h"

#include <map>
#include <vector>

// Event callbacks
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void window_resize_callback(GLFWwindow* window, int width, int height);


class Event
{
public:
	static std::map<int, int> keys;
	static std::map<int, int> buttons;
	static glm::vec2 mouse_pos;

	// Window width and height
	static int win_width;
	static int win_height;

	// Keyboard functions
	static bool key_pressed(int key);
	static bool key_hold(int key);
	static bool key_not_pressed(const std::vector<int>& keys);

	// Mouse functions
	static bool mouse_button_pressed(int button);
	static bool mouse_button_hold(int button);
};

