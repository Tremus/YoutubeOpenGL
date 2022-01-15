#include "./VAO.h"

VAO::VAO()
{
    glGenVertexArrays(1, &ID);
}

void VAO::LinkVBO(VBO& vbo, GLuint layout)
{
    vbo.Bind();
    // Configurations of the the Vertex we're using so OpenGL understands our specific VBO
    glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
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
