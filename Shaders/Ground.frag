#version 330

in vec3 aTextCoord;
uniform sampler2D GroundTex2D;

out vec4 fColor;

void main() 
{
    fColor = texture(GroundTex2D, aTextCoord.xy);
}