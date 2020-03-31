#version 330 
 
uniform sampler2D baseTexture; 
in vec2 texcoord;
out vec4 fColor;

uniform bool useVertexColor;
in vec4 osg_Color_f;

uniform bool useBaseColorFactor;
uniform vec4 baseColorFactor;

uniform bool useBaseColorTexture;
uniform sampler2D baseColorTexture;

uniform bool useMetallicRoughnessTexture;
uniform sampler2D metallicRoughnessTexture;

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
 
void get_color()
{
    if (useBaseColorTexture)
    {
        vec4 bct = texture2D( baseTexture, texcoord);
        if (useBaseColorFactor)
        {
            vec4 tmp = vec4(bct.x * baseColorFactor.x,
                            bct.y * baseColorFactor.y,
                            bct.z * baseColorFactor.z,
                            bct.w * baseColorFactor.w);
            fColor = tmp;
        }
        else
        {
            fColor = bct;
        }
    }
    else if (useBaseColorFactor)
    {
        fColor = baseColorFactor;
    }
    else if (useVertexColor)
    {
        fColor = osg_Color_f;
    }
}

void main() 
{
	get_color();
}