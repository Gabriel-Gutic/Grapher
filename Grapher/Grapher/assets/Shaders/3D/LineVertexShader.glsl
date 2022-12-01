#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 Color;

uniform mat4 u_VP;

out vec4 o_Color;


void main()
{
	o_Color = Color;
	gl_Position = u_VP * vec4(Position, 1.0f);;
}