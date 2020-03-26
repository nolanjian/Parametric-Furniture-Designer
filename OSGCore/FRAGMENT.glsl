#version 330 
 
uniform sampler2D baseTexture; 
in vec2 texcoord;
out vec4 fColor;

uniform int useBaseColorFactor;
uniform vec4 baseColorFactor;
 
void main() 
{ 
	 if (useBaseColorFactor == 1)
	 {
		 fColor = baseColorFactor;
	 }
	 else
	 {
		 fColor = texture2D( baseTexture, texcoord);
	 }
}