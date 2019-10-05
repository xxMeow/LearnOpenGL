//
//  CoordSysVertexShader.glsl
//  HelloWindow
//
//  Created by Mingxian Xiao on 04/10/2019.
//  Copyright © 2019 XMX. All rights reserved.
//

#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main()
{
    gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);
    ourColor = aColor;
    texCoord = vec2(aTexCoord.x, aTexCoord.y);
}
