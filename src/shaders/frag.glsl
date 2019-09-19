#version 330 core

in vec4 ex_Color;

void main()
{
    gl_FragColor = vec4(ex_Color);
}