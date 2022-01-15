#include "./VBO.h"

VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
    // Create reference container
	glGenBuffers(1, &ID);
    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    // Adds the vertices to the VBO
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}
void VBO::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}
void VBO::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void VBO::Delete()
{
    glDeleteBuffers(1, &ID);
}
