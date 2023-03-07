#pragma once
#include <glm/glm.hpp>

#include <string>
#include <vector>

class Shader {
	public:
		Shader();

		//returns a non-zero error code if compilation failed for whatever reason
		int compile(std::string vertFile, std::string fragFile);

		//sets the shader to be the active one
		void useShader();

		//gets the location of a specified attribute
		int getAttribLocation(std::string name);

		//gets the location of a uniform
		int getUniformLocation(std::string name);

		//free the shader
		void free();
	private:

		//returns a non-zero error code if compilation failed for whatever reason
		int compileShader(std::string sourceFile, unsigned int* shaderId);

		unsigned int _vertShader, _fragShader, _shaderProgram;
};