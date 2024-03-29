//
//  TextureFragmentShader.glsl
//  HelloWindow
//
//  Created by XMX on 2019/09/19.
//  Copyright © 2019 XMX. All rights reserved.
//

#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

// Texture Samplers
uniform sampler2D tex1, tex2;

uniform float mixRatio;

void main()
{
    FragColor = mix(texture(tex1, texCoord) * vec4(ourColor, 1.0), texture(tex2, texCoord), mixRatio);
    // texture() will output the color obtained by sampling the texture with configured conditions
}
