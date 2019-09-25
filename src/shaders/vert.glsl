#version 330
layout (location = 0) in vec3 in_Pos;
layout (location = 1) in vec4 in_Color;
layout (location = 2) in vec2 in_TexCoord;

out vec4 ex_Color;
out vec2 ex_TexCoord;

uniform vec4 tintColor;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(in_Pos.x, in_Pos.y, in_Pos.z, 1.0);
    ex_Color = in_Color * tintColor;
    ex_TexCoord = in_TexCoord;
}