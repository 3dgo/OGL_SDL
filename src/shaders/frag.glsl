#version 330
// It was expressed that some drivers required this next line to function properly
precision highp float;
 
in vec4 ex_Color;
in vec2 ex_TexCoord;
out vec4 frag_color;

uniform sampler2D tex1;
 
void main(void) {
    frag_color = ex_Color * texture(tex1, ex_TexCoord);
}