#pragma once

#include "Core/Dependency.h"
#include "Gui/Gui.h"
#include "Core/Event.h"
#include "Renderer/Renderer.h"

#include <string>

class WorkSpace
{
private:
	std::string m_Title;
	GLFWwindow* window;

	bool show_ui = false;

	// Fps stuff
	double previous_tick;
	int fps;
	int fps_counter;

public:
	void Run(const std::string& title, int width, int height, bool resizeable = true);
	void Shutdown();

	// Virtual functions to be override
	virtual void OnStart() {};
	virtual void OnUpdate() {};
	virtual void OnImGuiRender() {};

	// Getters
	inline int GetWidth() { return Event::win_width; }
	inline int GetHeight() { return Event::win_height; }
	inline GLFWwindow* GetWindow() { return window; }

private:
	bool Init(bool resizeable);
	void UpdateViewport();
};
