#version 330 
 
uniform mat4 osg_ModelViewProjectionMatrix; 
uniform mat3 osg_NormalMatrix; 
uniform vec3 ecLightDir; 
uniform int useVertexColor;

uniform bool enableAlphaCutoff;
uniform float alphaCutoff;

uniform bool useBaseColorFactor;
uniform vec4 baseColorFactor;

uniform bool useBaseColorTexture;
uniform sampler2D baseColorTexture;

uniform bool useMetallicRoughnessTexture;
uniform sampler2D metallicRoughnessTexture;

uniform bool useNormalTexture;
uniform float normalTextureScale;
uniform sampler2D normalTexture;

uniform bool useOcclusionTexture;
uniform float occlusionTextureStrength;
uniform sampler2D occlusionTexture;

uniform bool useEmissiveTexture;
uniform vec3 emissiveFactor;
uniform sampler2D emissiveTexture;

uniform float metallicFactor;
uniform float roughnessFactor;

uniform bool MaterialDoubleSided;

in vec4 osg_Vertex; 
in vec3 osg_Normal; 
in vec4 osg_Color;
out vec4 osg_Color_f;
out vec2 texcoord;

layout(location = 15) in vec4 tangent;  // fix pos

void main() 
{
    vec3 ecNormal = normalize( osg_NormalMatrix * osg_Normal ); 
    float diffuse = max( dot( ecLightDir, ecNormal ), 0. ); 
    osg_Color_f = osg_Color;
    texcoord = gl_MultiTexCoord0.xy;	
    gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex; 
}