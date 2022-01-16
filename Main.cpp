#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"


// Some shape (equalateral triangle)
GLfloat vertices[] =
{   // COORDINATES         // COLOURS           // Tex Coord
    -0.5f, 0.0f,  0.5f,    0.83f, 0.7f,  0.44f,    0.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,    0.83f, 0.7f,  0.44f,    5.0f, 0.0f,
     0.5f, 0.0f, -0.5f,    0.83f, 0.7f,  0.44f,    0.0f, 0.0f,
     0.5f, 0.0f,  0.5f,    0.83f, 0.7f,  0.44f,    5.0f, 0.0f,
     0.0f, 0.8f,  0.0f,    0.83f, 0.86f, 0.76f,    2.5f, 5.0f,
};

GLuint indices[] =
{
    0,1,2,
    0,2,3,
    0,1,4,
    1,2,4,
    2,3,4,
    3,0,4,
};

const unsigned int width = 800;
const unsigned int height = 800;

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
    GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);
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
    glViewport(0, 0, width, height);

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

    // Texture
    Texture brickTex("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    brickTex.textUnit(shaderProgram, "tex0", 0);

    // Enables the Depth Buffer
    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    // Main while loop
    while (!glfwWindowShouldClose(window))
    {
        // Add background colour
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clears the back buffer & depth bufferand assigns the background colour to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Tell OpenGL the Shader Program we're using
        shaderProgram.Activate();

        // handle mouse & keyboard inputs
        camera.Inputs(window);
        // update and send camera matrix to vertex shader
        camera.Matrix(45.0, 0.1f, 100.0f, shaderProgram, "camMatrix");


        // Binds texture so it renders
        brickTex.Bind();
        // Bind VAO so OpenGL knows about it
        vao1.Bind();
        // Draws a triangle using the Triangle promitive
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
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
    brickTex.Delete();
    //glDeleteTextures(1, &texture);
    shaderProgram.Delete();
    // Delete the OpenGL window
    glfwDestroyWindow(window);
    // turn off GLFW
    glfwTerminate();
    return 0;
}
