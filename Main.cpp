#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Some shaders
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(0.8f, 0.3f, 0.2f, 1.0f);\n"
"}\n\0";


int main()
{
    // Initialise GLFW
    glfwInit();

    // Tell GLFW what version of OpenGL we're using (3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell GLFW we are using the CORE profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Some shape (equalateral triangle)
    GLfloat vertices[] =
    {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // lower left corner
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,  // lower right corner
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // top
    };

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

    // Create vertex shader and get refernce
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach shader source to shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // compiles shader into machine code
    glCompileShader(vertexShader);
    // Do the same for the fragments
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Create shader program and get the reference
    GLuint shaderProgram = glCreateProgram();
    // attaches the shaders to the program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // wrap-up / link all the shaders to the program
    glLinkProgram(shaderProgram);
    // Cleanup old shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Create reference containers for the Vertex Array Object and Vertex Buffer Object
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Makes VAO the current Vertex Array Object by binding it
    glBindVertexArray(VAO);
    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Adds the vertices to the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configurations of the the Vertex we're using so OpenGL understands our specific VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // Enable it
    glEnableVertexAttribArray(0);

    // Bind both the VAO and VBO to 0 so we dont accdently modify the VAO/VBO we created
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // Add background colour
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    // Clears the back buffer and assigns the background colour to it
    glClear(GL_COLOR_BUFFER_BIT);
    // Swaps the front & back buffer, making our BG colour visible
    glfwSwapBuffers(window);

    // Main while loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Tell OpenGL the Shader Program we're using
        glUseProgram(shaderProgram);
        // Bind VAO so OpenGL knows about it
        glBindVertexArray(VAO);
        // Draws a triangle using the Triangle promitive
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);

        // handle GLFW events, such as closing the GLFW window, which breaks the loop
        glfwPollEvents();
    }


    // Cleanup
    // delete out objects
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    // Delete the OpenGL window
    glfwDestroyWindow(window);
    // turn off GLFW
    glfwTerminate();
    return 0;
}
