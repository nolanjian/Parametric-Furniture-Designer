#version 330 

uniform mat4 osg_ModelViewProjectionMatrix; 
uniform mat3 osg_NormalMatrix;

in vec2 texcoord;
out vec4 fColor;

in vec3 aosg_Normal;

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
        vec4 bct = texture2D(baseColorTexture, texcoord);
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

vec3 get_normal()
{
    if (useNormalTexture)
    {
        vec3 normal = texture2D(normalTexture, texcoord).xyz;
        normal = normal * 2 - 1;
        vec3 tmp = normalize( osg_NormalMatrix * normal ); 
        return tmp;
    }
    else
    {
        vec3 tmp = normalize( osg_NormalMatrix * aosg_Normal ); 
        return tmp;
    }
}

void main() 
{
	get_color();
    if (enableAlphaCutoff)
    {
        if (fColor.a < alphaCutoff)
        {
            discard;
        }
    }

    vec3 curNormal = get_normal();
}