//
//  main.cpp
//  HelloWindow
//
//  Created by XMX on 2019/09/9.
//  Copyright © 2019 XMX. All rights reserved.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

using namespace std;

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

// A callback function that will be called each time the size of window changed
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// The vertex shader and fragment shader must be self-defined
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "out vec4 vertexColor;"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "    vertexColor = vec4(0.3, 0.0, 0.2, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec4 vertexColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vertexColor;\n"
    "}\n\0";

int main(int argc, const char *argv[])
{
    // Initialize GLFW
    glfwInit();
    
    // Set OpenGL version number as 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Use the core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // MacOS is forward compatible
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // Create a GLFW window
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "xmxOpenGL", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window." << endl;
        glfwTerminate();
        return -1;
    }
    // Set the context of this window as the main context of current thread
    glfwMakeContextCurrent(window);
    
    /** Callback Functions should be registered after creating window and before initializing render loop **/
    // Pass the pointer of framebuffer_size_callback to the GLFW
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // Initialize GLAD : this should be done before using any openGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD." << endl;
        glfwTerminate(); // This line isn't in the official source code, but I think that it should be added here.
        return -1;
    }
    
    /** Bulid and compile shaders **/
    // Vertex Shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }
    // Fragment Shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }
    /** Link Shaders **/
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for link errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }
    // Shaders can be deleted after being linked to program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    /** Setup vertex data **/
    // Define vertices
    // Each vertex includes 3 coordinates (x, y, z:depth), the middle point of space is (0.0, 0.0, 0.0)
    float vertices[] = {
        -0.5f,  0.5f, 0.0f, // Top Left
         0.5f,  0.5f, 0.0f, // Top Right
         0.5f, -0.5f, 0.0f, // Bottom Right
        -0.5f, -0.5f, 0.0f  // Bottom Left
    };
    // Define the indices of vertices we want
    unsigned int indices[] = {
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };
    
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // Bind the VAO
    glBindVertexArray(VAO);
    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData() will copy datas to the buffer which is being bound right now
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Bind the EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // glVertexAttribPointer() will get each vertex attributes from the VBO which is being bound to GL_ARRAY_BUFFER right now
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    /** Unbind **/
    // Since glAttribArrayPointer() has registered VBO as the vertex attributes' bound vertex buffer object, now we can unbind it safely
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Similar to the VBO, the VAO can be also safely unbound now so that this VAO won't get accidentally modified by other VAO calls
    // However, modifying other VAOs required a call to glBindVertexArray() anyways so it's okay that don't do the unbinding to VAOs (but not VBOs) when it's not directly necessary
    glBindVertexArray(0);
    
    // Uncomment this call to draw in wireframe polygons
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Option: GL_FILL(default), GL_POINT, GL_LINE
    
    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Check for events
        processInput(window);
        
        /** Other rendering operations **/
        
        // Set background clolor
        glClearColor(0.2f, 0.2f, 0.5f, 0.2f); // Set color value (R,G,B,A) - Set Status
        glClear(GL_COLOR_BUFFER_BIT); // Use the color to clear screen - Use Status
        
        // Draw triangles
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // In fact, we don't need to bind the VAO every render loop here since we only have a single VAO. We just do it so to keep things a bit organized
        
        // Tell OpenGL to draw 6 vertices whose indices are stored in the VBO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glDrawElements() will access the elements in the VAO which is being bound right now
        
        //glBindVertexArray(0); // Again, we don't need to unbind it right now
        
        // Swap color buffers since double buffers are applied for rendering
        /** After updating my MacOS to Mojave10.14.6, this function dose not cause flickering now **/
        glfwSwapBuffers(window);
        // Update the status of window
        glfwPollEvents();
    }
    
    // Optional: De-allocate all resources once they've outlived their purpoose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    glfwTerminate();
    
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // Reset the size of glViewport
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    // Check if key ESCAPE is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { // If key did not get pressed it will return GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);
    }
}
