//
//  TextureVertexShader.glsl
//  HelloWindow
//
//  Created by XMX on 2019/09/19.
//  Copyright © 2019 XMX. All rights reserved.
//

#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 texCoord;

uniform mat4 transMatrix;

void main()
{
    gl_Position = transMatrix * vec4(aPos, 1.0f);
    ourColor = aColor;
    texCoord = vec2(aTexCoord.x, aTexCoord.y);
}
