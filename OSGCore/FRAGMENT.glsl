#version 330 
 
uniform sampler2D baseTexture; 
in vec2 texcoord;
out vec4 fColor;

uniform int useVertexColor;
in vec4 osg_Color_f;

uniform bool enableAlphaCutoff;
uniform float alphaCutoff

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