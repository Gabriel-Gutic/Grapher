#version 330 core
layout (location = 0) in vec3 inCoords;
layout (location = 1) in vec4 inColor;
layout (location = 2) in float inTexIndex;
layout (location = 3) in vec2 inTexCoords;
layout (location = 4) in float inType;

uniform mat4 u_VP;

out vec4 Color;
out float TexIndex;
out vec2 TexCoords;
out float Type;


void main()
{
	Color = inColor;
	TexIndex = inTexIndex;
	TexCoords = inTexCoords;
	Type = inType;
	gl_Position = u_VP * vec4(inCoords, 1.0f);
}
