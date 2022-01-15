#include "./shaderClass.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		// move reader to end of characters
		in.seekg(0, std::ios::end);
		// allocate memory in string
		contents.resize(in.tellg());
		// move reader to start
		in.seekg(0, std::ios::beg);
		// write to our string N characters, where N is the size of our string
		in.read(&contents[0], contents.size());
		// close file
		in.close();
		return contents;
	}
	throw (errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Create vertex shader and get refernce
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach shader source to shader object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// compiles shader into machine code
	glCompileShader(vertexShader);
	// Test success
	compileErrors(vertexShader, "VERTEX");

	// Do the same for the fragments
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	// Test success
	compileErrors(fragmentShader, "FRAGMENT");

	// Create shader program and get the reference
	ID = glCreateProgram();
	// attaches the shaders to the program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// wrap-up / link all the shaders to the program
	glLinkProgram(ID);
	// Test success
	compileErrors(ID, "PROGRAM");
	// Cleanup old shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
	glUseProgram(ID);
}
void Shader::Delete()
{
	glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char* type)
{
	GLint  hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << std::endl;
		}
	}
	else
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << std::endl;
		}
	}
}
