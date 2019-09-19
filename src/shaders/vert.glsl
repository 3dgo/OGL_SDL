#version 330 core
layout (position = 0) in vec3 in_Pos;
layout (position = 1) in vec4 in_Color;
out vec4 ex_Color;

void main()
{
    gl_Position = vec4(in_Pos.x, in_Pos.y, in_Pos.z, 1.0);
    ex_Color = in_Color;
}