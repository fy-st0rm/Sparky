#include "WorkSpace.h"


void WorkSpace::Run(const std::string& title, int width, int height, bool resizeable)
{
	// Assigning some vars
	m_Title = title;
	Event::win_width = width;
	Event::win_height = height;

	if (!Init(resizeable))
	{
		std::cout << "Failed to initialize" << std::endl;
		return;
	}

	OnStart();
	while (!glfwWindowShouldClose(window))
	{
		UpdateViewport();
		OnUpdate();

		if (show_ui)
		{
			Gui::BeginFrame();

			double current_tick = glfwGetTime();
			fps_counter++;
			if (current_tick - previous_tick >= 1.0)
			{
				fps = fps_counter;
				fps_counter = 0;
				previous_tick = current_tick;
			}

			ImGui::Text(("[FPS]: "+std::to_string(fps)).c_str());
			OnImGuiRender();
			
			Gui::EndFrame();
		}

		// Toggolable imgui window
		if (Event::key_pressed(GLFW_KEY_GRAVE_ACCENT))
		{
			if (show_ui)
				show_ui = false;
			else
				show_ui = true;
		}

		GLCall(glfwSwapBuffers(window));

		// Event callbacks
		glfwSetWindowSizeCallback(window, window_resize_callback);
		glfwSetCursorPosCallback(window, cursor_position_callback);
		glfwSetKeyCallback(window, key_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);

		glfwPollEvents();
	}
}

void WorkSpace::Shutdown()
{
	Gui::ShutDown();
	Renderer::Shutdown();
}

void WorkSpace::UpdateViewport()
{
	GLCall(glViewport(0, 0, Event::win_width, Event::win_height));
}

bool WorkSpace::Init(bool resizeable)
{
	if (!glfwInit())
	{
		std::cout << "[ERROR]: Failed to init glfw.\n";
		return false;
	}

	// Some opengl stuff idk
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (!resizeable)
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// Creating window
	window = glfwCreateWindow(Event::win_width, Event::win_height, m_Title.c_str(), NULL, NULL);
	if (!window)
	{
		std::cout << "[ERROR]: Failed to create window.\n";
		return false;
	}

	// Creating opengl context
	glfwMakeContextCurrent(window);
	
	// Vsync thingy
	glfwSwapInterval(1);

	// glew initialization
	if (glewInit() != GLEW_OK)
	{
		std::cout << "[ERROR]: Failed to init glew.\n";
		return false;
	}

	// Enabling some stuff`
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	// Creating a view port
	GLCall(glViewport(0, 0, Event::win_width, Event::win_height))

	// Initializing renderer
	Renderer::Init();

	// Initializing the imgui
	Gui::Init(window);

	return true;
}