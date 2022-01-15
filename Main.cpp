#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

// Some shape (equalateral triangle)
GLfloat vertices[] =
{   // COORDINATES         // COLOURS           // Tex Coord
    -0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f, // lower left corner
    -0.5f,  0.5f, 0.0f,    0.0f, 1.0f, 0.0f,    0.0f, 1.0f, // upper left corner
     0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f, // upper right
     0.5f, -0.5f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, // lower right
};

GLuint indices[] =
{
    0,2,1, // upper triangle
    0,3,2, // lower triangle
};

int main()
{
    // Initialise GLFW
    glfwInit();

    // Tell GLFW what version of OpenGL we're using (3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell GLFW we are using the CORE profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 800, "YoutubeOpenGL", NULL, NULL);
    // If creating window failed, exit program
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    // Introduce the window to the current context
    glfwMakeContextCurrent(window);

    //Load GLAD so it configures OpenGL
    gladLoadGL();
    
    // Specify the OpenGL viewport (same as window)
    glViewport(0, 0, 800, 800);

    // Create shaders
    Shader shaderProgram("default.vert", "default.frag");

    // Generate and bind Vertex Array Object
    VAO vao1;
    vao1.Bind();

    // Generate Vertex Buffer Object links it to vertices
    VBO vbo1(vertices, sizeof(vertices));
    // Generate Element Buffer Object links it to indices
    EBO ebo1(indices, sizeof(indices));

    // Links coordinates to VAO
    vao1.LinkAttrib(vbo1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    // Links colours to VAO
    vao1.LinkAttrib(vbo1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vao1.LinkAttrib(vbo1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    // Unbind all objects so we dont accdently modify the VAO/VBO we created
    vao1.Unbind();
    vbo1.Unbind();
    ebo1.Unbind();

    // Gets ID of uniform called "scale"
    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    // Texture
    Texture popCat("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    popCat.textUnit(shaderProgram, "tex0", 0);
    

    GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
    shaderProgram.Activate();
    glUniform1i(tex0Uni, 0);


    // Main while loop
    while (!glfwWindowShouldClose(window))
    {
        // Add background colour
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clears the back buffer and assigns the background colour to it
        glClear(GL_COLOR_BUFFER_BIT);
        // Tell OpenGL the Shader Program we're using
        shaderProgram.Activate();
        // Assigns a value to the uniform (can only be done after the shader program is activated)
        glUniform1f(uniID, 0.5f);
        // Binds texture so it renders
        popCat.Bind();
        // Bind VAO so OpenGL knows about it
        vao1.Bind();
        // Draws a triangle using the Triangle promitive
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // Swaps the front & back buffer, making our BG colour visible
        glfwSwapBuffers(window);
        // handle GLFW events, such as closing the GLFW window, which breaks the loop
        glfwPollEvents();
    }


    // Cleanup
    // delete our objects
    vao1.Delete();
    vbo1.Delete();
    ebo1.Delete();
    popCat.Delete();
    //glDeleteTextures(1, &texture);
    shaderProgram.Delete();
    // Delete the OpenGL window
    glfwDestroyWindow(window);
    // turn off GLFW
    glfwTerminate();
    return 0;
}
