#version 330 
 
uniform mat4 osg_ModelViewProjectionMatrix; 
uniform mat3 osg_NormalMatrix; 
uniform vec3 ecLightDir; 
uniform int useBaseColorFactor; 
uniform vec4 baseColorFactor; 
 
in vec4 osg_Vertex; 
in vec3 osg_Normal; 
out vec2 texcoord;

void main() 
{
    vec3 ecNormal = normalize( osg_NormalMatrix * osg_Normal ); 
    float diffuse = max( dot( ecLightDir, ecNormal ), 0. ); 
    //color = vec4( vec3( diffuse ), 1. ); 
    texcoord = gl_MultiTexCoord0.xy;	
    gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex; 
}