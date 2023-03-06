#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

using namespace std;

const int WIDTH = 640;
const int HEIGHT = 480;

int main() {
	//initialize GLFW (windows and stuff)
	if (!glfwInit())
	{
		cerr << "Failed to initialize glfw" << endl;
		return EXIT_FAILURE;
	}

	//make glfw settings
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //open gl version 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); //forward compatiblity
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //use modern opengl
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //do not allow window resizing
	
	//create the window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Window Example", NULL, NULL);

	//set window as target
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); //vsync enabled, set to 0 for no vsync

	//initializes glew
	glewExperimental = true;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cerr << "GLEW failed to initialize" << endl;
		return EXIT_FAILURE;
	}

	//solid black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//various opengl settings
	glCullFace(GL_BACK); //cull backfaces to reduce render time
	glEnable(GL_DEPTH_TEST); //allow depth testing for 3D

	//main window loop
	while (!glfwWindowShouldClose(window))
	{
		//poll input to make windows not freak out
		glfwPollEvents();

		//clear both color and depth buffer (will explain later)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//swap buffers to prevent flickering
		glfwSwapBuffers(window);
	}

	return EXIT_SUCCESS;
}