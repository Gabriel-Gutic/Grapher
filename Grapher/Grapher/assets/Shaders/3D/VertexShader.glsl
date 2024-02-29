#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 Color;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec2 TexCoords;
layout (location = 4) in float TexIndex;

uniform mat4 u_VP;

out vec3 o_Position;
out vec4 o_Color;
out vec3 o_Normal;
out vec2 o_TexCoords;
out float o_TexIndex;


void main()
{
	o_Position = Position;
	o_Color = vec4(Color.a * Color.rgb, Color.a);
	o_Normal = Normal;
	o_TexCoords = TexCoords;
	o_TexIndex = TexIndex;
	gl_Position = u_VP * vec4(Position, 1.0f);
}
