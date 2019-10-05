//
//  main.cpp
//  HelloWindow
//
//  Created by XMX on 2019/09/9.
//  Copyright © 2019 XMX. All rights reserved.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

#include "../Headers/Shader.h"
#include "../Headers/stb_image.h"

using namespace std;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

float mixRatio = 0.2f;

// A callback function that will be called each time the size of window changed
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

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
    Shader ourShader("../Shaders/CoordSysVertexShader.glsl", "../Shaders/CoordSysFragmentShader.glsl");
    
    /** Setup vertex data **/
    // Define vertices
    // Each vertex includes 3 coordinates (x, y, z:depth), the middle point of space is (0.0, 0.0, 0.0)
    float vertices[] = {
        // Positions          // Colors           // Texture Coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left
    };
    // Define the indices of vertices we want
    unsigned int indices[] = {
        0, 1, 3, // Triangle1
        1, 2, 3  // Triangle2
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
    
    /** glVertexAttribPointer() will get each vertex attributes from the VBO
     *  void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
     **/
    // Position Pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color Pointer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture Coordinate Pointer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    /** Load 1st texture **/
    // Assign texture ID and gengeration
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // Set the texture wrapping parameters (for 2D tex: S, T)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters (Minify, Magnify)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    /** Load image and configure texture **/
    stbi_set_flip_vertically_on_load(true);
    int width, height, colorChannels; // After loading the image, stb_image will fill them
    unsigned char *data = stbi_load("../Images/container.jpg", &width, &height, &colorChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // Automatically generate all the required mipmaps for the currently bound texture.
    } else {
        cout << "Failed to load texture" << endl;
    }
    // Always free image memory
    stbi_image_free(data);
    
    /** Load 2nd texture **/
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("../Images/awesomeface.png", &width, &height, &colorChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // Automatically generate all the required mipmaps for the currently bound texture.
        cout << "Face loaded." << endl;
    } else {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free(data);
    
    /** Set uniform **/
    ourShader.use(); // Active shader before set uniform
    // Tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    ourShader.setInt("tex1", 0);
    ourShader.setInt("tex2", 1);
    
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
        
        /** Bind texture on corresponding texture units **/
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        // Same as the VAO, we actuall don't need to bind it since we only have a single texture (fot each texture unit)
        
        /** Create coordinate transformation matrix **/
        // Initialize them as identity matrix
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::mat4 viewMatrix = glm::mat4(1.0f);
        glm::mat4 projMatrix = glm::mat4(1.0f);
        // Set matrices
        modelMatrix = glm::rotate(modelMatrix, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
        projMatrix = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 100.0f);
        
        ourShader.setFloat("mixRatio", mixRatio);
        
        /** Active shader **/
        ourShader.use();
        
        /** Set uniforms **/
        ourShader.setMat4("modelMatrix", modelMatrix);
        ourShader.setMat4("viewMatrix", viewMatrix);
        ourShader.setMat4("projMatrix", projMatrix);
        // Actually since projMatrix rarely changes it's often best practice to set it outside the main loop only once
        
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
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        mixRatio += 0.0002f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        mixRatio -= 0.0002f;
    }
}
