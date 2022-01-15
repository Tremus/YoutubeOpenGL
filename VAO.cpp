#include "./VAO.h"

VAO::VAO()
{
    glGenVertexArrays(1, &ID);
}

void VAO::LinkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
    vbo.Bind();
    // Configurations of the the Vertex we're using so OpenGL understands our specific VBO
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    // Enable it
    glEnableVertexAttribArray(layout);
    vbo.Unbind();
}

void VAO::Bind()
{
    // Makes VAO the current Vertex Array Object by binding it
    glBindVertexArray(ID);
}
void VAO::Unbind()
{
    glBindVertexArray(0);
}
void VAO::Delete()
{
    glDeleteVertexArrays(1, &ID);
}
