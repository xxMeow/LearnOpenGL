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
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


int main(int argc, const char * argv[])
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
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "xmxOpenGL", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window." << endl;
        glfwTerminate();
        return -1;
    }
    // Set the context of this window as the main context of current thread
    glfwMakeContextCurrent(window);
    
    /** Callback Functions should be registered after creating window and before initializing render loop**/
    // Pass the pointer of framebuffer_size_callback to the GLFW
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // Initialize GLAD : this should be done before using any openGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD." << endl;
        return -1;
    }
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Update the status of window
        glfwPollEvents();
        /**
         * Swap color buffers since double buffers are applied for rendering
         * // glfwSwapBuffers(window);
         * This glfwSwapBuffers() is used to swap the double buffer to avoid the screen flickering
         * However, it infact causes the problem on my Mac for some unknow reasons
         * This link may be helpful: https://github.com/glfw/glfw/issues/713
         **/
    }
    
    glfwTerminate();
    
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // Reset the size of glViewport
    glViewport(0, 0, width, height);
}
