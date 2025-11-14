#include "dependencies/glad/include/glad/glad.h"
#include "dependencies/glad/glad.c"

#include <GLFW/glfw3.h>
 
#include <iostream> 
 
void display_screen()
{	
	// (1) GLFW: Initialise & Configure
	// -----------------------------------------
	if (!glfwInit())
		exit(EXIT_FAILURE);
 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
 
	int monitor_width = mode->width; // Monitor's width.
	int monitor_height = mode->height;
 
	int window_width = (int)(monitor_width * 0.5f); // Window size will be 50% the monitor's size...
	int window_height = (int)(monitor_height * 0.5f); // ... Cast is simply to silence the compiler warning.
 
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "GLFW Test Window – Changing Colours", NULL, NULL);
	// GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Drawing Basic Shapes - Buffer Objects & Shaders", glfwGetPrimaryMonitor(), NULL); // Full Screen Mode ("Alt" + "F4" to Exit!)
 
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
 
	glfwMakeContextCurrent(window); // Set the window to be used and then centre that window on the monitor. 
	glfwSetWindowPos(window, (monitor_width - window_width) / 2, (monitor_height - window_height) / 2);
 
	glfwSwapInterval(1); // Set VSync rate 1:1 with monitor's refresh rate.
 
	// (2) GLAD: Load OpenGL Function Pointers
	// -------------------------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // For GLAD 2 use the following instead: gladLoadGL(glfwGetProcAddress)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
 
	// (3) Enter the Main-Loop
	// --------------------------------	
	float red = 0.0f; // Set individual colours here: OpenGL RGB [0, 1] = Actual RGB / 255
	float green = 0.0f;
	float blue = 0.0f;
 
	while (!glfwWindowShouldClose(window)) // Main-Loop
	{
		glClearColor(red , green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); // Clear the screen with... red, green, blue.
 
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	/* glfwDestroyWindow(window) // Call this function to destroy a specific window */
	glfwTerminate(); // Destroys all remaining windows and cursors, restores modified gamma ramps, and frees resources.
 
	exit(EXIT_SUCCESS); // Function call: exit() is a C/C++ function that performs various tasks to help clean up resources.
}
