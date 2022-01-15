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
	// Do the same for the fragments
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// Create shader program and get the reference
	ID = glCreateProgram();
	// attaches the shaders to the program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// wrap-up / link all the shaders to the program
	glLinkProgram(ID);
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
