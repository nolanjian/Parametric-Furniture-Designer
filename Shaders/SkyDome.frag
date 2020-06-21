#version 330

in vec3 aTextCoord;
uniform samplerCube SkyDomeCubeMap;

out vec4 fColor;

void main() 
{
    fColor = texture(SkyDomeCubeMap, aTextCoord);
}