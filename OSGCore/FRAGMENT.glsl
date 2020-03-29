#version 330 
 
uniform sampler2D baseTexture; 
in vec2 texcoord;
out vec4 fColor;

uniform int useVertexColor;
in vec4 osg_Color_f;

uniform bool enableAlphaCutoff;
uniform float alphaCutoff;

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
 
void main() 
{ 
	 if (useVertexColor == 1)
	 {
		 fColor = osg_Color_f;
	 }
	 else
	 {
		 fColor = texture2D( baseTexture, texcoord);
	 }
}