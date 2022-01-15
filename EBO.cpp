#include "./EBO.h"

EBO::EBO(GLuint* indices, GLsizeiptr size)
{
    // Create reference container
    glGenBuffers(1, &ID);
    // Bind the EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    // Adds the indices to the EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}
void EBO::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}
void EBO::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void EBO::Delete()
{
    glDeleteBuffers(1, &ID);
}
