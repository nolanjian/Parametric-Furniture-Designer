#version 330

uniform mat4 osg_ModelViewProjectionMatrix; 

in vec3 aTextCoord;
uniform samplerCube skybox;

out vec4 fColor;

void main() 
{
    fColor = texture(skybox, aTextCoord);
}