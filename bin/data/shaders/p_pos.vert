#version 330

layout (location = 0) in vec4 position;
layout (location = 3) in vec2 texcoord;

out vec2 v_texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    v_texcoord = texcoord;
    gl_Position = position;
}
