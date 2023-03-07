#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Shader.h"

using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int RING_COUNT = 18;

const float FOV = 90.0f;

unsigned int vbo, vao, ebo, texture;

glm::mat4 model, view, projection, mvp;

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
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CHAOS", NULL, NULL);

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
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST); //allow depth testing for 3D

	//create shader
	Shader shader;
	shader.compile("shaders/chaos.vert", "shaders/chaos.frag");

	//initialize the buffers
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao);

	//assign the data to the vao
	glBindVertexArray(vao);
	shader.useShader();

	//copied from https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/7.3.camera_mouse_zoom/camera_mouse_zoom.cpp
	//I am NOT writing this by hand
	float vertices[] = {
		//front face
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		//back face
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		//right face
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		//left face
		 0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		//bottom face
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		//top face
		-0.5f, 0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, 0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, 0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, 0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, 0.5f,  0.5f,  0.0f, 0.0f,
	};

	//specify vertices is the data
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//define the spacing of position and uv
	int posLocation = shader.getAttribLocation("in_pos");
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(posLocation);

	int uvLocation = shader.getAttribLocation("in_uv");
	glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(uvLocation);

	//unbind the vbo since its data is now specified (this does not work with EBOs)
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//unbind vao
	glBindVertexArray(0);

	//generate the texture
	glGenTextures(1, &texture);

	//set the texture paramaters
	glBindTexture(GL_TEXTURE_2D, texture);
	//set texture to repeat on out of bounds
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//use linear filtering for textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//load the image data into the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("textures/texture.jpg", &width, &height, &nrChannels, 4);

	if (!data) {
		cerr << "failed to load image" << endl;
	}

	//set the texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	//create the projection matrix
	projection = glm::perspective(glm::radians(FOV), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.001f, 1000.0f);

	//create the view matrix
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, -15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//main window loop
	while (!glfwWindowShouldClose(window))
	{
		//poll input to make windows not freak out
		glfwPollEvents();

		//clear both color and depth buffer (will explain later)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//bind texture to active
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		//bind the shader, vao then draw the triangles
		shader.useShader();

		//set the texture uniform
		glUniform1i(shader.getUniformLocation("texUniform"), 0);

		//bind vao
		glBindVertexArray(vao);

		//first model should be larger scale saul at spawn and pulse
		model = glm::scale(glm::mat4(1.0f), glm::vec3(10.0f));
		int pulse = 1;
		glUniform1iv(shader.getUniformLocation("pulse"), 1, &pulse);

		//use time
		float time = (float)glfwGetTime();
		glUniform1fv(shader.getUniformLocation("time"), 1, &time);

		//set the mvp value
		mvp = projection * view * model;
		glUniformMatrix4fv(shader.getUniformLocation("mvp"), 1, GL_FALSE, &mvp[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//draw ring of sauls
		pulse = 0;
		glUniform1iv(shader.getUniformLocation("pulse"), 1, &pulse);
		for (int i = 0; i < RING_COUNT; i++) {
			float spot = (float)i + (float)glfwGetTime();
			//ring making
			model = glm::rotate(glm::mat4(1.0f), glm::radians(360.0f / RING_COUNT * spot), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

			//random spot rotation
			model = glm::rotate(model, glm::radians(45.0f * spot * 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(25.0f * spot * 2.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(180.0f * spot * 2.0f), glm::vec3(0.0f, 0.0f, 1.0f));

			mvp = projection * view * model;
			glUniformMatrix4fv(shader.getUniformLocation("mvp"), 1, GL_FALSE, &mvp[0][0]);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//swap buffers to prevent flickering
		glfwSwapBuffers(window);
	}

	//make sure to free buffers and shader
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
	glDeleteTextures(1, &texture);
	shader.free();

	//destroy all resources
	glfwTerminate();

	return EXIT_SUCCESS;
}