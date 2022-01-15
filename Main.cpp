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

    // Gets ID of uniform called "scale"
    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    // Texture
    Texture brickTex("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    brickTex.textUnit(shaderProgram, "tex0", 0);

    // simple timer and accumulator for rotating he pyramid
    float rotation = 0.0f;
    double prevTime = glfwGetTime();

    // Enables the Depth Buffer
    glEnable(GL_DEPTH_TEST);

    // Main while loop
    while (!glfwWindowShouldClose(window))
    {
        // Add background colour
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clears the back buffer & depth bufferand assigns the background colour to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Tell OpenGL the Shader Program we're using
        shaderProgram.Activate();

        // check if the pyramid should rotate (60Hz)
        double crntTime = glfwGetTime();
        if (crntTime - prevTime >= 1.0 / 60)
        {
            rotation += 0.5f;
            prevTime = crntTime;
        }

        // Init matrices
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view  = glm::mat4(1.0f);
        glm::mat4 proj  = glm::mat4(1.0f);

        // Assign various tranformations
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0, 0.0f));
        view = glm::translate(view, glm::vec3(0, -0.5f, -2.0f));
        // set camera angle, aspect ratio, and field of view
        proj = glm::perspective(glm::radians(45.0f), (float)(width/ height), 0.1f, 100.0f);

        // Inputs the matrices into the Vertex Shader
        int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        // Assigns a value to the uniform (can only be done after the shader program is activated)
        glUniform1f(uniID, 0.5f);
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
