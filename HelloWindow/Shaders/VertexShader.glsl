//
//  VertexShader.glsl
//  HelloWindow
//
//  Created by XMX on 2019/09/16.
//  Copyright © 2019 XMX. All rights reserved.
//

#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    
    float texCoord[] = {
        0.0f, 0.0f, // Left
        1.0f, 0.0f, // Right
        0.5f, 1.0f  // Top
    };
    
    ourColor = aColor;
}
