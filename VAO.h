#pragma once
#include <glad/glad.h>
#include "./VBO.h"

class VAO
{
public:
	GLuint ID;
	VAO();

	void LinkVBO(VBO& vbo, GLuint layout);
	void Bind();
	void Unbind();
	void Delete();
};
