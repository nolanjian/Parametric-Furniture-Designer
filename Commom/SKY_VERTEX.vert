#version 330

uniform mat4 osg_ModelViewProjectionMatrix;

in vec4 osg_Vertex;

out vec3 aTextCoord;

void main() 
{
    gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex;
    aTextCoord = osg_Vertex.xyz;
}