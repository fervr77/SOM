#include "ShaderLoader.h"
#include <glad/glad.h>
#include <sstream>
#include <fstream>
#include <iostream>

unsigned int ShaderLoader::Load(const std::string& name)
{
	std::stringstream ss;
	std::string vertexFileName = name + ".vs";
	std::ifstream inputFile(vertexFileName);
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
	int success;
	char infoLog[512];

	if (inputFile.is_open())
	{
		ss << inputFile.rdbuf();

		std::string fileString = ss.str();

		const char* vertexSource = fileString.c_str();

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);

		

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		inputFile.close();
	}

	std::stringstream ss2;
	std::string fragmentFileName = name + ".fs";
	inputFile.open(fragmentFileName);
	if (inputFile.is_open())
	{
		ss2 << inputFile.rdbuf();

		std::string fileString = ss2.str();

		const char* fragmentSource = fileString.c_str();

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);

		

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		inputFile.close();


	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK_ERROR\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	return shaderProgram;

	

}