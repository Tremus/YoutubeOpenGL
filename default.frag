#version 330 core

// Outputs colours in RGBA
out vec4 FragColor;

// Inputs the "color" the from the Vertex Shader
in vec3 color;
// Inputs "texCoord" from the Vertex Shader
in vec2 texCoord;

uniform sampler2D tex0;

void main()
{
    FragColor = texture(tex0, texCoord);
}