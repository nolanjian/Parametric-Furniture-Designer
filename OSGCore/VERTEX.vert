#version 330

uniform mat4 osg_ModelViewProjectionMatrix; 
uniform mat3 osg_NormalMatrix; 
uniform vec3 ecLightDir; 
uniform bool useVertexColor;

uniform bool enableAlphaCutoff;
uniform float alphaCutoff;

uniform bool useBaseColorFactor;
uniform vec4 baseColorFactor;

uniform bool useBaseColorTexture;
uniform sampler2D baseColorTexture;
uniform int baseColorTextureCoord;

uniform bool useMetallicRoughnessTexture;
uniform sampler2D metallicRoughnessTexture;
uniform int metallicRoughnessTextureCoord;

uniform bool useNormalTexture;
uniform float normalTextureScale;
uniform sampler2D normalTexture;
uniform int normalTextureCoord;

uniform bool useOcclusionTexture;
uniform float occlusionTextureStrength;
uniform sampler2D occlusionTexture;
uniform int occlusionTextureCoord;

uniform bool useEmissiveTexture;
uniform vec3 emissiveFactor;
uniform sampler2D emissiveTexture;
uniform int emissiveTextureCoord;

uniform float metallicFactor;
uniform float roughnessFactor;

uniform bool materialDoubleSided;

in vec4 osg_Vertex; 
in vec4 osg_Color;
out vec4 osg_Color_f;

in vec3 osg_Normal; 
out vec3 aosg_Normal;

layout(location = 15) in vec4 tangent;  // fix pos

out vec2 texcoord0;
out vec2 texcoord1;

void main() 
{
    vec3 ecNormal = normalize( osg_NormalMatrix * osg_Normal ); 
    float diffuse = max( dot( ecLightDir, ecNormal ), 0. );

    texcoord0 = gl_MultiTexCoord0.xy;
    texcoord1 = gl_MultiTexCoord1.xy;
    
    osg_Color_f = osg_Color;
    gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex; 
}