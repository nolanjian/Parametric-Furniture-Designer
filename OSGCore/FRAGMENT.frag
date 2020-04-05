#version 330 

uniform mat4 osg_ModelViewProjectionMatrix; 
uniform mat3 osg_NormalMatrix;

out vec4 fColor;

in vec3 aosg_Normal;

uniform bool useVertexColor;
in vec4 osg_Color_f;

uniform bool useBaseColorFactor;
uniform vec4 baseColorFactor;

uniform bool useBaseColorTexture;
uniform sampler2D baseColorTexture;
uniform int baseColorTextureCoord;

uniform bool useMetallicRoughnessTexture;
uniform sampler2D metallicRoughnessTexture;
uniform int metallicRoughnessTextureCoord;
uniform float metallicFactor;
uniform float roughnessFactor;

uniform bool enableAlphaCutoff;
uniform float alphaCutoff;

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

uniform bool materialDoubleSided;

in vec2 texcoord0;
in vec2 texcoord1;

vec2 get_texcoord(int index)
{
    if (index == 0)
    {
        return texcoord0;
    }
    return texcoord1;
}
 
void get_color()
{
    if (useBaseColorTexture)
    {
        vec2 texcoord = get_texcoord(baseColorTextureCoord);

        fColor = texture2D(baseColorTexture, texcoord);
        if (useBaseColorFactor)
        {
            fColor = vec4(fColor.x * baseColorFactor.x,
                            fColor.y * baseColorFactor.y,
                            fColor.z * baseColorFactor.z,
                            fColor.w * baseColorFactor.w);
        }
    }
    else if (useVertexColor)
    {
        fColor = osg_Color_f;
        if (useBaseColorFactor)
        {
            fColor = vec4(fColor.x * baseColorFactor.x,
                            fColor.y * baseColorFactor.y,
                            fColor.z * baseColorFactor.z,
                            fColor.w * baseColorFactor.w);
        }
    }
    else if (useBaseColorFactor)
    {
        fColor = baseColorFactor;
    }
    
}

vec3 get_normal()
{
    if (useNormalTexture)
    {
        vec2 texcoord = get_texcoord(normalTextureCoord);

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

// void get_roughness()
// {
//     if (useMetallicRoughnessTexture)
//     {
//         vec2 texcoord = get_texcoord(metallicRoughnessTextureCoord);
//         vec3 rgb = texture2D(metallicRoughnessTexture, texcoord).rgb;
//         int metallic = rgb.b;
//         metallic = metallic * metallicFactor;
//         int roughness = rgb.g;
//         roughness = roughness * roughnessFactor;
//     }
// }

// void get_()
// {
//     if (useOcclusionTexture)
//     {
//         vec2 texcoord = get_texcoord(occlusionTextureCoord);
//         vec3 ot = texture2D(metallicRoughnessTexture, texcoord).xyz;

//     }

// uniform bool useOcclusionTexture;
// uniform float occlusionTextureStrength;
// uniform sampler2D occlusionTexture;
// uniform int occlusionTextureCoord;
// }

void main() 
{
    vec3 curNormal = get_normal();
    get_color();
    if (enableAlphaCutoff)
    {
        if (fColor.a < alphaCutoff)
        {
            discard;
        }
    }

    if (gl_FrontFacing)
    {
        
    }
    else
    {
        if (!materialDoubleSided)
        {
            discard;
        }
    }
}