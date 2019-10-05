//
//  CoordSysVertexShader.glsl
//  HelloWindow
//
//  Created by Mingxian Xiao on 04/10/2019.
//  Copyright © 2019 XMX. All rights reserved.
//

#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main()
{
    gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);
    texCoord = vec2(aTexCoord.x, aTexCoord.y);
}
