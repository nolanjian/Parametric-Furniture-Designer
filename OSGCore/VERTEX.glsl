#version 330 
 
uniform mat4 osg_ModelViewProjectionMatrix; 
uniform mat3 osg_NormalMatrix; 
uniform vec3 ecLightDir; 
uniform int useVertexColor;

uniform bool enableAlphaCutoff;
uniform float alphaCutoff;
 
in vec4 osg_Vertex; 
in vec3 osg_Normal; 
in vec4 osg_Color;
out vec4 osg_Color_f;
out vec2 texcoord;

void main() 
{
    vec3 ecNormal = normalize( osg_NormalMatrix * osg_Normal ); 
    float diffuse = max( dot( ecLightDir, ecNormal ), 0. ); 
    osg_Color_f = osg_Color;
    texcoord = gl_MultiTexCoord0.xy;	
    gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex; 
}