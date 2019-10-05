//
//  CorrdSysFragmentShader.glsl
//  HelloWindow
//
//  Created by Mingxian Xiao on 04/10/2019.
//  Copyright © 2019 XMX. All rights reserved.
//

#version 330 core

out vec4 FragColor;

in vec2 texCoord;

// Texture Samplers
uniform sampler2D tex1, tex2;

uniform float mixRatio;

void main()
{
    FragColor = mix(texture(tex1, texCoord), texture(tex2, texCoord), mixRatio);
    // texture() will output the color obtained by sampling the texture with configured conditions
}
