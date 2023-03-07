#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Shader.h"

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;

unsigned int vbo, vao;

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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Triangle Example", NULL, NULL);

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

	//create shader
	Shader shader;
	shader.compile("shaders/basic.vert", "shaders/basic.frag");
	
	//initialize the buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	//assign the data to the vao
	glBindVertexArray(vao);
	shader.useShader();

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	//set vertices to be the position parameter of our vbo
	float vertices[] = {
		-0.5f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.5f, 0.0f, 0.0f
	};
	//specify vertices is the data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//define the spacing of the parameter
	int posLocation = shader.getAttribLocation("in_pos");
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(posLocation);

	//unbind the vbo since its data is now specified (this does not work with EBOs)
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//unbind vao
	glBindVertexArray(0);

	//main window loop
	while (!glfwWindowShouldClose(window))
	{
		//poll input to make windows not freak out
		glfwPollEvents();

		//clear both color and depth buffer (will explain later)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//bind the shader, vao then draw the triangles
		shader.useShader();
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		//swap buffers to prevent flickering
		glfwSwapBuffers(window);
	}

	//make sure to free buffers and shader
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	shader.free();

	//destroy all resources
	glfwTerminate();

	return EXIT_SUCCESS;
}